/*
 * trajectory_segment.c
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
#include "comTypes.h"
#include "accel.h"
#include "gyro.h"
#include "trajectory_segment.h"
#define NUMAF    				(5)  	// number of running average points
#define GYRO_SENSITIVITY		(128)  	// with current settings : 1dps is 128 counts
#define ACCEL_SENSITIVITY		(2048)	// with current settings : 1g is 2048 counts
#define GYRO_TURNING_THRESHOLD	(10 * GYRO_SENSITIVITY) // lower limit for turn valid detection
#define GYRO_VARIATION_MAX		(105 * GYRO_SENSITIVITY) // maximum data variance for turn to be valid
#define VEL_PARK_THRESHOLD		(100)	//1mph minimum speed
#define VEL_SPEED_MAX			(3000) 	//30mph maximum speed
#define ONE_SECOND				(1000)  //1000 ms
static 	int16_t 	afArray[NUMAF];		// running average array
static 	int16_t 	intVelX;            // vehicle velocity
static 	uint8_t     velX_MSB;			// MSB and LSB
static 	uint8_t     velX_LSB;           // of velocity value
static  int16_t     thetaZ;				// turn angular speed
static 	uint8_t     thetaZ_MSB;         //MSB and LSN
static 	uint8_t     thetaZ_LSB;			//of angular speed
static 	int32_t  	sumAF;				//sum of running average array
static 	uint16_t 	divNum;				//time divider used to define velocity based on acceleration
static 	uint8_t  	afIndex;
static 	uint16_t    curCntr;
const	uint16_t 	defPeriod=200;     //default sampling period in ms
/************************************************************************
 * get_segment() function evaluates trajectory segment
 * based on vehicle linear and angular velocity
 * Parameters - intVelX and ThetaZ
 * returns - current trajectory segment identifier
 * ************************************************************************/
segment_t traj_get_segment()
{
	uint8_t curSegment = SegStop;

	//collect motion data using running average
	sumAF=sumAF-afArray[afIndex]; //subtract the one going away
	afArray[afIndex]=accel_get_af();
	sumAF=sumAF+afArray[afIndex]; //add the one coming in
	afIndex++;
	if(afIndex>=NUMAF)
	{
			afIndex=0;
	}
//	intVelX=1000;
//	thetaZ=0;
	intVelX=intVelX+(sumAF/divNum);
	thetaZ=gyro_get_turn();
//	if(afIndex>10)
//	{
//		thetaZ=30*128;
//	}
//	if(afIndex>50)
//	{
//		thetaZ=0;
//	}
//	if(afIndex>80)
//	{
//		thetaZ=30*128;
//	}
//	if(afIndex>120)
//	{
//		thetaZ=0;
//	}
//////////////////////////////////////////////
	velX_MSB=(uint8_t)(intVelX>>Shift1_Byte);
	velX_LSB=(uint8_t)intVelX;
	thetaZ_MSB=(uint8_t)(thetaZ>>Shift1_Byte);
	thetaZ_LSB=(uint8_t)thetaZ;
	if ((intVelX > VEL_PARK_THRESHOLD)&&(intVelX<VEL_SPEED_MAX))
	{
		curSegment = SegForward; //driving forward
		if (thetaZ > GYRO_TURNING_THRESHOLD)
		{
			curSegment = SegFwd_CCW_Turn; //CCW turn fwd
		}
		else if (thetaZ < (-GYRO_TURNING_THRESHOLD))
		{
			curSegment = SegFwd_CW_Turn; //CW turn fwd
		}
	}
	else if (intVelX < (-VEL_PARK_THRESHOLD))
	{
		curSegment = SegReverse;
		if (thetaZ > GYRO_TURNING_THRESHOLD)
		{
			curSegment = SegRev_CCW_Turn; //CCW turn rev
		}
		else if (thetaZ < (-GYRO_TURNING_THRESHOLD))
		{
			curSegment = SegRev_CW_Turn; //CW turn rev
		}
	}
	return curSegment;
}
/************************************************************************
 * validate_turn() verifies that vehicle is in valid turn segment
 * forcing the logger to add data points
 * Parameters - current segment, previous segment
 * returns - turn valid
 * ************************************************************************/
turn_t traj_validate_turn(uint8_t curSegment, uint8_t prevSegment)
{
	if(curSegment>SegReverse) //turning
	{
		if((prevSegment==curSegment) || (prevSegment<1)) // all non-turning segments are set to 0
		{
			//handling normal data collection during qualified turn
			return turnValid;
		}
		else //we were turning in opposite direction
		{
			//handling swerve motion when sign of turn changed abruptly
			return turnCompleted;
		}
	}
	else //going straight
	{
		if (prevSegment > SegReverse)// previous segment data indicate that we've just finished turn
		{
			return turnCompleted;
		}
	}
	// no turn before or at the moment
	return turnInvalid;
}
/************************************************************************
 * traj_init() resets the static variables and activates accel and gyro
 * Parameters - sample period in ms
 * returns - status, 0- OK, 1 - error in period settings, default is used
 * ************************************************************************/
uint8_t traj_init(uint16_t samplePeriod)
{
	  gyro_init();
	  accel_init();
	  accel_set_ref(); //reference acceleration
	  for(uint8_t i=0; i<NUMAF; i++)
	  {
		  afArray[i]=0x00;
	  }
	  intVelX=0x00;
	  afIndex=0;
	  sumAF=0;
	  curCntr=0;
	  if(samplePeriod>0)
	  {
		  divNum=(NUMAF*ONE_SECOND)/samplePeriod;//combine averages and time constant into single number
		  return statOK;
	  }
	  divNum=(NUMAF*ONE_SECOND)/defPeriod;// assume default period
	  return statErr;
}
/************************************************************************
 * traj_stop() de-activates accel and gyro
 * Parameters - none
 * returns - nothing
 * ************************************************************************/
void traj_stop()
{
	gyro_stop();
	accel_stop();
}
/************************************************************************
 * traj_get_velX_MSB() retrieves current velocity MSB
 * Parameters - none
 * returns - velocity MSB
 * ************************************************************************/
uint8_t traj_get_velX_MSB()
{
	return velX_MSB;
}
/************************************************************************
 * traj_get_velX_LSB() retrieves current velocity LSB
 * Parameters - none
 * returns - velocity LSB
 * ************************************************************************/
uint8_t traj_get_velX_LSB()
{
	return velX_LSB;
}
/************************************************************************
 * traj_get_velX_MSB() retrieves current turn velocity MSB
 * Parameters - none
 * returns - turn velocity MSB
 * ************************************************************************/
uint8_t traj_get_thetaZ_MSB()
{
	return thetaZ_MSB;
}
/************************************************************************
 * traj_get_velX_MSB() retrieves current turn velocity LSB
 * Parameters - none
 * returns - turn velocity LSB
 * ************************************************************************/
uint8_t traj_get_thetaZ_LSB()
{
	return thetaZ_LSB;
}
