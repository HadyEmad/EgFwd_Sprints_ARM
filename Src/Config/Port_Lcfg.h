/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Port_Lcfg.h
 *       Module:  Port
 *
 *  Description:  Header file for linking time configurations for port driver    
 *  
 *********************************************************************************************************************/
#ifndef PORT_LCFG_H
#define PORT_LCFG_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Port_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define PORT_PIN_CONFIG_SIZE		6

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
extern PortPinConfig_s PortPinConfig[PORT_PIN_CONFIG_SIZE];
extern Port_ConfigType Port_Config;
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

 
#endif  /* PORT_LCFG_H */

/**********************************************************************************************************************
 *  END OF FILE: Port_Lcfg.h
 *********************************************************************************************************************/
