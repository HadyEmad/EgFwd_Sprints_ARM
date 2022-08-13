/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Port_Types.h
 *       Module:  Port driver
 *
 *  Description:  Port driver needed data types
 *  
 *********************************************************************************************************************/
#ifndef PORT_TYPES_H
#define PORT_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
 
typedef	enum
{
	PORT_A_PIN_0,PORT_A_PIN_1,PORT_A_PIN_2,PORT_A_PIN_3,
	PORT_A_PIN_4,PORT_A_PIN_5,PORT_A_PIN_6,PORT_A_PIN_7,
	PORT_B_PIN_0,PORT_B_PIN_1,PORT_B_PIN_2,PORT_B_PIN_3,
	PORT_B_PIN_4,PORT_B_PIN_5,PORT_B_PIN_6,PORT_B_PIN_7,
	PORT_C_PIN_0,PORT_C_PIN_1,PORT_C_PIN_2,PORT_C_PIN_3, 
	PORT_C_PIN_4,PORT_C_PIN_5,PORT_C_PIN_6,PORT_C_PIN_7,
	PORT_D_PIN_0,PORT_D_PIN_1,PORT_D_PIN_2,PORT_D_PIN_3,
	PORT_D_PIN_4,PORT_D_PIN_5,PORT_D_PIN_6,PORT_D_PIN_7,
	PORT_E_PIN_0,PORT_E_PIN_1,PORT_E_PIN_2,PORT_E_PIN_3,
	PORT_E_PIN_4,PORT_E_PIN_5,
	PORT_F_PIN_0 = 40, PORT_F_PIN_1,PORT_F_PIN_2,
	PORT_F_PIN_3,PORT_F_PIN_4
}Port_PinType;

typedef enum
{
	PORT_PIN_IN, PORT_PIN_OUT
}Port_PinDirectionType;

 typedef enum
 {
		PORT_PIN_MODE_UART, PORT_PIN_MODE_CAN, PORT_PIN_MODE_ADC, PORT_PIN_MODE_DIO, PORT_PIN_MODE_SSI,
		PORT_PIN_MODE_I2C, PORT_PIN_MODE_PWM, PORT_PIN_MODE_GPT, PORT_PIN_MODE_JTAG
 }Port_PinModeType;
 
 typedef enum
 {
	 PORT_PIN_PULL_UP, PORT_PIN_PULL_DOWN, PORT_PIN_OPEN_DRAIN 
 }Port_PinInternalAttachType;
 
 typedef enum
 {
	 PORT_PIN_CURRENT_2MA, PORT_PIN_CURRENT_4MA, PORT_PIN_CURRENT_8MA
 }Port_PinOutputCurrentType;
 
 typedef struct
 {
	 Port_PinType Port_Pin;
	 Port_PinDirectionType Port_PinDirection;
	 Port_PinModeType Port_PinMode;
	 Port_PinInternalAttachType Port_PinInternalAttach;
	 Port_PinOutputCurrentType Port_PinOutputCurrent;
 }PortPinConfig_s;
 
 typedef struct
 {
	 uint8 PortPinConfig_numberOfInstances; 
 }Port_ConfigType;

#endif  /* PORT_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: Port_Types.h
 *********************************************************************************************************************/
