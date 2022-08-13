/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  IntCtrl_Types.h
 *       Module:  Interrupt control
 *
 *  Description:  Header file for needed types for the driver    
 *  
 *********************************************************************************************************************/
#ifndef INTCTRL_TYPES_H
#define INTCTRL_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef enum
{
	FAULT_MPU = 4, FAULT_BUS, FAULT_USAGE, SVCALL =11, DEBUG_MONITOR, PENDSV = 14, SYSTICK,
	INT_GPIO_PORT_A,INT_GPIO_PORT_B, INT_GPIO_PORT_C, INT_GPIO_PORT_D, INT_GPIO_PORT_E, INT_UART0,
	INT_UART1, INT_SSI0, INT_I2C0,INT_PWM0_FAULT,INT_PWM0_GENERATOR0,INT_PWM0_GENERATOR1,
	INT_PWM0_GENERATOR2,INT_QEI0,INT_ADC0_SEQ0,INT_ADC0_SEQ1,INT_ADC0_SEQ2, INT_ADC0_SEQ3,
	INT_WATCHDOG, INT_TIMER0A, INT_TIMER0B, INT_TIMER1A, INT_TIMER1B, INT_TIMER2A, INT_TIMER2B,
	INT_COMP0, INT_COMP1, INT_SYSCTL = 44, INT_FLASH, INT_GPIO_PORT_F, INT_UART2 = 49,INT_SSI1,
	INT_TIMER3A, INT_TIMER3B, INT_I2C1, INT_QEI1, INT_CAN0, INT_CAN1, INT_HIBERNATE = 59, INT_USB,
	INT_PWM0_GENERATOR3, INT_UDMA, INT_UDMA_ERROR, INT_ADC1_SEQ0, INT_ADC1_SEQ1, INT_ADC1_SEQ2, 
	INT_ADC1_SEQ3, INT_SSI2 = 73, INT_SSI3, INT_UART3, INT_UART4, INT_UART5, INT_UART6, INT_UART7,
	INT_I2C2 = 84, INT_I2C3, INT_TIMER4A, INT_TIMER4B, INT_TIMER5A = 108, INT_TIMER5B, INT_WTIMER0A,
  INT_WTIMER0B, INT_WTIMER1A, INT_WTIMER1B, INT_WTIMER2A, INT_WTIMER2B, INT_WTIMER3A, INT_WTIMER3B,
	INT_WTIMER4A, INT_WTIMER4B, INT_WTIMER5A, INT_WTIMER5B, INT_SYSEXC, INT_PWM1_GENERATOR0 = 150, 
	INT_PWM1_GENERATOR1, INT_PWM1_GENERATOR2, INT_PWM1_GENERATOR3, INT_PWM1_FAULT
}IntCtrl_InterruptType;

typedef enum
{
	G3SG0=4,G2SG1,G1SG2,G0SG3
}IntCtrlPriorityFormat_e;


typedef struct
{
	IntCtrl_InterruptType IntCtrl_Interrupt;
	boolean IntCtrl_InterruptEnable;
	uint8 IntCtrl_InterruptGroupPri;
	uint8 IntCtrl_InterruptSubGroupPri;	
}IntCtrlChannelConfig_s;

typedef struct
{
	IntCtrlPriorityFormat_e IntCtrlPriorityFormat;
	uint8 IntCtrlChannelConfig_numberOfInstances;
}IntCtrlDriverConfig_s;

#endif  /* INTCTRL_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: IntCtrl_Types.h
 *********************************************************************************************************************/
