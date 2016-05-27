/**
  ******************************************************************************
  * @file    lowpower.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file includes the low power driver for the STM320518-EVAL 
  *          demonstration.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM320518_EVAL_Demo
  * @{
  */

/** @defgroup LOWPOWER
  * @brief    This file includes the low power driver for the STM320518-EVAL 
  *           demonstration.
  * @{
  */

/** @defgroup LOWPOWER_Private_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup LOWPOWER_Private_Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup LOWPOWER_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup LOWPOWER_Private_Variables
  * @{
  */
static __IO uint8_t STOPModeStatus = 0;
uint32_t GPIOA_MODER = 0, GPIOB_MODER = 0, GPIOC_MODER = 0, GPIOD_MODER = 0,
                                        GPIOF_MODER = 0xA8000000;

uint32_t GPIOA_PUPDR = 0x64000000, GPIOB_PUPDR = 0x00000100, GPIOC_PUPDR = 0, GPIOD_PUPDR = 0,
                       GPIOF_PUPDR = 0;
/**
  * @}
  */

/** @defgroup LOWPOWER_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup LOWPOWER_Private_Functions
  * @{
  */

/**
  * @brief  Initializes Low Power application.
  * @param  None
  * @retval None
  */
void LowPower_Init(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  /* Enable Clock Security System(CSS) */
  RCC_ClockSecuritySystemCmd(ENABLE);
}

/**
  * @brief  Configures system clock after wake-up from STOP: enable HSE, PLL
  *         and select PLL as system clock source.
  * @param  None
  * @retval None
  */
void LowPower_SYSCLKConfig_STOP(void)
{
  ErrorStatus HSEStartUpStatus;
  
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  
  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable PLL */
    RCC_PLLCmd(ENABLE);
    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {}
    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)
    {}
  }
}

/**
  * @brief  Enters MCU in STOP mode. The wake-up from STOP mode is performed by 
  *         an external interrupt.
  * @param  None
  * @retval None
  */
void LowPower_EnterSTOPMode_EXTI(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Clear the STOPModeStatus variable */
  LowPower_Reset_STOPModeStatus();

  /* Clear the LCD */
 

  /* Set the LCD Back Color */
 
  /* Set the LCD Text Color */
  
  
  /* External Interrupt Disable */
  //Demo_IntExtOnOffCmd(DISABLE);
  
  
  
  /* Clear the RTC Alarm flag */
  RTC_ClearFlag(RTC_FLAG_ALRAF);

  /* Save the GPIO pins current configuration then put all GPIO pins in Analog
     Input mode ...*/
  LowPower_SaveGPIOConfig();
  
  /* Enable the debug when the MCU enter in stop mode */
  DBGMCU_Config(DBGMCU_STOP, ENABLE);
  
  /* Configure Button pin as input */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = KEY_BUTTON_PIN;
  GPIO_Init(KEY_BUTTON_GPIO_PORT, &GPIO_InitStructure); 
  
  /* Connect Button EXTI Line to Button GPIO Pin */
  SYSCFG_EXTILineConfig(KEY_BUTTON_EXTI_PORT_SOURCE, KEY_BUTTON_EXTI_PIN_SOURCE);
  
  /* Configure the EXTI Line 8 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Line = EXTI_Line8;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the EXTI9_5 Interrupt */
  //NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
  //NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //NVIC_Init(&NVIC_InitStructure);
  
  /* Request to enter STOP mode with regulator in low power */
  PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);

  /* Restore the GPIO Configurations*/
  LowPower_RestoreGPIOConfig();
  
  /* At this stage the system has resumed from STOP mode ************************/
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Line = EXTI_Line8;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the EXTI9_5 Interrupt */
  //NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
  //NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //NVIC_Init(&NVIC_InitStructure);
  
  /* Configures system clock after wake-up from STOP: enable HSE, PLL and select PLL
     as system clock source (HSE and PLL are disabled in STOP mode) */
  LowPower_SYSCLKConfig_STOP();
  
  if (STOPModeStatus != RESET)
  {
   
  }
  else
  {
    
  }

  
  
  /* External Interrupt Enable */
  //Demo_IntExtOnOffCmd(ENABLE);
}

/**
  * @brief  Enters MCU in STOP mode. The wake-up from STOP mode is performed by 
  *         an RTC Alarm.
  * @param  None
  * @retval None
  */
void LowPower_EnterSTOPMode_RTCAlarm(void)
{
  /* Clear the LCD */
  
  
  /* Set the LCD Back Color */
 // LCD_SetBackColor(LCD_COLOR_BLUE);
  
  /* Set the LCD Text Color */
  
  /* External Interrupt Disable */
  //Demo_IntExtOnOffCmd(DISABLE);
  
  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5AA5)
  {
    
    
    
    /* External Interrupt Enable */
    //Demo_IntExtOnOffCmd(ENABLE);
    return;
  }

  Calendar_AlarmPreAdjust_A();
  
 
  /* Save the GPIO pins current configuration then put all GPIO pins in Analog Input mode */
  LowPower_SaveGPIOConfig();

  /* Request to enter STOP mode with regulator in low power */
  PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
  /* Restore the GPIO Configurations*/
  LowPower_RestoreGPIOConfig();

  /* Configures system clock after wake-up from STOP: enable HSE, PLL and select PLL
     as system clock source (HSE and PLL are disabled in STOP mode) */
  LowPower_SYSCLKConfig_STOP();
  
  /* External Interrupt Enable */
  //Demo_IntExtOnOffCmd(ENABLE);
}

/**
  * @brief  Enters MCU in STANDBY mode. The wake-up from STANDBY mode is performed 
  *         when a rising edge is detected on WakeUp pin.
  * @param  None
  * @retval None
  */
void LowPower_EnterSTANDBYMode_WAKEUP_1(void)
{
//  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set the LCD Back Color */
//  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the LCD Text Color */
//  LCD_SetTextColor(LCD_COLOR_WHITE);
  
//  LCD_DisplayStringLine(LCD_LINE_7, " MCU in STANDBY Mode");
//  LCD_DisplayStringLine(LCD_LINE_8, " To exit press SEL  ");
  /* Check if the StandBy flag is set */
  if (PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)
  {       
    /* Clear StandBy flag */
    PWR_ClearFlag(PWR_FLAG_SB);
    
    RTC_WaitForSynchro();
  }
  
  RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
  
  /* Enable WakeUp pin */
  PWR_WakeUpPinCmd(PWR_WakeUpPin_1, ENABLE);
  
  /* Request to enter STANDBY mode (Wake Up flag is cleared in PWR_EnterSTANDBYMode function) */
  PWR_EnterSTANDBYMode();
}

/**
  * @brief  Enters MCU in STANDBY mode. The wake-up from STANDBY mode is performed 
  *         by an RTC Alarm event.
  * @param  None
  * @retval None
  */
void LowPower_EnterSTANDBYMode_RTCAlarm(void)
{
 // LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set the LCD Back Color */
//  LCD_SetBackColor(LCD_COLOR_BLUE);
  
  /* Set the LCD Text Color */
 // LCD_SetTextColor(LCD_COLOR_WHITE);
  
  /* External Interrupt Disable */
  //Demo_IntExtOnOffCmd(DISABLE);
  
  /* Enable WakeUp pin */
  PWR_WakeUpPinCmd(PWR_WakeUpPin_1, ENABLE);
    
  /* Check if the StandBy flag is set */
  if (PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)
  {       
    /* Clear StandBy flag */
    PWR_ClearFlag(PWR_FLAG_SB);
   
    RTC_WaitForSynchro();
  }
  
  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5AA5)
  {
  //  LCD_DisplayStringLine(LCD_LINE_1, "Time and Date are   ");
   // LCD_DisplayStringLine(LCD_LINE_2, "not configured,     ");
  //  LCD_DisplayStringLine(LCD_LINE_3, "please go to the    ");
  //  LCD_DisplayStringLine(LCD_LINE_4, "calendar menu and   ");
  //  LCD_DisplayStringLine(LCD_LINE_5, "set the time and    ");
  //  LCD_DisplayStringLine(LCD_LINE_6, "date parameters.    ");
  //  LCD_DisplayStringLine(LCD_LINE_7, "Press JoyStick to   ");
  //  LCD_DisplayStringLine(LCD_LINE_8, "continue...         ");
    
    
    /* External Interrupt Enable */
    //Demo_IntExtOnOffCmd(ENABLE);
    return;
  }
  
  Calendar_AlarmPreAdjust_A();

 // LCD_DisplayStringLine(LCD_LINE_7, " MCU in STANDBY Mode");
 // LCD_DisplayStringLine(LCD_LINE_8, " Wait For RTC Alarm ");
  
  /* Request to enter STANDBY mode (Wake Up flag is cleared in PWR_EnterSTANDBYMode function) */
  PWR_EnterSTANDBYMode();
}

/**
  * @brief  Sets STOPModeStatus variable.
  * @param  None
  * @retval None
  */
void LowPower_Set_STOPModeStatus(void)
{
  STOPModeStatus = 1;
}

/**
  * @brief  Reset the STOPModeStatus variable.
  * @param  None
  * @retval None
  */
void LowPower_Reset_STOPModeStatus(void)
{
  STOPModeStatus = 0;
}

/**
  * @brief  Save all GPIOs Configurations.
  * @param  None
  * @retval None
  */
void LowPower_SaveGPIOConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIOA_MODER = GPIOA->MODER;
  GPIOB_MODER = GPIOB->MODER;
  GPIOC_MODER = GPIOC->MODER;
  GPIOD_MODER = GPIOD->MODER;
  GPIOF_MODER = GPIOF->MODER;
  
  GPIOA_PUPDR = GPIOA->PUPDR;
  GPIOB_PUPDR = GPIOB->PUPDR;
  GPIOC_PUPDR = GPIOC->PUPDR;
  GPIOD_PUPDR = GPIOD->PUPDR;
  GPIOF_PUPDR = GPIOF->PUPDR;
  
  /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOF, &GPIO_InitStructure);
}

/**
  * @brief  Restores all GPIOs Configurations.
  * @param  None
  * @retval None
  */
void LowPower_RestoreGPIOConfig(void)
{
  GPIOA->MODER = GPIOA_MODER;
  GPIOB->MODER = GPIOB_MODER;
  GPIOC->MODER = GPIOC_MODER;
  GPIOD->MODER = GPIOD_MODER;
  GPIOF->MODER = GPIOF_MODER;
  
  GPIOA->PUPDR = GPIOA_PUPDR;
  GPIOB->PUPDR = GPIOB_PUPDR;
  GPIOC->PUPDR = GPIOC_PUPDR;
  GPIOD->PUPDR = GPIOD_PUPDR;
  GPIOF->PUPDR = GPIOF_PUPDR;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
