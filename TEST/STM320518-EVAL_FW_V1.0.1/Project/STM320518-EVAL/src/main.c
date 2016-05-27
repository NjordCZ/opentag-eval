/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   Main program body
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

/** @defgroup MAIN
  * @brief    Main program body
  * @{
  */

/** @defgroup MAIN_Private_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup MAIN_Private_Defines
  * @{
  */

/**
  * @}
  */

/** @defgroup MAIN_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup MAIN_Private_Variables
  * @{
  */

static __IO uint32_t TimingDelay = 0;
static __IO uint32_t LedShowStatus = 0;
static __IO uint32_t DOWNStatus = 0x00, UPStatus = 0x00, SELStatus = 0x00;
__IO uint32_t ADCConversionValue = 0x00;

//Timekeeping variable
volatile uint32_t MSec = 0;
volatile uint32_t tmp_nvic = 88;

//extern gun and player
extern player_typedef player; // vlastnosti hrace
extern gun_typedef gun; // vlastnosti zbrane


/**
  * @}
  */

/** @defgroup MAIN_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup MAIN_Private_Functions
  * @{
  */

//Standard delay function! Executes the
//nop instruction until T milliseconds have
//passed.
void Delay(uint32_t T){
	volatile uint32_t MSS = MSec;
	while((MSec-MSS)<T) asm volatile("nop");
}

//  error
void Error(char* error){
#ifdef DEBUG

  I2C_SSD1306_Fill(SSD1306_COLOR_BLACK);	
  I2C_SSD1306_GotoXY(10, 45);
  I2C_SSD1306_Puts(error, &TM_Font_11x18, SSD1306_COLOR_WHITE);
  /* Update screen, send changes to LCD */
  I2C_SSD1306_UpdateScreen();
  Delay(3000);
#endif  
}



        
        

    
    
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
  
    
    
    
    
int main(void)
{
 

  
  
/* Initialize the Demo */
//  Demo_Init();   

/* Configure the systick */
  SysTick_Init();
  
  
/* IO init */
  Output_Init();
  Input_Init();

  
/* RTC init, TIM14 */
Time_Date_Init();


  
/* Initialize the HD44780 - LCD */
 // H_HWInit();

//ClrDisp(); //Clear the current display.
//H_W8b(H_DispCtrl|H_DispOn|H_CursorOff|H_CursrPosNBlnk, 0);  // disable the blinking cursor
//PStr("START", 0, 1); //Print the string to the screen at 0,1!

 
/* Initialize the I2C OLED */
  I2C_SSD1306_Init();

  
//  Lasertag_LCD_Update();
    

    
 

 /*
 Initialize the SD card wave player
  
 This application assumes that the .WAV file to be played has the following format:
• Audio Format: PCM (an uncompressed wave data format in which each value
represents the amplitude of the signal at the time of sampling)
• Sample rate: 22050 Hz     /may be 8000, 11025, 22050 or 44100 Hz
• Bits Per Sample: 8-bit (Audio sample data values are in the range [0-255])
• Number of Channels: 1 (Mono)
*/
WavePlayer_Init();  

/* Lasertag init set default */
Lasertag_Init_default();
 
/* Lasertag init, fire timer */
Lasertag_Init(gun.FireRate);

/* IR HW init */  
IR_Encode_Init();   
IR_Decode_Init();   


Bluetooth_Init();


Delay(500);
 



#ifdef GUN
Lasertag_LCD_Init_Screen("GUN");
#endif
#ifdef HEADBAND
Lasertag_LCD_Init_Screen("HEADBAND");
#endif
#ifdef ADMIN
Lasertag_LCD_Init_Screen("ADMIN");
#endif






while (1)
  {
    
/////////////////////////////////////
// TEST
////////////////////////////////////
/*
  
  TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);
  
  TIM_Cmd(TIM16, ENABLE);


    
    
    TIM_ForcedOC1Config(TIM16, TIM_ForcedAction_Active);
    Delay(100);
    TIM_ForcedOC1Config(TIM16, TIM_ForcedAction_InActive);
    Delay(100);

*/ 
    
  } // while end
}

/**
  * @brief  Initializes the demonstration application.
  * @param  None
  * @retval None
  */
void Demo_Init(void)
{
  /*------------------- Resources Initialization -----------------------------*/
  /* Enable GPIOA, GPIOB and GPIOC clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | \
                        RCC_AHBPeriph_GPIOF, ENABLE);
  
  /* Initialize the LED's, Joystick, Button Key and Button Tamper */
  //Demo_EVAL_Init();
  
    

  /* Interrupt Configuration */
  //Demo_Interrupt_Init();

  

  /* Configure the systick */
  //SysTick_Init();
  
  
  /* Enable Leds toggling */
  //Demo_LedShow(ENABLE);
  
  
  /*------------------- Drivers Initialization -------------------------------*/
  
  /* Initialize the low power application */
  //LowPower_Init();
  
  /* If HSE is not detected at program startup */
  //if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
  //{
  //  /* Generate NMI exception */
  //  SCB->ICSR |= SCB_ICSR_NMIPENDSET_Msk;
  //}
  
  /* Checks the availability of the bitmap files */
  //Demo_CheckBitmapFilesStatus(); 
  
  /* Display the STM32 introduction */
  //Menu_STM32Intro();
  
  
  
}

/**
  * @brief  Configures the used IRQ Channels and sets their priority.
  * @param  None
  * @retval None
  */
void Demo_Interrupt_Init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  /* Enable the TIM6 Interrupt */
  //NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
  //NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //NVIC_Init(&NVIC_InitStructure);

  /* Enable the TIM15 UP Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM15_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  

  /* Enable the EXTI line 17 (connected to the RTC Alarm event) */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
   

  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  

  /* Enable the EXTI4_15_IRQn Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x03;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  

  /* Enable the EXTI line 15 (connected to SDCard detect) */
  //EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  //EXTI_InitStructure.EXTI_Line = EXTI_Line15;
  //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  //EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  //EXTI_Init(&EXTI_InitStructure);
    

  /* Enable the EXTI0 Interrupt */
  //NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
  //NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //NVIC_Init(&NVIC_InitStructure);
  
   /* Enable the TIM6 Interrupt */
  //NVIC_InitStructure.NVIC_IRQChannel = TS_IRQn;
 // NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configure a SysTick Base time to 10 ms.
  * @param  None
  * @retval None
  */
void SysTick_Init(void)
{
  /* Setup SysTick Timer for 10 ms interrupts  */
  if (SysTick_Config(SystemCoreClock / 2000))
  {
    /* Capture error */
    while (1);
  }

  /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}

/**
  * @brief  Enables or disables EXTI for the menu navigation keys, i.e. EXTI lines 0,
  *         6 and 7 which correpond respectively to "SEL", "UP" and "DOWN".
  * @param  NewState: New state of the navigation keys. 
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void Demo_IntExtOnOffCmd(FunctionalState NewState)
{
  EXTI_InitTypeDef EXTI_InitStructure;
 
  /* Initializes the EXTI_InitStructure */
  EXTI_StructInit(&EXTI_InitStructure);

  /* Disable the EXTI line 0, 6 and 7 on falling edge */
  if (NewState == DISABLE)
  {
    EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line6 | EXTI_Line7;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE; //DISABLE;
    EXTI_Init(&EXTI_InitStructure);
  }
  /* Enable the EXTI line 0, 6 and 7 on falling edge */
  else
  {
    /* Clear the the EXTI line 0, 6 and 7 interrupt pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0 | EXTI_Line6 | EXTI_Line7);
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line6 | EXTI_Line7;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
  }
}


/**
  * @brief  random number generate
  * @param  none
  * @retval uint32_t random number
  */

uint32_t getTrueRandomNumber(void) {

ADC_InitTypeDef ADC_InitStructure;

//enable ADC1 clock
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

// Initialize ADC 14MHz RC
RCC_ADCCLKConfig(RCC_ADCCLK_HSI14);
RCC_HSI14Cmd(ENABLE);
while (!RCC_GetFlagStatus(RCC_FLAG_HSI14RDY))
    ;

ADC_DeInit(ADC1);
ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_TRGO; //default
ADC_Init(ADC1, &ADC_InitStructure);

//enable internal channel
ADC_TempSensorCmd(ENABLE);

// Enable ADCperipheral
ADC_Cmd(ADC1, ENABLE);
while (ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN) == RESET)
    ;

ADC1->CHSELR = 0; //no channel selected
//Convert the ADC1 temperature sensor, user shortest sample time to generate most noise
ADC_ChannelConfig(ADC1, ADC_Channel_TempSensor, ADC_SampleTime_1_5Cycles);

// Enable CRC clock
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);

uint8_t i;
for (i = 0; i < 8; i++) {
    //Start ADC1 Software Conversion
    ADC_StartOfConversion(ADC1);
    //wait for conversion complete
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)) {
    }

    CRC_CalcCRC(ADC_GetConversionValue(ADC1));
    //clear EOC flag
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
}

//disable ADC1 to save power
ADC_Cmd(ADC1, DISABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE);

return CRC_CalcCRC(0xBADA55E5);
}











/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length (time base 10 ms).
  * @retval None
  */
void Demo_Delay(__IO uint32_t nCount)
{
  TimingDelay = nCount;
  /* Enable the SysTick Counter */
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
  while (TimingDelay != 0);
  /* Disable the SysTick Counter */
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
  /* Clear the SysTick Counter */
  SysTick->VAL = (uint32_t)0x0;
}


/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void Demo_DecrementTimingDelay(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

/**
  * @brief  Delay.
  * @param  nCount
  * @retval None
  */
void Delay_Decrement(__IO uint32_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}

/**
  * @brief  Configures the leds pins as output pushpull: LED1, LED2, LED3 and LED4
  * @param  None
  * @retval None
  */
void Demo_EVAL_Init(void)
{
  //TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  //TIM_OCInitTypeDef  TIM_OCInitStructure;
  
  /* Configure Joystick pins as input floating with EXTI */
  //STM_EVAL_PBInit(BUTTON_UP, BUTTON_MODE_EXTI);
  //STM_EVAL_PBInit(BUTTON_DOWN, BUTTON_MODE_EXTI);
  //STM_EVAL_PBInit(BUTTON_SEL, BUTTON_MODE_EXTI);
  
  //STM_EVAL_PBInit(BUTTON_LEFT, BUTTON_MODE_EXTI);
  //STM_EVAL_PBInit(BUTTON_RIGHT, BUTTON_MODE_EXTI);
  
  /* Configure LEDs GPIO: as output push-pull */
  //STM_EVAL_LEDInit(LED1);
  //STM_EVAL_LEDInit(LED2);
  //STM_EVAL_LEDInit(LED3);
  //STM_EVAL_LEDInit(LED4);
  
  /* TIM15 clocks enable */
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE); 
  
  /* Time Base configuration 100ms*/
  //TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  //TIM_TimeBaseStructure.TIM_Prescaler = 1000;
  //TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  //TIM_TimeBaseStructure.TIM_Period = 0x0C80;
  //TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  //TIM_TimeBaseInit(TIM15, &TIM_TimeBaseStructure);
  
  /* Channel 1, 2, 3 and 4 Configuration in Timing mode */
  //TIM_OCStructInit(&TIM_OCInitStructure);
  //TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  //TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  //TIM_OCInitStructure.TIM_Pulse = 0x0;
  
  //TIM_OC1Init(TIM15, &TIM_OCInitStructure);
}

/**
  * @brief  Enables or disables LED1, LED2, LED3 and LED4 toggling.
  * @param  None
  * @retval None
  */
void Demo_LedShow(FunctionalState NewState)
{
  /* Enable LEDs toggling */
  if (NewState == ENABLE)
  {
    LedShowStatus = 1;
    /* Enable the TIM Update Interrupt */
    TIM_ITConfig(TIM15, TIM_IT_Update, ENABLE);
    /* TIM counter enable */
    TIM_Cmd(TIM15, ENABLE);
  }
  /* Disable LEDs toggling */
  else
  {
    LedShowStatus = 0;
    /* Disable the TIM Update Interrupt */
    TIM_ITConfig(TIM15, TIM_IT_Update, DISABLE);
    /* TIM counter disable */
    TIM_Cmd(TIM15, DISABLE);
  }
}

/**
  * @brief  Get the LedShowStatus value.
  * @param  None
  * @retval LedShowStatus Value.
  */
uint32_t Demo_Get_LedShowStatus(void)
{
  return LedShowStatus;
}

/**
  * @brief  Checks the bitmap files availability and display a warning message
  *         if these files doesn't exit.
  * @param  None
  * @retval None
  */
void Demo_CheckBitmapFilesStatus(void)
{
  /* Checks if the Bitmap files are loaded */
  if (Menu_CheckBitmapFiles() != 0)
  {
    /* Double checking for Demo to make sure micro SD can be correctly read*/
    if (Menu_CheckBitmapFiles() != 0)
    {
       /* Double checking for Demo to make sure micro SD can be correctly read*/
       if (Menu_CheckBitmapFiles() != 0)
        {
          /* Set the LCD Back Color */
         
          /* Set the LCD Text Color */
          

         /* Deinitializes the RCC */
          RCC_DeInit();

         /* Demo Can't Start */
         while (1)
        {}
      }  
    }
  }
}

/**
  * @brief  Sets the DOWNStatus variable
  * @param  None
  * @retval None
  */
void Set_DOWNStatus(void)
{
  DOWNStatus = 1;
}

/**
  * @brief  Sets the UPStatus variable
  * @param  None
  * @retval None
  */
void Set_UPStatus(void)
{
  UPStatus = 1;
}

/**
  * @brief  Sets the SELStatus variable
  * @param  None
  * @retval None
  */
void Set_SELStatus(void)
{
  SELStatus = 1;
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  LCD_Clear(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_0, "   Assert Function  ");
  /* Infinite loop */
  while (1)
  {}
}
#endif

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
