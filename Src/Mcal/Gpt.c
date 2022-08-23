/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Gpt.c
 *        \brief  
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Gpt.h"
#include "Mcu_Hw.h"
/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/
#define SET_BIT(REG,BIT)   				((*((volatile uint32*)(((REG-PERI_BIT_BAND_REGION_BASE)*32 + BIT*4)+PERI_BIT_BAND_ALIAS_BASE))) = 1)
#define CLEAR_BIT(REG,BIT) 				((*((volatile uint32*)(((REG-PERI_BIT_BAND_REGION_BASE)*32 + BIT*4)+PERI_BIT_BAND_ALIAS_BASE))) = 0)
#define ACCESS_REGISTER(REG)			(*((volatile uint32*)(REG)))

#define MASK_16_BITS                            (0XFFFFU)
#define MASK_24_BITS                            (0XFFFFFFU)
#define MASK_32_BITS                            (0XFFFFFFFFU)

#if (GPT_PREDEF_TIMER_1US_ENABLING_GRADE != STD_OFF || GPT_PREDEF_TIMER_100US_32BIT_ENABLE == STD_ON)
#define GPT_MAX_CHANNEL_CONFIGURATION				11
#else
#define GPT_MAX_CHANNEL_CONFIGURATION				12
#endif

#define WTIMER_FULL_WIDTH                  (18446744073709551615ULL)  /* Wide timer channel (64-bits) full width */
/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/
  static uint32 Gpt_ChannelBaseAddress[12] = { TIMER0_BASE_ADDRESS,TIMER1_BASE_ADDRESS,TIMER2_BASE_ADDRESS,
	TIMER3_BASE_ADDRESS,TIMER4_BASE_ADDRESS,TIMER5_BASE_ADDRESS,WTIMER0_BASE_ADDRESS,WTIMER1_BASE_ADDRESS,
	WTIMER2_BASE_ADDRESS,WTIMER3_BASE_ADDRESS,WTIMER4_BASE_ADDRESS,WTIMER5_BASE_ADDRESS};

	
  static GptChannelSetting_s GptChannelSetting[GPT_MAX_CHANNEL_CONFIGURATION] = {0};
	
	/* Static member to save the state of each timer channel */
	static GptChannelState_e GptChannelState[GPT_MAX_CHANNEL_CONFIGURATION] = { CHANNEL_UNINIT };
	static GptChannelNotificationState_e	GptChannelNotificationState [GPT_MAX_CHANNEL_CONFIGURATION] = {NOTIFICATION_DISABLED};
	static Gpt_ValueType GptChannelTargetTime[GPT_MAX_CHANNEL_CONFIGURATION] = { 0 };
	void SystemCoreClockUpdate (void);
	extern uint32 SystemCoreClock;

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/




/******************************************************************************
* \Syntax          : void Gpt_Init(const Gpt_ConfigType *ConfigPtr)        
* \Description     : Initialises the GPT driver                                                                                                                
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : ConfigPtr   Pointer to the configurations                     
* \Parameters (out): None                                                      
* \Return value:   : None                                  
*******************************************************************************/
void Gpt_Init(const Gpt_ConfigType *ConfigPtr)
{
	uint8 iteration = 0;
	SystemCoreClockUpdate();
	for (iteration = 0; iteration < ConfigPtr ->GptChannelConfigSet_numberOfInstances ; iteration++)
  {
     GptChannelSetting[GptChannelConfigSet[iteration].GptChannelId].Gpt_ChannelConfigState = TRUE;
     GptChannelSetting[GptChannelConfigSet[iteration].GptChannelId].Gpt_ChannelConfigIndex = iteration;
  }
	
	for (iteration = 0; iteration < GPT_MAX_CHANNEL_CONFIGURATION; iteration++)
	{
		 if (GptChannelSetting[iteration].Gpt_ChannelConfigState == TRUE) /* Check if this channel is configured */
		 {
			 GptChannelState[iteration] = CHANNEL_INIT;
			 ACCESS_REGISTER(Gpt_ChannelBaseAddress[iteration]+TIMER_CTL_OFFSET) &= ~1U;
			 ACCESS_REGISTER(Gpt_ChannelBaseAddress[iteration]+TIMER_CFG_OFFSET) = 0x4;
			 ACCESS_REGISTER(Gpt_ChannelBaseAddress[iteration]+TIMER_TAMR_OFFSET) =
					GptChannelConfigSet[GptChannelSetting[iteration].Gpt_ChannelConfigIndex].GptChannelMode;	
			 CLEAR_BIT(Gpt_ChannelBaseAddress[iteration]+TIMER_IMR_OFFSET,0);
			 uint16 Prescale_value = SystemCoreClock / GptChannelConfigSet[GptChannelSetting[iteration].Gpt_ChannelConfigIndex].GptChannelTickFrequency;
			 if(Prescale_value != 1)
			 {
				 uint8 Power = 0;
				 while(Prescale_value != 1)
				 {
						Prescale_value >>= 1;
						Power++;
				 }		 
				 GptChannelSetting[iteration].Gpt_PrescaleShifts = Power;
			 }			 
		 }
	}

#if (GPT_PREDEF_TIMER_1US_ENABLING_GRADE != GPT_PREDEF_TIMER_1US_DISABLED || GPT_PREDEF_TIMER_100US_32BIT_ENABLE == STD_ON)
			ACCESS_REGISTER(WTIMER5_BASE_ADDRESS+TIMER_CTL_OFFSET) &= ~1U;
		  ACCESS_REGISTER(WTIMER5_BASE_ADDRESS+TIMER_CFG_OFFSET) = 0x0;
		  ACCESS_REGISTER(WTIMER5_BASE_ADDRESS+TIMER_TAMR_OFFSET) = GPT_CH_MODE_CONTINUOUS;
			CLEAR_BIT(WTIMER5_BASE_ADDRESS+TIMER_IMR_OFFSET,0);
			ACCESS_REGISTER(WTIMER5_BASE_ADDRESS+TIMER_TBILR_OFFSET) = WTIMER_FULL_WIDTH >> 32;
			ACCESS_REGISTER(WTIMER5_BASE_ADDRESS+TIMER_TAILR_OFFSET) = WTIMER_FULL_WIDTH & 0xFFFFFFFF;
			ACCESS_REGISTER(WTIMER5_BASE_ADDRESS+TIMER_CTL_OFFSET) |= 3U;
#endif
}


/******************************************************************************
* \Syntax          : void Gpt_StartTimer(Gpt_ChannelType Channel, Gpt_ValueType Value)        
* \Description     : Starts a timer channel                                                                                                          
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant (but not for the same timer channel)                                             
* \Parameters (in) : Channel		Numeric identifier of the GPT channel.
										 Value			Target time in number of ticks. 

* \Parameters (out): None                                                      
* \Return value:   : None                                 
*******************************************************************************/

void Gpt_StartTimer(Gpt_ChannelType Channel, Gpt_ValueType Value)
{
	if (Channel > GPT_CHANNEL_11)
	{
		
	}
	else if ( GptChannelState[Channel] == CHANNEL_RUNNING || 
		GptChannelSetting[Channel].Gpt_ChannelConfigState == FALSE)
	{
		
	}
	else 
	{
		
		 if(GptChannelSetting[Channel].Gpt_PrescaleShifts == 0)
		 {
			 ACCESS_REGISTER(Gpt_ChannelBaseAddress[Channel]+TIMER_TAILR_OFFSET) = Value;
		 }
		 else
		 {
			 uint8 shifts = 16;
			 uint32 ILR_Mask = 0xffff;
			 if(Channel>GPT_CHANNEL_5)
			 {
				 shifts = 32;
				 ILR_Mask = 0xffffffff;
			 }
			 ACCESS_REGISTER(Gpt_ChannelBaseAddress[Channel]+TIMER_TAPR_OFFSET) = 
			 (Value << GptChannelSetting[Channel].Gpt_PrescaleShifts) >> shifts;
			 ACCESS_REGISTER(Gpt_ChannelBaseAddress[Channel]+TIMER_TAILR_OFFSET) = 
			 (Value << GptChannelSetting[Channel].Gpt_PrescaleShifts) & ILR_Mask;
		 }
		 SET_BIT(Gpt_ChannelBaseAddress[Channel]+TIMER_CTL_OFFSET,1); //Enable timer stalling while debugging
		 SET_BIT(Gpt_ChannelBaseAddress[Channel]+TIMER_CTL_OFFSET,0); //Enable timer 
		 GptChannelState[Channel] = CHANNEL_RUNNING;
		 GptChannelTargetTime[Channel] = Value;
	}
}

/******************************************************************************
* \Syntax          : void Gpt_StopTimer(Gpt_ChannelType Channel)     
* \Description     : Stops a timer channel                                                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant (but not for the same timer channel)                                             
* \Parameters (in) : Channel		Numeric identifier of the GPT channel.                       
* \Parameters (out): None                                                      
* \Return value:   : None                           
*******************************************************************************/
void Gpt_StopTimer(Gpt_ChannelType Channel)
{
	if (Channel > GPT_CHANNEL_11)
	{
		
	}
	else if ( GptChannelState[Channel] == CHANNEL_STOPPED || GptChannelSetting[Channel].Gpt_ChannelConfigState == FALSE)
	{
		
	}
	else 
	{
		CLEAR_BIT(Gpt_ChannelBaseAddress[Channel]+TIMER_CTL_OFFSET,0);
		GptChannelState[Channel] = CHANNEL_STOPPED;
	}
}

/******************************************************************************
* \Syntax          : void Gpt_EnableNotification(Gpt_ChannelType Channel)
* \Description     : Enables the interrupt notification for a channel                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant (but not for the same timer channel)                                             
* \Parameters (in) : Channel		Numeric identifier of the GPT channel.                    
* \Parameters (out): None                                                      
* \Return value:   : None                                 
*******************************************************************************/
void Gpt_EnableNotification(Gpt_ChannelType Channel)
{
	if (Channel > GPT_CHANNEL_11)
	{
		
	}
	else if (GptChannelSetting[Channel].Gpt_ChannelConfigState == FALSE)
	{
		
	}
	else if ( GptChannelConfigSet[GptChannelSetting[Channel].Gpt_ChannelConfigIndex].GptNotification == NULL_PTR)
	{
		
	}
	else if (GptChannelNotificationState[Channel] == NOTIFICATION_ENABLED)
	{
		
	}
	else
	{
		SET_BIT(Gpt_ChannelBaseAddress[Channel]+TIMER_IMR_OFFSET,0);
		GptChannelNotificationState[Channel] = NOTIFICATION_ENABLED;
	}
}


/******************************************************************************
* \Syntax          : void Gpt_DisableNotification(Gpt_ChannelType Channel)        
* \Description     : Disables the interrupt notification for a channel                                                      
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant (but not for the same timer channel)                                             
* \Parameters (in) : Channel		Numeric identifier of the GPT channel.                     
* \Parameters (out): None                                                      
* \Return value:   : None                                
*******************************************************************************/
void Gpt_DisableNotification(Gpt_ChannelType Channel)
{
	if (Channel > GPT_CHANNEL_11)
	{
		
	}
	else if (GptChannelSetting[Channel].Gpt_ChannelConfigState == FALSE)
	{
		
	}
	else if ( GptChannelConfigSet[GptChannelSetting[Channel].Gpt_ChannelConfigIndex].GptNotification == NULL_PTR)
	{
		
	}
	else if (GptChannelNotificationState[Channel] == NOTIFICATION_DISABLED)
	{
		
	}
	else
	{
		CLEAR_BIT(Gpt_ChannelBaseAddress[Channel]+TIMER_IMR_OFFSET,0);
		GptChannelNotificationState[Channel] = NOTIFICATION_DISABLED;
	}
}


/******************************************************************************
* \Syntax          : Gpt_ValueType Gpt_GetTimeElapsed(Gpt_ChannelType Channel)      
* \Description     : Returns the time already elapsed.                                                                                                                 
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant (but not for the same timer channel)                                             
* \Parameters (in) : Channel		Numeric identifier of the GPT channel.                     
* \Parameters (out): None                                                      
* \Return value:   : Gpt_ValueType		Elapsed timer value (in number of ticks)                                  
*******************************************************************************/

Gpt_ValueType Gpt_GetTimeElapsed(Gpt_ChannelType Channel)
{
	if (Channel > GPT_CHANNEL_11)
	{
		return FALSE;
	}
	else if (GptChannelSetting[Channel].Gpt_ChannelConfigState == FALSE || GptChannelState[Channel] == CHANNEL_INIT)
	{
				return FALSE;
	}
	else if (GptChannelState[Channel] == CHANNEL_EXPIRED)
	{
		return GptChannelTargetTime[Channel];
	}
	else
	{
		uint64 Timer_Value = 0;
		if(GptChannelSetting[Channel].Gpt_PrescaleShifts == 0)
		{
			Timer_Value = ACCESS_REGISTER(Gpt_ChannelBaseAddress[Channel]+TIMER_TAR_OFFSET);
		}
		else 
		{
			if (Channel <= GPT_CHANNEL_5)
			{		
				Timer_Value = ACCESS_REGISTER(Gpt_ChannelBaseAddress[Channel]+TIMER_TAV_OFFSET) >> GptChannelSetting[Channel].Gpt_PrescaleShifts;
			}
			else 
			{
				uint16 Prescale_Value = (uint16) ACCESS_REGISTER(Gpt_ChannelBaseAddress[Channel]+TIMER_TAPV_OFFSET);
				Timer_Value = ACCESS_REGISTER(Gpt_ChannelBaseAddress[Channel]+TIMER_TAR_OFFSET);
				Timer_Value = (((uint64)Prescale_Value << 32) | Timer_Value) >> GptChannelSetting[Channel].Gpt_PrescaleShifts;
			}
		}
		if ((Timer_Value == 0) && (GptChannelConfigSet[GptChannelSetting[Channel].Gpt_ChannelConfigIndex].GptChannelMode == GPT_CH_MODE_ONESHOT)
                && (GptChannelState[Channel] == CHANNEL_RUNNING))
        {
            GptChannelState[Channel] = CHANNEL_EXPIRED;
            return GptChannelTargetTime[Channel];
        }
			else
			{
				return ((uint32)Timer_Value & GptChannelConfigSet[GptChannelSetting[Channel].Gpt_ChannelConfigIndex].GptChannelTickValueMax);
			}
	}
}

/******************************************************************************
* \Syntax          : Gpt_ValueType Gpt_GetTimeRemainig(Gpt_ChannelType Channel)      
* \Description     : Returns the time remaining until the target time is reached.                                                                                                                
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                          
* \Parameters (in) : Channel		Numeric identifier of the GPT channel.                     
* \Parameters (out): None                                                      
* \Return value:   : Gpt_ValueType		Elapsed timer value (in number of ticks)                                  
*******************************************************************************/

Gpt_ValueType Gpt_GetTimeRemaining(Gpt_ChannelType Channel)
{
	if (Channel > GPT_CHANNEL_11)
	{
			return FALSE;
	}
	else if (GptChannelSetting[Channel].Gpt_ChannelConfigState == FALSE || GptChannelState[Channel] == CHANNEL_INIT)
	{
			return FALSE;
	}
	else if (GptChannelState[Channel] == CHANNEL_EXPIRED)
	{
			return 0;
	}
	else
	{
		uint64 Timer_Value = 0;
		if(GptChannelSetting[Channel].Gpt_PrescaleShifts == 0)
		{
			Timer_Value = ACCESS_REGISTER(Gpt_ChannelBaseAddress[Channel]+TIMER_TAR_OFFSET);
		}
		else 
		{
			if (Channel <= GPT_CHANNEL_5)
			{		
				Timer_Value = ACCESS_REGISTER(Gpt_ChannelBaseAddress[Channel]+TIMER_TAV_OFFSET) >> GptChannelSetting[Channel].Gpt_PrescaleShifts;
			}
			else 
			{
				uint16 Prescale_Value = (uint16) ACCESS_REGISTER(Gpt_ChannelBaseAddress[Channel]+TIMER_TAPV_OFFSET);
				Timer_Value = ACCESS_REGISTER(Gpt_ChannelBaseAddress[Channel]+TIMER_TAR_OFFSET);
				Timer_Value = (((uint64)Prescale_Value << 32) | Timer_Value) >> GptChannelSetting[Channel].Gpt_PrescaleShifts;
			}
		}
		if ((Timer_Value == 0) && (GptChannelConfigSet[GptChannelSetting[Channel].Gpt_ChannelConfigIndex].GptChannelMode == GPT_CH_MODE_ONESHOT)
                && (GptChannelState[Channel] == CHANNEL_RUNNING))
        {
            GptChannelState[Channel] = CHANNEL_EXPIRED;
            return 0;
        }
			else
			{
				return (GptChannelTargetTime[Channel]-((uint32)Timer_Value & GptChannelConfigSet[GptChannelSetting[Channel].Gpt_ChannelConfigIndex].GptChannelTickValueMax));
			}
	}
}


/******************************************************************************
* \Syntax          : Std_ReturnType FunctionName(AnyType parameterName)        
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Describtion                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/

#if (GPT_PREDEF_TIMER_1US_ENABLING_GRADE != GPT_PREDEF_TIMER_1US_DISABLED || GPT_PREDEF_TIMER_100US_32BIT_ENABLE == STD_ON)

Std_ReturnType Gpt_GetPredefTimerValue( Gpt_PredefTimerType PredefTimer, uint32* TimeValuePtr)
{
	if (PredefTimer > GPT_PREDEF_TIMER_100US_32BIT ||
            ((PredefTimer > GPT_PREDEF_TIMER_1US_ENABLING_GRADE)
            && (PredefTimer < GPT_PREDEF_TIMER_100US_32BIT))
            || ((PredefTimer == GPT_PREDEF_TIMER_100US_32BIT)
            && (GPT_PREDEF_TIMER_100US_32BIT_ENABLE == STD_OFF)))
    {

        return E_NOT_OK;
    }
		else if(TimeValuePtr == NULL_PTR)
		{
			  return E_NOT_OK;
		}
		else
		{
				static uint8 API_first_entry = TRUE; /* To save if it is API first call or not */
        static double Prescaling = 0; /* To save pre-scaling needed */
        uint64 Timer_value_1us = 0; /* To save timer register value */

        /*
         * Check if it is the API first call and if it is,
         * then calculate needed pre-scaling and save it for next API calls
         */

        if (API_first_entry == TRUE)
        {
            /* Get Mcu clock frequency */
            uint32 Mcu_Freq = SystemCoreClock;

            /* Calculate needed ratio to obtain 1 microsecond tick */
            Prescaling =  Mcu_Freq / 1000000.0;

            API_first_entry = FALSE; /* Change API first entry flag */
        }
				
				uint32 Timer_HighByte = ACCESS_REGISTER(WTIMER5_BASE_ADDRESS+TIMER_TBR_OFFSET);
				uint32 Timer_LowByte  = ACCESS_REGISTER(WTIMER5_BASE_ADDRESS+TIMER_TAR_OFFSET);

        /* Get value of timer register and divide it by prescaling value so time is in micro seconds */
        Timer_value_1us = (((uint64)Timer_HighByte<<32)| (uint64)Timer_LowByte )/ Prescaling;


        /* Check which predefined timer is called */

        switch (PredefTimer)
        {
        case GPT_PREDEF_TIMER_1US_16BIT:

            *TimeValuePtr = Timer_value_1us & MASK_16_BITS;
            return E_OK;

        case GPT_PREDEF_TIMER_1US_24BIT:

            /* SWS_Gpt_00396, Save the value of the least significant first 24 bits of timer value after pre-scaling at pointer passed to API call*/
            *TimeValuePtr = Timer_value_1us & MASK_24_BITS;
            return E_OK;

        case GPT_PREDEF_TIMER_1US_32BIT:

            /* SWS_Gpt_00396, Save the value of the least significant first 32 bits of timer value after pre-scaling at pointer passed to API call*/
            *TimeValuePtr = Timer_value_1us & MASK_32_BITS;
            return E_OK;

        case GPT_PREDEF_TIMER_100US_32BIT:

            /* SWS_Gpt_00396, Save the value of the least significant 32 bits of timer value after pre-scaling at pointer passed to API call*/
            *TimeValuePtr = (Timer_value_1us / 100) & MASK_32_BITS;
            return E_OK;

        default:
            return E_NOT_OK;
        }
			}
}
#endif

void Gpt_Notification_0 (void){

    ACCESS_REGISTER(TIMER0_BASE_ADDRESS + TIMER_ICR_OFFSET) = 1; /* Clear interrupt flag */

    /* Change channel state in case of one shot to expired state */
    if (GptChannelConfigSet[GptChannelSetting[0].Gpt_ChannelConfigIndex].GptChannelMode == GPT_CH_MODE_ONESHOT)
    {
        GptChannelState[0] = CHANNEL_EXPIRED;
    }

    (*(GptChannelConfigSet[GptChannelSetting[0].Gpt_ChannelConfigIndex].GptNotification))();

}

void Gpt_Notification_1(void){

    ACCESS_REGISTER(TIMER1_BASE_ADDRESS + TIMER_ICR_OFFSET) = 1; /* Clear interrupt flag */

    /* Change channel state in case of one shot to expired state */
    if (GptChannelConfigSet[GptChannelSetting[1].Gpt_ChannelConfigIndex].GptChannelMode == GPT_CH_MODE_ONESHOT)
    {
        GptChannelState[1] = CHANNEL_EXPIRED;
    }

    (*(GptChannelConfigSet[GptChannelSetting[1].Gpt_ChannelConfigIndex].GptNotification))();

}

void Gpt_Notification_2 (void){

    ACCESS_REGISTER(TIMER2_BASE_ADDRESS + TIMER_ICR_OFFSET) = 1; /* Clear interrupt flag */

    /* Change channel state in case of one shot to expired state */
    if (GptChannelConfigSet[GptChannelSetting[2].Gpt_ChannelConfigIndex].GptChannelMode == GPT_CH_MODE_ONESHOT)
    {
        GptChannelState[2] = CHANNEL_EXPIRED;
    }

    (*(GptChannelConfigSet[GptChannelSetting[2].Gpt_ChannelConfigIndex].GptNotification))();

}

void Gpt_Notification_3 (void){

    ACCESS_REGISTER(TIMER3_BASE_ADDRESS + TIMER_ICR_OFFSET) = 1; /* Clear interrupt flag */

    /* Change channel state in case of one shot to expired state */
    if (GptChannelConfigSet[GptChannelSetting[3].Gpt_ChannelConfigIndex].GptChannelMode == GPT_CH_MODE_ONESHOT)
    {
        GptChannelState[3] = CHANNEL_EXPIRED;
    }

    (*(GptChannelConfigSet[GptChannelSetting[3].Gpt_ChannelConfigIndex].GptNotification))();

}

void Gpt_Notification_4 (void){

    ACCESS_REGISTER(TIMER4_BASE_ADDRESS + TIMER_ICR_OFFSET) = 1; /* Clear interrupt flag */

    /* Change channel state in case of one shot to expired state */
    if (GptChannelConfigSet[GptChannelSetting[4].Gpt_ChannelConfigIndex].GptChannelMode == GPT_CH_MODE_ONESHOT)
    {
        GptChannelState[4] = CHANNEL_EXPIRED;
    }

    (*(GptChannelConfigSet[GptChannelSetting[4].Gpt_ChannelConfigIndex].GptNotification))();

}

void Gpt_Notification_5 (void){

    ACCESS_REGISTER(TIMER5_BASE_ADDRESS + TIMER_ICR_OFFSET) = 1; /* Clear interrupt flag */

    /* Change channel state in case of one shot to expired state */
    if (GptChannelConfigSet[GptChannelSetting[5].Gpt_ChannelConfigIndex].GptChannelMode == GPT_CH_MODE_ONESHOT)
    {
        GptChannelState[5] = CHANNEL_EXPIRED;
    }

    (*(GptChannelConfigSet[GptChannelSetting[5].Gpt_ChannelConfigIndex].GptNotification))();

}

void Gpt_Notification_6 (void){

    ACCESS_REGISTER(WTIMER0_BASE_ADDRESS + TIMER_ICR_OFFSET) = 1; /* Clear interrupt flag */

    /* Change channel state in case of one shot to expired state */
    if (GptChannelConfigSet[GptChannelSetting[6].Gpt_ChannelConfigIndex].GptChannelMode == GPT_CH_MODE_ONESHOT)
    {
        GptChannelState[6] = CHANNEL_EXPIRED;
    }

    (*(GptChannelConfigSet[GptChannelSetting[6].Gpt_ChannelConfigIndex].GptNotification))();

}

void Gpt_Notification_7 (void){

    ACCESS_REGISTER(WTIMER1_BASE_ADDRESS + TIMER_ICR_OFFSET) = 1; /* Clear interrupt flag */

    /* Change channel state in case of one shot to expired state */
    if (GptChannelConfigSet[GptChannelSetting[7].Gpt_ChannelConfigIndex].GptChannelMode == GPT_CH_MODE_ONESHOT)
    {
        GptChannelState[7] = CHANNEL_EXPIRED;
    }

    (*(GptChannelConfigSet[GptChannelSetting[7].Gpt_ChannelConfigIndex].GptNotification))();

}

void Gpt_Notification_8 (void){

    ACCESS_REGISTER(WTIMER2_BASE_ADDRESS + TIMER_ICR_OFFSET) = 1; /* Clear interrupt flag */

    /* Change channel state in case of one shot to expired state */
    if (GptChannelConfigSet[GptChannelSetting[8].Gpt_ChannelConfigIndex].GptChannelMode == GPT_CH_MODE_ONESHOT)
    {
        GptChannelState[8] = CHANNEL_EXPIRED;
    }

    (*(GptChannelConfigSet[GptChannelSetting[8].Gpt_ChannelConfigIndex].GptNotification))();

}

void Gpt_Notification_9 (void){

    ACCESS_REGISTER(WTIMER3_BASE_ADDRESS + TIMER_ICR_OFFSET) = 1; /* Clear interrupt flag */

    /* Change channel state in case of one shot to expired state */
    if (GptChannelConfigSet[GptChannelSetting[9].Gpt_ChannelConfigIndex].GptChannelMode == GPT_CH_MODE_ONESHOT)
    {
        GptChannelState[9] = CHANNEL_EXPIRED;
    }

    (*(GptChannelConfigSet[GptChannelSetting[9].Gpt_ChannelConfigIndex].GptNotification))();

}

void Gpt_Notification_10 (void){

    ACCESS_REGISTER(WTIMER4_BASE_ADDRESS + TIMER_ICR_OFFSET) = 1; /* Clear interrupt flag */

    /* Change channel state in case of one shot to expired state */
    if (GptChannelConfigSet[GptChannelSetting[10].Gpt_ChannelConfigIndex].GptChannelMode == GPT_CH_MODE_ONESHOT)
    {
        GptChannelState[10] = CHANNEL_EXPIRED;
    }

    (*(GptChannelConfigSet[GptChannelSetting[10].Gpt_ChannelConfigIndex].GptNotification))();

}

void Gpt_Notification_11 (void){

    ACCESS_REGISTER(WTIMER5_BASE_ADDRESS + TIMER_ICR_OFFSET) = 1; /* Clear interrupt flag */

    /* Change channel state in case of one shot to expired state */
    if (GptChannelConfigSet[GptChannelSetting[11].Gpt_ChannelConfigIndex].GptChannelMode == GPT_CH_MODE_ONESHOT)
    {
        GptChannelState[11] = CHANNEL_EXPIRED;
    }

    (*(GptChannelConfigSet[GptChannelSetting[11].Gpt_ChannelConfigIndex].GptNotification))();

}
/**********************************************************************************************************************
 *  END OF FILE: FileName.c
 *********************************************************************************************************************/
