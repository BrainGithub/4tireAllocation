/*
 * locator.h
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

#ifndef SOURCES_LOCATOR_H_
#define SOURCES_LOCATOR_H_
typedef struct tpms_t
{
	uint32_t ID;
	uint16_t Confidence;
	uint16_t Period;
	uint8_t Direction;
	uint8_t Location;
	uint8_t Count;
}tpms_t;
/************************************************************************
 * loc_locate_tires() performs tire allocation algorithm
 * Parameters - current trajectory segment (turn)
 * returns - nothing
 * ************************************************************************/
void loc_locate_tires( uint8_t turnSegment);
 /************************************************************************
  * loc_eval_confidence() evaluates confidence of the new data point (for a selected side)
  * and either retains the existing allocation (with increased or
  * decreased confidence) or reassigns the new allocation
  * Parameters - margin data to compare, allocated position
  * returns - nothing
  * ************************************************************************/
 void loc_eval_confidence(uint8_t tireIndex, uint16_t locMargin, uint8_t locIndex, uint8_t nextLocIndex);
 /************************************************************************
  * loc_add_data() function validates and evaluates cumulative average
  * Parameters - current tire index
  * returns - nothing
  * ************************************************************************/
 void loc_add_data(uint8_t tireIndex, uint8_t rpMSB, uint8_t rpLSB);
 /************************************************************************
  * loc_get_data_location()
  * Parameters - current tire index
  * returns - tire location
  * ************************************************************************/
 uint8_t loc_get_data_location(uint8_t tireIndx);
 /************************************************************************
  * loc_get_data_period()
  * Parameters - current tire index
  * returns - averaged rotation period
  * ************************************************************************/
 uint16_t loc_get_data_period(uint8_t tireIndx);
 /************************************************************************
  * loc_get_data_direction()
  * Parameters - current tire index
  * returns - rotation direction
  * ************************************************************************/
 uint8_t loc_get_data_direction(uint8_t tireIndx);
 /************************************************************************
  * loc_get_data_conf()
  * Parameters - current tire index
  * returns - confidence margin
  * ************************************************************************/
 uint16_t loc_get_data_conf(uint8_t tireIndx);
 /************************************************************************
  * loc_get_data_conf_LSB()
  * Parameters - current tire index
  * returns - LSB of confidence margin
  * ************************************************************************/
 void loc_reset_data_all();
 /************************************************************************
  * loc_reset_period(tireIndex) resets sensor period data in array
  * Parameters - current tag index
  * returns - nothing
  * ************************************************************************/
 void loc_reset_period(uint8_t tireIndex);
 /************************************************************************
  * loc_reset_location_all() resets all sensor period and location data in array
  * Parameters - none
  * returns - nothing
  * ************************************************************************/
 void loc_reset_location_all();
 /************************************************************************
  * loc_store_id() verifies and adds to the white list new tag ID
  * Parameters - tag ID to add
  * returns - size of the list if list is full size is negative
  * ************************************************************************/
 int8_t loc_store_id(uint32_t curTagID);
 /************************************************************************
  * loc_get_tagID(tireIndex) retrieves sensor ID
  * Parameters - current tag index
  * returns - sensor tag ID
  * ************************************************************************/
 uint32_t loc_get_tag_id(uint8_t curTagIndex);
 /************************************************************************
  * loc_get_tag_index(tireID) retrieves sensor index in array
  * Parameters - current tag ID
  * returns - sensor tag index
  * ************************************************************************/
 uint8_t loc_get_tag_index(uint32_t curTagID);
 /************************************************************************
  * loc_set_tire_count() configures set number of tires/tags
  * Parameters - number of tires
  * returns - nothing
  * ************************************************************************/
 void loc_set_tire_count(uint8_t nTire);
 /************************************************************************
  * loc_get_tire_count() retrieves set number of tires/tags
  * Parameters - none
  * returns - tire count
  * ************************************************************************/
 uint8_t loc_get_tire_count();

#endif /* SOURCES_LOCATOR_H_ */
