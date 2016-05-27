/**
  ******************************************************************************
  * @file    waverecord.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file includes the wave record driver for the STM320518-EVAL demo.
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

/** @defgroup WAVERECORD
  * @brief    This file includes the wave record driver for the STM320518-EVAL
  *           demo.
  * @{
  */

/** @defgroup WAVERECORD_Private_Types
  * @{
  */

/**
  * @}
  */

/** @defgroup WAVERECORD_Private_Defines
  * @{
  */
#define MESSAGE1   "    Wave Recording  "
#define MESSAGE2   "    SEL      |     DOWN   " 
#define MESSAGE3   "  START REC  |     EXIT   "
#define MESSAGE4   "           DOWN           " 
#define MESSAGE5   "           EXIT           "

#define MESSAGE1P  "      SEL    |     DOWN   " 
#define MESSAGE2P  "     Play    |    Return  "

#define MESSAGE3P  " SEL | DOWN  |LEFT | RIGHT"
#define MESSAGE4P  "PAUSE| STOP  | BWD | FWD  "



/**
  * @}
  */

/** @defgroup WAVERECORD_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup WAVERECORD_Private_Variables
  * @{
  */
extern uint8_t Buffer1[_MAX_SS];
extern uint8_t Buffer2[_MAX_SS];
extern WAVE_FormatTypeDef WAVE_Format;
extern FATFS fs;
extern DIR dir;
extern FIL F;
extern UINT BytesWritten;
extern UINT BytesRead;
extern __IO uint32_t WaveDataLength; 
extern uint16_t TIM6ARRValue;
extern __IO uint32_t WaveCounter;

__IO uint32_t SelectedMaxRecordDataLength = 0x00;
extern __IO uint8_t  VoiceRecStop;                /* Stop Voice Recorder  Flag */
extern __IO uint8_t DOWNBoutonPressed;             /* Stop wave record enable */
extern __IO uint32_t LCDType;

/**
  * @}
  */

/** @defgroup WAVERECORD_Private_FunctionPrototypes
  * @{
  */
static void Rec_Inform_Display(void);
static uint8_t RecWave_LCDDisplayUpdate(void);
/**
  * @}
  */

/** @defgroup WAVERECORD_Private_Functions
  * @{
  */

/**
  * @brief  record wave files and store it on the SDcard.
  * @param  None
  * @retval None
  */
void Menu_WaveRecorderFunc(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  uint8_t pressedkey = 0;
  
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* External Interrupt Disable */
  Demo_IntExtOnOffCmd(DISABLE);

  while (Menu_ReadKey() != NOKEY)
  {}
  
  /* Set the text and the background color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_BLACK);
  LCD_Clear(LCD_COLOR_WHITE);
 
  /****************************************************************************/
  /* Display Image */
  LCD_SetDisplayWindow(130, 310, 64, 64);
  Storage_OpenReadFile(130, 310, "STFILES/RECC2.BMP");  
  LCD_WindowModeDisable();
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(Line0, (uint8_t *)MESSAGE1);
   
  /* Displays MESSAGE1 on line 5 */
  LCD_SetFont(&Font12x12);
  /* Set the LCD Back Color */
  LCD_SetBackColor(LCD_COLOR_CYAN);
  /* Set the LCD Text Color */
  LCD_SetTextColor(LCD_COLOR_BLACK); 
  LCD_DisplayStringLine(LINE(18), (uint8_t *)MESSAGE2);
  /* Set the LCD Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LINE(19), (uint8_t *)MESSAGE3);
  
  /* Displays MESSAGE1 on line 5 */
  LCD_SetFont(&Font16x24);
  /****************************************************************************/
  
  pressedkey = Menu_ReadKey();
  while ((pressedkey != SEL) && (pressedkey != DOWN))
  { 
    pressedkey = Menu_ReadKey();
    
    if (pressedkey == SEL)
    {
      /* Disable TIM15 to stop LED's toggling before start recording the wave */
      TIM_Cmd(TIM15, DISABLE);
      
      /* Display Image */
      LCD_SetDisplayWindow(130, 310, 64, 64);
      Storage_OpenReadFile(130, 310, "STFILES/RECC.BMP");
      LCD_WindowModeDisable();
      /* Displays MESSAGE1 on line 5 */
      LCD_SetFont(&Font16x24);
      /* Set the LCD Back Color */
      LCD_SetBackColor(LCD_COLOR_BLUE);
      LCD_SetTextColor(LCD_COLOR_WHITE);
      LCD_DisplayStringLine(Line0, (uint8_t *)MESSAGE1);
      
      /* Displays MESSAGE1 on line 5 */
      LCD_SetFont(&Font12x12);
      /* Set the LCD Back Color */
      LCD_SetBackColor(LCD_COLOR_CYAN);
      /* Set the LCD Text Color */
      LCD_SetTextColor(LCD_COLOR_BLACK); 
      LCD_DisplayStringLine(LINE(18), (uint8_t *)MESSAGE4);
      /* Set the LCD Back Color */
      LCD_SetBackColor(LCD_COLOR_BLUE);
      LCD_SetTextColor(LCD_COLOR_WHITE);
      LCD_DisplayStringLine(LINE(19), (uint8_t *)MESSAGE5);
      
      LCD_SetBackColor(LCD_COLOR_WHITE);
      LCD_SetTextColor(LCD_COLOR_BLACK);
      Rec_Inform_Display();
      /* Displays MESSAGE1 on line 5 */
      LCD_SetFont(&Font16x24);
      LCD_SetTextColor(LCD_COLOR_BLUE2);
      LCD_SetBackColor(LCD_COLOR_WHITE);
      LCD_ClearLine(LCD_LINE_7);
      LCD_DisplayStringLine(LCD_LINE_7, "   Record On Going  ");
      
      LCD_SetTextColor(LCD_COLOR_MAGENTA);
      /* Set the Back Color */
      LCD_SetBackColor(LCD_COLOR_BLUE);
      LCD_DrawRect(LCD_LINE_8, 310, 16, 300);
      
      LCD_SetTextColor(LCD_COLOR_BLUE2);
      LCD_SetBackColor(LCD_COLOR_WHITE);
      
      f_mount(0, &fs);
      /* Remove the old recorded file */
      f_unlink ("/Rec/Rec_Wave.Wav");
      /* Remove the old folder */
      f_unlink ("/Rec");
      /* Create new folder named RecWave */
      f_mkdir ("/Rec");
      /* Create a new file system */
      f_open (&F, "/Rec/Rec_Wave.Wav", FA_CREATE_ALWAYS | FA_WRITE);
      
      /* Initializes the EXTI_InitStructure */
      EXTI_StructInit(&EXTI_InitStructure);
      /* Clear the the EXTI line 10 interrupt pending bit */
      EXTI_ClearITPendingBit(DOWN_BUTTON_EXTI_LINE);
      EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
      EXTI_InitStructure.EXTI_Line = DOWN_BUTTON_EXTI_LINE;
      EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
      EXTI_InitStructure.EXTI_LineCmd = ENABLE;
      EXTI_Init(&EXTI_InitStructure);
      
      /* Enable the EXTI4_15 Interrupt */
      NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
      
      /* Init Wave recorder */
      WaveRec_Init();

      /* Initialize the header file in Buffer1*/
      WaveRec_HeaderInit();
      f_write (&F, Buffer1, _MAX_SS, &BytesWritten);

      /* Start Wave recorder */
      WaveRec_Start();

      /* Record Voice until 30s/KEY*/
      WaveRec_Recording();
    }
  }
  LCD_Clear(LCD_COLOR_WHITE);
  Menu_DisplayMenu();
  
  /* Enable TIM15 to stop LED's toggling before start recording the wave */
  TIM_Cmd(TIM15, ENABLE);
  
  /* External Interrupt Enable */
  Demo_IntExtOnOffCmd(ENABLE);
}

/**
  * @brief  Play the recorded wave files stored on the SDcard.
  * @param  None
  * @retval None
  */
void Menu_RecordedWavePlayFunc(void)
{
  uint8_t pressedkey = 0;

  LCD_Clear(LCD_COLOR_BLACK);

  /* External Interrupt Disable */
  Demo_IntExtOnOffCmd(DISABLE);

  /* TIM6 and DAC clocks enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 | RCC_APB1Periph_DAC, ENABLE);
  
  /* Display Image */
  f_mount(0, &fs);
  LCD_SetDisplayWindow(130, 310, 64, 64);
  Storage_OpenReadFile(130, 310, "STFILES/Music2.BMP");  
  LCD_WindowModeDisable();
  
  while (Menu_ReadKey() != NOKEY)
  {}
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(Black);
  LCD_SetTextColor(White);
  /* Display the Titles */
  LCD_DisplayStringLine(LCD_LINE_0, " *** Wave Player ***");
  
  /* Displays MESSAGE1 on line 5 */
  LCD_SetFont(&Font12x12);
  /* Set the LCD Back Color */
  LCD_SetBackColor(LCD_COLOR_CYAN);
  /* Set the LCD Text Color */
  LCD_SetTextColor(LCD_COLOR_BLACK); 
  LCD_DisplayStringLine(LINE(18), (uint8_t *)MESSAGE1P);
  /* Set the LCD Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LINE(19), (uint8_t *)MESSAGE2P);
  
  LCD_SetBackColor(LCD_COLOR_BLACK);
  LCD_SetTextColor(LCD_COLOR_WHITE); 
  Rec_Inform_Display();
  
  LCD_SetFont(&Font16x24);
  pressedkey = Menu_ReadKey();
  while ((pressedkey != SEL) && (pressedkey != DOWN))
  {
    pressedkey = Menu_ReadKey();
    
    /* Play */
    if (pressedkey == SEL)
    {
      /* Disable TIM15 to stop LED's toggling before start recording the wave */
      TIM_Cmd(TIM15, DISABLE);
      
      f_mount(0, &fs);
      /* Display Image */
      LCD_Clear(LCD_COLOR_BLACK);
      LCD_SetDisplayWindow(130, 310, 64, 64);
      Storage_OpenReadFile(130, 310, "STFILES/Play2.BMP"); 
      LCD_WindowModeDisable();
      LCD_SetFont(&Font16x24);
      
      /* Set the LCD Back Color */
      LCD_SetBackColor(Black);
      LCD_SetTextColor(Magenta);
      /* Display the Titles */
      LCD_DisplayStringLine(LCD_LINE_0, "     Now Playing    ");
      /* Displays MESSAGE1 on line 5 */
      LCD_SetFont(&Font12x12);
      /* Set the LCD Back Color */
      LCD_SetBackColor(LCD_COLOR_CYAN);
      /* Set the LCD Text Color */
      LCD_SetTextColor(LCD_COLOR_BLACK); 
      LCD_DisplayStringLine(LINE(18), (uint8_t *)MESSAGE3P);
      /* Set the LCD Back Color */
      LCD_SetBackColor(LCD_COLOR_BLUE);
      LCD_SetTextColor(LCD_COLOR_WHITE);
      LCD_DisplayStringLine(LINE(19), (uint8_t *)MESSAGE4P);
      LCD_SetFont(&Font16x24);
      pressedkey = PlayWaveRecorded_Start();
    }
  } 

  LCD_Clear(LCD_COLOR_WHITE);
  Menu_DisplayMenu();

  /* Enable TIM15 to stop LED's toggling before start recording the wave */
  TIM_Cmd(TIM15, ENABLE);

  /* External Interrupt Enable */
  Demo_IntExtOnOffCmd(ENABLE);
}

/**
  * @brief  Start playing the recorded wave
  * @param  None
  * @retval None
  */
uint8_t PlayWaveRecorded_Start(void)
{
  DAC_InitTypeDef            DAC_InitStructure;
  DMA_InitTypeDef            DMA_InitStructure;
  
  uint8_t tmp = 0,KeyState = NOKEY;

  WavePlayer_Init();

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* TIM6 Configuration */
  TIM_DeInit(TIM6);

  /* DMA1 channel2 configuration */
  DMA_DeInit(DMA1_Channel3);
  DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR8R1_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) & Buffer1;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = 512;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);

  /* Enable DMA1 Channel3 */
  DMA_Cmd(DMA1_Channel3, ENABLE);

  /* DAC deinitialize */
  DAC_DeInit();
  DAC_StructInit(&DAC_InitStructure);

  /* Fill DAC InitStructure */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

  /* DAC Channel1: 8bit right---------------------------------------------------*/
  /* DAC Channel1 Init */
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);
  /* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is
     automatically connected to the DAC converter. */
  DAC_Cmd(DAC_Channel_1, ENABLE);
  /* Enable DMA for DAC Channel1 */
  DAC_DMACmd(DAC_Channel_1, ENABLE);

  /* Open an existing directory */
  if (f_opendir(&dir, "/Rec") != FR_OK)
  {       
    /* Set the LCD Back Color */
    LCD_SetBackColor(Black);
    LCD_SetTextColor(Red);
    
    /* Display the Titles */
    LCD_DisplayStringLine(LCD_LINE_6, "  No wave file  ");
    LCD_DisplayStringLine(LCD_LINE_7, "   Recorded     ");   
    
    while (Menu_ReadKey() != DOWN)
    {}
    return DOWN;
  };
  
  /* Open read file */
  if( f_open (&F, "/Rec/Rec_Wave.Wav", FA_READ)!= FR_OK)
  {     
    /* Set the LCD Back Color */
    LCD_SetBackColor(Black);
    LCD_SetTextColor(Red);
    
    /* Display the Titles */
    LCD_DisplayStringLine(LCD_LINE_6, "  No wave file  ");
    LCD_DisplayStringLine(LCD_LINE_7, "   Recorded     ");   
    
    while (Menu_ReadKey() != DOWN)
    {}
    return DOWN;
  }
    
  /* Read data from a file */
  f_read (&F, Buffer1, _MAX_SS, &BytesRead);

  /* TIM6 TRGO selection */
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
  TIM6ARRValue = (uint32_t)(SystemCoreClock / SAMPLE_RATE_8000) -1;
  TIM_SetAutoreload(TIM6, TIM6ARRValue);

  /* Read the number of sample data ------------------------------------------*/
  /* Set WaveDataLenght to the Speech wave length */
  WaveDataLength = ReadUnit(Buffer1, 40, 4, LittleEndian);
  WAVE_Format.DataSize = WaveDataLength;
  /* Start TIM6 */
  TIM_Cmd(TIM6, ENABLE);

  LCD_SetTextColor(LCD_COLOR_MAGENTA);
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_DrawRect(LCD_LINE_7, 310, 16, 300);

  while (WaveDataLength)
  {
    LCD_CtrlLinesWrite(LCD_NCS_GPIO_PORT, LCD_NCS_PIN, Bit_SET);
    f_read (&F, Buffer2, _MAX_SS, &BytesRead);
    
    if (WaveDataLength) WaveDataLength -= 512;
    if (WaveDataLength < 512) WaveDataLength = 0;

    while (DMA_GetFlagStatus(DMA1_FLAG_TC3) == RESET)
    {
      tmp = (uint8_t) ((uint32_t)((WAVE_Format.DataSize - WaveDataLength) * 100) / WAVE_Format.DataSize);
      LCD_SetTextColor(LCD_COLOR_MAGENTA);
      
      LCD_DrawLine(LCD_LINE_7, 310 - ((tmp) * 3), 16, LCD_DIR_VERTICAL);
    }

    DMA1->IFCR = DMA1_FLAG_TC3;
    DMA1_Channel3->CCR = 0x0;

    KeyState = RecWave_LCDDisplayUpdate();
    if (KeyState == DOWN)
    {
      return DOWN;
    }
    
    DMA1_Channel3->CNDTR = 0x200;
    DMA1_Channel3->CPAR = DAC_DHR8R1_Address;
    DMA1_Channel3->CMAR = (uint32_t) & Buffer2;
    DMA1_Channel3->CCR = 0x2091;
    LCD_CtrlLinesWrite(LCD_NCS_GPIO_PORT, LCD_NCS_PIN, Bit_SET);
    f_read (&F, Buffer1, _MAX_SS, &BytesRead);
    if (WaveDataLength) WaveDataLength -= 512;
    if (WaveDataLength < 512) WaveDataLength = 0;

    while (DMA_GetFlagStatus(DMA1_FLAG_TC3) == RESET)
    {
      tmp = (uint8_t) ((uint32_t)((WAVE_Format.DataSize - WaveDataLength) * 100) / WAVE_Format.DataSize);
      LCD_SetTextColor(LCD_COLOR_MAGENTA);
      LCD_DrawLine(LCD_LINE_7, 310 - ((tmp) * 3), 16, LCD_DIR_VERTICAL);
    }

    DMA1->IFCR = DMA1_FLAG_TC3;
    DMA1_Channel3->CCR = 0x0;

    KeyState = RecWave_LCDDisplayUpdate();
    if (KeyState == DOWN)
    {
      return DOWN;
    }

    DMA1_Channel3->CNDTR = 0x200;
    DMA1_Channel3->CPAR = DAC_DHR8R1_Address;
    DMA1_Channel3->CMAR = (uint32_t) & Buffer1;
    DMA1_Channel3->CCR = 0x2091;
  }
  DMA1_Channel3->CCR = 0x0;

  /* Disable TIM6 */
  TIM_Cmd(TIM6, DISABLE);

  WaveDataLength = 0;

  return SEL;
}

/**
  * @brief  Initialize wave recording
  * @param  None
  * @retval None
  */
void WaveRec_Init(void)
{
  ADC_InitTypeDef ADC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Voice recorder stopped flag is reset*/
  VoiceRecStop = RESET;
  
  /* Enable GPIO ADC channel10 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  
  /* ADC1 clocks enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* TIM3 and TIM14 clocks enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM14, ENABLE); 
  
  /* Configure PC.00 (ADC Channel10) as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Enable the TIM3 Interrupt for capture compare event */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the TIM14 Interrupt to trigger the ADC */
  NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable The HSI (14Mhz) */
  RCC_HSI14Cmd(ENABLE);
  
  /* Configures the HSI 14MHz as ADC clock source */
  RCC_ADCCLKConfig(RCC_ADCCLK_HSI14);
  
  ADC_DeInit(ADC1);

  ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* TIM3 Peripheral Configuration */
  TIM_DeInit(TIM3);

  /* Set TIM3 period to 8000 Hz using system clock 48Mhz*/
  TIM_SetAutoreload(TIM3, 6000);

  /* TIM3 TRGO selection */
  TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);

  /* Enable TIM3 interrupts */
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

  /* TIM4 Peripheral deinitialize */
  TIM_DeInit(TIM14);

  /* Enable TIM14 interrupts */
  TIM_ITConfig(TIM14, TIM_IT_CC1, ENABLE);
}

/**
  * @brief  Start wave recording
  * @param  None
  * @retval None
  */
void WaveRec_Start(void)
{
  /* Initialize the max wave data lenght to record */
  SelectedMaxRecordDataLength =  ReadUnit(Buffer1, 40, 4, LittleEndian);

  /* Reset WaveCounter value */
  WaveCounter = 0;

  /* Reset the Down bouton variable status */
  DOWNBoutonPressed = 0x00;

  /* ADC1 regular channel10 configuration */
  ADC_ChannelConfig(ADC1, ADC_Channel_10, ADC_SampleTime_239_5Cycles);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN) == RESET)
  {}

  /* Disable TIM15 to stop LED's toggling before start recording the wave */
  TIM_Cmd(TIM15, DISABLE);

  /* Disable the RTC Clock */
  RCC_RTCCLKCmd(DISABLE);

  /* Disable the TIM14 */
  TIM_Cmd(TIM14, DISABLE);

  /* Start TIM3 */
  TIM_Cmd(TIM3, ENABLE);
  
  /* TIM6 and DAC clocks Disable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 | RCC_APB1Periph_DAC, DISABLE);
  
  /* ADCperipheral[PerIdx] regular Software Start Conv */ 
  ADC_StartOfConversion(ADC1);
}

/**
  * @brief  wait until Voice recording time finish or  a stop record action occurs
  * @param  None
  * @retval None
  */
void WaveRec_Recording(void)
{
  /* Wait until 30s or Button Key pressed */
  while (((WaveCounter + 1) < SelectedMaxRecordDataLength) && (VoiceRecStop == RESET))
  {}

  /* Disable TIM3 interrupts */
  TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);

  /* Disable TIM14 interrupts */
  TIM_ITConfig(TIM14, TIM_IT_CC1, DISABLE);

  /* Disable TIM3 and TIM14 */
  TIM_Cmd(TIM3, DISABLE);
  TIM_Cmd(TIM14, DISABLE);

  /* Stop conversion */
  ADC_Cmd(ADC1, DISABLE);

  /* Close recorded file*/
  f_close (&F);

  if (VoiceRecStop != RESET)
  {
    /* Open the recorded wave to adjust the header size */
    f_open (&F, "Rec/Rec_Wave.Wav", FA_WRITE);

    /* Initialize the header file in Buffer1*/
    WaveRec_HeaderInit();

    /* Update the wave file size */
    Buffer1[4] = (uint8_t)(WaveCounter + 512) ;
    Buffer1[5] = (uint8_t)((WaveCounter + 512) >> 8);
    Buffer1[6] = (uint8_t)((WaveCounter + 512) >> 16);
    Buffer1[7] = (uint8_t)((WaveCounter + 512) >> 24);

    /* Update the wave file data size */
    Buffer1[40] = (uint8_t)(WaveCounter);
    Buffer1[41] = (uint8_t)((WaveCounter) >> 8);
    Buffer1[42] = (uint8_t)((WaveCounter) >> 16);
    Buffer1[43] = (uint8_t)((WaveCounter) >> 24);

    f_write (&F, Buffer1, _MAX_SS, &BytesWritten);

    /* Close the recorded file*/
    f_close (&F);

    /* Reset the VoiceRecStop flag */
    VoiceRecStop = RESET;
  }

  /* Enable TIM15 to start LED's toggling after record wave */
  TIM_Cmd(TIM15, ENABLE);

  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);
}

/**
  * @brief  Initialize the wave header file
  * @param  None
  * @retval None
  */
void WaveRec_HeaderInit(void)
{
  uint16_t count = 0;

  /* write chunkID, must be 'RIFF'  ----------------------------------------------*/
  Buffer1[0] = 'R';
  Buffer1[1] = 'I';
  Buffer1[2] = 'F';
  Buffer1[3] = 'F';

  /* Write the file length ----------------------------------------------------*/
  /* The sampling time 8000 Hz
   To recorde 30s we need 8000 x 30 x 1 (8-Bit data) = 240000 =0x3A980*/
  Buffer1[4] = 0x80;
  Buffer1[5] = 0xA9;
  Buffer1[6] = 0x03;
  Buffer1[7] = 0x00;

  /* Write the file format, must be 'WAVE' ------------------------------------*/
  Buffer1[8]  = 'W';
  Buffer1[9]  = 'A';
  Buffer1[10] = 'V';
  Buffer1[11] = 'E';

  /* Write the format chunk, must be'fmt ' --------------------------------------*/
  Buffer1[12]  = 'f';
  Buffer1[13]  = 'm';
  Buffer1[14]  = 't';
  Buffer1[15]  = ' ';

  /* Write the length of the 'fmt' data, must be 0x10 -------------------------*/
  Buffer1[16]  = 0x10;
  Buffer1[17]  = 0x00;
  Buffer1[18]  = 0x00;
  Buffer1[19]  = 0x00;

  /* Write the audio format, must be 0x01 (PCM) -------------------------------*/
  Buffer1[20]  = 0x01;
  Buffer1[21]  = 0x00;

  /* Write the number of channels, must be 0x01 (Mono) ------------------------*/
  Buffer1[22]  = 0x01;
  Buffer1[23]  = 0x00;

  /* Write the Sample Rate 8000Hz---------------------------------------------*/
  /* Write Little Endian 8000 = 0x1F40 */
  Buffer1[24]  = 0x40;
  Buffer1[25]  = 0x1F;
  Buffer1[26]  = 0x00;
  Buffer1[27]  = 0x00;

  /* Write the Byte Rate ------------------------------------------------------*/
  Buffer1[28]  = 0x40; 
  Buffer1[29]  = 0x1F; 
  Buffer1[30]  = 0x00;
  Buffer1[31]  = 0x00;

  /* Write the block alignment ------------------------------------------------*/
  Buffer1[32]  = 0x01;
  Buffer1[33]  = 0x00;

  /* Write the number of bits per sample --------------------------------------*/
  Buffer1[34]  = 0x08; 
  Buffer1[35]  = 0x00;

  /* Write the Data chunk, must be 'data' ---------------------------------------*/
  Buffer1[36]  = 'd';
  Buffer1[37]  = 'a';
  Buffer1[38]  = 't';
  Buffer1[39]  = 'a';

  /* Write the number of sample data ------------------------------------------*/
  Buffer1[40]  = 0x80;
  Buffer1[41]  = 0xA9;
  Buffer1[42]  = 0x03;
  Buffer1[43]  = 0x00;

  /* Fill the missing bytes in Buffer1 with 0x80 */
  for (count = 44; count < 512 ; count ++)
  {
    Buffer1[count] = 0x80;
  }
}

/**
  * @brief  Displays the recorded wave info on the LCD.
  * @param  None
  * @retval None
  */
static void Rec_Inform_Display(void)
{   
  /* Displays MESSAGE1 on line 5 */
  LCD_SetFont(&Font12x12);
  
  /* Check if the LCD is HX8347D Controller */
  if(LCDType == LCD_HX8347D)
  {
    /* Display 16bits Stereo */
    LCD_DisplayChar(60 ,320 - 170, 'R'); 
    LCD_DisplayChar(60 ,320 -  158, 'E'); 
    LCD_DisplayChar(60 ,320 -  146, 'C'); 
    LCD_DisplayChar(60 ,320 -  138, '_'); 
    LCD_DisplayChar(60 ,320 -  130, 'W'); 
    LCD_DisplayChar(60 ,320 -  118, 'A'); 
    LCD_DisplayChar(60 ,320 -  106, 'V'); 
    LCD_DisplayChar(60 ,320 -  94, 'E'); 
  
    /* Display 8 bits MONO */
    LCD_DisplayChar(84 ,320 -  170, '8'); 
    LCD_DisplayChar(84 ,320 -  158, 'B'); 
    LCD_DisplayChar(84 ,320 -  146, 'i'); 
    LCD_DisplayChar(84 ,320 -  134, 't'); 
    LCD_DisplayChar(84 ,320 -  122, 's'); 
    LCD_DisplayChar(84 ,320 -  115, ','); 
    LCD_DisplayChar(84 ,320 -  98, 'M'); 
    LCD_DisplayChar(84 ,320 -  86, 'O'); 
    LCD_DisplayChar(84 ,320 -  74, 'N'); 
    LCD_DisplayChar(84 ,320 -  62, 'O'); 
  
    /* Sampling Frequency  */
    LCD_DisplayChar(72 ,320 -  170, '8'); 
    LCD_DisplayChar(72 ,320 -  158, '0'); 
    LCD_DisplayChar(72 ,320 -  146, '0'); 
    LCD_DisplayChar(72 ,320 -  134, '0'); 
    LCD_DisplayChar(72 ,320 -  122, 'H'); 
    LCD_DisplayChar(72 ,320 -  110, 'z'); 
  }
  else
  {
    /* Display 16bits Stereo */
    LCD_DisplayChar(60 , 170, 'R'); 
    LCD_DisplayChar(60 , 158, 'E'); 
    LCD_DisplayChar(60 , 146, 'C'); 
    LCD_DisplayChar(60 , 138, '_'); 
    LCD_DisplayChar(60 , 130, 'W'); 
    LCD_DisplayChar(60 , 118, 'A'); 
    LCD_DisplayChar(60 , 106, 'V'); 
    LCD_DisplayChar(60 , 94, 'E'); 
  
    /* Display 8 bits MONO */
    LCD_DisplayChar(84 , 170, '8'); 
    LCD_DisplayChar(84 , 158, 'B'); 
    LCD_DisplayChar(84 , 146, 'i'); 
    LCD_DisplayChar(84 , 134, 't'); 
    LCD_DisplayChar(84 , 122, 's'); 
    LCD_DisplayChar(84 , 115, ','); 
    LCD_DisplayChar(84 , 98, 'M'); 
    LCD_DisplayChar(84 , 86, 'O'); 
    LCD_DisplayChar(84 , 74, 'N'); 
    LCD_DisplayChar(84 , 62, 'O'); 
  
    /* Sampling Frequency  */
    LCD_DisplayChar(72 , 170, '8'); 
    LCD_DisplayChar(72 , 158, '0'); 
    LCD_DisplayChar(72 , 146, '0'); 
    LCD_DisplayChar(72 , 134, '0'); 
    LCD_DisplayChar(72 , 122, 'H'); 
    LCD_DisplayChar(72 , 110, 'z'); 
  }

  
  /* Displays MESSAGE1 on line 5 */
  LCD_SetFont(&Font16x24);
}

/**
  * @brief  Controls the wave player application LCD display messages
  * @param  None.
  * @retval None.
  */
static uint8_t RecWave_LCDDisplayUpdate(void)
{
  uint8_t KeyState = NOKEY;
  uint32_t tmp = 0, index = 0;

  KeyState = Menu_ReadKey();

  
  if (KeyState == SEL)
  {
    DMA1_Channel3->CCR = 0x0;

    /* Disable TIM6 */
    TIM_Cmd(TIM6, DISABLE);
    
    LCD_WriteRAM_Prepare();
    
    /* Displays MESSAGE1 on line 5 */
    LCD_SetFont(&Font12x12);
    /* Set the LCD Back Color */
    LCD_SetBackColor(LCD_COLOR_BLUE);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DisplayStringLine(LCD_LINE_19, (uint8_t *)MESSAGE2P);
    /* Set the LCD Back Color */
    LCD_SetBackColor(LCD_COLOR_CYAN);
    /* Set the LCD Text Color */
    LCD_SetTextColor(LCD_COLOR_BLACK); 
    LCD_DisplayStringLine(LCD_LINE_18, (uint8_t *)MESSAGE1P);
    LCD_SetFont(&Font16x24);
    /* Set the LCD Back Color */
    LCD_SetBackColor(Black);
    LCD_SetTextColor(Magenta);
    /* Display the Titles */
    LCD_DisplayStringLine(LCD_LINE_0, "      Paused        ");
    KeyState = Menu_ReadKey();

    while ((KeyState != SEL) && (KeyState != DOWN))
    {
      KeyState = Menu_ReadKey();
    }
    if (KeyState == SEL)
    {
      /* Displays MESSAGE1 on line 5 */
      LCD_SetFont(&Font12x12);
      /* Set the LCD Back Color */
      LCD_SetBackColor(LCD_COLOR_CYAN);
      /* Set the LCD Text Color */
      LCD_SetTextColor(LCD_COLOR_BLACK);
      LCD_DisplayStringLine(LCD_LINE_18, (uint8_t *)MESSAGE3P);
      /* Set the LCD Back Color */
      LCD_SetBackColor(LCD_COLOR_BLUE);
      LCD_SetTextColor(LCD_COLOR_WHITE);
      LCD_DisplayStringLine(LCD_LINE_19, (uint8_t *)MESSAGE4P);
      LCD_SetFont(&Font16x24);
      /* Set the LCD Back Color */
      LCD_SetBackColor(Black);
      LCD_SetTextColor(Magenta);
      /* Display the Titles */
      LCD_DisplayStringLine(LCD_LINE_0, "    Now Playing      ");
   
      DMA1_Channel3->CCR = 0x0;
      /* Enable TIM6 */
      TIM_Cmd(TIM6, ENABLE);
      return KeyState;
    }
  }
  if (KeyState == DOWN)
  {
    return KeyState;
  }
  if (KeyState == LEFT)
  {
    LCD_WriteRAM_Prepare();
    tmp = F.fptr;
    if (tmp > (F.fsize / 20))
    {
      tmp -= F.fsize / 20;
    }
    else
    {
      tmp = 0;
    }
    
    /* Set LCD control line(/CS) */
    LCD_CtrlLinesWrite(LCD_NCS_GPIO_PORT, LCD_NCS_PIN, Bit_SET);
    
    f_lseek(&F, tmp);
    
    LCD_WriteRAM_Prepare();

    WaveDataLength += (WAVE_Format.DataSize / 20);
    if (WaveDataLength > WAVE_Format.DataSize)
    {
      WaveDataLength = WAVE_Format.DataSize;
    }
    LCD_SetBackColor(LCD_COLOR_BLACK);
    LCD_ClearLine(LCD_LINE_7);
    LCD_SetTextColor(LCD_COLOR_MAGENTA);
    LCD_DrawRect(LCD_LINE_7, 310, 16, 300);
    tmp = (uint8_t) ((uint32_t)((WAVE_Format.DataSize - WaveDataLength) * 100) / WAVE_Format.DataSize);
    for (index = 0; index < tmp; index++)
    {
      LCD_DrawLine(LCD_LINE_7, 310 - ((index) * 3), 16, LCD_DIR_VERTICAL);
    }
  }
  if (KeyState == RIGHT)
  {
    LCD_WriteRAM_Prepare();
    tmp = F.fptr;
    tmp += F.fsize / 20;
    if (tmp >  F.fsize)
    {
      tmp = F.fsize;
    }

    /* Set LCD control line(/CS) */
    LCD_CtrlLinesWrite(LCD_NCS_GPIO_PORT, LCD_NCS_PIN, Bit_SET);
    
    f_lseek(&F, tmp);
    
    LCD_WriteRAM_Prepare();

    if (WaveDataLength < (WAVE_Format.DataSize / 20))
    {
      WaveDataLength = 0;
    }
    else
    {
      WaveDataLength -= (WAVE_Format.DataSize / 20);
    }
    LCD_SetBackColor(LCD_COLOR_BLACK);
    LCD_ClearLine(LCD_LINE_7);
    LCD_SetTextColor(LCD_COLOR_MAGENTA);
    LCD_DrawRect(LCD_LINE_7, 310, 16, 300);
    tmp = (uint8_t) ((uint32_t)((WAVE_Format.DataSize - WaveDataLength) * 100) / WAVE_Format.DataSize);
    for (index = 0; index < tmp; index++)
    {
      LCD_DrawLine(LCD_LINE_7, 310 - ((index) * 3), 16, LCD_DIR_VERTICAL);
    }
  }
  return NOKEY ;
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
