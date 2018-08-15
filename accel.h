/*
 * accel.h
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

#ifndef SOURCES_ACCEL_H_
#define SOURCES_ACCEL_H_

#include "i2cCom1.h"
#include "myinc.h"
/************************************************************************
 * Accel_Init() puts accelerometer into active mode
 * configures high resolution mode and 4G full scale, puts accel back in active mode
 * returns - nothing
 * ***********************************************************************
 */
void accel_init();
/************************************************************************
 * Accel_WriteRegister() writes value to specified address single byte
 * Parameters - register address, register value
 * returns - operation status success/fail (success=0, fail!=0)
 * ************************************************************************/
uint8_t accel_write_register(uint8_t regAddr, uint8_t regVal);
/************************************************************************
 * Accel_ReadRegister() reads value at specified register address single byte
 * Parameters - register address
 * returns - operation status success/fail (success=0, fail!=0)
 * ************************************************************************/
uint8_t accel_read_register(uint8_t regAddr, uint8_t * readVal);
/************************************************************************
 * Accel_ReadXYZ() function reads XYZ accelerometer data
 * Parameters - buffer to pass the XYZ data , size of the buffer (6 bytes)
 * returns - operation status success/fail (success=0, fail!=0)
 * ************************************************************************/
uint8_t accel_read_xyz(uint8_t * receiveBuff);
/************************************************************************
 * Accel_Stop() puts accelerometer into standby mode
 * returns - nothing
 * ************************************************************************/
void accel_stop();
/************************************************************************
 * Accel_GetAF() function evaluates forward acceleration (AF) of the vehicle
 * Parameters - none
 * returns - current AF value
 * ************************************************************************/
int16_t accel_get_af();
/************************************************************************
 * Accel_SetRef() updates reference values for subtraction of
 * the static tilt
 * Parameters - none
 * returns - nothing
 * ************************************************************************/
void accel_set_ref();

#endif /* SOURCES_ACCEL_H_ */
