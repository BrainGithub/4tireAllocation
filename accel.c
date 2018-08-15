/*
 * accel.c
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
#include "accel.h"
//#include "math.h"
#include "comTypes.h"

#define REGSTATUS		(0x00)	//address of status register
#define DATARDY         (0x03)  //status new data available
#define REGDATA			(0x01)  //address of data access register
#define REGDATACFG  	(0x0E)  //address of data configuration register
//defines for CTRL_REG
#define REGCTRL  		(0x2A)  // address of control register 1
#define ODR_12_HZ   	(0x28)	// output data rate set to 12 Hz
#define ACTIVE_MODE		(0x01)  // set device active
#define STBY_MODE 		(0x00)  // set device standing by
#define LOW_NOISE       (0x04)  // low noise operation mode
//********************************************************************************
//defines for CTRL_REG2
#define REGCTRL2 		(0x2B) //address of control register 2
#define HIRES_MODE      (0x02) //high resolution mode
#define FS_4G    		(0x01) //full scale set to +- 4g
#define NOISE_FLOOR 	(0x05) //noise rejection level
#define NUMRETRY		(0x10) //number of read retry if data is not ready
#define NUMAVER			(0x3F) //number of averages for baseline subtract
//const i2c_device_t i2cCom1_MasterConfig1 =
//    {
//        .address = 0x1E,
//        .baudRate_kbps = 400U
//    };
static int16_t xlX0;
static int16_t xlY0;
static int16_t xlZ0;

/************************************************************************
 * Accel_Init() puts accelerometer into active mode
 * configures resolution and full scale, puts accel back in active mode
 * returns - nothing
 * ************************************************************************/
void accel_init()
{
	accel_write_register((uint8_t)REGCTRL,(uint8_t)STBY_MODE);//device in standby mode
	accel_write_register((uint8_t)REGCTRL2, (uint8_t) HIRES_MODE);// high resolution mode
	accel_write_register((uint8_t)REGDATACFG,(uint8_t) FS_4G);//range 4g
	accel_write_register((uint8_t)REGCTRL,(uint8_t)(ODR_12_HZ|LOW_NOISE|ACTIVE_MODE));// 12.5 Hz low noise active mode
	xlX0=0;
	xlY0=0;
	xlZ0=0;
}
/************************************************************************
 * Accel_Stop() puts accelerometer into standby mode
 * returns - nothing
 * ************************************************************************/
void accel_stop()
{
	accel_write_register((uint8_t)REGCTRL,(uint8_t)STBY_MODE);
}
/************************************************************************
 * Accel_ReadRegister() reads value at specified register address single byte
 * Parameters - register address
 * returns - operation status (0 if success otherwise KSDK status value)
 * ************************************************************************/
uint8_t accel_read_register(uint8_t regAddr, uint8_t * regVal)
{
/*
    uint8_t cmdBuff[CMD_BUFF_SIZE] = {DATADEFAULT};
    uint8_t receiveBuff[CMD_BUFF_SIZE] = {DATADEFAULT};    // save data received from i2c slave
    i2c_status_t returnValue;
    cmdBuff[0] = regAddr;
	returnValue = I2C_DRV_MasterReceiveDataBlocking(
			FSL_I2CCOM1,
			&i2cCom1_MasterConfig1,
		   cmdBuff,
		   (uint32_t)CMD_BUFF_SIZE,
		   receiveBuff,
		   (uint32_t)CMD_BUFF_SIZE,
		   (uint32_t)TOUT_500_MS);
	if (returnValue == kStatus_I2C_Success)
	{
		* regVal=receiveBuff[0];
		return statOK;
	}
	else
	{
	   return statErr;
	}
*/
	printf("not impl error:%s,%s\n", __FILE__, __func__);
	return statOK;
}
/************************************************************************
 * Accel_WriteRegister() writes value to specified address single byte
 * Parameters - register address, register value
 * returns - operation status success/fail (success=0, fail!=0)
 * ************************************************************************/
uint8_t accel_write_register(uint8_t regAddr, uint8_t regVal)
{
/*
	uint8_t cmdBuff[CMD_BUFF_SIZE] = {DATADEFAULT};
	uint8_t sendBuff[CMD_BUFF_SIZE] = {DATADEFAULT};       // save data sent to i2c slave
	cmdBuff[0]  = regAddr;
	sendBuff[0] = regVal;
	i2c_status_t returnValue;
        returnValue = I2C_DRV_MasterSendDataBlocking(
        		FSL_I2CCOM1,
        		&i2cCom1_MasterConfig1,
				cmdBuff,
				(uint32_t)CMD_BUFF_SIZE,
				sendBuff,
				(uint32_t)CMD_BUFF_SIZE,
				(uint32_t)TOUT_500_MS);
        if (returnValue == kStatus_I2C_Success)
        {
           return statOK;
        }
        return statErr;
*/
	printf("not impl error:%s,%s\n", __FILE__, __func__);
	return statOK;
}
/************************************************************************
 * Accel_ReadXYZ() function reads XYZ accelerometer data
 * Parameters - buffer to store  the XYZ data
 * returns - returns the size of the buffer
 * ************************************************************************/
uint8_t accel_read_xyz(uint8_t * receiveBuff)
{
/*
    uint8_t cmdBuff[CMD_BUFF_SIZE] = {DATADEFAULT};
    cmdBuff[0] = REGDATA;
	I2C_DRV_MasterReceiveDataBlocking(
			FSL_I2CCOM1,
			&i2cCom1_MasterConfig1,
			cmdBuff,
			(uint32_t)CMD_BUFF_SIZE,
			receiveBuff,
			(uint32_t)XYZ_BUFF_SIZE,
			(uint32_t)TOUT_500_MS);
	 return XYZ_BUFF_SIZE;
*/
	printf("zxp not impl error:%s,%s\n", __FILE__, __func__);
	return XYZ_BUFF_SIZE;
}
/************************************************************************
 * accel_get_af() function evaluates forward acceleration  (AF)of the vehicle
 * Parameters - none
 * returns - current AF value
 * ************************************************************************/
int16_t accel_get_af()
{
	uint8_t afBuff[XYZ_BUFF_SIZE];
	int16_t curX;
	uint8_t cntr=0, readVal=0;

	/*
	accel_read_register(REGSTATUS, &readVal);
	while (readVal<DATARDY)
	{
		cntr++;
		if (cntr>NUMRETRY)
		{
			break;
		}
		accel_read_register(REGSTATUS, &readVal);
	}
	accel_read_xyz(&afBuff[0]);
	curX=(int16_t)(((afBuff[X_MSB]<<Shift1_Byte)|afBuff[X_LSB]));
	curX=(curX>>Shift_Accel)-xlX0; 	// subtract reference
	
	*/

	//zxp
	// to get acceleration forward from the simm queue
	curX = accForwardQueue.pop()


	if (curX>NOISE_FLOOR || curX<(-NOISE_FLOOR)) // within noise range?
	{
		return curX; // no, return measured value
	}
    return 0; //yes return zero
}
/************************************************************************
 * accel_set_ref() updates reference values for subtraction of
 * the static tilt
 * Parameters - none
 * returns - nothing
 * ************************************************************************/
void accel_set_ref()
{
	uint8_t refBuff[XYZ_BUFF_SIZE];
	int16_t readCount=0;
	int16_t curX=0;
	uint8_t cntr=0, readVal=0;
	accel_read_xyz(&refBuff[0]);
	xlX0= (int16_t)(((refBuff[X_MSB]<<Shift1_Byte)|refBuff[X_LSB]));
	xlX0=xlX0>> Shift_Accel;//accelerometer reports 14bit value, conversion to 16bit
	while(readCount<NUMAVER)
	{
		accel_read_register(REGSTATUS, &readVal);// check for the data availability
		while (readVal<DATARDY)
		{
			cntr++;
			if (cntr>NUMRETRY)
			{
				cntr=0;
				break;
			}
			accel_read_register(REGSTATUS, &readVal);
		}
		accel_read_xyz(&refBuff[0]);
		curX=(int16_t)(((refBuff[X_MSB]<<Shift1_Byte)|refBuff[X_LSB]));
		xlX0= (xlX0+(curX>>Shift_Accel))>>Shift_Aver;// 16bit conversion and running average
		readCount++;
	}
}

