/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <lpc21xx.h>
#include "GPIO.h"
#include <string.h>

extern unsigned int _1st_button;
extern unsigned int _2nd_button;
extern unsigned int _1st_load;
extern unsigned int _2nd_load;
extern unsigned int Transmit_UART;
extern unsigned int Receiv_UART;
extern unsigned int SumExe;
extern unsigned int LoadCPU;

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/
 

#define configUSE_EDF_SCHEDULER		1
#define configUSE_PREEMPTION		1
#define configUSE_IDLE_HOOK			0
#define configUSE_TICK_HOOK			1
#define configCPU_CLOCK_HZ			( ( unsigned long ) 60000000 )	/* =12.0MHz xtal multiplied by 5 using the PLL. */
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES		( 4 )
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 90 )
#define configTOTAL_HEAP_SIZE		( ( size_t ) 13 * 1024 )
#define configMAX_TASK_NAME_LEN		( 8 )
#define configUSE_TRACE_FACILITY	1
#define configUSE_16_BIT_TICKS		0
#define configIDLE_SHOULD_YIELD		1
#define configQUEUE_REGISTRY_SIZE 	0
/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1


#define TASK_NAME			(pxCurrentTCB->pcTaskName) 
#define SET_PIN(PORTX,PINx)		GPIO_write( PORTX , PINx , PIN_IS_HIGH)
#define CLEAR_PIN(PORTX,PINx)		GPIO_write( PORTX , PINx , PIN_IS_LOW) 
#define configUSE_STATS_FORMATTING_FUNCTIONS 1
#define configGENERATE_RUN_TIME_STATS 	1
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
#define portGET_RUN_TIME_COUNTER_VALUE()	(T1TC)
  
	
#define traceTASK_SWITCHED_IN()			 	if (strcmp( TASK_NAME, "Button_1_Monitor" ) == 0 ){GPIO_write( PORT_0 , PIN2 , PIN_IS_HIGH) ; _1st_button = T1TC;}  	\
																			if (strcmp( TASK_NAME, "Button_2_Monitor" ) == 0 ){GPIO_write( PORT_0 , PIN3 , PIN_IS_HIGH) ; _2nd_button = T1TC;}  	\
																			if (strcmp( TASK_NAME, "UART Receiver" ) == 0 ){GPIO_write( PORT_0 , PIN4 , PIN_IS_HIGH) ; Transmit_UART = T1TC;}  	\
																			if (strcmp( TASK_NAME, "Periodic Transmitter" ) == 0 ){GPIO_write( PORT_0 , PIN5 , PIN_IS_HIGH) ; Receiv_UART = T1TC;}  	\
																			if (strcmp( TASK_NAME, "Load_1_Simulation" ) == 0 ){GPIO_write( PORT_0 , PIN6 , PIN_IS_HIGH) ; _1st_load = T1TC;}  	\
																			if (strcmp( TASK_NAME, "Load_2_SimulationLoad_1_Simulation" ) == 0 ){GPIO_write( PORT_0 , PIN7 , PIN_IS_HIGH) ; _2nd_load = T1TC;}  	\
																			if (strcmp( TASK_NAME, "IDLE" ) == 0 ){GPIO_write( PORT_0 , PIN8 , PIN_IS_HIGH); }  				\
										 while(0)


#define traceTASK_SWITCHED_OUT()			if (strcmp( TASK_NAME, "Button_1_Monitor" ) == 0 ){GPIO_write( PORT_0 , PIN2 , PIN_IS_LOW) ; SumExe += T1TC-_1st_button;}  \
																			if (strcmp( TASK_NAME, "Button_2_Monitor" ) == 0 ){GPIO_write( PORT_0 , PIN3 , PIN_IS_LOW ); SumExe += T1TC-_2nd_button;}  \
																			if (strcmp( TASK_NAME, "UART Receiver" ) == 0 ){GPIO_write( PORT_0 , PIN4 , PIN_IS_LOW ); SumExe += T1TC-Transmit_UART;}  \
																			if (strcmp( TASK_NAME, "Periodic Transmitter" ) == 0 ){GPIO_write( PORT_0 , PIN5 , PIN_IS_LOW ); SumExe += T1TC-Receiv_UART;}  \
																			if (strcmp( TASK_NAME, "Load_1_Simulation" ) == 0 ){GPIO_write( PORT_0 , PIN6 , PIN_IS_LOW ); SumExe += T1TC-_1st_load;}  \
																			if (strcmp( TASK_NAME, "Load_2_Simulation" ) == 0 ){GPIO_write( PORT_0 , PIN7 , PIN_IS_LOW); SumExe += T1TC-_2nd_load;}  \
																			if (strcmp( TASK_NAME, "IDLE" ) == 0 ){GPIO_write( PORT_0 , PIN8 , PIN_IS_LOW ); }  						\
										 LoadCPU = SumExe * 100 / T1TC;														\
										 while(0)
#endif /* FREERTOS_CONFIG_H */  
										 