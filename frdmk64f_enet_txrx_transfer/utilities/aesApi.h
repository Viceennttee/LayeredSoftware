#ifndef AESAPI_H
#define AESAPI_H


#include "aes.h"
#include "aesApi_cfg.h"


/*physical layer initialization*/
int aesApi_init();
int encryptSendMsg(const char* message);
int aesApi_send();
int aesApi_receive();


#endif
