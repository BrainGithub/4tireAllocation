/*
 * gyro.c
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
#include "gyro.h"
#include "comTypes.h"
#include "myinc.h"
#define REGDATA			(0x01)	//address of data register
//defines for ctrl_reg1
#define REGCTRL1 		(0x13)  // address of control register
#define STBY_MODE    	(0x00)  // set device in standby
#define ACTIVE_MODE     (0x02)  // set device active
#define READY_MODE      (0x01)  // set device ready
#define ODR_25_HZ		(0x14)  // output data rate 25Hz
//****************************************************
//defines for ctrl_reg0
#define REGCTRL0 		(0x0D)  // address of reg control zero register
#define FS_250_DPS		(0x03)  // set full sacle +-250 degrees per seconf
//*****************************************************
/************************************************************************
 * Gyro_Init() puts gyro into active mode
 * returns - nothing
 * ************************************************************************/
void gyro_init()
{
	gyro_write_register((uint8_t) REGCTRL1,(uint8_t) STBY_MODE); //standby mode
	gyro_write_register((uint8_t) REGCTRL0,(uint8_t) FS_250_DPS);// set full scale 250dps
	gyro_write_register((uint8_t) REGCTRL1,(uint8_t) (ODR_25_HZ|ACTIVE_MODE|READY_MODE)); //set sample rate 25Hz
}
/************************************************************************
 * Gyro_Stop() puts gyro into idle mode
 * returns - nothing
 * ************************************************************************/
void gyro_stop()
{
	gyro_write_register((uint8_t) REGCTRL1,(uint8_t) STBY_MODE);
}
/************************************************************************
 * Gyro_ReadRegister() reads value at specified register address single byte
 * Parameters - register address
 * returns - status (success or fail)
 * ************************************************************************/
uint8_t gyro_read_register(uint8_t regAddr, uint8_t * regVal)
{
/*
    uint8_t cmdBuff[CMD_BUFF_SIZE] = {DATADEFAULT};
    uint8_t receiveBuff[CMD_BUFF_SIZE] = {DATADEFAULT};    // save data received from i2c slave
    i2c_status_t returnValue;
    cmdBuff[0] = regAddr;
	returnValue = I2C_DRV_MasterReceiveDataBlocking(
			FSL_I2CCOM1,
			&i2cCom1_MasterConfig0,
			cmdBuff,
			(uint32_t) CMD_BUFF_SIZE,
			receiveBuff,
			(uint32_t) CMD_BUFF_SIZE,
			TOUT_500_MS);
	if (returnValue == kStatus_I2C_Success)
	{
		* regVal= receiveBuff[0];
		return statOK;
	}
	return statErr;
*/
	printf("zxp not impl error:%s,%s\n", __FILE__, __func__);
	return statOK;
}
/************************************************************************
 * gyro_write_register() writes value to specified address single byte
 * Parameters - register address, register value
 * returns - operation status success/fail
 * ************************************************************************/
uint8_t gyro_write_register(uint8_t regAddr, uint8_t regVal)
{
/*
	uint8_t cmdBuff[CMD_BUFF_SIZE] = {DATADEFAULT};
	uint8_t sendBuff[CMD_BUFF_SIZE] = {DATADEFAULT};       // save data sent to i2c slave
	cmdBuff[0]  = regAddr;
	sendBuff[0] = regVal;
	i2c_status_t returnValue;
        returnValue = I2C_DRV_MasterSendDataBlocking(
        		FSL_I2CCOM1,
        		&i2cCom1_MasterConfig0,
				cmdBuff,
				(uint32_t) CMD_BUFF_SIZE,
				sendBuff,
				(uint32_t) CMD_BUFF_SIZE,
				TOUT_500_MS);
        if (returnValue == kStatus_I2C_Success)
        {
           return statOK;
        }
        return statErr;
*/
	printf("zxp not impl error:%s,%s\n", __FILE__, __func__);
	return statOK;
}
/************************************************************************
 * gyro_read_XYZ() function reads XYZ gyro data
 * Parameters - buffer to store the XYZ data
 * returns - size of the buffer
 * ************************************************************************/
uint8_t gyro_read_xyz(uint8_t * receiveBuff)
{
/*
    uint8_t cmdBuff[CMD_BUFF_SIZE] = {DATADEFAULT};
    cmdBuff[0] = REGDATA;
	I2C_DRV_MasterReceiveDataBlocking(
			FSL_I2CCOM1,
			&i2cCom1_MasterConfig0,
		   cmdBuff,
		   (uint32_t) CMD_BUFF_SIZE,
		   receiveBuff,
		   (uint32_t) XYZ_BUFF_SIZE,
		   TOUT_500_MS);
*/
	printf("zxp not impl error:%s,%s\n", __FILE__, __func__);
	return XYZ_BUFF_SIZE;
}
/************************************************************************
 * Gyro_getTurn() reads Z Axis rotation rate
 * returns - Z axis dps value
 * ************************************************************************/
int16_t gyro_get_turn()
{
	int16_t thetaZ=0;
/*
	uint8_t turnBuff[XYZ_BUFF_SIZE];
	gyro_read_xyz(&turnBuff[0]);//get data
	thetaZ = ((int16_t)((turnBuff[Z_MSB])<<Shift1_Byte)|turnBuff[Z_LSB]);
*/
	thetaZ = gyroThetaZReadFromEmitter();
	return thetaZ;
}
