/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Gpt_Lcfg.c
 *        \brief  
 *
 *      \details  
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Gpt_Lcfg.h"

/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
GptChannelConfigSet_s GptChannelConfigSet[GPT_CHANNEL_CONFIG_SET_SIZE] = 
{
	{
		.GptChannelId = 0,
		.GptChannelTickFrequency = 62500,
		.GptChannelTickValueMax = 65535,
		.GptNotification = NULL_PTR,
		.GptChannelMode = GPT_CH_MODE_CONTINUOUS	
	},
	{
		.GptChannelId = 1,
		.GptChannelTickFrequency = 8000000,
		.GptChannelTickValueMax = 65535,
		.GptNotification = NULL_PTR,
		.GptChannelMode = GPT_CH_MODE_ONESHOT	
	},
	{
		.GptChannelId = 2,
		.GptChannelTickFrequency = 4000000,
		.GptChannelTickValueMax = 65535,
		.GptNotification = NULL_PTR,
		.GptChannelMode = GPT_CH_MODE_ONESHOT	
	},
	{
		.GptChannelId = 6,
		.GptChannelTickFrequency = 16000000,
		.GptChannelTickValueMax = 4294967295,
		.GptNotification = LED_Toggle,
		.GptChannelMode = GPT_CH_MODE_ONESHOT	
	},
	{
		.GptChannelId = 7,
		.GptChannelTickFrequency = 8000000,
		.GptChannelTickValueMax = 4294967295,
		.GptNotification = NULL_PTR,
		.GptChannelMode = GPT_CH_MODE_ONESHOT	
	}
};

Gpt_ConfigType Gpt_Config = { .GptChannelConfigSet_numberOfInstances = GPT_CHANNEL_CONFIG_SET_SIZE};
/**********************************************************************************************************************
 *  END OF FILE: Gpt_Lcfg.c
 *********************************************************************************************************************/
