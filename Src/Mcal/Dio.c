/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Dio.c
 *        \brief  Source file for Dio driver module
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dio.h"
#include "Mcu_Hw.h"
/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/
#define SET_BIT(REG,BIT)   				((*((volatile uint32*)(((REG-PERI_BIT_BAND_REGION_BASE)*32 + BIT*4)+PERI_BIT_BAND_ALIAS_BASE))) = 1)
#define CLEAR_BIT(REG,BIT) 				((*((volatile uint32*)(((REG-PERI_BIT_BAND_REGION_BASE)*32 + BIT*4)+PERI_BIT_BAND_ALIAS_BASE))) = 0)
#define GET_BIT(REG,BIT)					(*((volatile uint32*)(((REG-PERI_BIT_BAND_REGION_BASE)*32 + BIT*4)+PERI_BIT_BAND_ALIAS_BASE)))
#define TOGGLE_BIT(REG,BIT)				((*((volatile uint32*)(((REG-PERI_BIT_BAND_REGION_BASE)*32 + BIT*4)+PERI_BIT_BAND_ALIAS_BASE))) ^= 1)
#define ACCESS_REGISTER(REG)			*((volatile uint32*)(REG))	

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/
static uint32 GPIO_BaseAddress [6] = {
	 GPIOA_BASE,GPIOB_BASE,GPIOC_BASE,GPIOD_BASE,GPIOE_BASE,GPIOF_BASE
 };
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


/******************************************************************************
* \Syntax          : Std_ReturnType FunctionName(AnyType parameterName)        
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Describtion                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelID)
{
	uint8 Pin,Port;
	Pin = ChannelID%8;
	Port = ChannelID/8;
	Dio_LevelType Channel_Level;
	Channel_Level = (Dio_LevelType) GET_BIT(GPIO_BaseAddress[Port]+GPIODATA_OFFSET,Pin);
	return Channel_Level;
}

/******************************************************************************
* \Syntax          : Std_ReturnType FunctionName(AnyType parameterName)        
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Describtion                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/
void Dio_WriteChannel (Dio_ChannelType ChannelID, Dio_LevelType Level)
{
	uint8 Pin,Port;
	Pin = ChannelID%8;
	Port = ChannelID/8;
	if( Level == CHANNEL_LEVEL_HIGH)
	{
		SET_BIT(GPIO_BaseAddress[Port]+GPIODATA_OFFSET,Pin);
	}
	else
	{
		CLEAR_BIT(GPIO_BaseAddress[Port]+GPIODATA_OFFSET,Pin);
	}
}

/******************************************************************************
* \Syntax          : Std_ReturnType FunctionName(AnyType parameterName)        
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Describtion                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortID)
{
	Dio_PortLevelType Port_Level;
	Port_Level =(Dio_PortLevelType) ACCESS_REGISTER(GPIO_BaseAddress[PortID]+GPIODATA_OFFSET);
	return Port_Level;
}

/******************************************************************************
* \Syntax          : Std_ReturnType FunctionName(AnyType parameterName)        
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Describtion                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/
void Dio_WritePort(Dio_PortType PortID, Dio_PortLevelType Level)
{
	ACCESS_REGISTER(GPIO_BaseAddress[PortID]+GPIODATA_OFFSET) = Level;
}

/******************************************************************************
* \Syntax          : Std_ReturnType FunctionName(AnyType parameterName)        
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Describtion                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelID)
{
	uint8 Pin,Port;
	Pin = ChannelID%8;
	Port = ChannelID/8;
	TOGGLE_BIT(GPIO_BaseAddress[Port]+GPIODATA_OFFSET,Pin);
	Dio_LevelType Channel_Level;
	Channel_Level = (Dio_LevelType) GET_BIT(GPIO_BaseAddress[Port]+GPIODATA_OFFSET,Pin);
	return Channel_Level;
}


/**********************************************************************************************************************
 *  END OF FILE: FileName.c
 *********************************************************************************************************************/
