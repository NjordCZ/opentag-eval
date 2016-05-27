/**
  ******************************************************************************
  * @file    help.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file includes the help driver for the STM320518-EVAL demonstration.
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

/** @defgroup HELP
  * @brief    This file includes the help driver for the STM320518-EVAL
  *           demonstration.
  * @{
  */

/** @defgroup HELP_Private_Types
  * @{
  */
/**
  * @}
  */


/** @defgroup HELP_Private_Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup HELP_Private_Macros
  * @{
  */


/**
  * @}
  */

/** @defgroup HELP_Private_Variables
  * @{
  */
__IO uint8_t JumperConfIndex = 0;

/**
  * @}
  */

/** @defgroup HELP_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup HELP_Private_Functions
  * @{
  */

/**
  * @brief  Display the push buttons configuration menu.
  * @param  None
  * @retval None
  */
void Menu_MenuNavigationFunc(void)
{
  Demo_IntExtOnOffCmd(DISABLE);
  
  LCD_SetDisplayWindow(239, 0x13F, 240, 320);
  Storage_OpenReadFile(239, 319, "STFILES/HELP.BMP");

  while (Menu_ReadKey() == NOKEY)
  {}

  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_0, "UP, DOWN, RIGHT and ");
  LCD_DisplayStringLine(LCD_LINE_1, "LEFT push-buttons   ");
  LCD_DisplayStringLine(LCD_LINE_2, "perform circular    ");
  LCD_DisplayStringLine(LCD_LINE_3, "navigation in the   ");
  LCD_DisplayStringLine(LCD_LINE_4, "main menu, current  ");
  LCD_DisplayStringLine(LCD_LINE_5, "menu items. SEL     ");
  LCD_DisplayStringLine(LCD_LINE_6, "push-button selects ");
  LCD_DisplayStringLine(LCD_LINE_7, "the current item. UP");
  LCD_DisplayStringLine(LCD_LINE_8, "and DOWN perform    ");
  LCD_DisplayStringLine(LCD_LINE_9, "vertical navigation.");

  while (Menu_ReadKey() == NOKEY)
  {}
  LCD_Clear(LCD_COLOR_WHITE);
  Menu_DisplayMenu();
  Demo_IntExtOnOffCmd(ENABLE);
}

/**
  * @brief  Display the needed Jumpers configuration menu.
  * @param  None
  * @retval None
  */
void Menu_JumpConfFunc(void)
{
  uint8_t pressedkey = 0;
  
  LCD_Clear(LCD_COLOR_WHITE);
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLACK);
  LCD_DisplayStringLine(LCD_LINE_0, " The needed Jumpers ");
  LCD_DisplayStringLine(LCD_LINE_1, "    Configuration:  ");
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);

  
  while (pressedkey != UP)
  {
    if (JumperConfIndex == 0x00)
    { 
      LCD_DisplayStringLine(LCD_LINE_3, "Use RIGHT and LEFT  ");
      LCD_DisplayStringLine(LCD_LINE_4, "to go to the        ");
      LCD_DisplayStringLine(LCD_LINE_5, "next/previous slide ");
      LCD_DisplayStringLine(LCD_LINE_6, "and UP to exit      ");
    }  
    
    if (JumperConfIndex == 0x01)
    {
      LCD_DisplayStringLine(LCD_LINE_2, "- VDD Adjust:       ");
      LCD_DisplayStringLine(LCD_LINE_3, "JP9 fitted pos VDD  ");
      LCD_DisplayStringLine(LCD_LINE_4, "- VDD Voltage:      ");
      LCD_DisplayStringLine(LCD_LINE_5, "JP10 fitted pos 3.3V");
      LCD_DisplayStringLine(LCD_LINE_6, "- VDD Analog:       ");
      LCD_DisplayStringLine(LCD_LINE_7, "JP11 fitted         ");
      LCD_DisplayStringLine(LCD_LINE_8, "- VDD VBat:         ");
      LCD_DisplayStringLine(LCD_LINE_9, "JP12 fitted pos VDD ");
    }
    
    if (JumperConfIndex == 0x02)
    {
      LCD_DisplayStringLine(LCD_LINE_2, "- VDD MCU:          ");
      LCD_DisplayStringLine(LCD_LINE_3, "JP7 fitted          ");
      LCD_DisplayStringLine(LCD_LINE_4, "- Audio Output:     ");
      LCD_DisplayStringLine(LCD_LINE_5, "JP13 fitted         ");
      LCD_DisplayStringLine(LCD_LINE_6, "- JP5, JP6 and CN8  ");
      LCD_DisplayStringLine(LCD_LINE_7, "are not fitted.     ");
      LCD_DisplayStringLine(LCD_LINE_8, "- JP1, JP2, JP3 and ");
      LCD_DisplayStringLine(LCD_LINE_9, "JP4 are not used.   ");
    }
    if (JumperConfIndex == 0x03)
    {
      LCD_DisplayStringLine(LCD_LINE_2, "When running the LDR");
      LCD_DisplayStringLine(LCD_LINE_3, "demo the JP13: Audio ");
      LCD_DisplayStringLine(LCD_LINE_4, "output jumper should");
      LCD_DisplayStringLine(LCD_LINE_5, "be removed to avoid ");
      LCD_DisplayStringLine(LCD_LINE_6, "the noise on speaker");
      LCD_DisplayStringLine(LCD_LINE_7, "                    ");
      LCD_DisplayStringLine(LCD_LINE_8, "                    ");
      LCD_DisplayStringLine(LCD_LINE_9, "                    ");
    }
    
    pressedkey = Menu_ReadKey();
    while (pressedkey == NOKEY)
    {
      pressedkey = Menu_ReadKey();
    }
    if (pressedkey == RIGHT)
    {
      if (JumperConfIndex == 0x03)
      {
        JumperConfIndex = 0x01;
      }
      else
      {
        JumperConfIndex++;
      }
    }
    if (pressedkey == LEFT)
    {
      if (JumperConfIndex == 0x01)
      {
        JumperConfIndex = 0x03;
      }
      else
      {
        JumperConfIndex--;
      }
    }
  }
  JumperConfIndex = 0x00;
  
  LCD_Clear(LCD_COLOR_WHITE);
  Menu_DisplayMenu();
  
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
