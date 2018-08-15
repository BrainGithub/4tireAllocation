/*!
** @file rtos_main_task.c
** @version 01.00
** @brief
*/         
/*!
**  @addtogroup rtos_main_task_module rtos_main_task module documentation
**  @{
*
* (c) Copyright 2016, Freescale Semiconductor, Inc. All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
************************************************************************************/
/* MODULE rtos_main_task */

//#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif 
/* User includes (#include below this line is not maintained by Processor Expert) */
//#define  ARM_MATH_CM0PLUS
//#include "math.h"
#include "comTypes.h"
#include "serialIO.h"
//#include "transceiver_drv.h"//zxp
//#include "premo.h"
#include "gyro.h"
#include "accel.h"
#include "locator.h"
#include "trajectory_segment.h"
///////////////////////////////////////////////////////////////////////////////
//  Defines for setting constants
///////////////////////////////////////////////////////////////////////////////
#define PREAMBL    		(0xAA) //stream host packet leading flag
#define PREAMBL_RSP     (0xAC) //response host packet leading fag
#define POSTAMBL   		(0x7F) //host packet trailing flag
#define POSTAMBL_RSP  	(0x7E) //host packet trailing flag
#define XCV_BUFF_SIZE	(0x20) //payload size TPMS
#define LF_BUFF_SIZE    (0x06) //payload size LF transmit packet
#define REGOPRSSI       (0x23) //reg addrs op rssi (data ready)
#define REGFIFO         (0x00) //reg addr xcvr fifo
#define REGVALRSSI		(0x24) //reg addr val rssi (data)
#define STARTRSSI		(0x01) //reg addr start rssi(control)
#define NPTS_DFLT		(0x01)  // span default number of averages
#define DFLT_BW_STEP    (0xA3) //span default sweep step
///////////////////////////////////////////////////////////////////////////////
//  Defines for read buffer location/indexes
///////////////////////////////////////////////////////////////////////////////
#define IND_PREAM		(0x00) //host packet preamble index
#define IND_PLD_LENGTH  (0x01) //host packet payload length index
#define IND_PLD_START   (0x02) //host packet message body start index
#define IND_PRD_MSB     (0x09) //TPMS packet rotation period byte indices
#define IND_PRD_LSB     (0x0A)
#define IND_PACK_TYPE   (0x01) //TPMS packet pack type byte index
#define FRAME_LENGTH	(0x02)
#define IND_ID1         (0x02) //rdBuffer Tag ID byte indexes
#define IND_ID2         (0x03)
#define IND_ID3         (0x04)
#define IND_ID4         (0x05)
///////////////////////////////////////////////////////////////////////////////
//  Defines for execution commands
///////////////////////////////////////////////////////////////////////////////
#define READ		(0x52) //R read device register
#define WRITE   	(0x57) //W write device register
#define INIT    	(0x49) //I init KWL
#define MODE		(0x4D) //M switch to standby or receive RF
#define STREAM		(0x46) //F start data stream
#define PING		(0x50) //P discover TPMS
///////////////////////////////////////////////////////////////////////////////
//  Defines for configuration commands
///////////////////////////////////////////////////////////////////////////////
#define SPAN		(0x41) //A
#define CFGSPAN 	(0x47) //G
#define CFGLF   	(0x4C) //L
#define DEVICE  	(0x56) //V
#define GYRO    	(0x59) //Y
#define XCV			(0x54) //T
#define ACCEL   	(0x58) //X
#define LOCATOR 	(0x4F) //O
#define DONE		(0x44) //D

#define BW_315_9K  	(0x41) // 315MHz 9600 baud bit rate
#define BW_315_19K  (0x42) // 315MHz 19230 baud bit rate
#define BW_434_9K  	(0x43) // 434MHz 9600 baud bit rate
#define BW_434_19K  (0x44) // 434MHz 19230 baud bit rate

///////////////////////////////////////////////////////////////////////////////
//  Defines for autolocalization algo
///////////////////////////////////////////////////////////////////////////////

#define PARK_MODE_PACKET_TYPE		0x24
#define SPEED_MODE_PACKET_TYPE		0x25
#define PRESSURE_MODE_PACKET_TYPE	0x26
#define SAMPLE_PERIOD           200   // trajectory sampling period in ms
#define RESET_PERIOD			(15000/SAMPLE_PERIOD)
#define ALLOC_MARGIN			100 // not used in current implementation
///////////////////////////////////////////////////////////////////////////////
//  task scoped variables
///////////////////////////////////////////////////////////////////////////////
static uint8_t rdBuffer[XCV_BUFF_SIZE];
/* Initialization of Processor Expert components function prototype */
#ifdef MainTask_PEX_RTOS_COMPONENTS_INIT
extern void PEX_components_init(void);
#endif 

/*
** ===================================================================
**     Event       :  main_task (module rtos_main_task)
**
**     Component   :  MainTask [OS_Task]
*/
/*!
**     @brief
**         RTOS task routine. The routine is generated into os_tasks.c
**         file.
**     @param
**         task_init_data  - Parameter to be passed to the
**         task when it is created.
*/
/* ===================================================================*/
//void main_task(os_task_param_t task_init_data)
uint8_t IS_DATARDY()
{
	return 1;
}

uint8_t IS_FIFO()
{
	return 1;
}


void main_task() //zxp
{
  /* Write your local variable definition here */
	 /* Write your code here ... */
#ifdef TERMINAL
   uint8_t buffSelOp[] = "\n\rSelect Operation R(ead)/W(rite)/I(nit)/M(ode)>>T(est)/F(IFO)\n\r";
   uint8_t buffSelReg[] ="\r\nSelect Register Address\r\n";
   uint8_t buffRegVal[] ="\r\nRegister Value\r\n";
   uint8_t buffSelMod[] ="\r\nSelect Mode S(tandBy)/R(eceive)\r\n";
   uint8_t buffSelVal[]="\r\nEnter Value\r\n";
   uint8_t buffDflt[]="\r\n Input not recognized\r\n";
#endif
   uint32_t curFrf, curTagID;
   uint16_t nPts=NPTS_DFLT, curSpan;
   uint8_t  opCode=0, addrVal=0, readVal=0, writeVal=0, errCode=0, curIndx=0;
   uint8_t  curDev= XCV;
   uint8_t  curBW = BW_315_9K;
   uint8_t  lfBuff[LF_BUFF_SIZE];
   /* Initialization of Processor Expert components (when some RTOS is active). DON'T REMOVE THIS CODE!!! */
#ifdef MainTask_PEX_RTOS_COMPONENTS_INIT
  PEX_components_init(); 
#endif 
  /* End of Processor Expert components initialization.  */
  //mkw01_drv_spi_init();
  //premo_init(); zxp
#define PEX_USE_RTOS
#ifdef PEX_USE_RTOS
  while (1) {
#endif
#ifdef TERMINAL
	  // Inform user of what to do
	    sio_send_rsp(&buffSelOp[0],sizeof(buffSelOp));
#endif
		 // Wait to receive input data
	        //opCode=sio_get_char();
	        char buf[128];
		memset(buf, 0, 128);
	        sio_get_str(buf, 128);
		opCode = buf[0];
		curDev = buf[1];
		sleep(1);
		switch (opCode)
		{
			case INIT:
			{
				curBW = sio_get_char();
				loc_set_tire_count(sio_get_input());
				reset_xcvr(curBW);
				send_val(DONE);
				break;
			}
			case PING:
			{
				discover_tpms();
				curIndx=IND_PLD_START;
				for(addrVal=0; addrVal<loc_get_tire_count(); addrVal++)
				{
					curTagID=loc_get_tag_id(addrVal);
					rdBuffer[curIndx++]=curTagID >> Shift3_Byte;
					rdBuffer[curIndx++]=curTagID >> Shift2_Byte;
					rdBuffer[curIndx++]=curTagID >> Shift1_Byte;
					rdBuffer[curIndx++]=curTagID;
				}
				rdBuffer[curIndx++]=POSTAMBL;
				rdBuffer[curIndx]=POSTAMBL;
				rdBuffer[IND_PLD_LENGTH]=curIndx++;
				rdBuffer[IND_PREAM]=PREAMBL;
#ifdef TERMINAL
				sio_send_rsp(&rdBuffer[0], curIndx);
#else
				sio_send_rsp(&rdBuffer[0], curIndx);
#endif
				break;
			}
			case DEVICE:
			{
#ifdef TERMINAL
				sio_send_rsp (&buffSelMod[0],sizeof(buffSelMod));
#endif
				curDev = sio_get_char();
				break;
			}
			case WRITE:
			{
#ifdef TERMINAL
				 sio_send_rsp(&buffSelReg[0],sizeof(buffSelReg));
#endif
				 addrVal=sio_get_input();
#ifdef TERMINAL
				 sio_send_rsp(&buffSelVal[0],sizeof(buffSelVal));
#endif
				 writeVal=sio_get_input();
				 switch (curDev)
				 {
					 case GYRO:
					 {
						 gyro_write_register(addrVal, writeVal);
						 break;
					 }
					 case ACCEL:
					 {
						 accel_write_register(addrVal, writeVal);
						 break;
					 }
					 default:  //case (XCV||SPAN||LOCATOR):
					 {
						 //mkw01_drv_write_register(addrVal, writeVal);
					 }
				}
				break;
			}
			case READ:
			{
#ifdef TERMINAL
				sio_send_rsp(&buffSelReg[0], sizeof(buffSelReg));
#endif
				addrVal=sio_get_input();
#ifdef TERMINAL
				sio_send_rsp(&buffRegVal[0],sizeof(buffRegVal));
#endif
				 switch (curDev)
				 {
					case GYRO:
					{
						errCode=gyro_read_register(addrVal, &readVal);
						break;
					}
					case ACCEL:
					{
						errCode=accel_read_register(addrVal, &readVal);
						break;
					}
					default: //case (XCV||SPAN||LOCATOR):
					{
						//errCode=mkw01_drv_read_register(addrVal, &readVal);
					}
				 }
				 if(!errCode)
				 {
					 send_val(readVal);
					 break;
				 }
				 else
				 {
					 send_val(errCode);
					 break;
				 }
			}
			case MODE:
			{
#ifdef TERMINAL
				sio_send_rsp (&buffSelMod[0],sizeof(buffSelMod));
#endif
				addrVal = sio_get_char();
				//mkw01_drv_set_op_mode(addrVal);
				break;
			}
			case STREAM:
			{
				printf("start data stream\n");
				 switch (curDev)
				 {
					 case LOCATOR:
					 {
						printf("get_locator_data\n");
						get_locator_data();
						break;
					 }
					 case SPAN:
					 {
						get_span_data(curFrf,curSpan, nPts);
						reset_xcvr(curBW);
						break;
					 }
					 case GYRO:
					 {
						 get_motion_data(curDev);
						 break;
					 }
					 case ACCEL:
					 {
						 get_motion_data(curDev);
						 break;
					 }
					 case XCV:
					 {
						 get_tpms_data();
						 break;
					 }
					 default:
					 {
						//unrecognized operation code
					 }
				 }
				send_val(DONE);
				break;
			}
			case CFGSPAN:
			{
				 curFrf=0;
				 curSpan=0;
				 nPts=sio_get_input(); //number of samples per test run
				 curFrf|= ((uint32_t)sio_get_input() << Shift2_Byte);//CF High byte
				 curFrf|= ((uint32_t)sio_get_input() << Shift1_Byte);//CF Mid byte
				 curFrf|= (uint32_t)sio_get_input();                 //CF Low byte
				 curSpan|=((uint32_t)sio_get_input() << Shift1_Byte);//Sweep BW MSB
				 curSpan|=(uint32_t)sio_get_input();				//Sweep BW LSB
				 break;
			}
			case CFGLF:
			{
				readVal=sio_get_input();
				for (addrVal=0; addrVal<readVal; addrVal++)
				{
					lfBuff[addrVal]=sio_get_input();
				}
				//premo_send_packet(&lfBuff[0]); zxp
				send_val(DONE);
				break;
			}
			default:
			{
#ifdef TERMINAL
				sio_send_rsp(&buffDflt[0],sizeof(buffDflt));
#endif
			}

		}
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/**********************************************************************
 * reset_XCVR(uint8_t curConfig)
 * function configures receiver
 * curConfig - current RF configuration
 * returns- nothing
 * **********************************************************************/
void reset_xcvr(uint8_t curConfig)
{
/*
   mkw01_drv_set_op_mode(STANDBY);
   mkw01_drv_rf_init_315();
   switch (curConfig)
   {
	   case BW_315_19K://B
	   {
		   mkw01_drv_set_19230();
		   break;
	   }
	   case BW_434_9K: //C
	   {
		   mkw01_drv_set_434();
		   break;
	   }
	   case BW_434_19K: //D
	   {
		   mkw01_drv_set_434();
		   mkw01_drv_set_19230();
		   break;
	   }
	   default: //A
	   {
		   //this case is covered by default initialization init_315
	   }
   }
*/

   	
   printf("zxp not impl error:%s,%s\n", __FILE__, __func__);
}
/**********************************************************************
 * get_span_data(uint32_t curFrf, uint16_t curSpan, uint16_t nPts)
 * function configures and executes RSSI spectrum data collection
 * curFrf - carrier or center frequency
 * curSpan - half frequency sweep range
 * if curSpan is zero function is used to evaluate RSSI time series
 * nPts - number of data points to acquire
 * returns- 1 if user requested stop otherwise returns 0
 * **********************************************************************/
uint8_t get_span_data(uint32_t curFrf, uint16_t curSpan, uint16_t nPts)
{
	uint16_t curStep=DFLT_BW_STEP, curCount;
	uint8_t  curFrfMsb, curFrfMid, curFrfLsb, curIndx=0;
	uint8_t  rssiVal, readVal=0;
	sio_init_stop_char();
	//mkw01_drv_set_op_mode(STANDBY);
	while(1)
	{
		for (uint32_t curF=curFrf; curF<=(curFrf+curSpan); curF+=curStep)
		{
			curFrfMsb=(uint8_t)(curF >> Shift2_Byte);
			curFrfMid=(uint8_t)(curF >> Shift1_Byte);
			curFrfLsb=(uint8_t)curF;
			//mkw01_drv_set_freq(curFrfMsb, curFrfMid,curFrfLsb);
			curCount=0;
			rssiVal=0;
			//mkw01_drv_set_op_mode(RECEIVE);
			while (curCount<nPts)
			{
				//mkw01_drv_start_rssi();
				if (curCount>0)
				{
					//mkw01_drv_read_register(REGVALRSSI, &readVal);
					rssiVal=(rssiVal+readVal)>>1;
				}
				else
				{
					//mkw01_drv_read_register(REGVALRSSI, &rssiVal);
				}
				curCount++;
				//mkw01_drv_start_rssi();
			}
			//mkw01_drv_set_op_mode(STANDBY);
			curIndx=IND_PLD_START;
			rdBuffer[curIndx++]=curFrfMsb;
			rdBuffer[curIndx++]=curFrfMid;
			rdBuffer[curIndx++]=curFrfLsb;
			rdBuffer[curIndx++]=rssiVal;
			rdBuffer[curIndx++]=POSTAMBL;
			rdBuffer[curIndx]=POSTAMBL;
			rdBuffer[IND_PREAM]=PREAMBL;
			rdBuffer[IND_PLD_LENGTH]=curIndx++;
			sio_send_rsp(&rdBuffer[0], curIndx);
			if(sio_get_stop_char())
			{
				return statOK;
			}
		}
	}
	return statOK;
}
/************************************************************************
 * get_motion_data() function streams gyro or accel data from AGM01
 * Streaming stops once user sends any char over UART to interrupt
 * Parameters - none
 * returns - nothing
 * ************************************************************************/
void get_motion_data(uint8_t curDev)
{
	uint8_t  readVal=0, flagDone=0, curIndx=0;
	sio_init_stop_char();
	curTick=0;
	if(curDev==ACCEL)
	{
		accel_init();
	}
	else
	{
		gyro_init();
	}
	while(1)
	{
		if (!(curTick%5)) //default sampling interval 5ms
		{
			if (!flagDone)
			{
				curIndx=IND_PLD_START;
				if(curDev==ACCEL)
				{
					readVal=accel_read_xyz(&rdBuffer[curIndx]);//get accel data and index offset
				}
				else
				{
					readVal=gyro_read_xyz(&rdBuffer[curIndx]);//get gyro data and index offset
				}
				curIndx=IND_PLD_START+readVal; //shift the index
				curIndx++;
				rdBuffer[curIndx++]=(uint8_t)(curTick >> Shift3_Byte); //timestamp 1ms
				rdBuffer[curIndx++]=(uint8_t)(curTick >> Shift2_Byte);
				rdBuffer[curIndx++]=(uint8_t)(curTick >> Shift1_Byte);
				rdBuffer[curIndx++]=(uint8_t)(curTick);
				rdBuffer[curIndx++]=POSTAMBL;
				rdBuffer[curIndx]=POSTAMBL;
				rdBuffer[IND_PREAM]=PREAMBL;
				rdBuffer[IND_PLD_LENGTH]=curIndx++;
				sio_send_msg(&rdBuffer[0], curIndx);
				flagDone=1;
			}
		}
		else
		{
			flagDone=0;
		}
		if(sio_get_stop_char())
		{
			if(curDev==ACCEL)
			{
				accel_stop();
			}
			else
			{
				gyro_stop();
			}
			return;
		}
	}
	sio_end_stop_char();
	if(curDev==ACCEL)
	{
		accel_stop();
	}
	else
	{
		gyro_stop();
	}
	return;
}

/************************************************************************
 * get_locator_data() function streams TPMS and sampled vehicle data
 * Streaming stops once user sends any char over UART to interrupt
 * Parameters - none
 * returns - nothing
 * ************************************************************************/
void get_locator_data()
{
	uint32_t curTagID=0;
	uint16_t curPeriod, curConMarg;
	uint8_t  curSegment=0, prevSegment=0;
	uint8_t  flagDone=0, curIndx=0;
	uint8_t  validTurn=0, curTock=0, curTagIdx;
	sio_init_stop_char();
	//mkw01_drv_set_op_mode(RECEIVE);
	//mkw01_drv_start_rssi();
	traj_init(SAMPLE_PERIOD);
	curTick=0;
	while(1)
	{
		sleep(1);
		printf("get_locator_data while(1)\n");
		printf("curTick=%d\n", curTick);
		if ((curTick % SAMPLE_PERIOD)==0)
		{
			if (!flagDone)
			{
				flagDone=1;
				curSegment = traj_get_segment();
				switch(traj_validate_turn(curSegment, prevSegment))
				{
					case(turnValid): //vehicle is turning acquire and store the sample
					{
						prevSegment=curSegment;
						validTurn=1;
						curTock=0;
						break;
					}
					case(turnCompleted): //vehicle just finished the turn, process data
					{
						loc_locate_tires(prevSegment);
						prevSegment=0;
						validTurn=0;
						curTock=0;
						break;
					}
					default:
					{
						// straight/stop segment continued
						curTock++;
						//reset previously assigned locations
						//in case we haven't turned for longer than RESET_PERIOD
						if(curTock>RESET_PERIOD)
						{
							loc_reset_location_all();
							curTock=0;
						}
						prevSegment=0;
						validTurn=0;
					}
				}
			}
		}
		else
		{
			flagDone=0;
		}
		if (IS_DATARDY)
		{
			curTagID= get_tpms_packet();
			curTagIdx= loc_get_tag_index(curTagID);
			if(curTagIdx!=0xFF)
			{
				curIndx=rdBuffer[0]; //length of the tpms packet
				if (rdBuffer[IND_PACK_TYPE] == SPEED_MODE_PACKET_TYPE)
				{
					if(validTurn) //turning
					{
						loc_add_data(curTagIdx, rdBuffer[IND_PRD_MSB],rdBuffer[IND_PRD_LSB]);
					}
					else
					{
						loc_reset_period(curTagIdx);
					}
				}
				else
				{
					//insert dummy bytes for RR for Pressure and Park packets
					rdBuffer[curIndx+2]=rdBuffer[curIndx];//shift RSSI byte
					rdBuffer[curIndx+1]=rdBuffer[curIndx-1];//frame ID LSB byte
					rdBuffer[curIndx]=rdBuffer[curIndx-2]; //Frame ID MSB
					rdBuffer[curIndx-1]=0;// dummy bytes for rotation period insertion
					rdBuffer[curIndx-2]=0;
					curIndx=curIndx+2;//move index to the RSSI byte position
				}
				curPeriod=loc_get_data_period(curTagIdx);
				curConMarg=loc_get_data_conf(curTagIdx);
				curIndx++;
				rdBuffer[curIndx++]=traj_get_velX_MSB();
				rdBuffer[curIndx++]=traj_get_velX_LSB();
				rdBuffer[curIndx++]=traj_get_thetaZ_MSB();
				rdBuffer[curIndx++]=traj_get_thetaZ_LSB();
				rdBuffer[curIndx++]=curSegment;
				rdBuffer[curIndx++]=loc_get_data_location(curTagIdx); 	//tire location (1-LR 2-LF 3-RF 4-RR)  and segment indicator (0-Park, 1- FWD, 2 Right turn, 3 left turn)
				rdBuffer[curIndx++]=loc_get_data_direction(curTagIdx);	//rotation direction
				rdBuffer[curIndx++]=(uint8_t)(curPeriod>>Shift1_Byte);	//rotation period MSB
				rdBuffer[curIndx++]=(uint8_t)curPeriod;					//rotation period LSB
				rdBuffer[curIndx++]=(uint8_t)(curConMarg>>Shift1_Byte); //confidence margin MSB
				rdBuffer[curIndx++]=(uint8_t)curConMarg;				//confidence margin LSB
				rdBuffer[curIndx++]=(uint8_t)(curTick >> Shift3_Byte); 	//timestamp 1ms
				rdBuffer[curIndx++]=(uint8_t)(curTick >> Shift2_Byte);
				rdBuffer[curIndx++]=(uint8_t)(curTick >> Shift1_Byte);
				rdBuffer[curIndx++]=(uint8_t)(curTick);
				rdBuffer[curIndx++]=POSTAMBL;
				rdBuffer[curIndx]=POSTAMBL;
				rdBuffer[IND_PREAM]=PREAMBL;
				rdBuffer[IND_PLD_LENGTH]=curIndx++;
				sio_send_msg(&rdBuffer[0], curIndx);
			}
		}
		//check if user requested to stop the routine
		if(sio_get_stop_char())
		{
			//mkw01_drv_set_op_mode(STANDBY);
			traj_stop();
			return;
		}
	}
	return;
}

/************************************************************************
 * discover_tpms() function detects and stores a list of active
 * TPMS transmitters (used for sensor Array Tag ID initialization)
 * Parameters - none
 * returns - nothing
 * ************************************************************************/
void discover_tpms()
{
	uint32_t curTagID;
	loc_reset_data_all();
	sio_init_stop_char();
	//mkw01_drv_set_op_mode(RECEIVE);
	while(1)
	{
		if (IS_DATARDY)
		{
			curTagID=get_tpms_packet(); //Tire ID
			if(loc_store_id(curTagID)<0)
			{
				sio_end_stop_char();
				//mkw01_drv_set_op_mode(STANDBY);
				return;
			}
		}
		if(sio_get_stop_char())
		{
			//mkw01_drv_set_op_mode(STANDBY);
			return;
		}
	}
	return;
}
/************************************************************************
 * get_tpms_data() function streams TPMS data as it arriving to receiver's FIFO
 * Streaming stops once user sends any char over UART to interrupt
 * Parameters - none
 * returns - nothing
 * ************************************************************************/
void get_tpms_data()
{
	uint32_t curTagID=0;
	uint8_t  curIndx;
	sio_init_stop_char();
	//mkw01_drv_set_op_mode(RECEIVE);
	//mkw01_drv_start_rssi();
	curTick=0;
	while(1)
	{
		if (IS_DATARDY)
		{
			curTagID=get_tpms_packet();
			if(loc_get_tag_index(curTagID)>=0) //tag is in the white list
			{
				curIndx=rdBuffer[0]+1;
				rdBuffer[curIndx++]=(uint8_t)(curTick >> Shift3_Byte); //timestamp 1ms
				rdBuffer[curIndx++]=(uint8_t)(curTick >> Shift2_Byte);
				rdBuffer[curIndx++]=(uint8_t)(curTick >> Shift1_Byte);
				rdBuffer[curIndx++]=(uint8_t)(curTick);
				rdBuffer[curIndx++]=POSTAMBL;
				rdBuffer[curIndx]=POSTAMBL;
				rdBuffer[IND_PREAM]=PREAMBL;
				rdBuffer[IND_PLD_LENGTH]=curIndx++;//add preamble byte to total length
				sio_send_msg(&rdBuffer[0], curIndx);
			}
		}
		if(sio_get_stop_char())
		{
			//mkw01_drv_set_op_mode(STANDBY);
			return;
		}

	}
	return;
}
/************************************************************************
 * get_tpms_packet() function receives single TPMS data sample
 * and saves it in the rdBuffer array for the caller to process
 * Parameters - none
 * returns - tag ID for ID filtering purpose
 * ************************************************************************/
uint32_t get_tpms_packet()
{
	uint8_t curIndx;
	uint8_t rssiVal;
	//mkw01_drv_set_op_mode(STANDBY);
	//mkw01_drv_read_register(REGFIFO, &curIndx);//get payload length
	//mkw01_drv_read_fifo(curIndx+1,&rdBuffer[0]);
	while (IS_FIFO)
	{
		//mkw01_drv_read_register(REGFIFO, &rssiVal); //empty extra data
	}
	//mkw01_drv_read_register(REGVALRSSI, &rssiVal);
	//mkw01_drv_start_rssi();
	//mkw01_drv_set_op_mode(RECEIVE);
	curIndx++;
	rdBuffer[curIndx]=rssiVal;	//RSSI
	rdBuffer[0]=curIndx;
	return (uint32_t)(rdBuffer[IND_ID1] << Shift3_Byte)|(uint32_t)(rdBuffer[IND_ID2] << Shift2_Byte)|(uint32_t)(rdBuffer[IND_ID3] << Shift1_Byte)|(uint32_t)(rdBuffer[IND_ID4]); //Tire ID
}
/**********************************************************************
 * send_val(uint8_t msgVal)
 * function sends single hex value over serial io (hyperterminal mode)
 * function sends formatted message packet (LV GUI mode)
 * returns - nothing
 * **********************************************************************/
void send_val(uint8_t msgVal)
{
	uint8_t curIndx=IND_PLD_START;
#ifdef TERMINAL
	uint8_t sendVal=0;
	sendVal=(msgVal>>4);
	if (sendVal<(0x0A))
	{
		sendVal|=(0x30);
	}
	else
	{
		sendVal=sendVal-9;
		sendVal|=(0x40);
	}
	sio_send_rsp(&sendVal, TXSINGLE);
	sendVal=(msgVal & 0x0F);
	if (sendVal<(0x0A))
	{
		sendVal|=(0x30);
	}
	else
	{
		sendVal=sendVal-9;
		sendVal|=(0x40);
	}
	sio_send_rsp(&sendVal, TXSINGLE);
#else
	rdBuffer[curIndx++]=msgVal;
	rdBuffer[curIndx++]=POSTAMBL_RSP;
	rdBuffer[curIndx]=POSTAMBL_RSP;
	rdBuffer[IND_PREAM]=PREAMBL_RSP;
	rdBuffer[IND_PLD_LENGTH]=curIndx++;
	sio_send_rsp(&rdBuffer[0], curIndx);
#endif
}
/* END rtos_main_task */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/

