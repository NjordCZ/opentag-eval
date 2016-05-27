/**
  ******************************************************************************
  * @file    rf_eeprom.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file includes the RF driver for the STM32F0518_EVAL demo.
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

/** @defgroup RF_EEPROM
  * @{
  */

/** @defgroup RF_EEPROM_Private_Types
  * @{
  */

/**
  * @}
  */

/** @defgroup RF_EEPROM_Private_Defines
  * @{
  */

#define sEE_WRITE_ADDRESS1     (uint16_t) 0x0000   /* EEPROM adress 1 */
#define sEE_WRITE_ADDRESS2     (uint16_t) 0x0004   /* EEPROM adress 2 */
#define sEE_WRITE_ADDRESS3     (uint16_t) 0x0008   /* EEPROM adress 3 */
#define sEE_WRITE_ADDRESS4     (uint16_t) 0x001C   /* EEPROM adress 4 */
#define LOGGER_START_ADDRESS   (uint16_t) 0x0000   /* start data logger adress */ 

#define BUFFER_SIZE ((uint8_t)2)
#define START   0x11 /* Predefined System Status */
#define PAUSED  0x22 /* Predefined System Status */
#define RUNNING 0x33 /* Predefined System Status */
#define STOPPED 0x44 /* Predefined System Status */
#define UPDATE  0x55 /* Predefined System Status */
#define OTHER   0x66 /* Predefined System Status */

#define MESSAGE1   "Run M24LRxx Applica-"
#define MESSAGE2   "-tion Software then "
#define MESSAGE3   "select Demo ESL to  "
#define MESSAGE4   "Configure the EEPROM"

#define MESSAGE5   "Er:E2PROM RF removed"
#define MESSAGE6   "Please check E2PROM "
#define MESSAGE7   "to continue or press"
#define MESSAGE8   "DOWN button to exit "

#define MESSAGE9   "SEL    -> Re-start  "
#define MESSAGE10  "DOWN   -> EXIT      "

/**
  * @}
  */

/** @defgroup RF_EEPROM_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup RF_EEPROM_Private_Variables
  * @{
  */
extern FATFS fs;
__IO uint8_t TempCelsiusEEPROM[4]     = "+abc";
static int32_t TempValueCelsius = 0;
extern __IO uint32_t LCDType;

uint8_t  RFStatus         = 0x00, RFStatus2 = 0;  /* SYSTEM BYTE */
uint8_t  DelaySleep       = 0x13;                 /* SYSTEM BYTE */
uint8_t  OverWrite        = 0x00;                 /* SYSTEM BYTE */
uint8_t  NbTemperature[2] = {0x00,0x00};          /* SYSTEM BYTE */

__IO uint16_t FreeSpace     = 0x0FFC; /* 4092 values */
__IO uint16_t ActualPointer = 0x0008; /* first data will be writen here */
__IO uint16_t StatusAdd     = 0x0000; /* address of the SYSTEM BYTE */
__IO uint16_t OverWriteAdd  = 0x0001; /* address of the SYSTEM BYTE */
__IO uint16_t DelayAdd      = 0x0002; /* address of the SYSTEM BYTE */
__IO uint16_t NbTempAdd    = 0x0004;  /* address of the SYSTEM BYTE */

__IO uint32_t ErrorCode2;
__IO uint32_t ErrorFunction;
extern __IO uint8_t sEEStatus;
__IO uint16_t RFTemp1 =0, RFTemp1Prev = 65, RFTemp2 = 0, RFIndex =0;
__IO uint8_t RFKeyStatus = NOKEY;
__IO uint8_t exitdemo = 0x0, DataStatus = 0;
__IO uint32_t seetimeOut = 0x1FFFF;
__IO uint16_t RFNumDataRead = 4;

/**
  * @}
  */

/** @defgroup RF_EEPROM_Private_FunctionPrototypes
  * @{
  */

static uint32_t start_acquisition(void); /*return value of the function */
static uint32_t stop_acquisition(void); /*return value of the function */
static uint32_t acquisition_running(void); /*return value of the function */
static uint32_t acquisition_update(void); /*return value of the function */
static uint32_t idle_acquisition(void);     /*return value of the function */
/**
  * @}
  */

/** @defgroup RF_EEPROM_Private_Functions
  * @{
  */

/**
  * @brief  RF EEprom ESL function
  * @param  None
  * @retval None
  */
void RF_EEprom_ESLFunc(void)
{
  uint8_t  Rx1_Buffer[5], text[20];
  uint16_t  tmp = 0;
  /* De-Initialize the Thermometer application */
  LM75_DeInit();
  
  /* External Interrupt Disable */
  Demo_IntExtOnOffCmd(DISABLE);
  
  /* Display Image */
  LCD_SetDisplayWindow(239, 0x13F, 240, 320);
  Storage_OpenReadFile(239, 319, "STFILES/RF10.BMP");
  
  /* Insert a delay time */
  Demo_Delay(200);
  
  /* Clear LCD */
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Initialize the I2C EEPROM driver */
  sEE_Init();  
  
  /* Set the LCD Text size */
  LCD_SetFont(&Font16x24);
   
  /* Loop until Exit by DOWN button */
  while (Menu_ReadKey() != DOWN)
  {
    /***** Check the Data  **********/
    RFNumDataRead = 4;
    /* Check on the EEprom status */
    if(sEE_WaitEepromStandbyState() == sEE_OK)
    { 
      
      /* Read from SPI EEPROM from sEE_READ_ADDRESS3 */
      sEE_ReadBuffer(Rx1_Buffer, sEE_WRITE_ADDRESS1, (uint16_t *)(&RFNumDataRead));
      seetimeOut = 0x1FFFF;
      while ((RFNumDataRead > 0) && (seetimeOut > 0x0))
      {
        seetimeOut--;
      } 
    }
    
    if((Rx1_Buffer[1] < 3) && (Rx1_Buffer[2] < 3))
    {
      /* Set the LCD Back Color */
      LCD_SetBackColor(LCD_COLOR_WHITE);
      
      if(DataStatus !=0)
      {
        /* Clear Line 6 ->9 */
        LCD_ClearLine(LCD_LINE_6);
        LCD_ClearLine(LCD_LINE_7);
        LCD_ClearLine(LCD_LINE_8);
        LCD_ClearLine(LCD_LINE_9);
        DataStatus = 0;
      }
      
      /***** Display Icons  **********/
      RFNumDataRead = 4;
      /* Check on the EEprom status */
      if(sEE_WaitEepromStandbyState() == sEE_OK)
      { 
        /* Read from SPI EEPROM from sEE_READ_ADDRESS3 */
        sEE_ReadBuffer(Rx1_Buffer, sEE_WRITE_ADDRESS1, (uint16_t *)(&RFNumDataRead));
        seetimeOut = 0x1FFFF;
        while ((RFNumDataRead > 0) && (seetimeOut != 0x0))
        {
          seetimeOut--;
        } 
      }
      if(Rx1_Buffer[1] ==1)
      {
        LCD_SetDisplayWindow(210, 160, 64, 64);
        
        Storage_OpenReadFile(210, 160, "STFILES/PROMO.BMP");
      }
      else  if(Rx1_Buffer[1] ==2)
      {
        LCD_SetDisplayWindow(210, 160, 64, 64);
        
        Storage_OpenReadFile(210, 160, "STFILES/DISCOUNT.BMP");
      }
      else if(Rx1_Buffer[1] ==0)
      {
        LCD_SetDisplayWindow(210, 160, 64, 64);
        
        Storage_OpenReadFile(210, 160, "STFILES/WHITE.BMP");
      }
      
      if(Rx1_Buffer[2] ==1)
      {
        LCD_SetDisplayWindow(210, 85, 64, 64);
        
        Storage_OpenReadFile(210, 85, "STFILES/ARROWU.BMP");
      }
      else  if(Rx1_Buffer[2] ==2)
      {
        LCD_SetDisplayWindow(210, 85, 64, 64);
        
        Storage_OpenReadFile(210, 85, "STFILES/ARROWD.BMP");
      }
      else if(Rx1_Buffer[2] ==0)
      {
        LCD_SetDisplayWindow(210, 85, 64, 64);
        Storage_OpenReadFile(210, 85, "STFILES/WHITE.BMP");
      }
      /* Disable LCD Window mode */
      LCD_WindowModeDisable();
      
      /************* Display Price ****************************/
      RFNumDataRead = 4;
      /* Check on the EEprom status */
      if(sEE_WaitEepromStandbyState() == sEE_OK)
      { 
        /* Read from SPI EEPROM from sEE_READ_ADDRESS3 */
        sEE_ReadBuffer(Rx1_Buffer, sEE_WRITE_ADDRESS2 , (uint16_t *)(&RFNumDataRead));
        seetimeOut = 0x1FFFF;
        while ((RFNumDataRead > 0) && (seetimeOut != 0x0))
        {
          seetimeOut--;
        } 
        
        tmp = (Rx1_Buffer[0] >> 4);
        text[0] = (Rx1_Buffer[0] & 0x0F);
        if (text[0] == 0xA) text[0] = '.';
        text[1] = (Rx1_Buffer[1] >> 4);
        if (text[1] == 0xA) text[1] = '.';
        text[2] = (Rx1_Buffer[1] & 0x0F);
        if (text[2] == 0xA) text[2] = '.';
        text[3] = (Rx1_Buffer[2] >> 4);
        if (text[3] == 0xA) text[3] = '.';
        text[4] = (Rx1_Buffer[2] & 0x0F);
        if (text[4] == 0xA) text[4] = '.';
        text[5] = (Rx1_Buffer[3] >> 4);
        if (text[5] == 0xA) text[5] = '.';
        text[6] = (Rx1_Buffer[3] & 0x0F);
        if (text[6] == 0xA) text[6] = '.';
        /* Set the Back Color */
        LCD_SetBackColor(LCD_COLOR_WHITE);
        
        /* Set the LCD Text size */
        LCD_SetFont(&Font16x24);
        
        /* Set the Text Color */
        LCD_SetTextColor(LCD_COLOR_WHITE);
        /* Set the Back Color */
        LCD_SetBackColor(LCD_COLOR_WHITE);
        
        for(tmp = 0 ; tmp < 7; tmp++)
        {
          /* Check if the LCD is HX8347D Controller */
          if(LCDType == LCD_HX8347D)
          {
            LCD_DisplayChar(95, 20 + (tmp * 16), ' ');
          }
          else
          {
            LCD_DisplayChar(95, 300 - (tmp * 16), ' ');
          }
        }
        /* Set the Back Color */
        LCD_SetBackColor(LCD_COLOR_BLACK);
        
        for(tmp = 0 ; tmp < (Rx1_Buffer[0] >> 4); tmp++)
        {
          if(text[tmp] == '.')
          {
            /* Check if the LCD is HX8347D Controller */
            if(LCDType == LCD_HX8347D)
            {
              LCD_DisplayChar(95, 20 + (tmp * 16), '.');
            }
            else
            {
              LCD_DisplayChar(95, 300 - (tmp * 16), '.');
            }
          }
          else
          {
            /* Check if the LCD is HX8347D Controller */
            if(LCDType == LCD_HX8347D)
            {
              LCD_DisplayChar(95, 20 + (tmp * 16), text[tmp] + 0x30);
            }
            else
            {
              LCD_DisplayChar(95, 300 - (tmp * 16), text[tmp] + 0x30);
            }
          }
        }
      }
      
      /**************************** Product Name ****************************/
      /* Set the Back Color */
      LCD_SetBackColor(LCD_COLOR_WHITE);
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_BLACK);
      /* Set the LCD Text size */
      LCD_SetFont(&Font12x12);
      for(tmp= 0; tmp < 5; tmp++)
      {
        RFNumDataRead = 4;
        /* Check on the EEprom status */
        if(sEE_WaitEepromStandbyState() == sEE_OK)
        { 
          /* Read from SPI EEPROM from sEE_READ_ADDRESS3 */
          sEE_ReadBuffer(Rx1_Buffer,(sEE_WRITE_ADDRESS3 + (tmp * 0x4)) , (uint16_t *)(&RFNumDataRead));
          seetimeOut = 0x1FFFF;
          while ((RFNumDataRead > 0) && (seetimeOut != 0x0))
          {
            seetimeOut--;
          }  
          
          text[0 + (tmp *4)] = Rx1_Buffer[0];
          /* Check if the LCD is HX8347D Controller */
          if(LCDType == LCD_HX8347D)
          {
            LCD_DisplayChar(40, 20 + (0 + (48 *tmp)), text[0 + (tmp *4)]);
          }
          else
          {
            LCD_DisplayChar(40, 300 - (0 + (48 *tmp)), text[0 + (tmp *4)]);
          }
          text[1 + (tmp *4)] = Rx1_Buffer[1];
          /* Check if the LCD is HX8347D Controller */
          if(LCDType == LCD_HX8347D)
          {
            LCD_DisplayChar(40, 20 + (12+ (48 *tmp)), text[1 + (tmp *4)]);
          }
          else
          {
            LCD_DisplayChar(40, 300 - (12+ (48 *tmp)), text[1 + (tmp *4)]);
          }
          text[2 + (tmp *4)] = Rx1_Buffer[2];
          /* Check if the LCD is HX8347D Controller */
          if(LCDType == LCD_HX8347D)
          {
            LCD_DisplayChar(40, 20 + (24+ (48 *tmp)), text[2 + (tmp *4)]);
          }
          else
          {
            LCD_DisplayChar(40, 300 - (24+ (48 *tmp)), text[2 + (tmp *4)]);
          }
          text[3 + (tmp *4)] = Rx1_Buffer[3];
          /* Check if the LCD is HX8347D Controller */
          if(LCDType == LCD_HX8347D)
          {
            LCD_DisplayChar(40, 20 + (36+ (48 *tmp)), text[3 + (tmp *4)]);
          }
          else
          {
            LCD_DisplayChar(40, 300 - (36+ (48 *tmp)), text[3 + (tmp *4)]);
          }
        }
      }
      
      /**************************** Product Name ****************************/
      /* Set the Back Color */
      LCD_SetBackColor(LCD_COLOR_WHITE);
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_BLACK);
      
      for(tmp= 0; tmp < 20; tmp++)
      {
        text[tmp] = ' ';
      }
      
      tmp = 0;
      for(tmp= 0; tmp < 5; tmp++)
      {
        RFNumDataRead = 4;
        
        /* Check on the EEprom status */
        if(sEE_WaitEepromStandbyState() == sEE_OK)
        { 
          /* Read from SPI EEPROM from sEE_READ_ADDRESS3 */
          sEE_ReadBuffer(Rx1_Buffer,(sEE_WRITE_ADDRESS4 + (tmp * 0x4)) , (uint16_t *)(&RFNumDataRead));
          seetimeOut = 0x1FFFF;
          while ((RFNumDataRead > 0) && (seetimeOut != 0x0))
          {
            seetimeOut--;
          } 
          
          text[0 + (tmp *4)] = Rx1_Buffer[0];
          /* Check if the LCD is HX8347D Controller */
          if(LCDType == LCD_HX8347D)
          {
            LCD_DisplayChar(65, 20 + (0 + (48 *tmp)), text[0 + (tmp *4)]);
          }
          else
          {
            LCD_DisplayChar(65, 300 - (0 + (48 *tmp)), text[0 + (tmp *4)]);
          }
          text[1 + (tmp *4)] = Rx1_Buffer[1];
          /* Check if the LCD is HX8347D Controller */
          if(LCDType == LCD_HX8347D)
          {
            LCD_DisplayChar(65, 20 + (12+ (48 *tmp)), text[1 + (tmp *4)]);
          }
          else
          {
            LCD_DisplayChar(65, 300 - (12+ (48 *tmp)), text[1 + (tmp *4)]);
          }
          text[2 + (tmp *4)] = Rx1_Buffer[2];
          /* Check if the LCD is HX8347D Controller */
          if(LCDType == LCD_HX8347D)
          {
            LCD_DisplayChar(65, 20 + (24+ (48 *tmp)), text[2 + (tmp *4)]);
          }
          else
          {
            LCD_DisplayChar(65, 300 - (24+ (48 *tmp)), text[2 + (tmp *4)]);
          }
          text[3 + (tmp *4)] = Rx1_Buffer[3];
          /* Check if the LCD is HX8347D Controller */
          if(LCDType == LCD_HX8347D)
          {
            LCD_DisplayChar(65, 20 + (36+ (48 *tmp)), text[3 + (tmp *4)]);
          }
          else
          {
            LCD_DisplayChar(65, 300 - (36+ (48 *tmp)), text[3 + (tmp *4)]);
          }     
        }
      }
      for(tmp= 0; tmp < 20; tmp++)
      {
        text[tmp] = ' ';
      }
      tmp = 0;
      /* Insert a delay time*/
      Demo_Delay(40);
    }
    else
    {
      /* Set the LCD Text size */
      LCD_SetFont(&Font16x24);
      /* Set the LCD Back Color */
      LCD_SetBackColor(White);
      
      /* Set the LCD Text Color */
      LCD_SetTextColor(White);
      
      /* Clear Lines */
      LCD_ClearLine(LCD_LINE_0);
      LCD_ClearLine(LCD_LINE_1);
      LCD_ClearLine(LCD_LINE_2);
      LCD_ClearLine(LCD_LINE_3);
      LCD_ClearLine(LCD_LINE_4);
      LCD_ClearLine(LCD_LINE_5);
      /* Set the LCD Back Color */
      LCD_SetBackColor(Blue);
      
      if(seetimeOut ==0)
      {
        /* Set the LCD Text Color */
        LCD_SetTextColor(Red);
        /* Displays MESSAGE1 on line 1 */
        LCD_DisplayStringLine(LINE(6), (uint8_t *)MESSAGE5);
        LCD_DisplayStringLine(LINE(7), (uint8_t *)MESSAGE6);
        /* Set the LCD Text Color */
        LCD_SetTextColor(White);
        LCD_DisplayStringLine(LINE(8), (uint8_t *)MESSAGE7);
        LCD_DisplayStringLine(LINE(9), (uint8_t *)MESSAGE8);
      }
      else
      {
        /* Displays MESSAGE1 on line 1 */
        LCD_DisplayStringLine(LINE(6), (uint8_t *)MESSAGE1);
        LCD_DisplayStringLine(LINE(7), (uint8_t *)MESSAGE2);
        LCD_DisplayStringLine(LINE(8), (uint8_t *)MESSAGE3);
        LCD_DisplayStringLine(LINE(9), (uint8_t *)MESSAGE4);
      }
      DataStatus = 1;
      
      /* Insert a delay time*/
      Demo_Delay(40);
    }
  } 
  
  /* Set the LCD Text size */
  LCD_SetFont(&Font16x24);
  
  /* Checks the availability of the bitmap files */
  Demo_CheckBitmapFilesStatus(); 
  
  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* External Interrupt Enable */
  Demo_IntExtOnOffCmd(ENABLE);
  
  /* Display Window */
  Menu_DisplayMenu();  
}

/**
  * @brief  RF_EEprom Datalogger init function
  * @param  None
  * @retval None
  */
void RF_EEprom_DataLogger_Init(void)
{
  uint16_t pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0;
  
  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set the Text and Back Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_SetBackColor(LCD_COLOR_BLUE);
  
  LCD_DisplayStringLine(LCD_LINE_0, " M24LR64 DataLogger ");
  
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_RED);
  LCD_SetBackColor(LCD_COLOR_WHITE);
  
  /* Display a Full rectangle on the LCD */
  LCD_DrawFullRect(33, 310,302, 200 );  
  /* Set the Back Color */
  LCD_SetTextColor(LCD_COLOR_BLACK);
  LCD_SetBackColor(LCD_COLOR_CYAN);
  
  /* Display a Full rectangle on the LCD */
  LCD_DrawFullRect(40, 280,270, 180 );   
  
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  LCD_SetTextColor(LCD_COLOR_BLACK);
  
  /* Set the LCD Text size */
  LCD_SetFont(&Font8x8);
  
  /****** Configure the Curve graduation ( from -25°C to +35 °C) **************/  
  /* Check if the LCD is HX8347D Controller */
  if(LCDType == LCD_HX8347D)
  {
    pos1 = 279;
    pos2 = 11;
    pos3 = 20;
    pos4 = 29;
  }
  else
  {
    pos1 = 279;
    pos2 = 309;
    pos3 = 300;
    pos4 = 291;
  }
  LCD_DrawLine(220, pos1,3,Horizontal);/* -25°C  */
  LCD_DisplayChar(215, pos2, 0x2D);
  LCD_DisplayChar(215, pos3, 0x32);
  LCD_DisplayChar(215, pos4, 0x35);
  LCD_DrawLine(205, pos1,3,Horizontal);/* -20°C */
  LCD_DisplayChar(200, pos2, 0x2D);
  LCD_DisplayChar(200, pos3, 0x32);
  LCD_DisplayChar(200, pos4, 0x30);
  LCD_DrawLine(190, pos1,3,Horizontal);/* -15°C */
  LCD_DisplayChar(185, pos2, 0x2D);
  LCD_DisplayChar(185, pos3, 0x31);
  LCD_DisplayChar(185, pos4, 0x35);
  LCD_DrawLine(175, pos1,3,Horizontal);/* -10°C */
  LCD_DisplayChar(170, pos2, 0x2D);
  LCD_DisplayChar(170, pos3, 0x31);
  LCD_DisplayChar(170, pos4, 0x30);
  LCD_DrawLine(160, pos1,3,Horizontal);/* -05°C */
  LCD_DisplayChar(155, pos2, 0x2D);
  LCD_DisplayChar(155, pos3, 0x30);
  LCD_DisplayChar(155, pos4, 0x35);
  LCD_DrawLine(145, pos1,3,Horizontal);/* 0°C */
  LCD_DisplayChar(140, pos2, 0x2B);
  LCD_DisplayChar(140, pos3, 0x30);
  LCD_DisplayChar(140, pos4, 0x30); 
  LCD_DrawLine(130, pos1,3,Horizontal); /*  +05°C  */
  LCD_DisplayChar(125, pos2, 0x2B);
  LCD_DisplayChar(125, pos3, 0x30);
  LCD_DisplayChar(125, pos4, 0x35);  
  LCD_DrawLine(115, pos1,3,Horizontal);/*  +10°C  */
  LCD_DisplayChar(110, pos2, 0x2B);
  LCD_DisplayChar(110, pos3, 0x31);
  LCD_DisplayChar(110, pos4, 0x30);
  LCD_DrawLine(100, pos1,3,Horizontal);/*  +15°C  */
  LCD_DisplayChar(95, pos2, 0x2B);
  LCD_DisplayChar(95, pos3, 0x31);
  LCD_DisplayChar(95, pos4, 0x35);
  LCD_DrawLine(85, pos1,3,Horizontal);/*  +20°C  */
  LCD_DisplayChar(80, pos2, 0x2B);
  LCD_DisplayChar(80, pos3, 0x32);
  LCD_DisplayChar(80, pos4, 0x30);
  LCD_DrawLine(70, pos1,3,Horizontal); /*  +25°C  */
  LCD_DisplayChar(65, pos2, 0x2B);
  LCD_DisplayChar(65, pos3, 0x32);
  LCD_DisplayChar(65, pos4, 0x35);
  LCD_DrawLine(55, pos1,3,Horizontal); /*  +30°C  */
  LCD_DisplayChar(50, pos2, 0x2B);
  LCD_DisplayChar(50, pos3, 0x33);
  LCD_DisplayChar(50, pos4, 0x30);
  LCD_DrawLine(40, pos1,3,Horizontal); /*  +35°C  */
  LCD_DisplayChar(35, pos2, 0x2B);
  LCD_DisplayChar(35, pos3, 0x33);
  LCD_DisplayChar(35, pos4, 0x35);
  
  /********************************* End Config. ******************************/
  /* Set the LCD Text size */
  LCD_SetFont(&Font16x24);
}

/**
  * @brief  RF_EEprom Datalogger function
  * @param  None
  * @retval None
  */
void RF_EEprom_DataLogger(void)
{
  uint16_t NumDataRead = 0;
  
  /* External Interrupt Disable */
  Demo_IntExtOnOffCmd(DISABLE);
  
  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);
  
  sEE_Init(); /* init eeprom dual*/
  
  /* Initialize the application */
  ErrorFunction = stop_acquisition();
  
  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Display Image */
  f_mount(0, &fs);
  
  LCD_SetDisplayWindow(239, 0x13F, 240, 320);
  Storage_OpenReadFile(239, 319, "STFILES/RF4.BMP"); 
  
  LCD_SetFont(&Font12x12);
  /* Set the Text and Back Color */
  LCD_SetTextColor(LCD_COLOR_BLACK);
  LCD_SetBackColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_15, " START => USER INTERFACE  ");
  LCD_DisplayStringLine(LCD_LINE_16, " DOWN  => EXIT             ");
  LCD_SetFont(&Font16x24);
  
  /* Loop until Exit by pressing the DOWN button */
  while((Menu_ReadKey() != DOWN))
  {
    RFStatus2 = RFStatus;
    if(RFStatus == OTHER) /* Fisrt Boot */
    {
      /* Initialize the application */
      ErrorFunction = idle_acquisition();
    }
    else
    {
      NumDataRead = 1;
      /* read program status */
      ErrorFunction = sEE_ReadBuffer(&RFStatus2, StatusAdd, (uint16_t *)(&NumDataRead));
      
      RFStatus = RFStatus2;
      /* insert 50 ms */
      Delay(2000);
      
      if(ErrorFunction == sEE_OK)
      {
        if(RFStatus == STOPPED)
        {
          /* Initialize the application */
          ErrorFunction = stop_acquisition();
          
          /* Clear the LCD */
          LCD_Clear(LCD_COLOR_WHITE);
          
          /* Display Image */
          f_mount(0, &fs);
          LCD_SetDisplayWindow(239, 0x13F, 240, 320);
          Storage_OpenReadFile(239, 319, "STFILES/RF4.BMP"); 
          LCD_SetFont(&Font12x12);
          /* Set the Text and Back Color */
          LCD_SetTextColor(LCD_COLOR_BLACK);
          LCD_SetBackColor(LCD_COLOR_WHITE);
          LCD_DisplayStringLine(LCD_LINE_15, " START => USER INTERFACE  ");
          LCD_DisplayStringLine(LCD_LINE_16, " DOWN  => EXIT             ");
          LCD_SetFont(&Font16x24);  
        }
        
        else if(RFStatus == START)
        {
          /* Go to start function */
          ErrorFunction = start_acquisition();
          
          /* Initialize the Interface */
          RF_EEprom_DataLogger_Init();
        }
        else if(RFStatus == RUNNING)
        {
          /* Go to Acquisation function */
          ErrorFunction = acquisition_running();
        }
        else if(RFStatus == UPDATE)
        {
          /* Go to update function */
          ErrorFunction = acquisition_update();
        }
      }
    }    
  }
  /* Checks the availability of the bitmap files */
  Demo_CheckBitmapFilesStatus(); 
  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* External Interrupt Enable */
  Demo_IntExtOnOffCmd(ENABLE);
  
  /* Display Window */
  Menu_DisplayMenu();
}

/**
  * @brief  Start acquisition.
  * @param  None.
  * @retval EEprom write operation satuts.
  */
static uint32_t start_acquisition(void)
{  
  /* reset system variable */
  ActualPointer    = 0x0008;
  
  /* init the number of temperature data */
  NbTemperature[0] = 0x00;
  NbTemperature[1] = 0x00;
  
  /* 4092 values */
  FreeSpace        = 0x0FFC;
  RFStatus            = RUNNING;
  
  /* set delay value to 1 second */
  DelaySleep  = 0xd; 
  
  sEEStatus = sEE_FAIL;
  
  while(sEEStatus != sEE_OK)
  {
    sEE_WaitEepromStandbyState();
    sEEStatus = sEE_OK;
    sEE_WriteBuffer(&DelaySleep, DelayAdd, 1);
  }
  
  sEEStatus = sEE_FAIL;
  
  while(sEEStatus != sEE_OK)
  {
    sEE_WaitEepromStandbyState();
    sEEStatus = sEE_OK;
    /* Write "RUNNIG" Status to EEPROM register */
    sEE_WriteBuffer(&RFStatus, StatusAdd, 1);
  }
  
  sEEStatus = sEE_FAIL;
  
  while(sEEStatus != sEE_OK)
  {
    sEE_WaitEepromStandbyState();
    sEEStatus = sEE_OK;
    
    /* Configure the number of temperature value */
    sEE_WriteBuffer(NbTemperature, NbTempAdd, 2);
  }
  
  return sEE_OK;
}

/**
  * @brief  idle acquisition.
  * @param  None.
  * @retval EEprom write operation satuts.
  */
static uint32_t idle_acquisition(void)
{
  RFStatus = 0x00;
  
  sEEStatus = sEE_FAIL;
  
  while(sEEStatus != sEE_OK)
  {
    sEE_WaitEepromStandbyState();
    sEEStatus = sEE_OK;
    /* Write "RUNNIG" Status to EEPROM register */
    sEE_WriteBuffer(&RFStatus, StatusAdd, 1);
  }
  
  return sEE_OK;
}

/**
  * @brief  Runnig acquisition.
  * @param  None.
  * @retval EEprom write operation satuts.
  */
static uint32_t acquisition_running(void)
{
  uint16_t NumDataRead = 0,data_sensor2=0, data_sensor1 = 0;
  uint8_t  data_sensor[2] = {0x00,0x00}; /* to store temperature */
  I2C_InitTypeDef  I2C_InitStructure;
	  
  /* Check on the EEPROM register status "FULL" or not */
  if(FreeSpace == 0x0000)
  {
    /* Init. ErrorCode2 value */
    ErrorCode2 = sEE_FAIL ;
    
    while(ErrorCode2 != sEE_OK)
    {
      NumDataRead = 1;
      /* Check on the EEprom status */
      ErrorCode2 = sEE_ReadBuffer(&OverWrite, OverWriteAdd, (uint16_t *)(&NumDataRead));/* READ OVERWRITE */   
    }
    
    if(OverWrite == 0x11)/* Authorized */
    {
      ErrorFunction = start_acquisition();
    }
    else /* Non Authorized */
    {
      ErrorFunction = stop_acquisition();
    }
  }
  else
  {
    /* De-Initialize the I2C EEPROM driver */
    sEE_DeInit(); 
    
    /* Insert Delays */
    Demo_Delay(50); 
    
    /* Initialize the Temperature Sensor */   
    LM75_LowLevel_Init();
    
    /* LM75_I2C configuration */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
    I2C_InitStructure.I2C_DigitalFilter = 0x00;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_Timing = LM75_I2C_TIMING;
    
    /* Apply LM75_I2C configuration after enabling it */
    I2C_Init(LM75_I2C, &I2C_InitStructure);
    
    /* LM75_I2C Peripheral Enable */
    I2C_Cmd(LM75_I2C, ENABLE);
    
    if (LM75_GetStatus() == SUCCESS)
    {
      /* Get double of Temperature value */
      data_sensor2 = LM75_ReadTemp();
      
      if (data_sensor2 <= 256)
      {
        /* Positive temperature measured */
        TempCelsiusEEPROM[0] = '+';
        /* Initialize the temperature sensor value*/
        TempValueCelsius = data_sensor2;
      }
      else
      {
        /* Negative temperature measured */
        TempCelsiusEEPROM[0] = '-';
        /* Remove temperature value sign */
        TempValueCelsius = 0x200 - data_sensor2;
      }
      
      /* Calculate temperature digits in °C */
      if ((TempValueCelsius & 0x01) == 0x01)
      {
        TempCelsiusEEPROM[3] = 0x35;
      }
      else
      {
        TempCelsiusEEPROM[3] =  0x30;
      }
      
      TempValueCelsius >>= 1;
      
      TempCelsiusEEPROM[1] = ((TempValueCelsius % 100) / 10) + 0x30;
      TempCelsiusEEPROM[2] = ((TempValueCelsius % 100) % 10) + 0x30; 
    }
    
    /***** Display pixels on the LCD Write the Curve (data value) ***********/    
    /* Check on the values sign (+)/(-)*/
    if (TempCelsiusEEPROM[0] == 0x2B)
    {  
      /* de-limited the T°C to +35°C */
      if(TempValueCelsius > 35) 
      {
        TempCelsiusEEPROM[1] = 0x33;
        TempCelsiusEEPROM[2] = 0x35;
      }
      
      RFTemp1 = (145 - (((TempCelsiusEEPROM[1]-0x30) *10) + (TempCelsiusEEPROM[2]-0x30))* 3 );
      RFTemp2 = (TempCelsiusEEPROM[3] - 0x30);
      
      /* Check on the fraction value exp." 20.5°C "  */
      if (RFTemp2 == 5)
      {  
        RFTemp1 = RFTemp1 + 2;/* add to pixels */
      }
    }
    else /* Negatives values */
    {
      /* de-limited the T°C to +35°C */
      if(TempValueCelsius > 25) 
      {
        TempCelsiusEEPROM[1] = 0x35;
        TempCelsiusEEPROM[2] = 0x30;
      }
      RFTemp1 = (145 + (((TempCelsiusEEPROM[1]-0x30) *10) + (TempCelsiusEEPROM[2]-0x30))* 3 );
      RFTemp2 = (TempCelsiusEEPROM[3] - 0x30);
      
      if (RFTemp1 >= 220)
      {
        RFTemp1 = 220;
        RFTemp2 = 0;
      }  
      
      /* Check on the fraction value exp." -20.5°C "  */
      if (RFTemp2 == 5)
      {  
        RFTemp1 = RFTemp1- 2; /*Substract 2 pixels */
      }
    }

    /* set text Colors */
    LCD_SetTextColor(LCD_COLOR_BLACK);
    /* Check on the display level between the previous an dthe next data*/
    if (RFTemp1 > RFTemp1Prev) 
    { 
      /* Drow lines for the Continuities between the previous and the next pixels*/
      LCD_DrawLine(RFTemp1Prev , (280 - RFIndex +1) , RFTemp1 - RFTemp1Prev ,Vertical);  
    }
    else if (RFTemp1 < RFTemp1Prev)
    {
      /* Drow lines for the Continuities between the previous and the next pixels*/
      LCD_DrawLine(RFTemp1 , (280 - RFIndex +1) , RFTemp1Prev - RFTemp1 ,Vertical);  
    }
    /* Drw the new data as a pixel */
    LCD_DrawLine(RFTemp1 , (280 - RFIndex) , 1 ,Vertical);
    
    /* Update the previous value */
    RFTemp1Prev = RFTemp1;
    
    /* Increament Index for the colonnes pixels */
    RFIndex++;
    
    /* Check on the Colonnes number */
    if(RFIndex == 271)
    {
      RFIndex = 0;
      /* Set the Back Color */
      LCD_SetBackColor(LCD_COLOR_CYAN);
      
      /* Display a Full rectangle on the LCD */
      LCD_DrawFullRect(40, 280,270, 180 ); 
    }     
    /*********** Write to EEPROM ********************************************/
    
    data_sensor1 =  (uint16_t)(data_sensor2  << 7);
    
    data_sensor[0] = (uint8_t)(0xFF & (data_sensor1 >> 8));
    data_sensor[1] = (uint8_t)(data_sensor1 & 0x00FF);  
    
    /* De-Initialize the Thermometer application */
    LM75_DeInit();
    
    /* Insert a refresh time : acquisation each 10 ms */
    Delay((uint16_t)9000);     /* TEMPO */
    
    /* Initialize the I2C EEPROM driver */
    sEE_Init(); 
    
    sEEStatus = sEE_FAIL;
    
    while(sEEStatus != sEE_OK)
    {
      sEE_WaitEepromStandbyState();
      sEEStatus = sEE_OK;
      sEE_WriteBuffer(data_sensor, ActualPointer, 2);/* write dual eeprom */
    }
    
    Delay((uint16_t)2000); 	/* to let the time writting to dual eeprom */
    ActualPointer += 2;
    FreeSpace --;
    
    if(NbTemperature[0]==0xFF)
    {
      NbTemperature[1]++;
      NbTemperature[0]=0x00;
    }
    else
    {
      NbTemperature[0] ++;
    }
    
    sEEStatus = sEE_FAIL;
    
    while(sEEStatus != sEE_OK)
    {
      sEE_WaitEepromStandbyState();
      sEEStatus = sEE_OK;
      sEE_WriteBuffer(NbTemperature, NbTempAdd , 2);/*ECRITURE EE*/
    }
  }
  return sEE_OK;
}

/** 
  * @brief  stop acquisition.
  * @param  None.
  * @retval EEprom write operation satuts.
  */
static uint32_t stop_acquisition(void)
{
  uint8_t Status_temp= STOPPED;
  RFStatus = OTHER ;
  
  sEEStatus = sEE_FAIL;
  
  while(sEEStatus != sEE_OK)
  {
    sEE_WaitEepromStandbyState();
    sEEStatus = sEE_OK;
    /* Write "STOP" cmd. to EEPROM register */
    sEE_WriteBuffer(&Status_temp, StatusAdd, 1);
  }
  
  /* Initailize variable */
  RFTemp1 =0;
  RFTemp1Prev = 65;
  RFTemp2 = 0;
  RFIndex =0;
  
  /* return status */  
  return sEE_OK;
}

/**
  * @brief  stop acquisition.
  * @param  None.
  * @retval EEprom write operation satuts.
  */
static uint32_t acquisition_update(void)
{
  uint8_t Status_temp= RUNNING;
  
  /* Initialize the dual eeprom */
  sEE_Init(); 
  sEEStatus = sEE_FAIL;
  
  while(sEEStatus != sEE_OK)
  {
    sEE_WaitEepromStandbyState();
    sEEStatus = sEE_OK;
    /* Write "RUNNIG" Status to EEPROM register */
    sEE_WriteBuffer(&Status_temp, StatusAdd, 1);
  }
  return sEE_OK;
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
