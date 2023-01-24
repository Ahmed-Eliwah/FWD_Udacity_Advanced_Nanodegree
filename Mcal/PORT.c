/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  PORT.c
 *        \brief  GPIO Driver
 *
 *      \details  The Driver Configure All GPIO Pins 
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Bit_Math.h"
#include "Port_Regs.h"
#include "PORT.h"


/* Used for Simulator --> Map Ranges to be Added 0x40000000, 0x400FFFFF */

/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/	

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

void Dio_FlipChannel(Port_Num PortNum, Port_PinNum PinNum)
{
	GPIODATA(PortNum) ^= (1 << PinNum);
}

uint8 Get_Bit_Value(Port_Num PortNum, Port_PinNum PinNum)
{
	return ((GPIODATA(PortNum) & (1 << PinNum)) >> PinNum);
}



/******************************************************************************
* \Syntax          : void Port_Init(void)                                      
* \Description     : initialize Selected Ports and Specific Pins modes 
*                                                        
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : None                     
* \Parameters (out): None                                                      
* \Return value:   : None
*******************************************************************************/
void Port_Init(const Port_ConfigType* ConfigPtr)
{
    uint8 u8_PinCounter =0;
		
			
		for(u8_PinCounter =0; u8_PinCounter < PORT_CONFIGURED_PINS; u8_PinCounter++)
			{
				Port_Num     port_num = ConfigPtr[u8_PinCounter].PortNum;
				Port_PinNum  pin_num  = ConfigPtr[u8_PinCounter].PortPinNum;
				
				/* Step one -> Initiate The Clock of The Selected Port (Only when using hardware)*/
				
				/* Step Two -> Set the Direction of The Selected Pin */
				
				/* Configure Pin Direction (Output Case) */
				
				if(PORT_PIN_OUT == ConfigPtr[u8_PinCounter].PortPinDirection)
				{
					/* Set Output Direction */
					SET_BIT(GPIODIR(port_num), pin_num);
					
					/* Step Three -> Set the Level of The Selected Pin */
					/* Configure Pin Level */
					if(PORT_PIN_LEVEL_HIGH == ConfigPtr[u8_PinCounter].PortPinLevelValue)
					{
						/* Set Level High */
						SET_BIT(GPIODATA(port_num), pin_num);
						
					}
					
					else if(PORT_PIN_LEVEL_LOW == ConfigPtr[u8_PinCounter].PortPinLevelValue)
					{
						/* Set Level Low */
						CLEAR_BIT(GPIODATA(port_num), pin_num);
					}

					else 
					{
						/* Invalid Cofig Error */
						break;
					}
					
					
					
					
					/* Step Four -> Set the Output Current of The Selected Pin */
					/* Set the Output Current */
					if(PORT_PIN_CURRENT_2MA == ConfigPtr[u8_PinCounter].PortOutputCurrent)
					{
							/* Set the Output Current to 2MA */
						SET_BIT(GPIODR2R(port_num), pin_num);
						
					}
					
					else if(PORT_PIN_CURRENT_4MA == ConfigPtr[u8_PinCounter].PortOutputCurrent)
					{
							/* Set the Output Current to 4MA*/
						SET_BIT(GPIODR4R(port_num), pin_num);
						
					}
					
					else if(PORT_PIN_CURRENT_8MA == ConfigPtr[u8_PinCounter].PortOutputCurrent)
					{
							/* Set the Output Current 8MA */
						SET_BIT(GPIODR8R(port_num), pin_num);
						
					}
					
					else
					{
						/* Invalid Cofig Error */
						break;
					}
					
				}
				
				/* Step Five -> Set the Mode of The Selected Pin */
				/* Configure the Pin */
				if(PORT_PIN_DEN == ConfigPtr[u8_PinCounter].PortPinMode)
				{
					/* Clear The Corresponding Bit in the GPIOAFSEL Register to Disable the Alternative Functionality of this Pin */
					CLEAR_BIT(GPIOAFSEL(port_num), pin_num);
					
					/* Set the Corresponding Bit in the GPIODEN Register to Enable Digital Functionality of this Pin */
					SET_BIT(GPIODEN(port_num), pin_num);
					
					/* Clear the Corresponding Bit in the GPIOAMSEL Register to Disable Analog Functionality of this Pin */
					CLEAR_BIT(GPIOAMSEL(port_num), pin_num);
				}
				
				else
				{
						/* Invalid Cofig Error */
						break;
				}
					
				
			}
			
			
		

}

/**********************************************************************************************************************
 *  END OF FILE: PORT.c
 *********************************************************************************************************************/
