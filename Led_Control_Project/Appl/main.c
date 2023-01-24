#include "PORT.h"
#include "IntCtrl.h"
#include "SysTick.h"
#include "Bit_Math.h"

static uint8 on_overflows_needed;
static uint8 off_overflows_needed;




void Handle_SysTick_Driver_isr(void)
{
	
	if((on_overflows_needed > 0) && (off_overflows_needed == 1))
	{
			/* Blink The Bit That's Connected to The Led */
			Dio_WriteChannel(Port_PinConfig[0].PortNum, Port_PinConfig[0].PortPinNum);
	}
	
	else if((on_overflows_needed == 0) && (off_overflows_needed > 1))
	{
			/* Blink The Bit That's Connected to The Led */			
			Dio_ClearChannel(Port_PinConfig[0].PortNum, Port_PinConfig[0].PortPinNum);
	}
	
	else 
	{
		Dio_FlipChannel(Port_PinConfig[0].PortNum, Port_PinConfig[0].PortPinNum);
	}

}




int main(void)
{
	/* Suppose to Enable SYSTICK Peripheral but it's already Activated by Default */
	
	/*IntCrtl_Init(ExceptionsTypeConfig);*/
	
	SysTick_Init(&SysTick_Config_Modes);
	
	Port_Init(Port_PinConfig);

	Interrupt_CB(Handle_SysTick_Driver_isr);
	
	uint8 PortNum_on  = Port_PinConfig[1].PortNum;
	uint8 PinNum_on   = Port_PinConfig[1].PortPinNum;
	
	uint8 PortNum_off = Port_PinConfig[2].PortNum;
	uint8 PinNum_off  = Port_PinConfig[2].PortPinNum;
	
	
	uint8 ON_time = 0, OFF_time = 0;
	
	while(1){
		
		ON_time = Dio_ReadChannel(PortNum_on, PinNum_on);
		
		on_overflows_needed = SysTick_Set_Counts_Needed(ON_time);
		
		OFF_time = Dio_ReadChannel(PortNum_off, PinNum_off);
		
		off_overflows_needed = SysTick_Set_Counts_Needed(OFF_time) + 1;

	}
	
}
