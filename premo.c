/*
 * premo.c
 *
 *  Created on: Jun 2, 2016
 *      Author: B47167
 *
 * (c) Copyright 2016, Freescale Semiconductor, Inc. All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
************************************************************************************/
#include "premo.h"
#include "gpio1.h"
#define SystemClock (0x7D00)
#define HIPIN       (0x01)
#define LOPIN		(0x00)
#define CLK_HI    	(0x08)
#define HALF_BIT	(0x200)   //was 172
#define PAMBL_SIZE 	(0x10)
#define CLK_NUM   	(0x10)
#define BYTE_SIZE   (0x08)
#define MASK_POS    (0x01)
#define MASK_NEG    (0x02)
#define MASK_OFF	(0x00)
#define DIV1        (0x3E8)
#define DIV2        (0x14)
#define MSB         (0x07)
#define LSB         (0x01)
static uint32_t 	portVal;
const uint8_t t9Buffer[18]={0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x00,0x01,0x01,0x00,0x00,0x01,0x01,0x00,0x00,0x01,0x00};// 9T pattern
const uint8_t stopBuffer[4]={0x01,0x01,0x01,0x00};
/************************************************************************
 * premo_init() initializes GPIO port for LF transmitter
 * returns - nothing
 * ************************************************************************/
void premo_init()
{
	//read the port status to OR with the new value in case it needs to be preserved
	portVal=GPIO_HAL_ReadPortOutput(GPIOB_BASE_PTR);
}
/************************************************************************
 * premo_send_packet() encodes and transmits the LF packet over serial GPIO interface
 * Parameters -  buffer containing the message to be Manchester encoded
 * returns - nothing
 * ************************************************************************/
void premo_send_packet(uint8_t * lfMsg)
{
	uint8_t i;
	uint8_t j;
	uint8_t curByte;
	bool curBit;
	for (i=0; i<PAMBL_SIZE; i++)	//preamble
	{
		premo_set_pins(HIPIN);
		premo_set_pins(LOPIN);
	}
	for (i=0; i<sizeof(t9Buffer); i++)
	{
		premo_set_pins(t9Buffer[i]);
	}
	for (i=0; i<sizeof(lfMsg); i++)
	{
		curByte=lfMsg[i];
		for(j=0; j<BYTE_SIZE; j++)
		{
			curBit=curByte&(LSB<<(MSB-j));
			premo_set_pins(!curBit);
			premo_set_pins(curBit);
		}
	}
	for (i=0; i<sizeof(stopBuffer); i++)
	{
		premo_set_pins(stopBuffer[i]);
	}
}
/************************************************************************
 * premo_set_pins() sets serial data lines and holds the value for half bit duration
 * Parameters -  data line state
 * returns - nothing
 * ************************************************************************/
void premo_set_pins(bool setVal)
{
	if(setVal)
	{

		for(uint8_t cntClk=0;cntClk<CLK_NUM;cntClk++)
		{
			GPIO_HAL_WritePortOutput(GPIOB_BASE_PTR,MASK_POS);
			premo_delay_us(CLK_HI);
			GPIO_HAL_WritePortOutput(GPIOB_BASE_PTR,MASK_NEG);
			premo_delay_us(CLK_HI);
		}
		GPIO_HAL_WritePortOutput(GPIOB_BASE_PTR,MASK_OFF);
	}
	else
	{
		GPIO_HAL_WritePortOutput(GPIOB_BASE_PTR,MASK_OFF);
		premo_delay_us(HALF_BIT);//half bit duration in us
	}

}

/************************************************************************************
* premo_delay_us - delays execution for specified time in microseconds
* Parameter- delay time
* Returns - nothing
************************************************************************************/
void premo_delay_us(uint16_t u16us)
{
  uint32_t u32ctr;
  for(u32ctr = 0; u32ctr < (((uint32_t)SystemClock / (uint32_t)DIV1 / (uint32_t)DIV2) * u16us); u32ctr++)
  {
    __asm ("NOP");
  }
}

/* LF_PREMO.c
 *
 *  Created on: Apr 25, 2016
 *      Author: B47167
 */




