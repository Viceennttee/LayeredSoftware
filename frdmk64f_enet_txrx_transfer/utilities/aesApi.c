#include "aesApi.h"
#include "ethApi.h"
////////////parameters for AES//----------------->
static uint8_t key[16] = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};
struct AES_ctx ctx;

uint8_t mensaje_ecb[16] = "HolaECB12345678";
uint8_t cifrado_ecb[16];
uint8_t descifrado_ecb[16];
//////////<------------------


/*################ENCRYPTION LAYER------------->*/
int aesApi_init(){

	//aes init
	 AES_init_ctx(&ctx, key);

    /* 1. this whole init could be contained in a single ethInit function
     * making static fucntions that are just called by ethInit
     * */
    ethernet_init_hardware();

    if (ethernet_init_phy() != kStatus_Success) {
        PRINTF("PHY initialization failed!\r\n");
        return -1;
    }

    ethernet_init_enet();
}
int encryptMsg(const char* message){
	//AES_ECB_encrypt(&ctx, message);
	ethernet_build_frame(message);
}
int aesApi_receive(){
	ethernet_receive_frame();
}
int aesApi_send() {
	uint32_t testTxNum = 0;
    /* Transmisión - solo las primeras veces */
    if (testTxNum < ENET_TRANSMIT_DATA_NUM) {
        if (ethernet_send_test_frame() == kStatus_Success) {
            testTxNum++;
            PRINTF("The %d frame transmitted success!\r\n", testTxNum);
            SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        }

}
}

