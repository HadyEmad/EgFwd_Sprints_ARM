/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Port.c
 *        \brief  Souce file for Port driver functions
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Port.h"
#include "Mcu_Hw.h"

/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/
#define SET_BIT(REG,BIT)   				((*((volatile uint32*)(((REG-PERI_BIT_BAND_REGION_BASE)*32 + BIT*4)+PERI_BIT_BAND_ALIAS_BASE))) = 1)
#define CLEAR_BIT(REG,BIT) 				((*((volatile uint32*)(((REG-PERI_BIT_BAND_REGION_BASE)*32 + BIT*4)+PERI_BIT_BAND_ALIAS_BASE))) = 0)
#define ACCESS_REGISTER(REG)			*((volatile uint32*)(REG))	

#define PORTS_NUMBER							6
#define FUNCTIONS_NUMBER					12
#define	PINS_PER_PORT							8

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/
 static uint32 GPIO_BaseAddress [6] = {
	 GPIOA_BASE,GPIOB_BASE,GPIOC_BASE,GPIOD_BASE,GPIOE_BASE,GPIOF_BASE
 };
 
 static uint8 PMC_Encoding [FUNCTIONS_NUMBER][PORTS_NUMBER] =
 {
	{1,1,1,1,1,1}, //UART Encoding
	{8,8,0,0,8,3}, //CAN Encoding
	{2,2,0,2,0,2}, //SSI Encoding
	{3,3,0,3,3,0}, //I2C Encoding
	{0,0,0,0,0,9}, //Analog comparator digital output
	{0,4,4,4,4,4}, //PWM M0
	{5,0,0,5,5,5}, //PWM M1
	{0,7,7,7,0,7}, //GPTM
	{0,0,1,0,0,0}, //JTAG
	{0,0,8,8,0,0}, //USB
	{0,0,0,8,0,8}, //NMI
	{0,0,6,6,0,6}	 //QEI
};

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/


/******************************************************************************
* \Syntax          : void Port_Init(const Port_ConfigType *ConfigPtr)
* \Description     : Initializes port pins based on the passed configurations                                                                                                               
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : Pointer to port configurations                   
* \Parameters (out): None                                                      
* \Return value:   : None                                  
*******************************************************************************/
void Port_Init(const Port_ConfigType *ConfigPtr)
{
		uint8 Iteration = 0;
		uint8 Pin,Port;
    for(Iteration = 0; Iteration < ConfigPtr ->PortPinConfig_numberOfInstances ; Iteration++)
		{
			
			Pin = PortPinConfig[Iteration].Port_Pin%PINS_PER_PORT;
			Port = PortPinConfig[Iteration].Port_Pin/PINS_PER_PORT;
			//ACCESS_REGISTER(GPIO_BaseAddress[Port]+GPIOLOCK_OFFSET) = GPIO_UNLOCK_KEY;
			//SET_BIT(GPIO_BaseAddress[Port]+GPIOCR_OFFSET,Pin);
			if((PortPinConfig[Iteration].Port_Pin >= PORT_C_PIN_0 && PortPinConfig[Iteration].Port_Pin <= PORT_C_PIN_3)
				|| PortPinConfig[Iteration].Port_Pin == PORT_D_PIN_7 || PortPinConfig[Iteration].Port_Pin == PORT_F_PIN_0)
			{
				ACCESS_REGISTER(GPIO_BaseAddress[Port]+GPIOLOCK_OFFSET) = GPIO_UNLOCK_KEY;
				SET_BIT(GPIO_BaseAddress[Port]+GPIOCR_OFFSET,Pin);
			}
			if(PortPinConfig[Iteration].Port_PinInternalAttach != PORT_PIN_PULL_NONE)
			{
				SET_BIT(GPIO_BaseAddress[Port]+GPIOODR_OFFSET+PortPinConfig[Iteration].Port_PinInternalAttach*4,Pin);			
			}
			
			if(PortPinConfig[Iteration].Port_PinOutputCurrent != PORT_PIN_CURRENT_NONE)
			{
				SET_BIT(GPIO_BaseAddress[Port]+GPIODR2R_OFFSET+PortPinConfig[Iteration].Port_PinOutputCurrent*4,Pin);			
			}
			
			if(PortPinConfig[Iteration].Port_PinMode == PORT_PIN_MODE_DIO)
			{
				SET_BIT(GPIO_BaseAddress[Port]+GPIODEN_OFFSET,Pin);			
				CLEAR_BIT(GPIO_BaseAddress[Port]+GPIOAFSEL_OFFSET,Pin);
				if(PortPinConfig[Iteration].Port_PinDirection == PORT_PIN_OUT)
				{
					SET_BIT(GPIO_BaseAddress[Port]+GPIODIR_OFFSET,Pin);
					if(PortPinConfig[Iteration].Port_PinInitialValue == PORT_PIN_LOW)
					{
						CLEAR_BIT(GPIO_BaseAddress[Port]+GPIODATA_OFFSET,Pin);			
					}
					else
					{
						SET_BIT(GPIO_BaseAddress[Port]+GPIODATA_OFFSET,Pin);			
					}
				}
				else
				{
					CLEAR_BIT(GPIO_BaseAddress[Port]+GPIODIR_OFFSET,Pin);			
				}			
			}
			else if ( PortPinConfig[Iteration].Port_PinMode >= PORT_PIN_MODE_ADC)
			{
				SET_BIT(GPIO_BaseAddress[Port]+GPIOAFSEL_OFFSET,Pin);
				CLEAR_BIT(GPIO_BaseAddress[Port]+GPIODEN_OFFSET,Pin);	
				if(PortPinConfig[Iteration].Port_PinMode == PORT_PIN_MODE_ADC)
				{
					SET_BIT(GPIO_BaseAddress[Port]+GPIOAMSEL_OFFSET,Pin);
				}
				else if(PortPinConfig[Iteration].Port_PinMode == PORT_PIN_MODE_ACOMP_IN)
				{
					CLEAR_BIT(GPIO_BaseAddress[Port]+GPIODIR_OFFSET,Pin);			
				}		
			}
			else if( PMC_Encoding[PortPinConfig[Iteration].Port_PinMode][Port] != 0)
			{
				SET_BIT(GPIO_BaseAddress[Port]+GPIODEN_OFFSET,Pin);			
				SET_BIT(GPIO_BaseAddress[Port]+GPIOAFSEL_OFFSET,Pin);
				ACCESS_REGISTER(GPIO_BaseAddress[Port]+GPIOPCTL_OFFSET) &= ~(0xFU <<Pin);
				ACCESS_REGISTER(GPIO_BaseAddress[Port]+GPIOPCTL_OFFSET) |= ((uint32)PMC_Encoding[PortPinConfig[Iteration].Port_PinMode][Port] << Pin);
			}
		}
    
}

/**********************************************************************************************************************
 *  END OF FILE: FileName.c
 *********************************************************************************************************************/
