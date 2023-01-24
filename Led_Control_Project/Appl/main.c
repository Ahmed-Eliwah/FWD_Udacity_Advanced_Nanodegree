#include "PORT.h"
#include "IntCtrl.h"
#include "SysTick.h"
#include "Bit_Math.h"


static uint8 flag;

void Handle_SysTick_Driver_isr(void)
{
	static uint8 Isr_flag; 
		
	/* Blink The Bit That's Connected to The Led */
	Dio_FlipChannel(Port_PinConfig[0].PortNum, Port_PinConfig[0].PortPinNum);
	
	
	if(flag == 1)
	{
		Isr_flag = 1;
	}
	
	flag++;
	
	if(Isr_flag == 1)
	{
		flag = 0;
		Isr_flag = 0;
	}
	
}

int main(void)
{
	/* Suppose to Enable SYSTICK Peripheral but it's already Activated by Default */
	
	/*IntCrtl_Init(ExceptionsTypeConfig);*/
	
	SysTick_Init(&SysTick_Config_Modes);
	
	Port_Init(Port_PinConfig);

	Interrupt_CB(Handle_SysTick_Driver_isr);
	
	flag = 0;

	while(1){
		
		/* The Fisrt One to Enter the Desird On Time */
		
		if(flag == 0)
		{
			/* Use Port Configrations to Get the Value of the Desird On Time in Seconds */
			SysTick_Set_ReloadValue(Get_Bit_Value(Port_PinConfig[1].PortNum, Port_PinConfig[1].PortPinNum));
		}
		
		else if(flag == 1)
		{
			/* Use Port Configrations to Get the Value of the Desird Off Time in Seconds */
			SysTick_Set_ReloadValue(Get_Bit_Value(Port_PinConfig[2].PortNum, Port_PinConfig[2].PortPinNum));
		}
		
	}
	
}