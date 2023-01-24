/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  SysTick.h
 *       Module:  SysTick
 *
 *  Description:  header file for SysTick Peripheral    
 *  
 *********************************************************************************************************************/
#ifndef SYSTICK_H
#define SYSTICK_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
 
 
 
typedef uint32   SysTick_Reload_Value;
 
typedef enum 
{
	SysTick_Disable,
	SysTick_Enable,
	
}SysTick_Mode;
 
 
typedef enum
{
	Interrupt_Disable,
	Interrupt_Enable,
	
}SysTick_Interrupt_Mode;


typedef enum
{
	Internal_Osc,
	System_Clock,
	
}SysTick_ClockSource;

typedef struct
{

	SysTick_Mode 							SysTickMode;
	SysTick_Interrupt_Mode 		InterruptMode;
	SysTick_ClockSource				ClockSource;
	
	
}SysTick_Config;


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

extern const SysTick_Config SysTick_Config_Modes;

 
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
 
void SysTick_Init(const SysTick_Config* SysTickConfig_Ptr);
void SysTick_Set_ReloadValue(SysTick_Reload_Value ReloadValue);
void Clear_SysTick_Regs(void);
 
#endif  /* SYSTICK_H */

/**********************************************************************************************************************
 *  END OF FILE: SysTick.h
 *********************************************************************************************************************/
