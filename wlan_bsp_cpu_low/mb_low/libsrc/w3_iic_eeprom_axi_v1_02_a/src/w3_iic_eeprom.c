/*****************************************************************
* File: w3_iic_eeprom.c
* Copyright (c) 2012-2016 Mango Communications, all rights reserved
* Released under the WARP License
* See http://warp.rice.edu/license for details
*****************************************************************/

/** \file w3_iic_eeprom.c

\mainpage
This is the driver for the w3_iic_eeprom_axi core, which implements an I2C 
master for accessing the EEPROM on the WARP v3 board.

This driver implements functions for reading and writing individual bytes 
in the EEPROM. Functions are also provided for accessing EEPROM entries 
written during manufacturing (serial number, etc.).

The EEPROM is readable/writable from user-code.  However, addresses 
greater than 16000 are reserved and write-protected.

Refer to the <a href="http://warp.rice.edu/trac/wiki/HardwareUsersGuides/WARPv3/EEPROM">WARP v3 User Guide</a>
for details on the data written to the EEPROM during manufacturing.

To allow the w3_iic_eeprom_axi core to be used as a shared peripheral in
a multiple CPU environment, mutex functionality is implemented.  The 
mutex register, IIC_EEPROM_REG_MUTEX, has a special "lock bit" (bit 31)
that can lock the ability to update the other bits in the register. The
software sequence to use the mutex functionality:
    - To lock the peripheral:
        - Write CPU ID and Lock Bit to the mutex register
        - Read mutex register and check if CPU ID and Lock bit matches
            - If yes, then CPU has locked the peripheral and can update it
            - If no, then CPU should not access peripheral 

    - To unlock the peripheral
        - Write 0 to the mutex register (this will set the lock bit to zero, 
          so the register is writeable again) 

This approach does not have the unlock protections of the mutex peripheral 
(i.e. the mutex peripheral uses the CPU ID and potentially the bus master 
ID to enforce only the CPU who has locked the mutex is unlocking the mutex) 
but given we are not trying to protect against hacking, the sequence described 
above should be sufficient to protect the SW driver functions.

In order to speed up performance in a multiple CPU environment, when the 
EEPROM is initialized, it will read all of the defined data values and then 
cache them into registers within the peripheral.  This means that future 
reads of the defined data values will be much faster, but the initialization
of the EEPROM will take longer.  The defined data value registers are:
    IIC_EEPROM_REG_SERIAL_NUM
    IIC_EEPROM_REG_ETH_A_MAC_ADDR_0
    IIC_EEPROM_REG_ETH_A_MAC_ADDR_1
    IIC_EEPROM_REG_ETH_B_MAC_ADDR_0
    IIC_EEPROM_REG_ETH_B_MAC_ADDR_1
    IIC_EEPROM_REG_FPGA_DNA_0
    IIC_EEPROM_REG_FPGA_DNA_1

@version 1.02.a
@author Patrick Murphy
@copyright (c) 2012 Mango Communications, Inc. All rights reserved.<br>
Released under the WARP open source license (see http://warp.rice.edu/license)

\brief Main source for EEPROM controller driver

*/

/***************************** Include Files *********************************/
#include "w3_iic_eeprom.h"
#include "stdio.h"


/*************************** Functions Prototypes ****************************/

inline int    iic_eeprom_wait_for_rx_ack(u32 ba);
u32           iic_eeprom_read_serial_num(u32 ba);
u32           iic_eeprom_read_fpga_dna(u32 ba, int lo_hi);
void          iic_eeprom_read_eth_addr(u32 ba, u8 addr_sel, u8* addr_buf);
int           iic_eeprom_write_byte_internal(u32 ba, u16 addr_to_write, u8 byte_to_write);
int           iic_eeprom_read_byte_internal(u32 ba, u16 addr_to_read);


/******************************** Functions **********************************/


/*****************************************************************************/
/**
\defgroup user_functions Functions
\brief Functions to call from user code
\addtogroup user_functions

Example:
\code{.c}
// Assumes user code sets EEPROM_BASEADDR to base address of w3_iic_eeprom 
// core, as set in xparameters.h

int x;
u32 board_sn;
u32 mutex;

// Wait until you lock the EEPROM
//     In a multiple CPU environment where the EEPROM is a shared 
//     peripheral, the EEPROM mutex must be used for the functions:
//         - iic_eeprom_init()
//         - iic_eeprom_write_byte()
//         - iic_eeprom_read_byte()
//     since these functions access the I2C bus.  All other data access 
//     functions are safe to use without the EEPROM mutex because they 
//     only access registers within the EEPROM peripheral.
//
do {
    mutex = iic_eeprom_trylock(EEPROM_BASEADDR, XPAR_CPU_ID);
    
    //
    // Can optionally implement a timeout that will unlock EEPROM.  For 
    // reference, the execution time of iic_eeprom_init() is roughly 15 ms on
    // WARP v3, so any timeout should be much larger than that and greatly 
    // depends on your application.
    //
} while (mutex != IIC_EEPROM_READY);

// Initialize the EEPROM controller
//     - This must be run once before any other EEPROM functions are used
iic_eeprom_init(EEPROM_BASEADDR, 0x64);

// Write a value to the EEPROM (set EEPROM byte address 2345 to 182)
x = iic_eeprom_write_byte(EEPROM_BASEADDR, 2345, 182);
if(x != 0) xil_printf("EEPROM Write Error!\n");

// Read the value back from EEPROM
x = iic_eeprom_read_byte(EEPROM_BASEADDR, 2345);
if(x != 182) xil_printf("EEPROM Read Error (read %d, should be 182)!\n", x);

// Unlock the EEPROM
iic_eeprom_unlock(EEPROM_BASEADDR);

// Read the WARP v3 board serial number from the EEPROM
//    - Does not need to lock the peripheral using the mutex
board_sn = w3_eeprom_read_serial_num(EEPROM_BASEADDDR);
xil_printf("Board s/n: W3-a-%05d\n", board_sn);

\endcode

@{
*/



/*****************************************************************************/
/**
\brief Initializes the EEPROM controller. This function must be called once at 
boot before any EEPROM read/write operations.

\param ba Base memory address of w3_iic_eeprom pcore
\param clk_div Clock divider for IIC clock (set 0x64 for 160MHz bus)
\param id CPU ID for mutex lock

\returns status:  IIC_EEPROM_SUCCESS - EEPROM Initialized
                  IIC_EEPROM_FAILURE - EEPROM not initialized
*/
int iic_eeprom_init(u32 ba, u8 clk_div, u32 id) {
    u32 status;
    u32 tmp;
    u8  addr_tmp[6];
    int mutex;

    // Is the peripheral initialized
    status = Xil_In32(ba + IIC_EEPROM_REG_CONFIG_STATUS);
    
    if (!(status & IIC_EEPROM_REGMASK_INIT)) {

        // EEPROM not initialized, try to get a mutex lock    
        mutex = iic_eeprom_trylock(ba, id);
        
        if (mutex != IIC_EEPROM_READY) {
            // Could not get a mutex lock.  Return failure.  
            //     This condition really means that another CPU is currently 
            //     initializing the EEPROM.  It should be safe to poll on the 
            //     init until it is ready:
            //
            //         while (iic_eeprom_init() != IIC_EEPROM_SUCCESS) { };
            //
            //     since this function will return IIC_EEPROM_SUCCESS immediately 
            //     after being initialized.
            //
            return IIC_EEPROM_FAILURE;
        }
    
        // Check status again
        //     There is a potential race condition if two CPUs are executing this function:
        //         1) CPU A reads status NOT_INIT  (ie enters code above to try to lock EEPROM)
        //         2) CPU B writes status INIT (ie has EEPROM lock and has finished init)
        //         3) CPU B unlocks EEPROM
        //         4) CPU A executes trylock
        //
        //     This would cause CPU A to re-initialize the EEPROM even though CPU B 
        //     has just finished initializing the EEPROM.  Therefore, if we check 
        //     one more time here that the EEPROM is truly not initialized, then
        //     there is no more race condition.
        // 
        status = Xil_In32(ba + IIC_EEPROM_REG_CONFIG_STATUS);
    
        if (status & IIC_EEPROM_REGMASK_INIT) { return IIC_EEPROM_SUCCESS; }   
    
        // Configure the IIC master core
        Xil_Out32((ba + IIC_EEPROM_REG_CMD), 0);
        Xil_Out32((ba + IIC_EEPROM_REG_CONFIG_STATUS), (IIC_EEPROM_REGMASK_CLKDIV & clk_div));
        Xil_Out32((ba + IIC_EEPROM_REG_CONFIG_STATUS), (Xil_In32(ba + IIC_EEPROM_REG_CONFIG_STATUS) | IIC_EEPROM_REGMASK_CORE_EN));

        // Update the cached registers
        //     The serial number should be updated before the MAC addresses because
        //     the MAC addresses potentially use the serial number.
        //
        
        // Serial Number
        tmp = iic_eeprom_read_serial_num(ba);
        Xil_Out32((ba + IIC_EEPROM_REG_SERIAL_NUM), tmp);
        
        // Ethernet A MAC Address
        iic_eeprom_read_eth_addr(ba, 0, addr_tmp);
        
        tmp = ((addr_tmp[3] << 24) | (addr_tmp[2] << 16) | (addr_tmp[1] << 8) | addr_tmp[0]);
        Xil_Out32((ba + IIC_EEPROM_REG_ETH_A_MAC_ADDR_0), tmp);

        tmp = ((addr_tmp[5] << 8) | addr_tmp[4]);
        Xil_Out32((ba + IIC_EEPROM_REG_ETH_A_MAC_ADDR_1), tmp);
        
        // Ethernet B MAC Address
        iic_eeprom_read_eth_addr(ba, 1, addr_tmp);
        
        tmp = ((addr_tmp[3] << 24) | (addr_tmp[2] << 16) | (addr_tmp[1] << 8) | addr_tmp[0]);
        Xil_Out32((ba + IIC_EEPROM_REG_ETH_B_MAC_ADDR_0), tmp);

        tmp = ((addr_tmp[5] << 8) | addr_tmp[4]);
        Xil_Out32((ba + IIC_EEPROM_REG_ETH_B_MAC_ADDR_1), tmp);
        
        // FPGA DNA        
        tmp = iic_eeprom_read_fpga_dna(ba, 0);
        Xil_Out32((ba + IIC_EEPROM_REG_FPGA_DNA_0), tmp);
        
        tmp = iic_eeprom_read_fpga_dna(ba, 1);
        Xil_Out32((ba + IIC_EEPROM_REG_FPGA_DNA_1), tmp);
        
        // Set IIC_EEPROM_REGMASK_INIT bit in status register
        Xil_Out32((ba + IIC_EEPROM_REG_CONFIG_STATUS), (Xil_In32(ba + IIC_EEPROM_REG_CONFIG_STATUS) | IIC_EEPROM_REGMASK_INIT));
        
        // Unlock the EEPROM
        iic_eeprom_unlock(ba);
    }

    return IIC_EEPROM_SUCCESS;    
}


/*****************************************************************************/
/**
\brief Try to lock the EEPROM mutex
\param ba Base memory address of w3_iic_eeprom pcore
\param id ID value to write to the mutex
\return Returns IIC_EEPROM_READY or IIC_EEPROM_LOCKED
*/
int iic_eeprom_trylock(u32 ba, u32 id) {
    u32 mutex_wr_val;
    u32 mutex_rd_val;

    // Set value to write to mutex register
    mutex_wr_val = (IIC_EEPROM_REGMASK_LOCK | id);
    
    // Write mutex register
    Xil_Out32((ba + IIC_EEPROM_REG_MUTEX), mutex_wr_val);

    // Read mutex register
    mutex_rd_val = Xil_In32(ba + IIC_EEPROM_REG_MUTEX);

    if (mutex_wr_val == mutex_rd_val) {
        return IIC_EEPROM_READY;
    } else {
        return IIC_EEPROM_LOCKED;
    }
}



/*****************************************************************************/
/**
\brief Unlock the EEPROM mutex
\param ba Base memory address of w3_iic_eeprom pcore
*/
void iic_eeprom_unlock(u32 ba) {
    // Set mutex register to zero
    Xil_Out32((ba + IIC_EEPROM_REG_MUTEX), 0);
    
    //
    // By only writing 0 to the mutex register once, this allows the mutex
    // register to retain the last ID that was used to lock the mutex (ie the
    // only lock bit is set to zero).  This might be useful for debug in the 
    // future and will not affect subsequent writes to the mutex register.
    //
}



/*****************************************************************************/
/**
\brief Writes one bytes to the EEPROM

This method will return IIC_EEPROM_FAILURE if it fails to obtain a mutex lock.  
It will unlock the mutex after writing the byte.  The calling function must 
check the output value is not IIC_EEPROM_FAILURE before proceeding to write the 
next byte.

NOTE:  Addresses greater than 16000 are reserved for manufacturing information.
    These addresses are not write protected but modifying them can cause issues
    with the behaviour of the node.

\param ba Base memory address of w3_iic_eeprom peripheral
\param addr_to_write Byte address to write, in [0,16000] (addresses > 16000 are reserved)
\param byte_to_write Byte value to write
\return Returns IIC_EEPROM_SUCCESS if EEPROM write succeeds. Returns 
    IIC_EEPROM_FAILURE if an error occurs.
*/
int iic_eeprom_write_byte(u32 ba, u16 addr_to_write, u8 byte_to_write, u32 id)
{
    int status;
    int mutex;
    
    // Try to get a mutex lock
    mutex = iic_eeprom_trylock(ba, id);
        
    if (mutex != IIC_EEPROM_READY) {
        // Could not get a mutex lock.  Return failure.  
        return IIC_EEPROM_FAILURE;
    }

    // Write the byte using the internal method
    status = iic_eeprom_write_byte_internal(ba, addr_to_write, byte_to_write);
    
    // Unlock the EEPROM
    iic_eeprom_unlock(ba);
    
    return status;
}



/*****************************************************************************/
/**
\brief Reads one bytes to the EEPROM

This method will return IIC_EEPROM_FAILURE if it fails to obtain a mutex lock.  
It will unlock the mutex after reading the byte.  The calling function must 
check the output value is not IIC_EEPROM_FAILURE before proceeding to read the 
next byte.

\param ba Base memory address of w3_iic_eeprom peripheral
\param addr_to_read Byte address to read (in [0, 16383])
\return If EEPROM read succeeds, the read byte is returned in the LSB. If an 
    error occurs, returns IIC_EEPROM_FAILURE.
*/
int iic_eeprom_read_byte(u32 ba, u16 addr_to_read, u32 id)
{
    int value;
    int mutex;
    
    // Try to get a mutex lock
    mutex = iic_eeprom_trylock(ba, id);
        
    if (mutex != IIC_EEPROM_READY) {
        // Could not get a mutex lock.  Return failure.  
        return IIC_EEPROM_FAILURE;
    }

    // Read the byte using the internal method
    value = iic_eeprom_read_byte_internal(ba, addr_to_read);
    
    // Unlock the EEPROM
    iic_eeprom_unlock(ba);
    
    return value;
}



/*****************************************************************************/
/**
\brief Reads the WARP v3 board serial number (programmed during manufacturing)
\param ba Base memory address of w3_iic_eeprom pcore
\return Numeric part of board serial number (prefix "W3-a-" not stored in EEPROM)
*/
u32 w3_eeprom_read_serial_num(u32 ba)
{
    u32 status;

    status = Xil_In32(ba + IIC_EEPROM_REG_CONFIG_STATUS);
    
    if (!(status & IIC_EEPROM_REGMASK_INIT)) {
        xil_printf("WARNING: EEPROM not initialized\n");
        return 0;
    }

    return (Xil_In32(ba + IIC_EEPROM_REG_SERIAL_NUM));
}



/*****************************************************************************/
/**
\brief Reads one of the WARP v3 board Ethernet MAC addresses (programmed during manufacturing)
\param ba Base memory address of w3_iic_eeprom pcore
\param addr_sel Selection of Ethernet address to retrieve (0=ETH_A address, 1=ETH_B address)
\param addr_buf Pointer to array of 6 bytes to store retrieved address. This function will overwrite 6 bytes starting at addr_buf.
*/
void w3_eeprom_read_eth_addr(u32 ba, u8 addr_sel, u8* addr_buf)
{
    u32 status;
    u32 tmp_0;
    u32 tmp_1;

    // Check that the cached copy has been initialized
    status = Xil_In32(ba + IIC_EEPROM_REG_CONFIG_STATUS);
    
    if (!(status & IIC_EEPROM_REGMASK_INIT)) {
        xil_printf("WARNING: EEPROM not initialized\n");
        
        // Update the address buffer
        addr_buf[0] = 0;
        addr_buf[1] = 0;
        addr_buf[2] = 0;
        addr_buf[3] = 0;
        addr_buf[4] = 0;
        addr_buf[5] = 0;
        
    } else {
        // Get cached copy of MAC Address
        if (addr_sel == 0) {
            // Ethernet A MAC Address
            tmp_0 = Xil_In32(ba + IIC_EEPROM_REG_ETH_A_MAC_ADDR_0);
            tmp_1 = Xil_In32(ba + IIC_EEPROM_REG_ETH_A_MAC_ADDR_1);
        } else {
            // Ethernet B MAC Address
            tmp_0 = Xil_In32(ba + IIC_EEPROM_REG_ETH_B_MAC_ADDR_0);
            tmp_1 = Xil_In32(ba + IIC_EEPROM_REG_ETH_B_MAC_ADDR_1);
        }    

        // Update the address buffer
        addr_buf[0] = (tmp_0 & 0x000000FF);
        addr_buf[1] = (tmp_0 & 0x0000FF00) >>  8;
        addr_buf[2] = (tmp_0 & 0x00FF0000) >> 16;
        addr_buf[3] = (tmp_0 & 0xFF000000) >> 24;
        addr_buf[4] = (tmp_1 & 0x000000FF);
        addr_buf[5] = (tmp_1 & 0x0000FF00) >>  8;
    }    
}



/*****************************************************************************/
/**
\brief Reads part of the 56-bit Virtex-6 FPGA DNA value (copied to EEPROM during manufacturing)
\param ba Base memory address of w3_iic_eeprom pcore
\param lo_hi Selects between 32 LSB or 24 MSB of DNA value (0=LSB, 1=MSB)
\return Returns selected portion of FPGA DNA value
*/
u32 w3_eeprom_read_fpga_dna(u32 ba, int lo_hi)
{
    u32 status;
    
    // Check that the cached copy has been initialized
    status = Xil_In32(ba + IIC_EEPROM_REG_CONFIG_STATUS);
    
    if (!(status & IIC_EEPROM_REGMASK_INIT)) {
        xil_printf("WARNING: EEPROM not initialized\n");
        return 0;
    }
    
    // Return cached copy of FPGA DNA
    if (lo_hi == 0) {
        return (Xil_In32(ba + IIC_EEPROM_REG_FPGA_DNA_0));
    } else {
        return (Xil_In32(ba + IIC_EEPROM_REG_FPGA_DNA_1));
    }
}

/** @}*/ //END group user_functions

/// @cond EXCLUDE_FROM_DOCS

/***************************** Local Functions *******************************/

/*****************************************************************************/
/**
\brief Reads the WARP v3 board serial number (programmed during manufacturing)
\param ba Base memory address of w3_iic_eeprom pcore
\return Numeric part of board serial number (prefix "W3-a-" not stored in EEPROM)
*/
u32 iic_eeprom_read_serial_num(u32 ba)
{
    int x0, x1, x2;

    x0 = (int) iic_eeprom_read_byte_internal(ba, 16372);
    x1 = (int) iic_eeprom_read_byte_internal(ba, 16373);
    x2 = (int) iic_eeprom_read_byte_internal(ba, 16374);

    return ((x2 << 16) | (x1 << 8) | x0);
}



/*****************************************************************************/
/**
\brief Reads one of the WARP v3 board Ethernet MAC addresses (programmed during manufacturing)
\param ba Base memory address of w3_iic_eeprom pcore
\param addr_sel Selection of Ethernet address to retrieve (0=ETH_A address, 1=ETH_B address)
\param addr_buf Pointer to array of 6 bytes to store retrieved address. This function will overwrite 6 bytes starting at addr_buf.
*/
void iic_eeprom_read_eth_addr(u32 ba, u8 addr_sel, u8* addr_buf)
{
    u8 addr_offset;
    u32 sn;

    addr_offset = addr_sel ? 6 : 0;

    addr_buf[5] = iic_eeprom_read_byte_internal(ba, (16352 + addr_offset));
    addr_buf[4] = iic_eeprom_read_byte_internal(ba, (16353 + addr_offset));
    addr_buf[3] = iic_eeprom_read_byte_internal(ba, (16354 + addr_offset));
    addr_buf[2] = iic_eeprom_read_byte_internal(ba, (16355 + addr_offset));
    addr_buf[1] = iic_eeprom_read_byte_internal(ba, (16356 + addr_offset));
    addr_buf[0] = iic_eeprom_read_byte_internal(ba, (16357 + addr_offset));

    if (((addr_buf[0] == 0x40) && (addr_buf[1] == 0xD8) && (addr_buf[2] == 0x55)) == 0) {
        // EEPROM contains invalid (or no) MAC address
        //     - Use the node serial number to compute a valid address instead
        //     - See http://warpproject.org/trac/wiki/HardwareUsersGuides/WARPv3/Ethernet#MACAddresses
        //
        sn          = 2 * Xil_In32(ba + IIC_EEPROM_REG_SERIAL_NUM);
        
        addr_buf[0] = 0x40;
        addr_buf[1] = 0xD8;
        addr_buf[2] = 0x55;
        addr_buf[3] = 0x04;
        addr_buf[4] = 0x20 + ((sn >> 8) & 0xF);
        addr_buf[5] = 0x00 + (sn & 0xFF) + (addr_sel & 0x1);
    }
    
    // If the first three octets match, then the node has a serial number that does not follow the 
    // serial_number * 2 scheme.  However, we still need to check to make sure that octet [3] and [4]
    // are correct.
    if (addr_buf[3] != 0x04) {                         // addr_buf[3] must be 0x04
        addr_buf[3] = 0x04; 
    }    

    if ((addr_buf[4] & 0xF0) != 0x20) {                // addr_buf[4] must be 0x2X, where X is in [0..F]
        addr_buf[4] = 0x20 | (addr_buf[4] & 0x0F); 
    }
}



/*****************************************************************************/
/**
\brief Reads part of the 56-bit Virtex-6 FPGA DNA value (copied to EEPROM during manufacturing)
\param ba Base memory address of w3_iic_eeprom pcore
\param lo_hi Selects between 32 LSB or 24 MSB of DNA value (0=LSB, 1=MSB)
\return Returns selected portion of FPGA DNA value
*/
u32 iic_eeprom_read_fpga_dna(u32 ba, int lo_hi)
{
    int x0, x1, x2, x3;
    
    if (lo_hi == 0) {
        x0 = (int) iic_eeprom_read_byte_internal(ba, 16376);
        x1 = (int) iic_eeprom_read_byte_internal(ba, 16377);
        x2 = (int) iic_eeprom_read_byte_internal(ba, 16378);
        x3 = (int) iic_eeprom_read_byte_internal(ba, 16379);
        
    } else if (lo_hi == 1) {
        x0 = (int) iic_eeprom_read_byte_internal(ba, 16380);
        x1 = (int) iic_eeprom_read_byte_internal(ba, 16381);
        x2 = (int) iic_eeprom_read_byte_internal(ba, 16382);
        x3 = (int) iic_eeprom_read_byte_internal(ba, 16383);

    } else {
        return 0;
    }

    return ((x3 << 24) | (x2 << 16) | (x1 << 8) | x0);
}



/*****************************************************************************/
/**
\brief Writes one bytes to the EEPROM

\param ba Base memory address of w3_iic_eeprom pcore
\param addr_to_write Byte address to write, in [0,16000] (addresses >16000 are reserved)
\param byte_to_write Byte value to write
\return Returns 0 if EEPROM write succeeds. Returns -1 if an error occurs.
*/
int iic_eeprom_write_byte_internal(u32 ba, u16 addr_to_write, u8 byte_to_write)
{
    int write_done;
    
    /* Process to write 1 byte to random address in IIC EEPROM
        - Write EEPROM control word to Tx register {1 0 1 0 0 0 0 RNW}, RNW=0
            - Assert START and WRITE command bits
            - Poll TIP bit, wait for TIP=0
            - Read RXACK status bit, should be 0
        - Write top 8 bits of target address to Tx register
            - Assert WRITE command bit
            - Poll TIP bit, wait for TIP=0
            - Read RXACK status bit, should be 0
        - Write bottom 8 bits of target address to Tx register
            - Assert WRITE command bit
            - Poll TIP bit, wait for TIP=0
            - Read RXACK status bit, should be 0
        - Write data byte to Tx register
            - Assert STOP and WRITE command bits
            - Poll TIP bit, wait for TIP=0
            - Read RXACK status bit, should be 0
    */

// Protect upper EEPROM bytes    
#if 1
    if(addr_to_write > 16000) {
        xil_printf("ERROR! High bytes read-only by default. Edit %s to override!\n", __FILE__);
        return IIC_EEPROM_FAILURE;
    }
#endif

    Xil_Out32((ba + IIC_EEPROM_REG_TX), (IIC_EEPROM_CONTROL_WORD_WR));
    Xil_Out32((ba + IIC_EEPROM_REG_CMD), (IIC_EEPROM_REGMASK_START | IIC_EEPROM_REGMASK_WRITE));
    if (iic_eeprom_wait_for_rx_ack(ba)) { print("EEPROM: WR Error (1)!\r"); return IIC_EEPROM_FAILURE; }

    Xil_Out32((ba + IIC_EEPROM_REG_TX), ((addr_to_write >> 8) & 0xFF));
    Xil_Out32((ba + IIC_EEPROM_REG_CMD), (IIC_EEPROM_REGMASK_WRITE));
    if (iic_eeprom_wait_for_rx_ack(ba)) { print("EEPROM: WR Error (2)!\r"); return IIC_EEPROM_FAILURE; }

    Xil_Out32((ba + IIC_EEPROM_REG_TX), (addr_to_write & 0xFF));
    Xil_Out32((ba + IIC_EEPROM_REG_CMD), (IIC_EEPROM_REGMASK_WRITE));
    if (iic_eeprom_wait_for_rx_ack(ba)) { print("EEPROM: WR Error (3)!\r"); return IIC_EEPROM_FAILURE; }

    Xil_Out32((ba + IIC_EEPROM_REG_TX), (byte_to_write & 0xFF));
    Xil_Out32((ba + IIC_EEPROM_REG_CMD), (IIC_EEPROM_REGMASK_STOP | IIC_EEPROM_REGMASK_WRITE));
    if (iic_eeprom_wait_for_rx_ack(ba)) { print("EEPROM: WR Error (4)!\r"); return IIC_EEPROM_FAILURE; }

    /* Poll the EEPROM until its internal write cycle is complete
       This is done by:
          - Send START
          - Write control word for write command
         - Check for ACK; no ACK means internal write is still ongoing
    */
    write_done = 0;
    
    while (write_done == 0)
    {
        Xil_Out32((ba + IIC_EEPROM_REG_TX), (IIC_EEPROM_CONTROL_WORD_WR));
        Xil_Out32((ba + IIC_EEPROM_REG_CMD), (IIC_EEPROM_REGMASK_START | IIC_EEPROM_REGMASK_WRITE));
        if (iic_eeprom_wait_for_rx_ack(ba) == 0) { write_done = 1; }
    }

    return IIC_EEPROM_SUCCESS;
}



/*****************************************************************************/
/**
\brief Reads one bytes to the EEPROM

\param ba Base memory address of w3_iic_eeprom pcore
\param addr_to_read Byte address to read (in [0,16383])
\return If EEPROM read succeeds, the read byte is returned in the LSB. If an error occurs, returns -1.
*/
int iic_eeprom_read_byte_internal(u32 ba, u16 addr_to_read)
{
    /* Process to read 1 byte from random address in IIC EEPROM
        - Write EEPROM control word to Tx register {1 0 1 0 0 0 0 RNW}, RNW=0
            - Assert START and WRITE command bits
            - Poll TIP bit, wait for TIP=0
            - Read RXACK status bit, should be 0
        - Write top 8 bits of target address to Tx register
            - Assert WRITE command bit
            - Poll TIP bit, wait for TIP=0
            - Read RXACK status bit, should be 0
        - Write bottom 8 bits of target address to Tx register
            - Assert WRITE command bit
            - Poll TIP bit, wait for TIP=0
            - Read RXACK status bit, should be 0
        - Write EEPROM control word to Tx register {1 0 1 0 0 0 0 RNW}, RNW=1
            - Assert START and WRITE command bits (causes repeat START event)
            - Poll TIP bit, wait for TIP=0
            - Read RXACK status bit, should be 0
        - Assert STOP, READ and ACK command bits
        - Read received byte from rx register
    */

    Xil_Out32((ba + IIC_EEPROM_REG_TX), (IIC_EEPROM_CONTROL_WORD_WR));
    Xil_Out32((ba + IIC_EEPROM_REG_CMD), (IIC_EEPROM_REGMASK_START | IIC_EEPROM_REGMASK_WRITE));
    if (iic_eeprom_wait_for_rx_ack(ba)) { print("EEPROM: RD Error (1)!\r"); return IIC_EEPROM_FAILURE; }

    Xil_Out32((ba + IIC_EEPROM_REG_TX), ((addr_to_read >> 8) & 0xFF));
    Xil_Out32((ba + IIC_EEPROM_REG_CMD), (IIC_EEPROM_REGMASK_WRITE));
    if (iic_eeprom_wait_for_rx_ack(ba)) { print("EEPROM: RD Error (2)!\r"); return IIC_EEPROM_FAILURE; }

    Xil_Out32((ba + IIC_EEPROM_REG_TX), (addr_to_read & 0xFF));
    Xil_Out32((ba + IIC_EEPROM_REG_CMD), (IIC_EEPROM_REGMASK_WRITE));
    if (iic_eeprom_wait_for_rx_ack(ba)) { print("EEPROM: RD Error (3)!\r"); return IIC_EEPROM_FAILURE; }

    Xil_Out32((ba + IIC_EEPROM_REG_TX), (IIC_EEPROM_CONTROL_WORD_RD));
    Xil_Out32((ba + IIC_EEPROM_REG_CMD), (IIC_EEPROM_REGMASK_START | IIC_EEPROM_REGMASK_WRITE));
    if (iic_eeprom_wait_for_rx_ack(ba)) { print("EEPROM: RD Error (4)!\r"); return IIC_EEPROM_FAILURE; }

    Xil_Out32((ba + IIC_EEPROM_REG_CMD), (IIC_EEPROM_REGMASK_STOP | IIC_EEPROM_REGMASK_READ | IIC_EEPROM_REGMASK_ACK));
    while(Xil_In32(ba+IIC_EEPROM_REG_CONFIG_STATUS) & IIC_EEPROM_REGMASK_TIP) {}

    return (Xil_In32(ba + IIC_EEPROM_REG_RX) & 0xFF);
}



/*****************************************************************************/
/**
\brief Wait for receive acknowledgement
\param ba Base memory address of w3_iic_eeprom pcore
\return Returns 0 if IIC bus ACK is detected
*/
inline int iic_eeprom_wait_for_rx_ack(u32 ba)
{
//     xil_printf("Status: 0x%08x CMD: 0x%08x\r", Xil_In32(ba+IIC_EEPROM_REG_CONFIG_STATUS), Xil_In32(ba+IIC_EEPROM_REG_CMD));

    while(Xil_In32(ba+IIC_EEPROM_REG_CONFIG_STATUS) & IIC_EEPROM_REGMASK_TIP) {}

    return (0 != (Xil_In32(ba+IIC_EEPROM_REG_CONFIG_STATUS) & IIC_EEPROM_REGMASK_RXACK));
}


/// @endcond
