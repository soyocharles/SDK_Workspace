/*****************************************************************
* File: w3_iic_eeprom.h
* Copyright (c) 2012 Mango Communications, all rights reseved
* Released under the WARP License
* See http://warp.rice.edu/license for details
*****************************************************************/

/// @cond EXCLUDE_FROM_DOCS
// User code never uses the #define's from this header, so exclude them from the API docs

#ifndef W3_IIC_EEPROM_H
#define W3_IIC_EEPROM_H

#include "xil_io.h"

/*************************** Constant Definitions ****************************/

// --------------------------------------------------------
// Address offset for each slave register; exclude from docs, as users never use these directly
//
#define W3_IIC_EEPROM_USER_SLV_SPACE_OFFSET               (0x00000000)
#define W3_IIC_EEPROM_SLV_REG_00_OFFSET                   (W3_IIC_EEPROM_USER_SLV_SPACE_OFFSET + 0x00000000)
#define W3_IIC_EEPROM_SLV_REG_01_OFFSET                   (W3_IIC_EEPROM_USER_SLV_SPACE_OFFSET + 0x00000004)
#define W3_IIC_EEPROM_SLV_REG_02_OFFSET                   (W3_IIC_EEPROM_USER_SLV_SPACE_OFFSET + 0x00000008)
#define W3_IIC_EEPROM_SLV_REG_03_OFFSET                   (W3_IIC_EEPROM_USER_SLV_SPACE_OFFSET + 0x0000000C)
#define W3_IIC_EEPROM_SLV_REG_04_OFFSET                   (W3_IIC_EEPROM_USER_SLV_SPACE_OFFSET + 0x00000010)
#define W3_IIC_EEPROM_SLV_REG_05_OFFSET                   (W3_IIC_EEPROM_USER_SLV_SPACE_OFFSET + 0x00000014)
#define W3_IIC_EEPROM_SLV_REG_06_OFFSET                   (W3_IIC_EEPROM_USER_SLV_SPACE_OFFSET + 0x00000018)
#define W3_IIC_EEPROM_SLV_REG_07_OFFSET                   (W3_IIC_EEPROM_USER_SLV_SPACE_OFFSET + 0x0000001C)
#define W3_IIC_EEPROM_SLV_REG_08_OFFSET                   (W3_IIC_EEPROM_USER_SLV_SPACE_OFFSET + 0x00000020)
#define W3_IIC_EEPROM_SLV_REG_09_OFFSET                   (W3_IIC_EEPROM_USER_SLV_SPACE_OFFSET + 0x00000024)
#define W3_IIC_EEPROM_SLV_REG_10_OFFSET                   (W3_IIC_EEPROM_USER_SLV_SPACE_OFFSET + 0x00000028)
#define W3_IIC_EEPROM_SLV_REG_11_OFFSET                   (W3_IIC_EEPROM_USER_SLV_SPACE_OFFSET + 0x0000002C)
#define W3_IIC_EEPROM_SLV_REG_12_OFFSET                   (W3_IIC_EEPROM_USER_SLV_SPACE_OFFSET + 0x00000030)
#define W3_IIC_EEPROM_SLV_REG_13_OFFSET                   (W3_IIC_EEPROM_USER_SLV_SPACE_OFFSET + 0x00000034)
#define W3_IIC_EEPROM_SLV_REG_14_OFFSET                   (W3_IIC_EEPROM_USER_SLV_SPACE_OFFSET + 0x00000038)
#define W3_IIC_EEPROM_SLV_REG_15_OFFSET                   (W3_IIC_EEPROM_USER_SLV_SPACE_OFFSET + 0x0000003C)

// --------------------------------------------------------
// Register common names
//
#define IIC_EEPROM_REG_CONFIG_STATUS                       W3_IIC_EEPROM_SLV_REG_00_OFFSET
#define IIC_EEPROM_REG_CMD                                 W3_IIC_EEPROM_SLV_REG_01_OFFSET
#define IIC_EEPROM_REG_TX                                  W3_IIC_EEPROM_SLV_REG_02_OFFSET
#define IIC_EEPROM_REG_RX                                  W3_IIC_EEPROM_SLV_REG_03_OFFSET
#define IIC_EEPROM_REG_MUTEX                               W3_IIC_EEPROM_SLV_REG_04_OFFSET
#define IIC_EEPROM_REG_SERIAL_NUM                          W3_IIC_EEPROM_SLV_REG_05_OFFSET
#define IIC_EEPROM_REG_ETH_A_MAC_ADDR_0                    W3_IIC_EEPROM_SLV_REG_06_OFFSET
#define IIC_EEPROM_REG_ETH_A_MAC_ADDR_1                    W3_IIC_EEPROM_SLV_REG_07_OFFSET
#define IIC_EEPROM_REG_ETH_B_MAC_ADDR_0                    W3_IIC_EEPROM_SLV_REG_08_OFFSET
#define IIC_EEPROM_REG_ETH_B_MAC_ADDR_1                    W3_IIC_EEPROM_SLV_REG_09_OFFSET
#define IIC_EEPROM_REG_FPGA_DNA_0                          W3_IIC_EEPROM_SLV_REG_10_OFFSET
#define IIC_EEPROM_REG_FPGA_DNA_1                          W3_IIC_EEPROM_SLV_REG_11_OFFSET

// --------------------------------------------------------
// Masks for config/status register
//
#define IIC_EEPROM_REGMASK_CLKDIV                          0x000000FF
#define IIC_EEPROM_REGMASK_CORE_EN                         0x00000100
#define IIC_EEPROM_REGMASK_RXACK                           0x00010000
#define IIC_EEPROM_REGMASK_BUSY                            0x00020000
#define IIC_EEPROM_REGMASK_AL                              0x00040000
#define IIC_EEPROM_REGMASK_TIP                             0x00080000
#define IIC_EEPROM_REGMASK_INIT                            0x80000000

// --------------------------------------------------------
// Masks for command register
//
#define IIC_EEPROM_REGMASK_START                           0x00000001
#define IIC_EEPROM_REGMASK_STOP                            0x00000002
#define IIC_EEPROM_REGMASK_READ                            0x00000004
#define IIC_EEPROM_REGMASK_WRITE                           0x00000008
#define IIC_EEPROM_REGMASK_ACK                             0x00000010

// --------------------------------------------------------
// Masks for mutex register
//
#define IIC_EEPROM_REGMASK_LOCK                            0x80000000

#define IIC_EEPROM_READY                                   0
#define IIC_EEPROM_LOCKED                                  1

// --------------------------------------------------------
// Control Words
//
#define IIC_EEPROM_CONTROL_WORD_RD 	                       0xA1
#define IIC_EEPROM_CONTROL_WORD_WR 	                       0xA0


// --------------------------------------------------------
// Misc
//
#define IIC_EEPROM_SUCCESS                                 0
#define IIC_EEPROM_FAILURE                                 -1


// --------------------------------------------------------
// Macros (for backward compatibility)
//
#define iic_eeprom_readByte(ba, addr)                      iic_eeprom_read_byte(ba, addr, XPAR_CPU_ID)
#define iic_eeprom_writeByte(ba, addr, val)                iic_eeprom_write_byte(ba, addr, val, XPAR_CPU_ID)
#define w3_eeprom_readSerialNum(ba)                        w3_eeprom_read_serial_num(ba)
#define w3_eeprom_readEthAddr(ba, sel, buf)                w3_eeprom_read_eth_addr(ba, sel, buf)



/*************************** Function Prototypes *****************************/

// EEPROM functions
int           iic_eeprom_init(u32 ba, u8 clk_div, u32 id);

int           iic_eeprom_trylock(u32 ba, u32 id);
void          iic_eeprom_unlock(u32 ba);

int           iic_eeprom_write_byte(u32 ba, u16 addr_to_write, u8 byte_to_write, u32 id);
int           iic_eeprom_read_byte(u32 ba, u16 addr_to_read, u32 id);

// Data access functions
u32           w3_eeprom_read_serial_num(u32 ba);
u32           w3_eeprom_read_fpga_dna(u32 ba, int lo_hi);
void          w3_eeprom_read_eth_addr(u32 ba, u8 addr_sel, u8* addr_buf);

#endif /** W3_IIC_EEPROM_H */
/// @endcond
