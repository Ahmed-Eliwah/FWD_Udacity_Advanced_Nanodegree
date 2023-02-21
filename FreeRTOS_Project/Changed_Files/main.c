#include "app.h"

// Macros for Each Task Period(DeadLine)
#define BUTTON_ONE_PERIOD              			50
#define BUTTON_TWO_PERIOD              			50
#define UART_TRANSC_PERIOD            			20
#define PERIODC_TRANSMIT_PERIOD             100
#define LOAD_ONE_PERIOD             				10
#define LOAD_TWO_PERIOD              				100


/*-----------------------------------------------------------*/

/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/

 
 /*Tasks Implemntations */
 
 /*
 Task One --> Button_1_Monitor
 
 The Functionality of this task is to catch any change on the Port0-Pin0 that refers to button one
 
 and send data if any changes happend to the consumer task
 
 */
 
 void Button_1_Monitor( void * pvParameters )
{
	pinState_t Button_one_current_state;
	
	pinState_t Button_one_previous_state = GPIO_read(PORT_0 , PIN0);
	
	TickType_t Current_Tick = xTaskGetTickCount();
	
	char Edge_Sign = 0;
	
	
	for( ;; )
	{
		//Get the Curent status of the pin0
		Button_one_current_state = GPIO_read(PORT_0 , PIN0);
		
		if( Button_one_current_state == PIN_IS_HIGH && Button_one_previous_state == PIN_IS_LOW)
		{
	
			// The last status was low and now is high so this is a positive edge
			
		   	Edge_Sign = 'p';
		}
		
		else if (Button_one_current_state == PIN_IS_LOW && Button_one_previous_state == PIN_IS_HIGH)
		{
				
			// The last status was high and now is low so this is a negative edge
			
				Edge_Sign = 'n';	
		}
		
		// This part for protection if there's no change on the button one pin
		else
		{	
				Edge_Sign = '.';
		}
		
		// Update the previous state of the Button1 with the current state
		Button_one_previous_state = Button_one_current_state;
		
		//Send data to the UART Recieve Task to print the Right string
		
		xQueueOverwrite( Q_1 , &Edge_Sign );
		
		
		// This part for simulation to visulaize when this task come in the logic analyzer
		GPIO_write(PORT_0,PIN0,PIN_IS_LOW);
		vTaskDelayUntil( &Current_Tick ,50);
		GPIO_write(PORT_0,PIN0,PIN_IS_HIGH);
		
	}
}


 /*
 Task One --> Button_2_Monitor
 
 The Functionality of this task is to catch any change on the Port0-Pin1 that refers to button two
 
 and send data if any changes happend to the consumer task
 
 */

void Button_2_Monitor( void * pvParameters )
{
	pinState_t Button_two_current_state;
	
	pinState_t Button_two_previous_state = GPIO_read(PORT_0 , PIN1);
	
	TickType_t Current_Tick = xTaskGetTickCount();
	
	char Edge_Sign = 0;
	
	
	for( ;; )
	{
		//Get the Curent status of the pin0
		Button_two_current_state = GPIO_read(PORT_0 , PIN1);
		
		if( Button_two_current_state == PIN_IS_HIGH && Button_two_previous_state == PIN_IS_LOW)
		{
	
			// The last status was low and now is high so this is a positive edge
			
		   	Edge_Sign = 'p';
		}
		
		else if (Button_two_current_state == PIN_IS_LOW && Button_two_previous_state == PIN_IS_HIGH)
		{
				
			// The last status was high and now is low so this is a negative edge
			
				Edge_Sign = 'n';	
		}
			
		// This part for protection if there's no change on the button two pin
		else
		{	
				Edge_Sign = '.';
		}
		
		// Update the previous state of the Button1 with the current state
		Button_two_previous_state = Button_two_current_state;
		
		//Send data to the UART Recieve Task to print the Right string
		xQueueOverwrite( Q_2 , &Edge_Sign ); 
		
		
		// This part for simulation to visulaize when this task come in the logic analyzer
		GPIO_write(PORT_0,PIN1,PIN_IS_LOW);
		vTaskDelayUntil( &Current_Tick , 50);
		GPIO_write(PORT_0,PIN1,PIN_IS_HIGH);

	}
}

 
 
 
 /*
 Task Three --> Periodic Transmit
 
 This Function Will send Peiodic string which is New_Period Every 100ms to the conumer task (UART Transc) 
 
 */
 void Periodic_Transmitter (void * pvParameters )
{   
	
	TickType_t Current_Tick = xTaskGetTickCount();
	
	char String_s[] = "\nNew_Period.";
	
	int size = sizeof(String_s)/sizeof(String_s[0]);
	int i =0;
	
	
	for(;;)
	{
		for(i=0 ; i<size ; i++)
		{	
			xQueueSend(Q_3 , String_s+i ,100); 
		}
		
		// This part for simulation to visulaize when this task come in the logic analyzer
		GPIO_write(PORT_0,PIN2,PIN_IS_LOW);
		vTaskDelayUntil( &Current_Tick , 100);
		GPIO_write(PORT_0,PIN2,PIN_IS_HIGH);
		
	}
}



/*
Task Four --> The Conusmer Task (UART Transciever)

Thsi Functionnality of this task is to recieve the data from other task and diplay it on the screen 
by using UART communication protocol 

*/
void Uart_Receiver (void * pvParameters )
{   
	uint8_t size = 15, i = 0;
	char Button_one_edge, Button_two_edge;
	
	TickType_t Current_Tick = xTaskGetTickCount();
	
	char String_Recieved[15];
	char Positive_Edge[]="\nPositive Edge Catched\n";
	char Negative_Edge[]="\nNegative Edge Catched\n";
	
	for( ; ; )
	{
		// Check if the data is coming from Queue one which refers to Button one Task,
		// and make sure that there's a chnage happend on the button one pin
		if( (xQueueReceive(Q_1, &Button_one_edge , 0)) && (Button_one_edge != '.'))
		{
			switch(Button_one_edge)
			{
				// If the Recieved char is p, this means that a positive edge is detected in button one pin
				case 'p': vSerialPutString((signed char *) Positive_Edge, strlen(Positive_Edge));
									break;
				// If the Recieved char is n, this means that a negtaive edge is detected in button one pin
				case 'n':	vSerialPutString((signed char *) Negative_Edge, strlen(Negative_Edge));
									break;	
			}	
		}
		
		
		// Check if the data is coming from Queue Two which refers to Button Two Task,
		// and make sure that there's a chnage happend on the button Two pin
		if( (xQueueReceive(Q_2, &Button_two_edge , 0)) && (Button_two_edge != '.'))
		{
			switch(Button_two_edge)
			{
				// If the Recieved char is p, this means that a positive edge is detected in button two pin
				case 'p': vSerialPutString((signed char *) Positive_Edge, strlen(Positive_Edge));
									break;
				// If the Recieved char is n, this means that a negtaive edge is detected in button two pin
				case 'n':	vSerialPutString((signed char *) Negative_Edge, strlen(Negative_Edge));
									break;
			}		
		}
		
		// Here, we recieve the data that is coming from the Periodic Transmit Task Every 100ms
		if( uxQueueMessagesWaiting(Q_3) >0)
		{
			for(i=0 ; i<size ; i++)
			{
				xQueueReceive( Q_3, (String_Recieved+i) , 0);
			}
			vSerialPutString( (signed char *) String_Recieved, strlen(String_Recieved));
			xQueueReset(Q_3);
		}
			

		// This part for simulation to visulaize when this task come in the logic analyzer
		GPIO_write(PORT_0,PIN3,PIN_IS_LOW);
		vTaskDelayUntil( &Current_Tick , 20);
		GPIO_write(PORT_0,PIN3,PIN_IS_HIGH);
	}
}


 /*
 Task Five --> Load_1_Simulation
 
 The Functionality of this task is to make load on the cpu to see if this will cause CPU load or not
 
 */
void Load_1_Simulation ( void * pvParameters )
{
	uint32_t i = 0;
	TickType_t Current_Tick = xTaskGetTickCount();

	for( ; ; )
	{
		// We use 12Mhz CPU so 12000 is equal to 1ms ,, we need 5ms so we need 5 x 12000 counts 
		
		for( i=0 ; i <= 12000*5; i++);
		
		// This part for simulation to visulaize when this task come in the logic analyzer
		GPIO_write(PORT_0,PIN5,PIN_IS_LOW);
		vTaskDelayUntil( &Current_Tick , 20);
		GPIO_write(PORT_0,PIN5,PIN_IS_HIGH);
		
	}
}


 /*
 Task Five --> Load_2_Simulation
 
 The Functionality of this task is to make load on the cpu to see if this will cause CPU load or not
 
 */
void Load_2_Simulation ( void * pvParameters )
{
	uint32_t i = 0;
	
	TickType_t Current_Tick = xTaskGetTickCount();
	for( ; ; )
	{
		// We use 12Mhz CPU so 12000 is equal to 1ms ,, we need 12ms so we need 12 x 12000 counts
		for( i=0 ; i <= 12000*12; i++);

		// This part for simulation to visulaize when this task come in the logic analyzer
		GPIO_write(PORT_0,PIN6,PIN_IS_LOW);
		vTaskDelayUntil( &Current_Tick , 100);
		GPIO_write(PORT_0,PIN6,PIN_IS_HIGH);
		
	}
}
 
 
int main( void )
{
	/*Hardware setup */
	prvSetupHardware();
 
  // Here we create three Queues to put in them the data that sent from Task1, Task2 and Task3 to the conumer task
	Q_1 = xQueueCreate( 1, sizeof(char) );
	Q_2 = xQueueCreate( 1, sizeof(char) );
	Q_3 = xQueueCreate( 15, sizeof(char) );
	
    /* Create Tasks here */  
	
	xTaskPeriodicCreate(Button_1_Monitor, "Button1", 100, ( void * ) 0, 1, &B1_taskHandler,	BUTTON_ONE_PERIOD);      	
					
	xTaskPeriodicCreate(Button_2_Monitor, "Button2", 100, ( void * ) 0, 1,	&B2_taskHandler, BUTTON_TWO_PERIOD);      				
										
	xTaskPeriodicCreate(Periodic_Transmitter, "Uart_Trans", 100, ( void * ) 0, 1, &Trans_TaskHandler, UART_TRANSC_PERIOD);  
										
	xTaskPeriodicCreate(Uart_Receiver, "Peroidic_Transmit", 100, ( void * ) 0, 1, &Receiv_TaskHandler,	PERIODC_TRANSMIT_PERIOD);      				

	xTaskPeriodicCreate(Load_1_Simulation,  "Load_one", 100, ( void * ) 0, 1, &L1_TaskHandler, LOAD_ONE_PERIOD);      				
																		
	xTaskPeriodicCreate(Load_2_Simulation, "Load_two", 100, ( void * ) 0, 1, &L2_TaskHandler, LOAD_TWO_PERIOD); 
										



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


