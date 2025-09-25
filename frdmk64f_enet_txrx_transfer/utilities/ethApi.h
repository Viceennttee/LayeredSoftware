#ifndef ETHAPI_H
#define ETHEAPI_H

#include <stdlib.h>
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_enet.h"
#include "fsl_phy.h"
#if defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET
#include "fsl_memory.h"
#endif
#include "fsl_enet_mdio.h"
#include "fsl_phyksz8081.h"
#include "fsl_common.h"
#include "fsl_sysmpu.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_ENET        ENET
#define EXAMPLE_PHY_ADDRESS 0x00U
#define EXAMPLE_MDIO_OPS enet_ops
#define EXAMPLE_PHY_OPS phyksz8081_ops
#define EXAMPLE_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define ENET_RXBD_NUM          (4)
#define ENET_TXBD_NUM          (4)
#define ENET_RXBUFF_SIZE       (ENET_FRAME_MAX_FRAMELEN)
#define ENET_TXBUFF_SIZE       (ENET_FRAME_MAX_FRAMELEN)
#define ENET_DATA_LENGTH       (1000)
#define ENET_TRANSMIT_DATA_NUM (20)

#ifndef MAC_ADDRESS
#define MAC_ADDRESS {0xd4, 0xbe, 0xd9, 0x45, 0x22, 0x60}
#endif
#ifndef MAC_ADDRS_PC
#define MAC_ADDRS_PC {0x00, 0xE0, 0x4C, 0x68, 0x00, 0xCF}
#endif

status_t ethernet_receive_frame(void);
void ethernet_build_frame(const char* message);
status_t ethernet_send_test_frame(void);
status_t ethernet_init_hardware(void);
status_t ethernet_init_phy(void);
status_t ethernet_init_enet(void);

///dinamic buffer
uint8_t* ethernet_buildPadding(uint8_t* message);
status_t ethernet_sendPadding(uint8_t* etheBuffer, uint16_t lentgh);


#endif
