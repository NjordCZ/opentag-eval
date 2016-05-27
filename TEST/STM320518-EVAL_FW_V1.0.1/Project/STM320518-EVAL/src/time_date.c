/**
  ******************************************************************************
  * @file    time.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    
  * @brief   This file includes the time driver.
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "time_date.h"

/** @addtogroup Time
  * @{
  */

RTC_TimeTypeDef Time_Date_Time;
RTC_DateTypeDef Time_Date_Date;    
    
    
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RTC_InitTypeDef   RTC_InitStructure;
__IO uint32_t LsiFreq = 0;
__IO uint32_t CaptureNumber = 0, PeriodValue = 0;

/* Private function prototypes -----------------------------------------------*/
static void RTC_Config(void);
static uint32_t GetLSIFrequency(void);
static void RTC_Alarm(void);
static void RTC_TamperConfig(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
static void RTC_Config(void)
{
  RTC_TimeTypeDef RTC_TimeStructure;
 
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  
  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);

/* LSI used as RTC source clock */
/* The RTC Clock may varies due to LSI frequency dispersion. */   
  /* Enable the LSI OSC */ 
  RCC_LSICmd(ENABLE);

  /* Wait till LSI is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
   
  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();

  /* Calendar Configuration */
  //RTC_InitStructure.RTC_AsynchPrediv = 99;
  //RTC_InitStructure.RTC_SynchPrediv	=  399; /* (40KHz / 100) - 1 = 399*/
  //RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
  //RTC_Init(&RTC_InitStructure);  

 
  
  /* Set the time to 00h 00mn 00s AM */
  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  RTC_TimeStructure.RTC_Hours   = 0x00;
  RTC_TimeStructure.RTC_Minutes = 0x00;
  RTC_TimeStructure.RTC_Seconds = 0x00;  
  
  RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
  
  
    /* Get the LSI frequency:  TIM14 is used to measure the LSI frequency */
  LsiFreq = GetLSIFrequency();

  
  /* Calendar Configuration */
  RTC_InitStructure.RTC_AsynchPrediv = 99;
  RTC_InitStructure.RTC_SynchPrediv	=  (LsiFreq/100) - 1;
  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
  RTC_Init(&RTC_InitStructure);
  
}

/**
  * @brief  Configures TIM14 to measure the LSI oscillator frequency. 
  * @param  None
  * @retval LSI Frequency
  */
static uint32_t GetLSIFrequency(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;
  RCC_ClocksTypeDef  RCC_ClockFreq;

  /* TIM14 configuration *******************************************************/ 
  /* Enable TIM14 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
  
  /* Reset TIM14 registers */
  TIM_DeInit(TIM14);

  /* Configure TIM14 prescaler */
  TIM_PrescalerConfig(TIM14, 0, TIM_PSCReloadMode_Immediate);

  /* Connect internally the TIM14_CH1 to the RTC clock output */
  TIM_RemapConfig(TIM14, TIM14_RTC_CLK);

  /* TIM14 configuration: Input Capture mode ---------------------
     The reference clock(LSE or external) is connected to TIM14 CH1
     The Rising edge is used as active edge,
     The TIM14 CCR1 is used to compute the frequency value 
  ------------------------------------------------------------ */
  TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM14, &TIM_ICInitStructure);

  /* Enable the TIM14 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable TIM14 counter */
  TIM_Cmd(TIM14, ENABLE);

  /* Reset the flags */
  TIM14->SR = 0;
    
  /* Enable the CC1 Interrupt Request */  
  TIM_ITConfig(TIM14, TIM_IT_CC1, ENABLE);


  /* Wait until the TIM14 get 2 LSI edges (refer to TIM14_IRQHandler() in 
    stm32F0xx_it.c file) ******************************************************/
  while(CaptureNumber != 2)
  {
  }
  /* Deinitialize the TIM14 peripheral registers to their default reset values */
  TIM_DeInit(TIM14);


  /* Compute the LSI frequency, depending on TIM14 input clock frequency (PCLK1)*/
  /* Get SYSCLK, HCLK and PCLKx frequency */
  RCC_GetClocksFreq(&RCC_ClockFreq);
  
  /* PCLK1 prescaler equal to 1 => TIMCLK = PCLK1 */
  return ((RCC_ClockFreq.PCLK_Frequency / PeriodValue) * 8);
}


void RTC_Alarm(void)
{
  //RTC_InitTypeDef RTC_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  EXTI_InitTypeDef  EXTI_InitStructure;
  RTC_AlarmTypeDef  RTC_AlarmStructure;
  
  /* RTC Alarm A Interrupt Configuration */
  /* EXTI configuration *********************************************************/
  EXTI_ClearITPendingBit(EXTI_Line17);
  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the RTC Alarm Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Disable the Alarm A */
  RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
  
  
  //RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
  //RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = 0;
  //RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0;
  //RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 2;
  

  /* Set the Alarm A */
  //RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
  //RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_Weekday_Monday;    
  //RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;

  /* Configure the RTC Alarm A register */
  //RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
  

  



 /* Set the alarmA Masks */
 RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_All;
 RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);
  
  /* Set AlarmA subseconds and enable SubSec Alarm : generate 6 interripts per Second */
 RTC_AlarmSubSecondConfig(RTC_Alarm_A, 0x00007FFF, RTC_AlarmSubSecondMask_SS14_6);

  /* Enable AlarmA interrupt */
  RTC_ITConfig(RTC_IT_ALRA, ENABLE);
  
  

 /* Enable the RTC Alarm A Interrupt */
 // RTC_ITConfig(RTC_IT_ALRA, ENABLE);
   
  /* Enable the alarm  A */
  RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

}


/**
* @brief  RTC Tamper Configuration..
* @param  None
* @retval None
*/
void RTC_TamperConfig(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* EXTI configuration *********************************************************/
  EXTI_ClearITPendingBit(EXTI_Line19);
  EXTI_InitStructure.EXTI_Line = EXTI_Line19;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable RTC_IRQn */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* determines the number of active pulse for the specific level */
  RTC_TamperFilterConfig(RTC_TamperFilter_2Sample);
  
  /* Determines the frequency at which each of the tamper inputs are sampled */
  RTC_TamperSamplingFreqConfig(RTC_TamperSamplingFreq_RTCCLK_Div32768);
  
  RTC_TamperPullUpCmd(DISABLE);
  
  /* Select the tamper 1 with High level */
  RTC_TamperTriggerConfig(RTC_Tamper_1, RTC_TamperTrigger_LowLevel );
  
  /* Clear tamper 1 flag */
  RTC_ClearFlag(RTC_FLAG_TAMP1F);
}

/**
  * @brief  Init RTC. 
  * @param  None
  * @retval None
  */
void Time_Date_Init()
{
  
  RTC_Config();
  RTC_Alarm();
  //RTC_TamperConfig();
}




/**
  * @}
  */

/****************************END OF FILE****/
