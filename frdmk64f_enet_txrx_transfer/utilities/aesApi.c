#include "aesApi.h"
#include "ethApi.h"

/*################ENCRYPTION LAYER------------->*/
int aesApi_init(){

	//aes init
	 AES_init_ctx_iv(&ctx_cfg, key_cfg,iv_cfg);

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

	AES_CBC_encrypt_buffer(&ctx_cfg, encryptedMsg, total_length);

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
	    AES_ECB_decrypt(&ctx_cfg, ethMsg);
	}
    free(ethMsg);

}

