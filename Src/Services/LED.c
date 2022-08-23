#include "Gpt.h"
#include "IntCtrl.h"
#include "Dio.h"
#include "Port.h"
#include "Mcu_Hw.h"
#include "LED.h"

extern uint32 SystemCoreClock;

uint32 LED_OnTicks;
uint32 LED_OffTicks;

void LED_Init(uint32 ON_Time, uint32 OFF_Time)
{
		IntCtrl_Init();
		RCGCGPIO = 0x3F;
		RCGCTIMER = 0x3F;
		RCGCWTIMER = 0x3F;
		Port_Init(&Port_Config);
		Gpt_Init(&Gpt_Config);
		Gpt_EnableNotification(GPT_CHANNEL_6);
		LED_OnTicks = ON_Time * SystemCoreClock;
		LED_OffTicks = OFF_Time * SystemCoreClock;
		Gpt_StartTimer(GPT_CHANNEL_6,LED_OffTicks); 
}

void LED_ChangePeriod(uint32 ON_Time, uint32 OFF_Time)
{
	LED_OnTicks = ON_Time * SystemCoreClock;
	LED_OffTicks = OFF_Time * SystemCoreClock;
}

/* ISR called when timer overflows */
void LED_Toggle(void)
{
	if(Dio_ReadChannel(PF1) == CHANNEL_LEVEL_LOW)
	{
		Dio_WriteChannel(PF1, CHANNEL_LEVEL_HIGH);
		Gpt_StartTimer(GPT_CHANNEL_6,LED_OnTicks); 
	}
	else
	{
		Dio_WriteChannel(PF1, CHANNEL_LEVEL_LOW);
		Gpt_StartTimer(GPT_CHANNEL_6,LED_OffTicks);
	}
}
