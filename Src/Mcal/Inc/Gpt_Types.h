/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Gpt_Types.h
 *       Module:  Gpt
 *
 *  Description:  <Write File DESCRIPTION here>     
 *  
 *********************************************************************************************************************/
#ifndef GPT_TYPES_H
#define GPT_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
 
#define PREDEF_TIMER_1US_16BIT_EN							0
#define PREDEF_TIMER_1US_24BIT_EN							1
#define PREDEF_TIMER_1US_32BIT_EN							2
#define GPT_PREDEF_TIMER_1US_DISABLED					3

 typedef enum
{
    GPT_PREDEF_TIMER_1US_16BIT,
    GPT_PREDEF_TIMER_1US_24BIT,
    GPT_PREDEF_TIMER_1US_32BIT,
    GPT_PREDEF_TIMER_100US_32BIT
}Gpt_PredefTimerType;

typedef enum
{
	GPT_CHANNEL_0,GPT_CHANNEL_1,GPT_CHANNEL_2,GPT_CHANNEL_3,GPT_CHANNEL_4,GPT_CHANNEL_5,
	GPT_CHANNEL_6,GPT_CHANNEL_7,GPT_CHANNEL_8,GPT_CHANNEL_9,GPT_CHANNEL_10,GPT_CHANNEL_11
}Gpt_ChannelType;

typedef uint32 Gpt_ValueType;

typedef void (*EcucFunctionNameDef)(); /* Pointer to function */

typedef enum
{

    GPT_CH_MODE_ONESHOT = 0x11,
    GPT_CH_MODE_CONTINUOUS

}GptChannelMode_e;

typedef enum
{

    NOTIFICATION_DISABLED , NOTIFICATION_ENABLED

}GptChannelNotificationState_e;

typedef enum
{
	 CHANNEL_UNINIT, CHANNEL_INIT, CHANNEL_RUNNING, CHANNEL_STOPPED, CHANNEL_EXPIRED
}GptChannelState_e;

typedef struct
{
	uint32 GptChannelTickFrequency;
	uint64 GptChannelTickValueMax;
	EcucFunctionNameDef GptNotification;
	GptChannelMode_e GptChannelMode;
	uint8 GptChannelId;
}GptChannelConfigSet_s;

typedef struct
{
	
	uint8 GptChannelConfigSet_numberOfInstances;
	
}Gpt_ConfigType;

/* Data structures to be used statically in module only */
typedef struct
{
    boolean Gpt_ChannelConfigState;
    uint8 Gpt_ChannelConfigIndex;
		uint8 Gpt_PrescaleShifts;
	
}GptChannelSetting_s;

#endif  /* GPT_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: Gpt_Types.h
 *********************************************************************************************************************/
