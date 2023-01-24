/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  SysTick.c
 *        \brief  
 *
 *      \details  
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "SysTick.h"
#include "SysTick_Regs.h"
#include "Bit_Math.h"



void SysTick_Init(const SysTick_Config* SysTickConfig_Ptr)
{
	/* Check if the SysTick Peripheral is Enabled or not */
	if(SysTick_Enable == SysTickConfig_Ptr->SysTickMode)
	{
		SET_BIT(STCTRL, ENABLE_BIT);
	
	}
	
	/*If SysTick Peripheral is Disabled Exit the Initilization Function */
	else if(SysTick_Disable == SysTickConfig_Ptr->SysTickMode)
	{
		return;
	}
	
	/* Check if the SysTick Interrupt is Enabled or not */
	if(Interrupt_Enable == SysTickConfig_Ptr->InterruptMode)
	{
		SET_BIT(STCTRL, INTERRUPT_BIT);
	}

		
	/* Check if the SysTick Clock Source is System CLock */
	if(System_Clock == SysTickConfig_Ptr->ClockSource)
	{
		SET_BIT(STCTRL, CLOCK_SOURCE_BIT);
	}
		
	/* Check if the SysTick Clock Source is Internal Oscillator */
	else if(Internal_Osc == SysTickConfig_Ptr->ClockSource)
	{
		CLEAR_BIT(STCTRL, CLOCK_SOURCE_BIT);
	}
	
	/* Put The Value of Counts You Want Timer to Start at in the RELOAD Register */
	/* STRELOAD  = ReloadValue; */
}



void SysTick_Set_ReloadValue(SysTick_Reload_Value ReloadValue)
{
	//CLEAR_BIT(STCTRL, COUNT_BIT);
	/* STRELOAD = 0; */
	
	
	STRELOAD  = ReloadValue;
	
	/*STCURRENT = 0;*/
}

void Clear_SysTick_Regs(void)
{
	STRELOAD = 0x0;
	STCURRENT = 0x0;
}

/**********************************************************************************************************************
 *  END OF FILE: SysTick.c
 *********************************************************************************************************************/
