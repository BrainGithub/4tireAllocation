/*
* LF_PREMO.h
*
*  Created on: Apr 25, 2016
*      Author: B47167
* (c) Copyright 2016, Freescale Semiconductor, Inc. All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
************************************************************************************/
#ifndef SOURCES_PREMO_H_
#define SOURCES_PREMO_H_
#include <stdio.h>
/************************************************************************
 * premo_send_packet() encodes and transmits the LF packet over serial GPIO interface
 * Parameters -  buffer containing the message to be Manchester encoded
 * returns - nothing
 * ************************************************************************/
void premo_send_packet(uint8_t * lfMsg);
/************************************************************************
 * premo_set_pins() sets serial data lines and holds the value for half bit duration
 * Parameters -  data line state
 * returns - nothing
 * ************************************************************************/
void premo_set_pins(bool setVal);
/************************************************************************************
* premo_delay_us - delays execution for specified time in microseconds
* Parameter- delay time
* Returns - nothing
************************************************************************************/
void premo_delay_us(uint16_t u16us);
/************************************************************************************
* premo_delay_msdelays execution for specified time in milliseconds
* Parameter- delay time
* Returns - nothing
*************************************************************************************/
void premo_delay_ms(uint16_t u16ms);
/************************************************************************
 *  premo_init() initializes GPIO port for LF transmitter
 * returns - nothing
 * ************************************************************************/
void premo_init(void);
#endif /* SOURCES_PREMO_H_ */
