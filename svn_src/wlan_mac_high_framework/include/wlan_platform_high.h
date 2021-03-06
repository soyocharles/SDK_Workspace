/** @file wlan_platform_high.h
 *  @brief High Platform Defines for 802.11 Ref design
 *
 *  @copyright Copyright 2014-2017, Mango Communications. All rights reserved.
 *          Distributed under the Mango Communications Reference Design License
 *                See LICENSE.txt included in the design archive or
 *                at http://mangocomm.com/802.11/license
 *
 *  This file is part of the Mango 802.11 Reference Design (https://mangocomm.com/802.11)
 */

#ifndef WLAN_PLATFORM_HIGH_H_
#define WLAN_PLATFORM_HIGH_H_

#include "xintc.h"
#include "xaxiethernet.h"
#include "xil_types.h"
#include "wlan_common_types.h"
#include "wlan_high_types.h"

//---------------------------------------
// Platform information struct
typedef struct platform_high_dev_info_t{
	u32		dlmb_baseaddr;
	u32		dlmb_size;
	u32		ilmb_baseaddr;
	u32		ilmb_size;
	u32		aux_bram_baseaddr;
	u32		aux_bram_size;
	u32		dram_baseaddr;
	u32		dram_size;
	u32		intc_dev_id;
	u32		timer_dev_id;
	u32		timer_int_id;
	u32		timer_freq;
	u32		cdma_dev_id;
	u32 	mailbox_int_id;
	u32		wlan_exp_eth_mac_dev_id;
	u32		wlan_exp_eth_dma_dev_id;
	u32		wlan_exp_phy_addr;
} platform_high_dev_info_t;

typedef enum userio_disp_status_t{
	USERIO_DISP_STATUS_IDENTIFY     		= 0,
	USERIO_DISP_STATUS_APPLICATION_ROLE     = 1,
	USERIO_DISP_STATUS_MEMBER_LIST_UPDATE   = 2,
	USERIO_DISP_STATUS_WLAN_EXP_CONFIGURE   = 3,
	USERIO_DISP_STATUS_CPU_ERROR    		= 255
} userio_disp_high_status_t;

//---------------------------------------
// Public functions that WLAN MAC High Framework directly calls
platform_high_dev_info_t wlan_platform_high_get_dev_info();
int wlan_platform_high_init(XIntc* intc);
void wlan_platform_free_queue_entry_notify();
// User IO functions
void wlan_platform_high_userio_disp_status(userio_disp_high_status_t status, ...);

struct cmd_resp;
int wlan_platform_wlan_exp_process_node_cmd(u8* cmd_processed, u32 cmd_id, int socket_index, void* from, struct cmd_resp* command, struct cmd_resp* response, u32 max_resp_len);
int wlan_platform_wlan_exp_eth_init(XAxiEthernet* eth_ptr);

// Functions implemented in files other than wlan_platform_high.c
#if WLAN_SW_CONFIG_ENABLE_ETH_BRIDGE
int wlan_platform_ethernet_send(u8* pkt_ptr, u32 length);
#endif //WLAN_SW_CONFIG_ENABLE_ETH_BRIDGE

#endif /* WLAN_PLATFORM_HIGH_H_ */
