/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  IntCtrl_Lcfg.c
 *        \brief  Dynamic configurations for IntCtrl driver
 * 
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "IntCtrl_Lcfg.h"


/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
 IntCtrlDriverConfig_s IntCtrlDriverConfig =
 {
	 .IntCtrlPriorityFormat = G2SG1,
	 .IntCtrlChannelConfig_numberOfInstances = 5
 };
 
 IntCtrlChannelConfig_s IntCtrlChannelConfig [5] = 
 {
	 {
		.IntCtrl_Interrupt = INT_GPIO_PORT_F, 
		.IntCtrl_InterruptEnable = TRUE,
		.IntCtrl_InterruptGroupPri = 0,
		.IntCtrl_InterruptSubGroupPri = 0,
	 },
	 {
		.IntCtrl_Interrupt = INT_TIMER0A, 
		.IntCtrl_InterruptEnable = TRUE,
		.IntCtrl_InterruptGroupPri = 0,
		.IntCtrl_InterruptSubGroupPri = 0,
	 },
	 {
		.IntCtrl_Interrupt = INT_WTIMER1A, 
		.IntCtrl_InterruptEnable = TRUE,
		.IntCtrl_InterruptGroupPri = 1,
		.IntCtrl_InterruptSubGroupPri = 1,
	 },{
		.IntCtrl_Interrupt = INT_WTIMER0A, 
		.IntCtrl_InterruptEnable = TRUE,
		.IntCtrl_InterruptGroupPri = 0,
		.IntCtrl_InterruptSubGroupPri = 0,
	 },{
		.IntCtrl_Interrupt = INT_PWM1_GENERATOR3, 
		.IntCtrl_InterruptEnable = TRUE,
		.IntCtrl_InterruptGroupPri = 3,
		.IntCtrl_InterruptSubGroupPri = 1,
	 },
	 
 };


/**********************************************************************************************************************
 *  END OF FILE: FileName.c
 *********************************************************************************************************************/
