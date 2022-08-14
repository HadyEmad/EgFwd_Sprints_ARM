/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Port_Lcfg.c
 *        \brief  Source file for linking time configurations for Port driver
 * 
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Port_Lcfg.h"

/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
PortPinConfig_s PortPinConfig[PORT_PIN_CONFIG_SIZE] =
{
	{
		.Port_Pin = PORT_A_PIN_0,
		.Port_PinDirection = PORT_PIN_OUT,
		.Port_PinMode = PORT_PIN_MODE_DIO,
		.Port_PinInternalAttach = PORT_PIN_PULL_NONE,
		.Port_PinOutputCurrent = PORT_PIN_CURRENT_4MA,
		.Port_PinInitialValue = PORT_PIN_LOW
	},
	{
		.Port_Pin = PORT_A_PIN_1,
		.Port_PinDirection = PORT_PIN_OUT,
		.Port_PinMode = PORT_PIN_MODE_DIO,
		.Port_PinInternalAttach = PORT_PIN_PULL_NONE,
		.Port_PinOutputCurrent = PORT_PIN_CURRENT_8MA,
		.Port_PinInitialValue = PORT_PIN_LOW
	},
	{
		.Port_Pin = PORT_A_PIN_2,
		.Port_PinDirection = PORT_PIN_OUT,
		.Port_PinMode = PORT_PIN_MODE_DIO,
		.Port_PinInternalAttach = PORT_PIN_PULL_NONE,
		.Port_PinOutputCurrent = PORT_PIN_CURRENT_2MA,
		.Port_PinInitialValue = PORT_PIN_LOW
	},
	{
		.Port_Pin = PORT_A_PIN_3,
		.Port_PinDirection = PORT_PIN_OUT,
		.Port_PinMode = PORT_PIN_MODE_DIO,
		.Port_PinInternalAttach = PORT_PIN_PULL_NONE,
		.Port_PinOutputCurrent = PORT_PIN_CURRENT_4MA,
		.Port_PinInitialValue = PORT_PIN_LOW
	},
	{
		.Port_Pin = PORT_A_PIN_4,
		.Port_PinDirection = PORT_PIN_OUT,
		.Port_PinMode = PORT_PIN_MODE_DIO,
		.Port_PinInternalAttach = PORT_PIN_PULL_NONE,
		.Port_PinOutputCurrent = PORT_PIN_CURRENT_8MA,
		.Port_PinInitialValue = PORT_PIN_HIGH
	},
	{
		.Port_Pin = PORT_A_PIN_5,
		.Port_PinDirection = PORT_PIN_OUT,
		.Port_PinMode = PORT_PIN_MODE_DIO,
		.Port_PinInternalAttach = PORT_PIN_PULL_NONE,
		.Port_PinOutputCurrent = PORT_PIN_CURRENT_2MA,
		.Port_PinInitialValue = PORT_PIN_HIGH
	},
	{
		.Port_Pin = PORT_A_PIN_6,
		.Port_PinDirection = PORT_PIN_OUT,
		.Port_PinMode = PORT_PIN_MODE_DIO,
		.Port_PinInternalAttach = PORT_PIN_PULL_NONE,
		.Port_PinOutputCurrent = PORT_PIN_CURRENT_4MA,
		.Port_PinInitialValue = PORT_PIN_HIGH
	},
	{
		.Port_Pin = PORT_A_PIN_7,
		.Port_PinDirection = PORT_PIN_OUT,
		.Port_PinMode = PORT_PIN_MODE_DIO,
		.Port_PinInternalAttach = PORT_PIN_PULL_NONE,
		.Port_PinOutputCurrent = PORT_PIN_CURRENT_8MA,
		.Port_PinInitialValue = PORT_PIN_HIGH
	}
};

Port_ConfigType Port_Config = { .PortPinConfig_numberOfInstances = PORT_PIN_CONFIG_SIZE};
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/



/**********************************************************************************************************************
 *  END OF FILE: FileName.c
 *********************************************************************************************************************/
