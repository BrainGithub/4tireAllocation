/*
 * locator.c
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
#include "comTypes.h"
#include "locator.h"
#include "myinc.h" //zxp
#define NUMWHEELS 		(0x08) // max number of wheels on vehicle
#define MASK_PERIOD     (0x3F) // mask for TPMS rotation period
#define MASK_DIRECTION  (0xC0) // mask for TPMS rotation direction
#define SHIFT_DIRECTION (0x06) // bit shift for extracted direction data
#define SEED_VAL	    (0x61A8) //large value for min and max search
#define PERIOD_HIGH		(1100)   // upper  data sample rejection limit
#define PERIOD_LOW		(130)    // lower data sample rejection limit
#define VALID_COUNT     (0x03)   // min number of samples for valid dataset
#define VALID_DIRECTION	(0x01)   // validate if rotation direction is defined
static tpms_t  sensArray[NUMWHEELS]; // array to hold the tag information
static uint8_t tireCount=NUMWHEELS;
static uint8_t tagCount;
/************************************************************************
 * loc_set_tire_count() configures set number of tires/tags
 * Parameters - number of tires
 * returns - nothing
 * ************************************************************************/
void loc_set_tire_count(uint8_t nTire)
{
	if(nTire<=NUMWHEELS)
	{
		tireCount=nTire;
	}
	else
	{
		tireCount=NUMWHEELS;
	}
}
/************************************************************************
  * loc_get_tire_count() retrieves set number of tires/tags
  * Parameters - none
  * returns - tire count
  * ************************************************************************/
uint8_t loc_get_tire_count()
{
	return tireCount;
}
/************************************************************************
 * locate_tires() performs tire allocation algorithm
 * Parameters - current trajectory segment (turn)
 * returns - nothing
 * ************************************************************************/
void loc_locate_tires( uint8_t turnSegment)
{
	uint16_t fastPeriod=SEED_VAL, slowPeriod=0, curPeriod=0;
	uint16_t fastMargin=SEED_VAL, slowMargin=SEED_VAL, curMargin=0;
	uint8_t  fastIndex=0, slowIndex=0;
	uint8_t  i, j;
	//check if we have minimum data points before proceeding
	for (i = 0; i < tireCount; i++)
	{
		if (sensArray[i].Count < VALID_COUNT)
		{
			for (j = 0; j < tireCount; j++) // not enough data, reset
			{
				sensArray[j].Count = 0x00;
				sensArray[j].Period = 0x00;
				sensArray[j].Direction=0x00;
			}
			return;
		}
		sensArray[i].Period=sensArray[i].Period/sensArray[i].Count; //average the periods
	}
	//locate slow and fast period wheels i.e. the outer front and inner rear wheels on opposite sides
	for (i = 0; i < tireCount; i++)
	{
		curPeriod=sensArray[i].Period;
		if (curPeriod < fastPeriod)
		{
			fastIndex =i;
			fastPeriod = curPeriod;
		}
		if (curPeriod > slowPeriod)
		{
			slowIndex = i;
			slowPeriod = curPeriod;
		}
	}
	if (sensArray[fastIndex].Direction == sensArray[slowIndex].Direction)
	{
		//if fastest and slowest wheels are on the same side then something went wrong
		for (i = 0; i < tireCount; i++) //  reset
		{
			sensArray[i].Count = 0x00;
			sensArray[i].Period = 0x00;
			sensArray[i].Direction=0x00;
		}
		return;
	}
	for (i = 0; i < tireCount; i++) //evaluate smallest margins i.e. nearest alternative allocation
	{
		curPeriod=sensArray[i].Period;
		if (i!=fastIndex)//exclude min and max locations
		{
			curMargin=curPeriod-fastPeriod;//fast period is shortest
			if (curMargin < fastMargin)
			{
				fastMargin=curMargin;
			}
		}
		if(i!=slowIndex)
		{
			curMargin=slowPeriod-curPeriod;//slow period is longest
			if (curMargin < slowMargin)
			{
				slowMargin = curMargin;
			}
		}
	}
	//assign the positions for the fast and slow wheel
	switch (turnSegment)
	{
		case SegFwd_CCW_Turn:	//CCW turn forward
		{

			loc_eval_confidence(fastIndex, fastMargin, LocRightFront, LocRightRear);
			loc_eval_confidence(slowIndex, slowMargin, LocLeftRear, LocLeftFront);
			break;
		}
		case SegFwd_CW_Turn:	//CW turn fwd
		{
			loc_eval_confidence(fastIndex, fastMargin, LocLeftFront, LocLeftRear);
			loc_eval_confidence(slowIndex, slowMargin, LocRightRear, LocRightFront);
			break;
		}
		case SegRev_CCW_Turn:	//CCW turn rev
		{
			loc_eval_confidence(fastIndex, fastMargin, LocLeftFront, LocLeftRear);
			loc_eval_confidence(slowIndex, slowMargin, LocRightRear, LocRightFront);
			break;
		}
		case SegRev_CW_Turn:	//CW turn rev
		{
			loc_eval_confidence(fastIndex, fastMargin,LocRightFront, LocRightRear);
			loc_eval_confidence(slowIndex, slowMargin,LocLeftRear, LocLeftFront);
			break;
		}
		default:
		{

		}
	}
}
/************************************************************************
 * loc_eval_confidence() evaluates confidence of the new data point (for a selected side)
 * and either retains the existing allocation (with increased or
 * decreased confidence) or reassigns the new allocation
 * Parameters - margin data to compare, allocated position
 * returns - nothing
 * ************************************************************************/
void loc_eval_confidence(uint8_t tireIndex, uint16_t locMargin, uint8_t locIndex, uint8_t nextLocIndex)
{
	uint16_t newMargin=0;
	uint8_t reAllocate=0, curSide=0,i=0;
	//evaluate updated margin
	if(sensArray[tireIndex].Location==locIndex) //previous allocation unchanged
	{
		newMargin=sensArray[tireIndex].Confidence+locMargin;
	}
	else //allocation changed
	{
		if(sensArray[tireIndex].Location==0x00)
		{
			newMargin=locMargin;
			reAllocate=1;
		}
		else
		{
			if(sensArray[tireIndex].Confidence>locMargin) //not sure, keep locations
			{
				newMargin=sensArray[tireIndex].Confidence-locMargin;
			}
			else //sure, reassign locations
			{
				newMargin=locMargin-sensArray[tireIndex].Confidence;
				reAllocate=1;
			}
		}
	}
	curSide=sensArray[tireIndex].Direction;
	for (i=0; i<tireCount; i++)
	{
		if (sensArray[i].Direction==curSide) //finding the same side wheel - same rotation direction
		{
			sensArray[i].Confidence=newMargin; //assign new margin
			if(reAllocate) //reassign location if needed
			{
				if(i==tireIndex)
				{
					sensArray[i].Location=locIndex;
				}
				else
				{
					sensArray[i].Location=nextLocIndex;
				}
			}
		}
	}
}
/************************************************************************
 * loc_add_data() function validates and adds the next sample to
 * the averaged rotation period value
 * Parameters - current tire index, rotation period MSB rpMSB,
 * rotation period LSB rpLSB, tpms_t array - containing the tags info
 * returns - nothing
 * ************************************************************************/
void loc_add_data(uint8_t tireIndex, uint8_t rpMSB, uint8_t rpLSB)
{
	uint16_t periodData=0;
	uint8_t directionData=0;
	periodData = (uint16_t) ((rpMSB & MASK_PERIOD) << Shift1_Byte) | rpLSB;
	if (periodData > PERIOD_LOW && periodData < PERIOD_HIGH)
	{
		directionData = (rpMSB & MASK_DIRECTION)>> Shift_Direct; //extract rotation direction bits
		if (directionData > VALID_DIRECTION) //direction data valid
		{
			sensArray[tireIndex].Direction = directionData;
			sensArray[tireIndex].Count++;
			sensArray[tireIndex].Period = sensArray[tireIndex].Period + periodData; //sum the period
		}
	}
}
/************************************************************************
 * loc_reset_data_all() resets sensor data array
 * Parameters - none
 * returns - nothing
 * ************************************************************************/
void loc_reset_data_all()
{
	for(uint8_t i=0; i<tireCount; i++)
	{
		sensArray[i].ID=0x00;
		sensArray[i].Location=0x00;
		sensArray[i].Count=0x00;
		sensArray[i].Confidence=0x00;
		sensArray[i].Period=0x00;
	}
	tagCount=0;
}
/************************************************************************
 * loc_reset_period(tireIndex) resets sensor period  data in array
 * Parameters - current tag index
 * returns - nothing
 * ************************************************************************/
void loc_reset_period(uint8_t tireIndex)
{
	sensArray[tireIndex].Period=0x00;
	sensArray[tireIndex].Count=0x00;
}
/************************************************************************
 * loc_reset_location_all() resets all sensor period and location data in array
 * Parameters - none
 * returns - nothing
 * ************************************************************************/
void loc_reset_location_all()
{
	for(uint8_t i=0; i<tireCount; i++)
	{
		sensArray[i].Location=0x00;
		sensArray[i].Confidence=0x00;
		sensArray[i].Period=0x00;
		sensArray[i].Count=0x00;
	}
}

/************************************************************************
 * loc_store_id() verifies and adds to the white list new tag ID
 * Parameters - tag ID to add
 * returns - size of the list if list is full size is negative
 * ************************************************************************/
int8_t loc_store_id(uint32_t curTagID)
{
	if (curTagID!=0x00)
	{
		for(uint8_t i=0; i<tagCount; i++)
		{
			if (curTagID==sensArray[i].ID)
			{
				return tagCount;//tag was found before
			}
		}
		sensArray[tagCount].ID=curTagID; //add tag ID
		tagCount++;
	}
	if(tagCount>=tireCount)
	{
		return -1;//found all tags
	}
	return tagCount;
}
/************************************************************************
 * loc_get_tagID(tireIndex) retrieves sensor ID
 * Parameters - current tag index
 * returns - sensor tag ID
 * ************************************************************************/
uint32_t loc_get_tag_id(uint8_t curTagIndex)
{
	if ((curTagIndex>=0) && (curTagIndex<tireCount))
	{
		return sensArray[curTagIndex].ID;
	}
	return 0;
}
/************************************************************************
 * loc_get_tag_index(tireID) retrieves sensor index in array
 * Parameters - current tag ID
 * returns - sensor tag index
 * ************************************************************************/
uint8_t loc_get_tag_index(uint32_t curTagID)
{
	for(uint8_t i=0; i<tagCount; i++) //exclude 0 ID
	{
		if (curTagID==sensArray[i].ID)
		{
			return i; //found this ID in the white list
		}
	}
	return 0xFF; //not found return FF
}
/************************************************************************
 * loc_get_data_location()
 * Parameters - current tire index
 * returns - tire location
 * ************************************************************************/
uint8_t loc_get_data_location(uint8_t curIndx)
{
    return (uint8_t)(sensArray[curIndx].Location); //tire location (4 LSB 1-LR 2-LF 3-RF 4-RR)  and segment indicator (0-Park, 1- FWD, 2 Right turn, 3 left turn)
}
/************************************************************************
 * loc_get_data_period()
 * Parameters - current tire index
 * returns - averaged rotation period
 * ************************************************************************/
uint16_t loc_get_data_period(uint8_t curIndx)
{
	if (sensArray[curIndx].Count>0)
	{
		return (uint16_t)(sensArray[curIndx].Period/sensArray[curIndx].Count);
	}
	return 0;
}
/************************************************************************
 * loc_get_data_direction()
 * Parameters - current tire index
 * returns - rotation direction
 * ************************************************************************/
uint8_t loc_get_data_direction(uint8_t curIndx)
{
	return sensArray[curIndx].Direction;
}
/************************************************************************
 * loc_get_data_conf()
 * Parameters - current tire index
 * returns - confidence margin
 * ************************************************************************/
uint16_t loc_get_data_conf(uint8_t curIndx)
{
	return sensArray[curIndx].Confidence;
}

