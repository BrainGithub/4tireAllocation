/*!
** @file rtos_main_task.h
** @version 01.00
** @brief
*/         
/*!
**  @addtogroup rtos_main_task_module rtos_main_task module documentation
**  @{
*
*
* (c) Copyright 2016, Freescale Semiconductor, Inc. All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
************************************************************************************/

#ifndef __rtos_main_task_H
#define __rtos_main_task_H
/* MODULE rtos_main_task */

//zxp
//#include "fsl_device_registers.h"
//#include "clockMan1.h"
//#include "pin_init.h"
//#include "osa1.h"
//#include "mqx_ksdk.h"
//#include "uart1.h"
//#include "fsl_hwtimer1.h"
//#include "MainTask.h"
//#include "gpio1.h"
//#include "hwTim1.h"

#ifdef __cplusplus
extern "C" {
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
//void main_task(os_task_param_t task_init_data);
void main_task(); //zxp
/**********************************************************************
 * send_val(uint8_t msgVal)
 * function sends single hex value over UART using blocking method if hyperterminal is used
 * function sends formatted message if LV GUI is used
 * returns - nothing
 * **********************************************************************/
void send_val(uint8_t msgVal);
/**********************************************************************
 * get_span_data(uint32_t curFrf, uint16_t curSpan, uint16_t nPts)
 * function configures and executes RSSI spectrum data collection
 * curFrf - carrier or center frequency
 * curSpan - half frequency sweep range
 * if curSpan is zero function is used to evaluate RSSI time series
 * nPts - number of data points to acquire
 * returns- 1 if user requested stop otherwise returns 0
 * **********************************************************************/
uint8_t get_span_data(uint32_t curFrf, uint16_t curSpan, uint16_t nPts);
/************************************************************************
 * get_tpms_data() function streams TPMS data as it arriving to receiver's FIFO
 * Streaming stops once user sends any char over UART to interrupt
 * Parameters - none
 * returns - nothing
 * ************************************************************************/
void get_tpms_data();
/************************************************************************
 * get_motion_data() function streams gyro or accel data from AGM01
 * Streaming stops once user sends any char over UART to interrupt
 * Parameters - none
 * returns - nothing
 * ************************************************************************/
void get_motion_data(uint8_t curDev);
/************************************************************************
 * get_locator_data() function streams TPMS and sampled vehicle data
 * Streaming stops once user sends any char over UART to interrupt
 * Parameters - none
 * returns - nothing
 * ************************************************************************/
void get_locator_data();
/************************************************************************
 * discover_tpms() function detects and stores a list of active
 * TPMS transmitters (used for sensor Array Tag ID initialization)
 * Parameters - none
 * returns - nothing
 * ************************************************************************/
void discover_tpms();
/************************************************************************
 * get_tpms_packet() function receives single TPMS data sample
 * and saves it in the rdBuffer array for the caller to process
 * Parameters - none
 * returns - tag ID for ID filtering purpose
 * ************************************************************************/
uint32_t get_tpms_packet();
/**********************************************************************
 * reset_XCVR(uint8_t curConfig)
 * function configures receiver
 * curConfig - current RF configuration
 * returns- nothing
 * **********************************************************************/
void reset_xcvr(uint8_t curConfig);
volatile uint32_t curTick;

/* END rtos_main_task */
#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __rtos_main_task_H*/
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
