#include "aesApi.h"

/*variables used by static functions*/
static mdio_handle_t mdioHandle = {.ops = &EXAMPLE_MDIO_OPS};
static phy_handle_t phyHandle   = {.phyAddr = EXAMPLE_PHY_ADDRESS, .mdioHandle = &mdioHandle, .ops = &EXAMPLE_PHY_OPS};
/*variables used by static functions*/
AT_NONCACHEABLE_SECTION_ALIGN(enet_rx_bd_struct_t g_rxBuffDescrip[ENET_RXBD_NUM], ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(enet_tx_bd_struct_t g_txBuffDescrip[ENET_TXBD_NUM], ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t g_rxDataBuff[ENET_RXBD_NUM][SDK_SIZEALIGN(ENET_RXBUFF_SIZE, ENET_BUFF_ALIGNMENT)], ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t g_txDataBuff[ENET_TXBD_NUM][SDK_SIZEALIGN(ENET_TXBUFF_SIZE, ENET_BUFF_ALIGNMENT)], ENET_BUFF_ALIGNMENT);

enet_handle_t g_handle;
uint8_t g_frame[ENET_DATA_LENGTH + 14];
uint8_t g_macAddr[6] = MAC_ADDRESS;
uint8_t g_PCMacAddr[6] = MAC_ADDRS_PC;

uint32_t testTxNum = 0;




/*##################HARDWARE LAYER******------>*/
static status_t ethernet_init_hardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    SYSMPU_Enable(SYSMPU, false);
    PRINTF("\r\nENET example start.\r\n");
    return kStatus_Success;
}
/* PHY init*/
static status_t ethernet_init_phy(void)
{
    phy_config_t phyConfig = {0};
    bool link = false, autonego = false;
    uint32_t count = 0;
    status_t status;

    phyConfig.phyAddr = EXAMPLE_PHY_ADDRESS;
    phyConfig.autoNeg = true;
    mdioHandle.resource.base = EXAMPLE_ENET;
    mdioHandle.resource.csrClock_Hz = EXAMPLE_CLOCK_FREQ;

    PRINTF("Wait for PHY init...\r\n");

    do {
        status = PHY_Init(&phyHandle, &phyConfig);
        if (status == kStatus_Success) {
            PRINTF("Wait for PHY link up...\r\n");

            count = 100000;
            do {
                PHY_GetAutoNegotiationStatus(&phyHandle, &autonego);
                PHY_GetLinkStatus(&phyHandle, &link);
                if (autonego && link) break;
            } while (--count);

            if (!autonego) {
                PRINTF("PHY Auto-negotiation failed.\r\n");
            }
        }
    } while (!(link && autonego));

    return (link && autonego) ? kStatus_Success : kStatus_Fail;
}
/* ENET init */
static status_t ethernet_init_enet(void)
{
    enet_config_t config;
    phy_speed_t speed;
    phy_duplex_t duplex;

    /* Preparar configuración de buffers */
    enet_buffer_config_t buffConfig[] = {{
        ENET_RXBD_NUM,
        ENET_TXBD_NUM,
        SDK_SIZEALIGN(ENET_RXBUFF_SIZE, ENET_BUFF_ALIGNMENT),
        SDK_SIZEALIGN(ENET_TXBUFF_SIZE, ENET_BUFF_ALIGNMENT),
        &g_rxBuffDescrip[0],
        &g_txBuffDescrip[0],
        &g_rxDataBuff[0][0],
        &g_txDataBuff[0][0],
        true,
        true,
        NULL,
    }};

    ENET_GetDefaultConfig(&config);
    config.miiMode = kENET_RmiiMode;

    /* Obtener velocidad y duplex reales del PHY */
    PHY_GetLinkSpeedDuplex(&phyHandle, &speed, &duplex);
    config.miiSpeed = (enet_mii_speed_t)speed;
    config.miiDuplex = (enet_mii_duplex_t)duplex;

    /* Inicializar ENET */
    ENET_Init(EXAMPLE_ENET, &g_handle, &config, &buffConfig[0], &g_macAddr[0], EXAMPLE_CLOCK_FREQ);
    ENET_ActiveRead(EXAMPLE_ENET);

    return kStatus_Success;
}



/*build frame*/
void ethernet_build_frame(const char* message)
{
    uint32_t count = 0;
    uint32_t length = ENET_DATA_LENGTH - 14;
    uint32_t mensaje_len = strlen(message);

    /* Cabecera Ethernet */
    memcpy(&g_frame[0], &g_PCMacAddr[0], 6U);
    memcpy(&g_frame[6], &g_macAddr[0], 6U);
    g_frame[12] = (length >> 8) & 0xFFU;
    g_frame[13] = length & 0xFFU;
    ///AES128 encrypting

    ///AES128 encrypting
    /* msg data*/
    for (count = 0; count < length; count++) {
        g_frame[count + 14] = message[count % mensaje_len];
    }
}

status_t ethernet_send_test_frame(void)
{
    bool link = false;

    if (kStatus_Success == PHY_GetLinkStatus(&phyHandle, &link)) {
        if (link) {
            if (kStatus_Success == ENET_SendFrame(EXAMPLE_ENET, &g_handle, &g_frame[0], ENET_DATA_LENGTH, 0, false, NULL)) {
                return kStatus_Success;
            }
        }
    }
    return kStatus_Fail;
}

static void ethernet_process_received_data(uint8_t* data, uint32_t length)
{
    PRINTF(" A frame received. the length %d ", length);
    PRINTF(" Dest Address %02x:%02x:%02x:%02x:%02x:%02x Src Address %02x:%02x:%02x:%02x:%02x:%02x \r\n",
           data[0], data[1], data[2], data[3], data[4], data[5],
           data[6], data[7], data[8], data[9], data[10], data[11]);

    /*AES decoding*/

    /*AES decoding*/


}
status_t ethernet_receive_frame(void)
{
    uint32_t length = 0;
    status_t status;
    enet_data_error_stats_t eErrStatic;

    status = ENET_GetRxFrameSize(&g_handle, &length, 0);

    if (length != 0) {
        uint8_t *data = (uint8_t *)malloc(length);
        status = ENET_ReadFrame(EXAMPLE_ENET, &g_handle, data, length, 0, NULL);
        if (status == kStatus_Success) {
            ethernet_process_received_data(data, length);
        }
        free(data);
        return status;
    }
    else if (status == kStatus_ENET_RxFrameError) {
        ENET_GetRxErrBeforeReadFrame(&g_handle, &eErrStatic, 0);
        ENET_ReadFrame(EXAMPLE_ENET, &g_handle, NULL, 0, 0, NULL);
    }

    return status;
}
/*<----------####HARDWARE LAYER******************/



/*################ENCRYPTION LAYER------------->*/
int aesApi_init(){
    /* hardwareInit*/
    ethernet_init_hardware();

    if (ethernet_init_phy() != kStatus_Success) {
        PRINTF("PHY initialization failed!\r\n");
        return -1;
    }

    //ethernet_init_enet();
}
int encryptMsg(const char* message){
	ethernet_build_frame(message);
}
int aesApi_receive(){
	ethernet_receive_frame();
}
int aesApi_send() {
    /* Transmit a few times*/
    if (testTxNum < ENET_TRANSMIT_DATA_NUM) {
        if (ethernet_send_test_frame() == kStatus_Success) {
            testTxNum++;
            PRINTF("The %d frame transmitted success!\r\n", testTxNum);
            SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        }

}
}

