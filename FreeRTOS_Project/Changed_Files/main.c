/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Bit_Math.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"
#include "queue.h"
/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"


/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )

/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/

/*Variables used in exe*/
unsigned int _1st_button,_2nd_button,_1st_load,_2nd_load,Transmit_UART,Receiv_UART,SumExe,LoadCPU;

/*All task Handlers in exe*/
BaseType_t xReturned;
TaskHandle_t B1_taskHandler,B2_taskHandler,Trans_TaskHandler,Receiv_TaskHandler,L1_TaskHandler,L2_TaskHandler;
QueueHandle_t q1,q2,q3;

void Periodic_Transmitter (void * pvParameters );
void Uart_Receiver (void * pvParameters );
void Button_1_Monitor( void * pvParameters );
void Load_2_Simulation ( void * pvParameters );
void Button_2_Monitor( void * pvParameters );
void Load_1_Simulation ( void * pvParameters );


char RT_state[400];

/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
 
 /*Tasks Implemntations */
 void Periodic_Transmitter (void * pvParameters )
{   
	uint8_t i = 0,size=15;
	char stringToSend[15] = "\nnew period.";
	TickType_t final_WT = xTaskGetTickCount();
	for( ; ; )
	{
		for( i=0 ; i<size ; i++)
		{	//Transmit the string
			xQueueSend( q3 , stringToSend+i ,100); 
		}
		
		GPIO_write(PORT_1,PIN2,PIN_IS_LOW);
		vTaskDelayUntil( &final_WT , 100);
		
		GPIO_write(PORT_1,PIN2,PIN_IS_HIGH);
		
		GPIO_write(PORT_1,PIN0,PIN_IS_LOW);
		GPIO_write(PORT_1,PIN1,PIN_IS_LOW);
		
		GPIO_write(PORT_1,PIN3,PIN_IS_LOW);
		
		GPIO_write(PORT_1,PIN4,PIN_IS_LOW);
		GPIO_write(PORT_1,PIN5,PIN_IS_LOW);
		GPIO_write(PORT_1,PIN6,PIN_IS_LOW);
	}
}


void Uart_Receiver (void * pvParameters )
{   
	uint8_t size = 15,i=0;
	signed char B1, B2;
	//get the tick count
	TickType_t final_WT = xTaskGetTickCount();
	char recv_arr[15];
	for( ; ; )
	{
		if( xQueueReceive( q1, &B1 , 0) && B1 != '.')
		{
			//first queue
		xSerialPutChar('\n');	
			if(B1=='+')
			{   char pos_string[]="Positive edge from button1\n";
							vSerialPutString((signed char *) pos_string, strlen(pos_string));
			}
			else
			{  char neg_string[]="Negative edge from button1\n";
							vSerialPutString((signed char *) neg_string, strlen(neg_string));
			}
		}
		else
		{
			//add space
			int x=0;
			for(x=0;x<5;x++){
			xSerialPutChar(' ');		
}
		}
		if( xQueueReceive( q2, &B2 , 0) && B2 != '.')
		{
			//second queue
			xSerialPutChar('\n');		
			if(B2=='+')
			{ char pos_string[]=" Positive edge from button 2 \n";
							vSerialPutString((signed char *) pos_string, strlen(pos_string));
			}
			else
			{	char neg_string[]=" Negative edge from button 2 \n";
							vSerialPutString((signed char *) neg_string, strlen(neg_string));
			}
		}
		else
		{
		 //add space
			int x=0;
			for(x=0;x<5;x++){
			xSerialPutChar(' ');		
}
		}
		
		//third queue
		if( uxQueueMessagesWaiting(q3) >0)
		{
			for( i=0 ; i<size ; i++)
			{
				xQueueReceive( q3, (recv_arr+i) , 0);
			}
			vSerialPutString( (signed char *) recv_arr, strlen(recv_arr));
			xQueueReset(q3);
		}
				
		GPIO_write(PORT_1,PIN3,PIN_IS_LOW);
		vTaskDelayUntil( &final_WT , 20);
		GPIO_write(PORT_1,PIN3,PIN_IS_HIGH);
	
		GPIO_write(PORT_1,PIN0,PIN_IS_LOW);
		GPIO_write(PORT_1,PIN1,PIN_IS_LOW);
		
		GPIO_write(PORT_1,PIN2,PIN_IS_LOW);
		GPIO_write(PORT_1,PIN4,PIN_IS_LOW);
		
		GPIO_write(PORT_1,PIN5,PIN_IS_LOW);
		
		GPIO_write(PORT_1,PIN6,PIN_IS_LOW);
	}
}

void Button_1_Monitor( void * pvParameters )
{
	pinState_t B1_status, prev_status = GPIO_read(PORT_0 , PIN0); /*pin 16 */
	TickType_t final_WT = xTaskGetTickCount();
	signed char edges = 0;
	for( ;; )
	{
	B1_status = GPIO_read(PORT_0 , PIN0);
	if( B1_status == PIN_IS_HIGH && prev_status == PIN_IS_LOW)
		{
	//a positive edge is assigned
		   	edges = '+';
		}
	else if (B1_status == PIN_IS_LOW && prev_status == PIN_IS_HIGH)
	{
	//a negative edge is assigned
		edges = '-';	
		}
		else
		{	//not positive nor negative
			edges = '.';
		}
		// update the prev state to the current state
		prev_status = B1_status;
		
		//send the corresponding data
		xQueueOverwrite( q1 , &edges );
		
	  GPIO_write(PORT_1,PIN0,PIN_IS_LOW);
		vTaskDelayUntil( &final_WT ,50);
		GPIO_write(PORT_1,PIN0,PIN_IS_HIGH);
		GPIO_write(PORT_1,PIN1,PIN_IS_LOW);
		
		GPIO_write(PORT_1,PIN2,PIN_IS_LOW);
		GPIO_write(PORT_1,PIN3,PIN_IS_LOW);
		
		GPIO_write(PORT_1,PIN4,PIN_IS_LOW);
		GPIO_write(PORT_1,PIN5,PIN_IS_LOW);
		GPIO_write(PORT_1,PIN6,PIN_IS_LOW);
	}
}

void Button_2_Monitor( void * pvParameters )
{
	pinState_t B2_status,prev_status = GPIO_read(PORT_0 , PIN1);
	//get the tick count
	TickType_t final_WT = xTaskGetTickCount();
	signed char edges = 0;
	for( ;; )
	{
		B2_status = GPIO_read(PORT_0 , PIN1);
	if( B2_status == PIN_IS_HIGH && prev_status == PIN_IS_LOW)
		{
		//a positive edge is assigned
			edges = '+';
		}
	else if (B2_status == PIN_IS_LOW && prev_status == PIN_IS_HIGH)
		{
	//a negative edge is assigned
			edges = '-';
		}
		else
		{
	  //not positive nor negative
		edges = '.';
		}
	prev_status = B2_status;		
		// update the prev state to the current state
		//send the corresponding data
		xQueueOverwrite( q2 , &edges ); 
		
		GPIO_write(PORT_1,PIN1,PIN_IS_LOW);
		vTaskDelayUntil( &final_WT , 50);
		
		GPIO_write(PORT_1,PIN1,PIN_IS_HIGH);

		GPIO_write(PORT_1,PIN0,PIN_IS_LOW);
		GPIO_write(PORT_1,PIN2,PIN_IS_LOW);
		GPIO_write(PORT_1,PIN3,PIN_IS_LOW);
		
		
		GPIO_write(PORT_1,PIN4,PIN_IS_LOW);
		GPIO_write(PORT_1,PIN5,PIN_IS_LOW);
		GPIO_write(PORT_1,PIN6,PIN_IS_LOW);
	}
}


	




void Load_1_Simulation ( void * pvParameters )
{
	uint32_t i = 0;
	//get the tick count
	TickType_t final_WT = xTaskGetTickCount();

	for( ; ; )
	{ //using 7 ms of total delay
	for( i=0 ; i <= 12000*7; i++);
		
		GPIO_write(PORT_1,PIN5,PIN_IS_LOW);
		vTaskDelayUntil( &final_WT , 20);
		GPIO_write(PORT_1,PIN5,PIN_IS_HIGH);
		GPIO_write(PORT_1,PIN0,PIN_IS_LOW);
		
		GPIO_write(PORT_1,PIN1,PIN_IS_LOW);
		GPIO_write(PORT_1,PIN2,PIN_IS_LOW);
		
		GPIO_write(PORT_1,PIN4,PIN_IS_LOW);
		GPIO_write(PORT_1,PIN3,PIN_IS_LOW);
		GPIO_write(PORT_1,PIN6,PIN_IS_LOW);
	}
}

void Load_2_Simulation ( void * pvParameters )
{
	uint32_t i = 0;
		//get the tick count
	TickType_t final_WT = xTaskGetTickCount();
	for( ; ; )
	{	//using 13 ms of total delay
	for( i=0 ; i <= 12000*12; i++);

		GPIO_write(PORT_1,PIN6,PIN_IS_LOW);
		vTaskDelayUntil( &final_WT , 100);
		
		GPIO_write(PORT_1,PIN6,PIN_IS_HIGH);
		GPIO_write(PORT_1,PIN0,PIN_IS_LOW);
		
		GPIO_write(PORT_1,PIN1,PIN_IS_LOW);
		
		GPIO_write(PORT_1,PIN2,PIN_IS_LOW);
		GPIO_write(PORT_1,PIN4,PIN_IS_LOW);
		GPIO_write(PORT_1,PIN3,PIN_IS_LOW);
			
		GPIO_write(PORT_1,PIN5,PIN_IS_LOW);
	}
}
 
 
int main( void )
{
	/*Hardware setup */
	prvSetupHardware();
  // assigning each queue to a xcreated queue
	q1 = xQueueCreate( 1, sizeof(char) );
	q2 = xQueueCreate( 1, sizeof(char) );
	q3 = xQueueCreate( 15, sizeof(char) );
    /* Create Tasks here */  
	
	xTaskPeriodicCreate(
                    Button_1_Monitor,       /* Function that implements the task. */
                    "first",     /* Text name for the task. */
                    100,      				/* Stack size in words, not bytes. */
                    ( void * ) 0,    		/* Parameter passed into the task. */
                    1,						/* Priority at which the task is created. */
                    &B1_taskHandler,		/* Used to pass out the created task's handle. */
										50);      				/* Task Deadline */
					
	xTaskPeriodicCreate(
                    Button_2_Monitor,       /* Function that implements the task. */
                    "second",     /* Text name for the task. */
                    100,      				/* Stack size in words, not bytes. */
                    ( void * ) 0,    		/* Parameter passed into the task. */
                    1,						/* Priority at which the task is created. */
                    &B2_taskHandler,		/* Used to pass out the created task's handle. */
										50);      				/* Task Deadline */
										
	xTaskPeriodicCreate(
                    Periodic_Transmitter,       /* Function that implements the task. */
                    "Trans",     /* Text name for the task. */
                    100,      				/* Stack size in words, not bytes. */
                    ( void * ) 0,    		/* Parameter passed into the task. */
                    1,						/* Priority at which the task is created. */
                    &Trans_TaskHandler,		/* Used to pass out the created task's handle. */
										100);  
										
	xTaskPeriodicCreate(
                    Uart_Receiver,       /* Function that implements the task. */
                    "Recv",     /* Text name for the task. */
                    100,      				/* Stack size in words, not bytes. */
                    ( void * ) 0,    		/* Parameter passed into the task. */
                    1,						/* Priority at which the task is created. */
                    &Receiv_TaskHandler,		/* Used to pass out the created task's handle. */
										20);      				/* Task Deadline */


	xTaskPeriodicCreate(
                    Load_1_Simulation,       /* Function that implements the task. */
                    "load1",     /* Text name for the task. */
                    100,      				/* Stack size in words, not bytes. */
                    ( void * ) 0,    		/* Parameter passed into the task. */
                    1,						/* Priority at which the task is created. */
                    &L1_TaskHandler,		/* Used to pass out the created task's handle. */
										10);      				/* Task Deadline */
										
										
	xTaskPeriodicCreate(
                    Load_2_Simulation,       /* Function that implements the task. */
                    "load2",     /* Text name for the task. */
                    100,      				/* Stack size in words, not bytes. */
                    ( void * ) 0,    		/* Parameter passed into the task. */
                    1,						/* Priority at which the task is created. */
                    &L2_TaskHandler,		/* Used to pass out the created task's handle. */
										100); 
										
										/* Task Deadline */	
	/* Now all the tasks have been started - start the scheduler.
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();
	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationTickHook (void)
{
	GPIO_write(PORT_0,PIN9, PIN_IS_HIGH);
	GPIO_write(PORT_0,PIN9, PIN_IS_LOW);
}

//initialize timer1
static void configTimer1(void)
{  T1PR = 1000; 
}

//reset timer1
void timer1Reset(void)
{  T1TCR |= 0x2; T1TCR &= ~0x2;
}

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();
	
	/* Config trace timer 1 and read T1TC to get current tick */
	configTimer1();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/


