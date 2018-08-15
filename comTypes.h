/*
 * commonTypes.h
 *
 *  Created on: Jun 20, 2016
 *      Author: B47167
 * (c) Copyright 2016, Freescale Semiconductor, Inc. All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
************************************************************************************/
#ifndef SOURCES_COMTYPES_H_
#define SOURCES_COMTYPES_H_
#include "myinc.h"//zxp
#define TOUT_500_MS 	(0x1F4) //time out 0.5s
#define TOUT_1K_MS 		(0x3E8) //timeout 1s
#define TOUT_5K_MS 		(0x1388) //timeout 5s
#define X_MSB			(0x00)  //index of X MSB byte
#define X_LSB           (0x01)  //index of X LSB byte
#define Y_MSB           (0x02)  //index of Y MSB byte
#define Y_LSB           (0x03)	//index of Y LSB byte
#define Z_MSB			(0x04)	//index of Z MSB byte
#define Z_LSB           (0x05)	//index of Z LSB byte
#define CMD_BUFF_SIZE	(0x01)	//size of buffer containing command code
#define XYZ_BUFF_SIZE 	(0x06)	//size of data buffer for 3 axis sensor
#define DATADEFAULT     (0xFF)	//default setting for interface response
//#define TERMINAL
typedef enum WheelLocation      // enum for wheel locations
{
	LocNone = 0x00, // undefined
	LocLeftRear,
	LocLeftFront,
	LocRightFront,
	LocRightRear
}wheel_loc_t;
typedef enum SegmentType       //enum for trajectory segment types
{
	SegStop =0x00,
	SegForward,
	SegReverse,
	SegFwd_CCW_Turn,
	SegFwd_CW_Turn,
	SegRev_CCW_Turn,
	SegRev_CW_Turn
}segment_t;
typedef enum turnStatus       //enum for turn valid conditions
{
	turnInvalid 	=0x00,
	turnValid    	=0x01,
	turnCompleted 	=0x02
}turn_t;
typedef enum ByteShift        //enums for various bit shifts
{
	Shift_Aver  = 0x01,      //average of 2 elements
	Shift_Accel = 0x02,      //14 bit to 16 bit conversion
	Shift_Segmnt= 0x04,      //extract segment
	Shift_Direct= 0x06,      //extract direction
	Shift1_Byte = 0x08,      //8 bit shift
	Shift2_Byte = 0x10,      //16bit shift
	Shift3_Byte = 0x18,      //24bit shift
}byte_shift_t;
typedef enum statusCode     //enum for return status
{
	statOK	=0x00,
	statErr =0x01
}status_code_t;
#endif /* SOURCES_COMTYPES_H_ */
