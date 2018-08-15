/* ###################################################################
**     Filename    : Events.h
**     Project     : KW01_TPMS_LPSCI
**     Processor   : MKW01Z128CHN4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-10-05, 14:37, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         Cpu_OnNMI - void Cpu_OnNMI(void);
**
** ###################################################################*/
/*!
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "myinc.h"
//#include "fsl_device_registers.h"
//#include "clockMan1.h"
//#include "pin_init.h"
//#include "osa1.h"
//#include "mqx_ksdk.h"
//#include "uart1.h"
//#include "fsl_hwtimer1.h"
//#include "MainTask.h"
//#include "lpsciCom1.h"
//#include "spiCom1.h"
//#include "gpio1.h"
//#include "hwTim1.h"
//#include "i2cCom1.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*
** ===================================================================
**     Event       :  Cpu_OnNMI (module Events)
**
**     Component   :  Cpu [MKW01Z128HN4_4SDK]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMI(void);


void uart1_TxCallback(uint32_t instance, void * uartState);
void uart1_RxCallback(uint32_t instance, void * uartState);
/*! spiCom1 IRQ handler */
void spiCom1_IRQHandler(void);

void lpsciCom1_RxCallback(uint32_t instance, void * uartState);

void hwTim1_OnTimeOut(void* data);

/*! ISF_I2C1 IRQ handler */
void ISF_I2C1_IRQHandler(void);

/*! i2cCom1 IRQ handler */
void i2cCom1_IRQHandler(void);

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
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
