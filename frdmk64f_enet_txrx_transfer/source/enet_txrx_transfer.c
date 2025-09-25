/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include "aesApi.h"

/*******************************************************************************
 * Main Function
 ******************************************************************************/
int main(void)
{

    /* 1 physical and encryption layers*/
    int status = aesApi_init();
    /* 2. encrypt desired MSg before sending */

    //test decoding


    while (1) {
        /* Receive and decode msgs*/
    	aesApi_receive();
    	/*encrypt and send msgs*/
        encryptSendMsg("Vicente");

   }

}
