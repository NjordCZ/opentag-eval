/**
  ******************************************************************************
  * @file    thermometer.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file includes the thermometer driver for the STM320518-EVAL
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

/** @defgroup THERMOMETER
  * @brief    This file includes the Thermometer driver for the STM320518-EVAL
  *           demonstration.
  * @{
  */

/** @defgroup THERMOMETER_Private_Types
  * @{
  */

/**
  * @}
  */


/** @defgroup THERMOMETER_Private_Defines
  * @{
  */
#define MESSAGE                 " Press DOWN to Exit "
#define TEMPERATURE_THYS        31
#define TEMPERATURE_TOS         32


/**
  * @}
  */

/** @defgroup THERMOMETER_Private_Macros
  * @{
  */


/**
  * @}
  */

/** @defgroup THERMOMETER_Private_Variables
  * @{
  */
uint8_t TempCelsiusDisplay[] = "       +abc.d C     ";
uint8_t TempFahrenheitDisplay[] = "       +abc.d F     ";
int32_t TempValue = 0, TempValueCelsius = 0, TempValueFahrenheit = 0;
__IO uint8_t SMbusAlertOccurred = 0;
extern __IO uint32_t LCDType;

/**
  * @}
  */

/** @defgroup THERMOMETER_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup THERMOMETER_Private_Functions
  * @{
  */

/**
  * @brief  Displays the temperature in Celsius and fahrenheit degree.
  * @param  None
  * @retval None
  */
void Menu_ThermometerFun(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  uint32_t i = 0, pressedkey = 0;

  while (Menu_ReadKey() != NOKEY)
  {}
  
  /* Initialize the Temperature Sensor */
  LM75_Init();

  if (LM75_GetStatus() == SUCCESS)
  {
    /* Disable the JoyStick Interrupts */
    Demo_IntExtOnOffCmd(DISABLE);
    
    /* Clear the LCD */
    LCD_Clear(LCD_COLOR_WHITE);
    
    /* Set the LCD Back Color */
    LCD_SetBackColor(LCD_COLOR_CYAN);
    /* Set the LCD Text Color */
    LCD_SetTextColor(LCD_COLOR_BLACK); 
    LCD_DisplayStringLine(LCD_LINE_9, (uint8_t *)MESSAGE);

    /* Set the Icon display window */
    LCD_SetDisplayWindow(170, 190, 64, 64);

    /* Display the Thermometer icon */
    Storage_OpenReadFile(170, 190, "STFILES/ICON6.BMP");
    /* Disable LCD Window mode */
    LCD_WindowModeDisable();
    /* Set the Back Color */
    LCD_SetBackColor(LCD_COLOR_BLUE);
    /* Set the Text Color */
    LCD_SetTextColor(LCD_COLOR_GREEN);
    LCD_DisplayStringLine(LCD_LINE_0, "     Temperature    ");
      
    /* Wait until no key is pressed */
    while (Menu_ReadKey() != NOKEY)
    {}

    /* Configure the Temperature sensor device STLM75:
          - Thermostat mode Interrupt
          - Fault tolerance: 00
        */
    LM75_WriteConfReg(0x02);

    /* Configure the THYS and TOS inorder to use the SMbus alert interrupt */
    LM75_WriteReg(LM75_REG_THYS, TEMPERATURE_THYS << 8);  /*31°C*/
    LM75_WriteReg(LM75_REG_TOS, TEMPERATURE_TOS << 8);   /*32°C*/
    
    /* Re-configure and enable I2C1 error interrupt to have the higher priority */
    NVIC_InitStructure.NVIC_IRQChannel = I2C1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* Enables the I2C SMBus Alert feature */
    I2C_SMBusAlertCmd(LM75_I2C, ENABLE);
    
    I2C_ClearFlag(LM75_I2C, I2C_FLAG_ALERT);
    
    SMbusAlertOccurred = 0;
    
    /*!< Enable SMBus Alert interrupt */
    I2C_ITConfig(LM75_I2C, I2C_IT_ERRI, ENABLE);

    /* Wait until UP Joysticks is pressed */
   // while (pressedkey != UP)
    while (pressedkey != DOWN)
    {
      /* Get double of Temperature value */
      TempValue = LM75_ReadTemp();
      

      if (TempValue <= 256)
      {
        /* Positive temperature measured */
        TempCelsiusDisplay[7] = '+';
        /* Initialize the temperature sensor value*/
        TempValueCelsius = TempValue;
      }
      else
      {
        /* Negative temperature measured */
        TempCelsiusDisplay[7] = '-';
        /* Remove temperature value sign */
        TempValueCelsius = 0x200 - TempValue;
      }

      /* Calculate temperature digits in ÝC */
      if ((TempValueCelsius & 0x01) == 0x01)
      {
        TempCelsiusDisplay[12] = 0x05 + 0x30;
        TempFahrenheitDisplay[12] = 0x05 + 0x30;
      }
      else
      {
        TempCelsiusDisplay[12] = 0x00 + 0x30;
        TempFahrenheitDisplay[12] = 0x00 + 0x30;
      }

      TempValueCelsius >>= 1;

      TempCelsiusDisplay[8] = (TempValueCelsius / 100) + 0x30;
      TempCelsiusDisplay[9] = ((TempValueCelsius % 100) / 10) + 0x30;
      TempCelsiusDisplay[10] = ((TempValueCelsius % 100) % 10) + 0x30;

      if (TempValue > 256)
      {
        if (((9 * TempValueCelsius) / 5) <= 32)
        {
          /* Convert temperature ÝC to Fahrenheit */
          TempValueFahrenheit = abs (32 - ((9 * TempValueCelsius) / 5));

          /* Calculate temperature digits in ÝF */
          TempFahrenheitDisplay[8] = (TempValueFahrenheit / 100) + 0x30;
          TempFahrenheitDisplay[9] = ((TempValueFahrenheit % 100) / 10) + 0x30;
          TempFahrenheitDisplay[10] = ((TempValueFahrenheit % 100) % 10) + 0x30;
          /* Positive temperature measured */
          TempFahrenheitDisplay[7] = '+';
        }
        else
        {
          /* Convert temperature °C to Fahrenheit */
          TempValueFahrenheit = abs(((9 * TempValueCelsius) / 5) - 32);
          /* Calculate temperature digits in °F */
          TempFahrenheitDisplay[8] = (TempValueFahrenheit / 100) + 0x30;
          TempFahrenheitDisplay[9] = ((TempValueFahrenheit % 100) / 10) + 0x30;
          TempFahrenheitDisplay[10] = ((TempValueFahrenheit % 100) % 10) + 0x30;

          /* Negative temperature measured */
          TempFahrenheitDisplay[7] = '-';
        }
      }
      else
      {
        /* Convert temperature °C to Fahrenheit */
        TempValueFahrenheit = ((9 * TempValueCelsius) / 5) + 32;

        /* Calculate temperature digits in °F */
        TempFahrenheitDisplay[8] = (TempValueFahrenheit / 100) + 0x30;
        TempFahrenheitDisplay[9] = ((TempValueFahrenheit % 100) / 10) + 0x30;
        TempFahrenheitDisplay[10] = ((TempValueFahrenheit % 100) % 10) + 0x30;

        /* Positive temperature measured */
        TempFahrenheitDisplay[7] = '+';
      }

      /* Check if the LCD is HX8347D Controller */
      if(LCDType == LCD_HX8347D)
      {
        /* Display Fahrenheit value on LCD */
        for (i = 0; i < 20; i++)
        {        
          LCD_DisplayChar(LCD_LINE_7, (16 * i), TempCelsiusDisplay[i]);
          
          LCD_DisplayChar(LCD_LINE_8, (16 * i), TempFahrenheitDisplay[i]);
        }
      }
      else
      {
        /* Display Fahrenheit value on LCD */
        for (i = 0; i < 20; i++)
        {        
          LCD_DisplayChar(LCD_LINE_7, (319 - (16 * i)), TempCelsiusDisplay[i]);
          
          LCD_DisplayChar(LCD_LINE_8, (319 - (16 * i)), TempFahrenheitDisplay[i]);
        }
      }

      if (SMbusAlertOccurred == 1)
      {
        /* Set the Back Color */
        LCD_SetBackColor(LCD_COLOR_BLUE);
        /* Set the Text Color */
        LCD_SetTextColor(LCD_COLOR_RED);
        LCD_DisplayStringLine(LCD_LINE_2, "Warning: Temp exceed");
        LCD_DisplayStringLine(LCD_LINE_3, "        32 C        ");
      }
      if (SMbusAlertOccurred == 2)
      {
        /* Set the Back Color */
        LCD_SetBackColor(LCD_COLOR_WHITE);
        /* Set the Text Color */
        LCD_SetTextColor(LCD_COLOR_WHITE);
        LCD_ClearLine(LCD_LINE_2);
        LCD_ClearLine(LCD_LINE_3);
        SMbusAlertOccurred = 0;
        /* Set the Back Color */
        LCD_SetBackColor(LCD_COLOR_BLUE);
        /* Set the Text Color */
        LCD_SetTextColor(LCD_COLOR_GREEN);
      }
      pressedkey = Menu_ReadKey();
    }
  }
  else
  {
    LCD_Clear(LCD_COLOR_WHITE);
    LCD_DisplayStringLine(LCD_LINE_2, " No TSENSOR Present ");
    LCD_DisplayStringLine(LCD_LINE_3, "Exit:  Push JoyStick");
    
  }
  
  /* Re-configure and enable I2C1 error interrupt to have the higher priority */
  NVIC_InitStructure.NVIC_IRQChannel = I2C1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  LCD_Clear(LCD_COLOR_WHITE);

  /* Display menu */
  Menu_DisplayMenu();

  /* Enable the JoyStick interrupt */
  Demo_IntExtOnOffCmd(ENABLE);
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
