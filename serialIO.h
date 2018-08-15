/*
 * serialIO.h
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
//zxp
//#include "uart1.h"
//#include "lpsciCom1.h"
#include "myinc.h"

#ifndef SOURCES_SERIALIO_H_
#define SOURCES_SERIALIO_H_
/**********************************************************************
 * send_msg(uint8_t * msgBuff, uint8_t msgSize)
 * function sends message over UART using non-blocking method
 * msgBuff - array containing the data to send
 * msgSize  - length of the message to send
 * returns - nothing
 * **********************************************************************/
void sio_send_msg(uint8_t * msgBuff, uint8_t msgSize);
/**********************************************************************
 * send_rsp(uint8_t * msgBuff, uint8_t msgSize)
 * function sends message over UART using blocking method
 * msgBuff - array containing the data to send
 * msgSize  - length of the message to send
 * returns - nothing
 * **********************************************************************/
void sio_send_rsp(uint8_t * msgBuff, uint8_t msgSize);
/**********************************************************************
 * send_char(uint8_t msgVal)
 * function sends single character over UART using blocking method
 * returns - nothing
 * **********************************************************************/
void sio_send_char(uint8_t msgVal);
/**********************************************************************
 * send_val(uint8_t msgVal)
 * function sends single hex value over UART using blocking method if hyperterminal is used
 * function sends formatted message if LV GUI is used
 * returns - nothing
 * *********************************************************************/
uint8_t sio_get_char();
/**********************************************************************
 * get_input()
 * function receives single character over UART using blocking method
 * if hyperterminal is used it echoes the input back to UART
 * CR terminates the input string input is limited to maximum 2 characters
 * returns - input ASCII string converted to hex format
 * **********************************************************************/
uint8_t sio_get_input();
/**********************************************************************
 * sio_get_stop_char()
 * function checks for the arrival of a stop command (any character) from PC
 * Parameters - none
 * returns - 0 if no command, 1 if stop command was received
 * **********************************************************************/
uint8_t sio_get_stop_char();
/**********************************************************************
 * sio_init_stop_char()
 * function starts non blocking read operation waiting for PC command
 * Parameters- none
 * returns - nothing
 * **********************************************************************/
void sio_init_stop_char();
/**********************************************************************
 * sio_end_stop_char()
 * function aborts non blocking read operation
 * Parameters- none
 * returns - nothing
 * **********************************************************************/
void sio_end_stop_char();

//add by zxp sim
int readsio_line(char *buf, int len, const char *file);
void sio_get_str(char *buf, int len);
#endif /* SOURCES_SERIALIO_H_ */
