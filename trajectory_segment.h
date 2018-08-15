/*
 * trajectory_segment.h
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

#ifndef SOURCES_TRAJECTORY_SEGMENT_H_
#define SOURCES_TRAJECTORY_SEGMENT_H_

/************************************************************************
 * get_segment() function evaluates trajectory segment
 * based on vehicle linear and angular velocity
 * Parameters - intVelX and ThetaZ
 * returns - current trajectory segment identifier
 * ************************************************************************/
segment_t traj_get_segment();
/************************************************************************
 * traj_validate_turn() verifies that vehicle is in valid turn segment
 * forcing the logger to add data points
 * Parameters - current segment, previous segment
 * returns - turn status
 * ************************************************************************/
turn_t traj_validate_turn(uint8_t curSegment, uint8_t prevSegment);
/************************************************************************
 * traj_init() resets the static variables and activates accel and gyro
 * Parameters - sample period in ms
 * returns - status, 0- OK, 1 - error in period settings, default is used
 * ************************************************************************/
uint8_t traj_init(uint16_t samplePeriod);
/************************************************************************
 * traj_stop() de-activates accel and gyro
 * Parameters - none
 * returns - nothing
 * ************************************************************************/
void traj_stop();
/************************************************************************
 * traj_get_velX_MSB() retrieves current velocity MSB
 * Parameters - none
 * returns - velocity MSB
 * ************************************************************************/
uint8_t traj_get_velX_MSB();
/************************************************************************
 * traj_get_velX_LSB() retrieves current velocity LSB
 * Parameters - none
 * returns - velocity LSB
 * ************************************************************************/
uint8_t traj_get_velX_LSB();
/************************************************************************
 * traj_get_velX_MSB() retrieves current turn velocity MSB
 * Parameters - none
 * returns - turn velocity MSB
 * ************************************************************************/
uint8_t traj_get_thetaZ_MSB();
/************************************************************************
 * traj_get_velX_MSB() retrieves current turn velocity LSB
 * Parameters - none
 * returns - turn velocity LSB
 * ************************************************************************/
uint8_t traj_get_thetaZ_LSB();

#endif /* SOURCES_TRAJECTORY_SEGMENT_H_ */
