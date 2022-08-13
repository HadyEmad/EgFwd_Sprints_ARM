/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  IntCtrl.c
 *        \brief  Source file for interrupt control driver
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "IntCtrl.h"
#include "Mcu_Hw.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/


/******************************************************************************
* \Syntax          : void IntCtrl_Init(void)        
* \Description     : Initializes Faults and Interrupts status and their priority
* 										through parsing configuration into NVIC/SCB registers                                                                                                       
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : None                     
* \Parameters (out): None                                                      
* \Return value:   : None                                 
*******************************************************************************/
void IntCtrl_Init(void)
{
	
		APINT = 0x05FA0000U | ((uint32)IntCtrlDriverConfig.IntCtrlPriorityFormat << 8U) ;
		uint8 Iteration = 0;
		for(Iteration = 0; Iteration < IntCtrlDriverConfig.IntCtrlChannelConfig_numberOfInstances; Iteration++)
		{
				uint8 Int_Priority = 0;
				switch (IntCtrlDriverConfig.IntCtrlPriorityFormat)
				{
					case G3SG0:
						Int_Priority = IntCtrlChannelConfig[Iteration].IntCtrl_InterruptGroupPri;
						break;				
					case G2SG1:
						Int_Priority = ((IntCtrlChannelConfig[Iteration].IntCtrl_InterruptGroupPri & 3) << 1) | (IntCtrlChannelConfig[Iteration].IntCtrl_InterruptSubGroupPri & 1);
						break;
					case G1SG2:
						Int_Priority = ((IntCtrlChannelConfig[Iteration].IntCtrl_InterruptGroupPri & 1) << 2) | (IntCtrlChannelConfig[Iteration].IntCtrl_InterruptSubGroupPri & 3);
						break;
					case G0SG3:
						Int_Priority = IntCtrlChannelConfig[Iteration].IntCtrl_InterruptSubGroupPri;
						break;
				}
				
				if((IntCtrlChannelConfig[Iteration].IntCtrl_Interrupt >= FAULT_MPU) && 
					(IntCtrlChannelConfig[Iteration].IntCtrl_Interrupt <= FAULT_USAGE))
				{
					SYSHNDCTRL &= ~(1<<(12+IntCtrlChannelConfig[Iteration].IntCtrl_Interrupt));
					SYSHNDCTRL |= ((uint32)IntCtrlChannelConfig[Iteration].IntCtrl_InterruptEnable <<(12+IntCtrlChannelConfig[Iteration].IntCtrl_Interrupt));
					SYSPRI1 &= ~(7U<< (5+((IntCtrlChannelConfig[Iteration].IntCtrl_Interrupt%4)*8)));
					SYSPRI1 |= ((uint32)Int_Priority << (5+((IntCtrlChannelConfig[Iteration].IntCtrl_Interrupt%4)*8)));
				}
				else if (IntCtrlChannelConfig[Iteration].IntCtrl_Interrupt==SVCALL)
				{
					SYSPRI2 = 0;
					SYSPRI2 = ((uint32)Int_Priority<<29);
				}
				else if (IntCtrlChannelConfig[Iteration].IntCtrl_Interrupt==PENDSV ||
					IntCtrlChannelConfig[Iteration].IntCtrl_Interrupt==DEBUG_MONITOR)
				{	
					SYSPRI3 &= ~(7U<< (5+((IntCtrlChannelConfig[Iteration].IntCtrl_Interrupt%4)*8)));
					SYSPRI3 |= ((uint32)Int_Priority << (5+((IntCtrlChannelConfig[Iteration].IntCtrl_Interrupt%4)*8)));
				}
				else if (IntCtrlChannelConfig[Iteration].IntCtrl_Interrupt==SYSTICK)
				{
					STCTRL &= ~(1U<<1);
					STCTRL |= ((uint32)IntCtrlChannelConfig[Iteration].IntCtrl_InterruptEnable <<1);
				}
				else if ((IntCtrlChannelConfig[Iteration].IntCtrl_Interrupt >= INT_GPIO_PORT_A) && 
					(IntCtrlChannelConfig[Iteration].IntCtrl_Interrupt <= INT_PWM1_FAULT))
				{
					uint8 Interrupt_Number = IntCtrlChannelConfig[Iteration].IntCtrl_Interrupt - 16;
					if(IntCtrlChannelConfig[Iteration].IntCtrl_InterruptEnable == TRUE)
					{
						uint32* Int_EN_Address = (uint32*)(CORTEXM4_PERI_BASE_ADDRESS + (0x100+(Interrupt_Number/32)*4));
						*Int_EN_Address |=(1U<< (Interrupt_Number%32));
					}
					else
					{
						uint32* Int_DIS_Address = (uint32*)(CORTEXM4_PERI_BASE_ADDRESS + (0x180+(Interrupt_Number/32)*4));
						*Int_DIS_Address |=(1U<< (Interrupt_Number%32));
					}
					uint32* Int_Pri_Address = (uint32*)(CORTEXM4_PERI_BASE_ADDRESS + (0x400+(Interrupt_Number/4)*4));
					*Int_Pri_Address &= ~(7U<< (5+((Interrupt_Number%4)*8))); 
					*Int_Pri_Address |= ((uint32)Int_Priority << (5+((Interrupt_Number%4)*8)));
				}
				
				
		}
	
}

/**********************************************************************************************************************
 *  END OF FILE: FileName.c
 *********************************************************************************************************************/
