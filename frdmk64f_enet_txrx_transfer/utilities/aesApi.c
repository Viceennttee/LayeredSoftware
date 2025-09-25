#include "aesApi.h"
#include "ethApi.h"
////////////parameters for AES//----------------->
static uint8_t key[16] = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};
static uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
struct AES_ctx ctx;
//////////<------------------


/*################ENCRYPTION LAYER------------->*/
int aesApi_init(){

	//aes init
	 AES_init_ctx_iv(&ctx, key,iv);

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
int encryptSendMsg(const char* message){
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

	AES_CBC_encrypt_buffer(&ctx, encryptedMsg, total_length);

	//transport layer protocol,
	uint8_t* encryptedBuffer = ethernet_buildPadding(encryptedMsg, total_length);
	ethernet_sendPadding(encryptedBuffer, total_length);
	free(encryptedBuffer);
	asm("nop");
}
int aesApi_receive(){
	/* TRASNPORT LAYER --------->*/
	uint8_t* ethMsg = ethernet_receive();
	/*<------------- TRASNPORT LAYER*/
	if(ethMsg != NULL){
	    AES_ECB_decrypt(&ctx, ethMsg);
	}
    free(ethMsg);

}

