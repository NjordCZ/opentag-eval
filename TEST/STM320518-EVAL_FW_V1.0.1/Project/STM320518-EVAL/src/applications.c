/**
  ******************************************************************************
  * @file    applications.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file includes the application driver for the STM32F0518-EVAL 
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

/** @defgroup APPLICATIONS
  * @brief    This file includes the application driver for the STM320518_EVAL
  *           demo.
  * @{
  */

/** @defgroup APPLICATIONS_Private_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup APPLICATIONS_Private_Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup APPLICATIONS_Private_Macros
  * @{  
  */

#define MESSAGE1     " **** STOPWATCH ****" 
#define MESSAGE2     " LEFT   |  RIGHT  |  DOWN   " 
#define MESSAGE7     " START  |   GET   |  EXIT   " 

#define MESSAGE4     " ****** Timer ******"
#define MESSAGE5     " LEFT   |  RIGHT  |  SEL  " 
#define MESSAGE6     " RESET  |  START  |  EXIT "
#define MESSAGE11    " LEFT        |        SEL " 
#define MESSAGE12    " RESET       |       EXIT " 

#define MESSAGE8     "        LDR         "

#define MESSAGE10   "EXIT : Keep pressing DOWN"


/**
  * @}
  */

/** @defgroup APPLICATIONS_Private_Variables
  * @{
  */
static __IO uint32_t TimingDelay;
__IO uint32_t SecondFrac = 0;
__IO uint8_t StartEvent = 0; 
RTC_InitTypeDef RTC_InitStructure;
RTC_TimeTypeDef  RTC_TimeStruct, RTC_TimeStruct1;
__IO uint8_t MenuApplication = 0;
extern __IO uint32_t RTCAlarmCount;
extern  __IO uint32_t SecondNumb;
extern __IO uint32_t CurrentTimeSec;
extern __IO uint32_t BackupIndex;
extern __IO uint32_t LCDType;
extern FATFS fs;
/**
  * @}
  */

/** @defgroup APPLICATIONS_Private_FunctionPrototypes
  * @{
  */
static void RTC_AlarmConfig(void);
static void RTC_Config(void);
static void RTC_TamperConfig(void);
static void RTC_Time_ClearDisplay(void);

static void COMP_Config(void);
static void DAC_Config(void);
/**
  * @}
  */

/** @defgroup APPLICATIONS_Private_Functions
  * @{
  */

/**
  * @brief  StopWatch application.
  * @param  None
  * @retval None
  */
void App_StopWatch_Start(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  
  MenuApplication = 1;
  StartEvent  = 0;
  
  /* External Interrupt Disable */
  Demo_IntExtOnOffCmd(DISABLE);
  
  /* Clear the LCD */ 
  LCD_Clear(White);
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(Blue);
  
  /* Set the LCD Text Color */
  LCD_SetTextColor(White);
  
  /* Displays MESSAGE1 on line 1 */
  LCD_DisplayStringLine(LINE(0), (uint8_t *)MESSAGE1);
  
  /* Set the LCD Text Color */
  LCD_SetTextColor(Red);
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(Red);
  LCD_DrawFullRect(31, 292,264,34);
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(White);
  LCD_DrawFullRect(33, 290 ,260,30);
  
  /* Configure the external interrupt on "RIGHT" and "LEFT" buttons */
  /* Connect Button EXTI Line to Button GPIO Pin */
  SYSCFG_EXTILineConfig(RIGHT_BUTTON_EXTI_PORT_SOURCE, RIGHT_BUTTON_EXTI_PIN_SOURCE);
  SYSCFG_EXTILineConfig(LEFT_BUTTON_EXTI_PORT_SOURCE, LEFT_BUTTON_EXTI_PIN_SOURCE);
  
  /* Configure Button EXTI line */
  EXTI_InitStructure.EXTI_Line = RIGHT_BUTTON_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  EXTI_InitStructure.EXTI_Line = LEFT_BUTTON_EXTI_LINE;
  EXTI_Init(&EXTI_InitStructure);
  
  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5AA5)
  {
    /* Configure the RTC peripheral by selecting the clock source.*/
    RTC_Config();
  } 
  
  /* Configure the RTC tamper register */
  RTC_TamperConfig();
    
  /* Set the LCD Text Color */
  LCD_SetTextColor(Black); 
  /* Displays MESSAGE1 on line 5 */
  LCD_SetFont(&Font12x12);
  /* Set the LCD Back Color */
  LCD_SetBackColor(Cyan);
  LCD_DisplayStringLine(LINE(18), (uint8_t *)MESSAGE2);
  /* Set the LCD Back Color */
  LCD_SetBackColor(Blue);
  LCD_SetTextColor(White);
  LCD_DisplayStringLine(LINE(19), (uint8_t *)MESSAGE7);
  LCD_SetFont(&Font16x24);
  
  /* Set the LCD Text Color */
  LCD_SetTextColor(Blue);
  
  /* Init time Stucture */
  RTC_TimeStructInit(&RTC_TimeStruct1); 
  
  /* Infinite loop */
  while (Menu_ReadKey() != DOWN)
  {
    LCD_SetFont(&Font16x24);
    /* Set the LCD Back Color */
    LCD_SetBackColor(White);
    
    /* Check on the event 'start'*/
    if(StartEvent != 0x0)
    {  
      /* Get the RTC sub second fraction */
      SecondFrac = (((256 - (uint32_t)RTC_GetSubSecond()) * 1000) / 256);
      
      /* Get the Curent time */
      RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct1);
      /* Display the curent time and the sub second on the LCD */
      RTC_Time_Display(37, Black , RTC_Get_Time(SecondFrac, &RTC_TimeStruct1));
    }
    else
    {
      RTC_Time_ClearDisplay();
    }  
  }
  
  MenuApplication = 0;
  StartEvent = 0;
  BackupIndex = 0;
  LCD_SetFont(&Font16x24);
  /* Checks the availability of the bitmap files */
  Demo_CheckBitmapFilesStatus(); 
  
  LCD_Clear(LCD_COLOR_WHITE);
  Menu_DisplayMenu();
  
  /* Configure Button EXTI line */
  EXTI_InitStructure.EXTI_Line = RIGHT_BUTTON_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  EXTI_InitStructure.EXTI_Line = LEFT_BUTTON_EXTI_LINE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* External Interrupt Enable */
  Demo_IntExtOnOffCmd(ENABLE);
}

/**
* @brief  lapTimer application.
* @param  None
* @retval None
*/
void App_LapTimer_Start(void)
{
  uint32_t index = 0;
  uint32_t tmp = 0;
  
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  MenuApplication = 2;
  
  /* External Interrupt Disable */
  Demo_IntExtOnOffCmd(DISABLE);
  
  /* Clear the LCD */ 
  LCD_Clear(White);
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(Blue);
  
  /* Set the LCD Text Color */
  LCD_SetTextColor(White);
   
  /* Displays MESSAGE1 on line 1 */
  LCD_DisplayStringLine(LINE(0), (uint8_t *)MESSAGE4);
  
    /* Set the LCD Back Color */
  LCD_SetBackColor(Cyan);
   
  /* Set the LCD Text Color */
  LCD_SetTextColor(Black);
 
  LCD_DrawUniLine(69, 121, 69,20);
  LCD_DrawUniLine(171, 121, 171, 20);
  
  /* Draw Full rectangle */
  LCD_DrawUniLine(70, 121, 120, 71);
  LCD_DrawUniLine(120, 71, 170,121);
  LCD_DrawUniLine(70, 20, 120, 70);
  LCD_DrawUniLine(120, 70, 170,20);
    
  for (index = 0; index < 100 ; index++)
  {
    if ((index % 2) ==0)
    {
      /* Set the LCD Text Color */
      LCD_SetTextColor(Blue);
      LCD_DrawLine(70 + (index/2) , 120 - (index/2)  , 101 - (index + 1) ,Horizontal);
      /* Set the LCD Text Color */
      LCD_SetTextColor(White);
      LCD_DrawLine(170 - (index/2) , 120 - (index/2)  , 101 - (index + 1) ,Horizontal);
    }
  }
  
  /* Set the LCD Text Color */
  LCD_SetTextColor(Blue); 
  /* Set the LCD Back Color */
  LCD_SetBackColor(White);

  /* Display String on the LCD  */
  LCD_DisplayStringLine(LINE(3), " Set Timer:");
  LCD_DisplayStringLine(95,      "  01:00    ");
  LCD_SetFont(&Font12x12);
  LCD_DisplayStringLine(120,    " UP(+)/DOWN(-)");
  LCD_SetFont(&Font16x24);
  /* Initialize Timer to 60 seconds */
  SecondNumb = 60;
  
  /* Configure the external interrupt on "RIGHT" and "LEFT" buttons */
  /* Connect Button EXTI Line to Button GPIO Pin */
  SYSCFG_EXTILineConfig(RIGHT_BUTTON_EXTI_PORT_SOURCE, RIGHT_BUTTON_EXTI_PIN_SOURCE);
  SYSCFG_EXTILineConfig(LEFT_BUTTON_EXTI_PORT_SOURCE, LEFT_BUTTON_EXTI_PIN_SOURCE);
  SYSCFG_EXTILineConfig(UP_BUTTON_EXTI_PORT_SOURCE, UP_BUTTON_EXTI_PIN_SOURCE);
  SYSCFG_EXTILineConfig(DOWN_BUTTON_EXTI_PORT_SOURCE, DOWN_BUTTON_EXTI_PIN_SOURCE);
  /* Configure Button EXTI line */
  EXTI_InitStructure.EXTI_Line = RIGHT_BUTTON_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  EXTI_InitStructure.EXTI_Line = LEFT_BUTTON_EXTI_LINE;
  EXTI_Init(&EXTI_InitStructure);
  
  EXTI_InitStructure.EXTI_Line = UP_BUTTON_EXTI_LINE;
  EXTI_Init(&EXTI_InitStructure);
  
  EXTI_InitStructure.EXTI_Line = DOWN_BUTTON_EXTI_LINE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable and set Button EXTI Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  
  NVIC_Init(&NVIC_InitStructure); 
  
  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5AA5)
  {
    /* Configure the RTC peripheral by selecting the clock source.*/
    RTC_Config();
  } 
  
  /* Configure RTC AlarmA register to generate 8 interrupts per 1 Second */
  RTC_AlarmConfig();
  
  /* Displays MESSAGE1 on line 5 */
  LCD_SetFont(&Font12x12);
  /* Set the LCD Back Color */
  LCD_SetBackColor(Cyan);
  /* Set the LCD Text Color */
  LCD_SetTextColor(Black); 
  LCD_DisplayStringLine(LINE(18), (uint8_t *)MESSAGE5);
  /* Set the LCD Back Color */
  LCD_SetBackColor(Blue);
  LCD_SetTextColor(White);
  LCD_DisplayStringLine(LINE(19), (uint8_t *)MESSAGE6);
  LCD_SetFont(&Font16x24);
  
  /* Infinite loop */
  while (Menu_ReadKey() != SEL)
  {
    if(StartEvent == 8)
    {
      if(RTCAlarmCount != 0 ) 
      {
        tmp = (uint32_t) ((RTCAlarmCount * 100)/ (8 * SecondNumb)); 
        
        /* First */
        Demo_Delay((SecondNumb/6));
        LCD_SetTextColor(Blue);
        LCD_DrawLine(120, 70, 2, Vertical);
        LCD_DrawLine(122, 68, 2, Vertical);
        LCD_DrawLine(122, 72, 2, Vertical);
        if(tmp <= 72)
        {  
          LCD_SetTextColor(White);
          LCD_DrawLine(130, 70, 2, Vertical);
          LCD_DrawLine(132, 68, 2, Vertical);
          LCD_DrawLine(132, 72, 2, Vertical);
        }
        LCD_SetTextColor(Blue);
        LCD_DrawLine(140, 70, 2, Vertical);
        LCD_DrawLine(142, 69, 2, Vertical);
        LCD_DrawLine(142, 72, 2, Vertical);
        
        /* Second */
        Demo_Delay((SecondNumb/6));
        if(tmp <= 90)
        { 
          LCD_SetTextColor(White);
          LCD_DrawLine(120, 70, 2, Vertical);
          LCD_DrawLine(122, 68, 2, Vertical);
          LCD_DrawLine(122, 72, 2, Vertical);
        }
        LCD_SetTextColor(Blue);
        LCD_DrawLine(130, 70, 2, Vertical);
        LCD_DrawLine(132, 68, 2, Vertical);
        LCD_DrawLine(132, 72, 2, Vertical);
        if(tmp <= 52)
        { 
          LCD_SetTextColor(White);
          LCD_DrawLine(140, 70, 2, Vertical);
          LCD_DrawLine(142, 68, 2, Vertical);
          LCD_DrawLine(142, 72, 2, Vertical);
        }
      }
    }
    else if (StartEvent == 9)
    {
      LCD_SetTextColor(Blue);
      LCD_DrawLine(120, 70, 2, Vertical);
      LCD_DrawLine(122, 68, 2, Vertical);
      LCD_DrawLine(122, 72, 2, Vertical);
      
      LCD_DrawLine(130, 70, 2, Vertical);
      LCD_DrawLine(132, 68, 2, Vertical);
      LCD_DrawLine(132, 72, 2, Vertical);
      
      LCD_DrawLine(140, 70, 2, Vertical);
      LCD_DrawLine(142, 68, 2, Vertical);
      LCD_DrawLine(142, 72, 2, Vertical);
      /* Displays MESSAGE1 on line 5 */
      LCD_SetFont(&Font12x12);
      /* Set the LCD Back Color */
      LCD_SetBackColor(Cyan);
      /* Set the LCD Text Color */
      LCD_SetTextColor(Black); 
      LCD_DisplayStringLine(LINE(18), (uint8_t *)MESSAGE11);
      /* Set the LCD Back Color */
      LCD_SetBackColor(Blue);
      LCD_SetTextColor(White);
      LCD_DisplayStringLine(LINE(19), (uint8_t *)MESSAGE12);
      LCD_SetFont(&Font16x24);
    }
    else
    {
      if(tmp <= 90)
      { 
        LCD_SetTextColor(White);
      }
      else
      {
        LCD_SetTextColor(Blue);
      }
      LCD_DrawLine(120, 70, 2, Vertical);
      LCD_DrawLine(122, 68, 2, Vertical);
      LCD_DrawLine(122, 72, 2, Vertical);
      
      if(tmp <= 72)
      { 
        LCD_SetTextColor(White);
      }
      else
      {
        LCD_SetTextColor(Blue);
      }
      LCD_DrawLine(130, 70, 2, Vertical);
      LCD_DrawLine(132, 68, 2, Vertical);
      LCD_DrawLine(132, 72, 2, Vertical);
      
      if(tmp <= 52)
      { 
        LCD_SetTextColor(White);
      }
      else
      {
        LCD_SetTextColor(Blue);
      }
      LCD_DrawLine(140, 70, 2, Vertical);
      LCD_DrawLine(142, 68, 2, Vertical);
      LCD_DrawLine(142, 72, 2, Vertical);
    }
  }
  
  RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
  MenuApplication = 0;
  RTCAlarmCount = 0;
  StartEvent = 0;
  LCD_SetFont(&Font16x24);
  
  /* Checks the availability of the bitmap files */
  Demo_CheckBitmapFilesStatus(); 
  
  LCD_Clear(LCD_COLOR_WHITE);
  Menu_DisplayMenu();

  /* Configure Button EXTI line */
  EXTI_InitStructure.EXTI_Line = RIGHT_BUTTON_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  EXTI_InitStructure.EXTI_Line = LEFT_BUTTON_EXTI_LINE;
  EXTI_Init(&EXTI_InitStructure);
  
  EXTI_InitStructure.EXTI_Line = UP_BUTTON_EXTI_LINE;
  EXTI_Init(&EXTI_InitStructure);
  
  EXTI_InitStructure.EXTI_Line = DOWN_BUTTON_EXTI_LINE;
  EXTI_Init(&EXTI_InitStructure);
  /* External Interrupt Enable */
  Demo_IntExtOnOffCmd(ENABLE);
}

/**
  * @brief  Displays the Light dependent resistor on the LCD.
  * @param  None.
  * @retval None
  */
void App_LightDependentResistor_Start(void)
{
  uint8_t ldrlevel = 11, ldrlevelp = 0, daclevel = 0;
  uint16_t tmp = 0;
  
  /* Clear the LCD */ 
  LCD_Clear(Black);
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(Black);
  
  /* Set the LCD Text Color */
  LCD_SetTextColor(White);
  
  /* Displays Light dependent resistor (LDR) message on line 1 */
  LCD_DisplayStringLine(LINE(0), (uint8_t *)MESSAGE8);
  
  LCD_SetFont(&Font12x12);
  LCD_SetBackColor(Blue);
  LCD_SetTextColor(White);
  LCD_DisplayStringLine(LINE(19), (uint8_t *)MESSAGE10);
  
  LCD_SetFont(&Font16x24);
  
    /* Set the LCD Back Color */
  LCD_SetBackColor(Black);
  
  /* Set the LCD Text Color */
  LCD_SetTextColor(White);
  
  COMP_Config();
  DAC_Config();

  /* Display Image */
  f_mount(0, &fs);
  
  /* Loop until Exit by Joystick button */
  while (Menu_ReadKey() == NOKEY)
  {
    for(daclevel = 0; daclevel < 11; daclevel++)
    {
      /* Set DAC Channel2 DHR register: DAC_OUT2 = (3.3 * 868) / 4095 ~ 0.7 V */
      DAC_SetChannel1Data(DAC_Align_12b_R, (uint16_t)(daclevel * 150));

      Demo_Delay(5);
      /* Check on the Comp output level*/
      if (COMP_GetOutputLevel(COMP_Selection_COMP1) == COMP_OutputLevel_High)
      {
        ldrlevel--;
      }
    }

    switch(ldrlevel)
    {
    case 1:
      Storage_OpenReadFile(140, 200, "STFILES/LDR0.BMP");
      /* Disable LCD Window mode */
      LCD_WindowModeDisable();
      /* Displays MESSAGE1 on line 1 */
      LCD_DisplayStringLine(LINE(7)-10, "       Level 0      ");
      break;
      
    case 2:
      Storage_OpenReadFile(140, 200, "STFILES/LDR1.BMP");
      /* Disable LCD Window mode */
      LCD_WindowModeDisable();
      /* Displays MESSAGE1 on line 1 */
      LCD_DisplayStringLine(LINE(7)-10, "       Level 1      ");
      break;
      
    case 3:
      Storage_OpenReadFile(140, 200, "STFILES/LDR2.BMP");
      /* Disable LCD Window mode */
      LCD_WindowModeDisable();
      /* Displays MESSAGE1 on line 1 */
      LCD_DisplayStringLine(LINE(7)-10, "       Level 2      ");
      break;
      
    case 4:
      Storage_OpenReadFile(140, 200, "STFILES/LDR3.BMP");
      /* Disable LCD Window mode */
      LCD_WindowModeDisable();
      /* Displays MESSAGE1 on line 1 */
      LCD_DisplayStringLine(LINE(7)-10, "       Level 3      ");
      break;
      
    case 5:
      Storage_OpenReadFile(140, 200, "STFILES/LDR4.BMP");
      /* Disable LCD Window mode */
      LCD_WindowModeDisable();
      /* Displays MESSAGE1 on line 1 */
      LCD_DisplayStringLine(LINE(7)-10, "       Level 4      ");
      break;
      
    case 6:
      Storage_OpenReadFile(140, 200, "STFILES/LDR5.BMP");
      /* Disable LCD Window mode */
      LCD_WindowModeDisable();
      /* Displays MESSAGE1 on line 1 */
      LCD_DisplayStringLine(LINE(7)-10, "       Level 5      ");
      break;
      
    case 7:
      Storage_OpenReadFile(140, 200, "STFILES/LDR6.BMP");
      /* Disable LCD Window mode */
      LCD_WindowModeDisable();
      /* Displays MESSAGE1 on line 1 */
      LCD_DisplayStringLine(LINE(7)-10, "       Level 6      ");
      break;
      
    case 8:
      Storage_OpenReadFile(140, 200, "STFILES/LDR7.BMP");
      /* Disable LCD Window mode */
      LCD_WindowModeDisable();
      /* Displays MESSAGE1 on line 1 */
      LCD_DisplayStringLine(LINE(7)-10, "       Level 7      ");
      break;
      
    case 9:
      Storage_OpenReadFile(140, 200, "STFILES/LDR8.BMP");
      /* Disable LCD Window mode */
      LCD_WindowModeDisable();
      /* Displays MESSAGE1 on line 1 */
      LCD_DisplayStringLine(LINE(7)-10, "       Level 8      ");
      break;
      
    case 10:
      Storage_OpenReadFile(140, 200, "STFILES/LDR9.BMP");
      /* Disable LCD Window mode */
      LCD_WindowModeDisable();
      /* Displays MESSAGE1 on line 1 */
      LCD_DisplayStringLine(LINE(7)-10, "       Level 9      ");
      break;
      
    case 11:
      Storage_OpenReadFile(140, 200, "STFILES/LDR10.BMP");
      /* Disable LCD Window mode */
      LCD_WindowModeDisable();
      /* Displays MESSAGE1 on line 1 */
      LCD_DisplayStringLine(LINE(7)-10, "       Level 10     ");
      break;
      
    default :
      Storage_OpenReadFile(140, 200, "STFILES/LDR0.BMP");
      /* Disable LCD Window mode */
      LCD_WindowModeDisable();
      /* Displays MESSAGE1 on line 1 */
      LCD_DisplayStringLine(LINE(7) -10 , "       Level 0      ");
      ldrlevel = 1;
      break;
    }
    
    if(ldrlevelp != ldrlevel)
    {
      /* Set the LCD Text Color */
      LCD_SetTextColor(White);
      /* Displays a rectangle on the LCD */
      LCD_DrawRect(189, 311, 22, 302 );
      
      /* Set the LCD Back Color */
      LCD_SetBackColor(Black);
      LCD_SetTextColor(Black);
      LCD_DrawFullRect(190, 310,  300, 20);
      
      LCD_SetTextColor(Yellow);
      /* Set the LCD Back Color */
      LCD_SetBackColor(Yellow);
      tmp = 30 * (ldrlevel-1);
      if (tmp ==0) tmp = 5;
      LCD_DrawFullRect(190, 310, tmp , 20);
    }
    ldrlevelp = ldrlevel;
    ldrlevel = 11;
    /* Set the LCD Back Color */
    LCD_SetBackColor(Black);
  }
  
  /* Set the LCD Text size */
  LCD_SetFont(&Font16x24);
  
  /* Checks the availability of the bitmap files */
  Demo_CheckBitmapFilesStatus(); 
  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);
  /* Display Window */
  Menu_DisplayMenu();
}

/**
  * @brief COMP Configuration.
  * @param None
  * @retval None
  */
static void COMP_Config(void)
{
  COMP_InitTypeDef COMP_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  /**************************** COMP2 Config **********************************/
  /* GPIOB Peripheral clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  /* Configure PA1 in analog mode: PA1 is connected to COMP1 non inverting input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* COMP1 Init: COMP1 is enabled as soon as inverting input is selected */
  /* use DAC1 output as a reference voltage: DAC1 output is connected to COMP1
  inverting input */
  COMP_InitStructure.COMP_InvertingInput = COMP_InvertingInput_DAC1;
  COMP_InitStructure.COMP_Output = COMP_Output_None;
  COMP_InitStructure.COMP_OutputPol = COMP_OutputPol_NonInverted;
  COMP_InitStructure.COMP_Hysteresis = COMP_Hysteresis_High;
  COMP_InitStructure.COMP_Mode = COMP_Mode_UltraLowPower;
  COMP_Init(COMP_Selection_COMP1, &COMP_InitStructure);
  
  /* Enable the COMP peripheral */
  COMP_Cmd(COMP_Selection_COMP1, ENABLE);
}

/**
  * @brief  Configures the DAC channel 2 with output buffer enabled.
  * @param  None
  * @retval None
  */
static void DAC_Config(void)
{
  DAC_InitTypeDef  DAC_InitStructure;
  
  /* DAC clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

  DAC_DeInit();
  /* Fill DAC InitStructure */ 
  /* DAC Channel1: 12bit right */
  /* DAC Channel1 Init */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);
  /* Enable DAC Channel1 */
  DAC_Cmd(DAC_Channel_1, ENABLE);
}

/**
  * @brief  Displays the current Time on the LCD.
  * @param  None
  * @retval None
  */
static void RTC_Time_ClearDisplay(void)
{
  uint8_t i = 0;
  
   /* Check if the LCD is HX8347D Controller */
    if(LCDType == LCD_HX8347D)
    {
      for (i=0;i<12;i++)
      {
        if ((i==2) || (i==5))
        {
          /* Display char on the LCD */
          LCD_DisplayChar(37, (40 +  (20 *i)), ':');
        }
        else if ((i==8) )
        {
          /* Display char on the LCD */
          LCD_DisplayChar(37, (40 +  (20 *i)), '.');
        }
        else
        {
          /* Display char on the LCD */
          LCD_DisplayChar(37, (40 +  (20 *i)), '0');
        }
      }
    }
    else 
    {
      for (i=0;i<12;i++)
      {
        if ((i==2) || (i==5))
        {
          /* Display char on the LCD */
          LCD_DisplayChar(37, (280 - (20 *i)), ':');
        }
        else if ((i==8) )
        {
          /* Display char on the LCD */
          LCD_DisplayChar(37, (280 - (20 *i)), '.');
        }
        else
        {
          /* Display char on the LCD */
          LCD_DisplayChar(37, (280 - (20 *i)), '0');
        }
      }
    }
}

/**
  * @brief  RTC Tamper Configuration..
  * @param  None
  * @retval None
  */
static void RTC_TamperConfig(void)
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

  /* Enable RTC_TAMP_STAMP_IRQn */
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
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
static void RTC_Config(void)
{
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  RCC_BackupResetCmd(ENABLE);
  RCC_BackupResetCmd(DISABLE);
  
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);

  /* Wait till LSE is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }
  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Configure the RTC data register and RTC prescaler */
  RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
  RTC_InitStructure.RTC_SynchPrediv  = 0xFF;
  RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;
  RTC_Init(&RTC_InitStructure);
  
  /* Set the time to 00h 00mn 00s AM */
  RTC_TimeStruct.RTC_H12     = RTC_H12_AM;
  RTC_TimeStruct.RTC_Hours   = 0x00;
  RTC_TimeStruct.RTC_Minutes = 0x00;
  RTC_TimeStruct.RTC_Seconds = 0x00;  
  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);

  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();
}

/**
  * @brief  Configures the RTC Alarm.
  * @param  None
  * @retval None
  */
static void RTC_AlarmConfig(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  RTC_AlarmTypeDef RTC_AlarmStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* EXTI configuration */
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
 
  /* Set the alarmA Masks */
  RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_All;
  RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);
  
  /* Set AlarmA subseconds and enable SubSec Alarm : generate 8 interripts per Second */
  RTC_AlarmSubSecondConfig(RTC_Alarm_A, 0xFF, RTC_AlarmSubSecondMask_SS14_5);
  
  /* Enable AlarmA interrupt */
  RTC_ITConfig(RTC_IT_ALRA, ENABLE);
  
  /* Disable the AlarmA */
  RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
}


/**
  * @brief  Initialize the time displays on the LCD.
  * @param  None.
  * @retval None
  */
void RTC_Time_InitDisplay(void)
{
  uint8_t i = 0;
  
  /* Check if the LCD is HX8347D Controller */
  if(LCDType == LCD_HX8347D)
  {
    for (i = 0;i < 12; i++)
    {
      if ((i == 2) || (i == 5))
      {
        /* Display char on the LCD */
        LCD_DisplayChar(37, (40 + (20 *i)), ':');
      }
      else if ((i==8) )
      {
        /* Display char on the LCD */
        LCD_DisplayChar(37, (40 + (20 *i)), '.');
      }
      else
      {
        /* Display char on the LCD */
        LCD_DisplayChar(37, (40 + (20 *i)), '0');
      }
    }
    
  }
  else
  {
    for (i = 0;i < 12; i++)
    {
      if ((i == 2) || (i == 5))
      {
        /* Display char on the LCD */
        LCD_DisplayChar(37, (280 - (20 *i)), ':');
      }
      else if ((i==8) )
      {
        /* Display char on the LCD */
        LCD_DisplayChar(37, (280 - (20 *i)), '.');
      }
      else
      {
        /* Display char on the LCD */
        LCD_DisplayChar(37, (280 - (20 *i)), '0');
      }
    }
  }
}
/**
  * @brief  Displays the current Time on the LCD.
  * @param  Line:  the Line where to display the Current time .
  *         This parameter can be one of the following values:
  *            @arg Linex: where x can be 0..9
  * @param  Color_x: specifies the Background Color.
  * @param  Table: the Current time and sub second.
  * @retval None
  */
void RTC_Time_Display(uint8_t Line, __IO uint16_t Color_x, Table_TypeDef Table)
{   
  uint8_t i = 0;

  /* Initialize table */
  LCD_SetTextColor(Color_x);
  
  for (i=0; i<12; i++)
  {
    /* Check if the LCD is HX8347D Controller */
    if(LCDType == LCD_HX8347D)
    {
      /* Display char on the LCD */
      LCD_DisplayChar(Line, (40 + (20 *i)), Table.Tab[i]);
    }
    else
    {
      /* Display char on the LCD */
      LCD_DisplayChar(Line, (280 - (20 *i)), Table.Tab[i]);
    }
  }  
}

/**
  * @brief  load the current time and sub second on a table forme.
  * @param  Second_fraction: the sub second fraction .
  * @param  RTC_TimeStructure: pointer to a RTC_TimeTypeDef structure that 
  *         contains the current time values. 
  * @retval table: return current time and sub second in a table form
  */
Table_TypeDef RTC_Get_Time(uint32_t Second_fraction , RTC_TimeTypeDef* RTC_TimeStructure)
{
  Table_TypeDef table2;
  uint32_t currenttime_sec = 0;
  
  /* Get the Current time in second */ 
  currenttime_sec = ((RTC_TimeStructure->RTC_Hours * 3600) +(RTC_TimeStructure->RTC_Minutes * 60) +
                     RTC_TimeStructure->RTC_Seconds) - CurrentTimeSec; 
  
  /* Fill the table2 fields with the current Time*/
  table2.Tab[0]  = ((uint8_t)((currenttime_sec/3600)/10) + 0x30);
  table2.Tab[1]  = ((uint8_t)((currenttime_sec/3600) %10)+ 0x30);
  table2.Tab[2]  = 0x3A;
  table2.Tab[3]  = (((uint8_t)(((currenttime_sec % 3600)/60) /10)) + 0x30);
  table2.Tab[4]  = (((uint8_t)(((currenttime_sec % 3600)/60) %10)) + 0x30);
  table2.Tab[5]  = 0x3A;
  table2.Tab[6]   = (((uint8_t)((currenttime_sec% 60) /10))+ 0x30);
  table2.Tab[7]   = (((uint8_t)((currenttime_sec% 60) %10)) + 0x30);
  table2.Tab[8]   = 0x2E;
  table2.Tab[9]   = (uint8_t)((Second_fraction / 100) + 0x30);
  table2.Tab[10]  = (uint8_t)(((Second_fraction % 100 ) / 10) +0x30);
  table2.Tab[11]  =  (uint8_t)((Second_fraction % 10) +0x30);

  /* return table2 */
  return table2;
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
