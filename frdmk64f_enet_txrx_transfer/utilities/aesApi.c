#include "aesApi.h"
#include "ethApi.h"
////////////parameters for AES//----------------->
static uint8_t key[16] = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};
struct AES_ctx ctx;
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
    int message_len = strlen(message);
    //total length with padding
    int padding = 16 - (message_len % 16);
    int total_length = message_len + padding;


    uint8_t encryptedMsg[total_length];
    memcpy(encryptedMsg, message, message_len);  //copy data from const

    //add KCS7 padding
    for (int i = message_len; i < total_length; i++) {
    	encryptedMsg[i] = (uint8_t)padding;
    }

	AES_ECB_encrypt(&ctx, encryptedMsg);

	//transport layer protocol,
	ethernet_build_frame(encryptedMsg);
	//ethernet_build_frame(message);
}
int aesApi_receive(){
	ethernet_receive_frame();
	uint8_t testMsg[] = {81,101,217,138,39,57,169,99,14,220,240,224,214,29,145,75};
    AES_ECB_decrypt(&ctx, testMsg);
    asm("nop");

}
int aesApi_send() {
	uint32_t testTxNum = 0;
	if (ethernet_send_test_frame() == kStatus_Success) {
        PRINTF("The %d frame transmitted success!\r\n", testTxNum);
        testTxNum++;
		SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

}
}

