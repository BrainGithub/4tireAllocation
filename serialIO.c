/*
 * serialIO.c
 *
 *  Created on: Jun 16, 2016
 *      Author: B47167
 (c) Copyright 2016, Freescale Semiconductor, Inc. All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
************************************************************************************/
#include "serialIO.h"
#include "comTypes.h"
#include "myinc.h"

#define TXSINGLE 		(0x01) //length of a single byte transmission

#define SIOFILEI "./sio_simm_i"
#define SIOFILEO "./sio_simm_o"
int readsio_line(char *buf, int len, const char *file)
{
	int n = 0;
	if (file == 0 || strlen(file) == 0)
		return -1;

	FILE *fd = fopen(file, "r");
	if (!fd)
		return -2;
	
	fgets(buf, len, fd);
	fclose(fd);
	return 0;
}


/**********************************************************************
 * send_msg(uint8_t * msgBuff, uint8_t msgSize)
 * function sends message over UART using non-blocking method
 * msgBuff - array containing the data to send
 * msgSize  - length of the message to send
 * returns - nothing
 * **********************************************************************/
void sio_send_msg(uint8_t * msgBuff, uint8_t msgSize)
{
	//LPSCI_DRV_SendData(FSL_LPSCICOM1, msgBuff, msgSize);
	
	//RAISE_EXP_unimpl;
	printf("sio_send_msg:<");
	int i=0;
	for (i=0; i<msgSize; i++)
		printf("%x ", msgBuff[i]);
	
	printf(">\n");
}
/**********************************************************************
 * send_rsp(uint8_t * msgBuff, uint8_t msgSize)
 * function sends message over UART using blocking method
 * msgBuff - array containing the data to send
 * msgSize  - length of the message to send
 * returns - nothing
 * **********************************************************************/
void sio_send_rsp(uint8_t * msgBuff, uint8_t msgSize)
{
	//LPSCI_DRV_SendDataBlocking(FSL_LPSCICOM1, msgBuff, msgSize,TOUT_1K_MS);
	
	//RAISE_EXP_unimpl;
	printf("sio_send_rsp:<");
        int i=0;
        for (i=0; i<msgSize; i++)
                printf("%x ", msgBuff[i]);

        printf(">\n");

}
/**********************************************************************
 * send_char(uint8_t msgVal)
 * function sends single character over UART using blocking method
 * returns - nothing
 * **********************************************************************/
void sio_send_char(uint8_t msgVal)
{
	//LPSCI_DRV_SendDataBlocking(FSL_LPSCICOM1, &msgVal, TXSINGLE,TOUT_5K_MS);
	//RAISE_EXP_unimpl;
	printf("sio_send_char:<");

        printf("%x", msgVal);

        printf(">\n");
}

/**********************************************************************
 * get_char()
 * function receives single character over UART using blocking method
 * if hyperterminal is used it echoes the input
 * returns - Host PC input
 * **********************************************************************/
uint8_t sio_get_char()
{
	uint8_t res;
	//LPSCI_DRV_ReceiveDataBlocking(FSL_LPSCICOM1, &res, TXSINGLE, TOUT_5K_MS);
#ifdef TERMINAL
	//send_char(res);
#endif
	
	//RAISE_EXP_unimpl;
	char tmp[128];
	tmp[0] = 0;
	if (0 == readsio_line(tmp, 128, SIOFILEI))
		system("rm "SIOFILEI);

	printf("gets from sio file:<%s>\n", tmp);
	res = tmp[0];
	return res;
}

void sio_get_str(char *buf, int len)
{
	if (0 == readsio_line(buf, len, SIOFILEI))
		system("rm "SIOFILEI);	
}

/**********************************************************************
 * get_input()
 * function receives single character over UART using blocking method
 * if hyperterminal is used it echoes the input back to UART
 * CR terminates the input string input is limited to maximum 2 characters
 * returns - input ASCII string converted to hex format
 * **********************************************************************/
uint8_t sio_get_input()
{
/*
	uint8_t res;
	uint8_t resVal=0;
	while(1)
	{
		LPSCI_DRV_ReceiveDataBlocking(FSL_LPSCICOM1, &res, TXSINGLE, TOUT_5K_MS);
		if (res==0x0D) //CR terminated input
		{
			return resVal;
		}
#ifdef TERMINAL
		sio_send_char(res); //echo
#endif
		if (res<0x41)  // process the ASCII character
		{
			res &=(0x0Fu);
		}
		else
		{
			res=res-(0x37);
		}
		resVal=(resVal<<4);
		resVal|=res;
	}
*/

	RAISE_EXP_unimpl;
}
/**********************************************************************
 * sio_get_stop_char()
 * function checks for the arrival of a stop command (any character) from PC
 * Parameters - none
 * returns - 0 if no command, 1 if stop command was received
 * **********************************************************************/
uint8_t sio_get_stop_char()
{
/*
	uint32_t nBytes;
	lpsci_status_t kStatus;
	kStatus=LPSCI_DRV_GetReceiveStatus(FSL_LPSCICOM1, &nBytes);
	if(kStatus==kStatus_LPSCI_Success)
	{
		LPSCI_DRV_AbortReceivingData(FSL_LPSCICOM1);
		LPSCI_DRV_AbortSendingData(FSL_LPSCICOM1);
		return statErr;
	}
*/
	RAISE_EXP_unimpl;
	return statOK;
}
/**********************************************************************
 * sio_init_stop_char()
 * function starts non blocking read operation waiting for PC command
 * Parameters- none
 * returns - nothing
 * **********************************************************************/
void sio_init_stop_char()
{
	uint8_t  readVal;
	//LPSCI_DRV_ReceiveData(FSL_LPSCICOM1, &readVal, sizeof(readVal));
	RAISE_EXP_unimpl;
}
/**********************************************************************
 * sio_end_stop_char()
 * function aborts non blocking read operation
 * Parameters- none
 * returns - nothing
 * **********************************************************************/
void sio_end_stop_char()
{
	//LPSCI_DRV_AbortReceivingData(FSL_LPSCICOM1);
	//LPSCI_DRV_AbortSendingData(FSL_LPSCICOM1);
	RAISE_EXP_unimpl;
}
