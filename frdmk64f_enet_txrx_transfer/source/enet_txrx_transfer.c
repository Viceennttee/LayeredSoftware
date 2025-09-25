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

    /* 1 physical and encryption layers init*/
    int status = aesApi_init();

    /* 2. encrypt desired MSg before sending */
    encryptMsg("refactor worked");

    while (1) {
        /* Receive and decode msgs*/
    	aesApi_receive();

        /*Transmit msgs */
    	aesApi_send();

   }

}
