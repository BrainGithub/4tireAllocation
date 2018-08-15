/*
 * gyro.h
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

#ifndef SOURCES_GYRO_H_
#define SOURCES_GYRO_H_
#include "i2cCom1.h"
#include "myinc.h"
/************************************************************************
 * Gyro_Init() puts gyro into active mode
 * returns - nothing
 * ************************************************************************/
void gyro_init();
/************************************************************************
 * Gyro_WriteRegister() writes value to specified address single byte
 * Parameters - register address, register value
 * returns - operation status (0 if success otherwise KSDK status value)
 * ************************************************************************/
uint8_t gyro_write_register(uint8_t regAddr, uint8_t regVal);
/************************************************************************
 * Gyro_ReadRegister() reads value at specified register address single byte
 * Parameters - register address
 * returns - operation status (0 if success otherwise KSDK status value)
 * ************************************************************************/
uint8_t gyro_read_register(uint8_t regAddr, uint8_t * regVal);
/************************************************************************
 * Gyro_ReadXYZ() function reads XYZ gyro data
 * Parameters - buffer to pass the XYZ data, buffer size (6 bytes)
 * returns - operation status (0 if success otherwise KSDK status value)
 * ************************************************************************/
uint8_t gyro_read_xyz(uint8_t * receiveBuff);
/************************************************************************
 * Gyro_Stop() puts gyro into idle mode
 * returns - nothing
 * ************************************************************************/
void gyro_stop();
/************************************************************************
 * Gyro_getTurn() reads Z Axis rotation rate
 * returns - Z axis dps value
 * ************************************************************************/
int16_t gyro_get_turn();
#endif /* SOURCES_GYRO_H_ */
