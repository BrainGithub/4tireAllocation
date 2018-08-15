/* ###################################################################
**     Filename    : main.c
**     Project     : KW01_TPMS_LPSCI
**     Processor   : MKW01Z128CHN4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */

//just for simm
#define SIMM_LINUX

#ifdef SIMM_LINUX

#include <stdio.h>
#include <stdlib.h>
#include "myinc.h"
#include "rtos_main_task.h"
#include "simm_emitter_task.h"


#else

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "clockMan1.h"
#include "pin_init.h"
#include "osa1.h"
#include "mqx_ksdk.h"
#include "uart1.h"
#include "fsl_hwtimer1.h"
#include "MainTask.h"
#include "lpsciCom1.h"
#include "spiCom1.h"
#include "gpio1.h"
#include "hwTim1.h"
#include "i2cCom1.h"
#if CPU_INIT_CONFIG
  #include "Init_Config.h"
#endif

#endif

int main(void)
{
	printf("start main task\n");

	pthread_t tid;
	if ((pthread_create(&tid, NULL, emitter_task, (void*)0)) == -1)
	{
		printf("create error!\n");
		return -1;
	}

	sleep(1);
	main_task();

	

	for(;;)
	{
		sleep(1);
		printf("in waiting loop\n");
	}


	pthread_join(tid, 0);
	return 0;
}

/* END main */
