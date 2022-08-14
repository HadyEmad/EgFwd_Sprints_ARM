/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Mcu_Hw.h
 *       Module:  -
 *
 *  Description:  <Write File DESCRIPTION here>     
 *  
 *********************************************************************************************************************/
#ifndef MCU_HW_H
#define MCU_HW_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Interrupt control needed registers */
#define	CORTEXM4_PERI_BASE_ADDRESS					0xE000E000
#define APINT																*((volatile uint32*)(CORTEXM4_PERI_BASE_ADDRESS+0xD0C))
#define SYSHNDCTRL													*((volatile uint32*)(CORTEXM4_PERI_BASE_ADDRESS+0xD24))
#define STCTRL															*((volatile uint32*)(CORTEXM4_PERI_BASE_ADDRESS+0x010))
#define SYSPRI1															*((volatile uint32*)(CORTEXM4_PERI_BASE_ADDRESS+0xD18))
#define SYSPRI2 														*((volatile uint32*)(CORTEXM4_PERI_BASE_ADDRESS+0xD1C))
#define SYSPRI3 														*((volatile uint32*)(CORTEXM4_PERI_BASE_ADDRESS+0xD20))


#define PERI_BIT_BAND_REGION_BASE							0x40000000
#define PERI_BIT_BAND_ALIAS_BASE							0x42000000

/* System control registers */

#define SYSTEM_CONTROL_BASE_ADDRESS						0x400FE000
#define RCGCGPIO															*((volatile uint32*)(SYSTEM_CONTROL_BASE_ADDRESS+0x608))
	
/* Port and Dio needed registers addresses */
#define GPIOA_BASE                      0x40004000UL
#define GPIOB_BASE                      0x40005000UL
#define GPIOC_BASE                      0x40006000UL
#define GPIOD_BASE                      0x40007000UL
#define GPIOE_BASE                      0x40024000UL
#define GPIOF_BASE                      0x40025000UL

#define GPIODATA_OFFSET									0x3FC
#define GPIODIR_OFFSET									0x400
#define	GPIOAFSEL_OFFSET								0x420
#define GPIODR2R_OFFSET									0x500		
#define GPIODR4R_OFFSET									0x504
#define GPIODR8R_OFFSET									0x508
#define GPIOODR_OFFSET									0x50C
#define GPIOPUR_OFFSET									0x510
#define GPIOPDR_OFFSET									0x514
#define GPIODEN_OFFSET									0x51C
#define GPIOLOCK_OFFSET									0x520
#define GPIOCR_OFFSET										0x524
#define GPIOAMSEL_OFFSET								0x528
#define GPIOPCTL_OFFSET									0x52C

#define GPIO_UNLOCK_KEY									0x4C4F434B
/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

 
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

 
#endif  /* MCU_HW_H */

/**********************************************************************************************************************
 *  END OF FILE: Mcu_Hw.h
 *********************************************************************************************************************/
