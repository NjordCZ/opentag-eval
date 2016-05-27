/**
  ******************************************************************************
  * @file    about.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file includes the about driver for the STM320518-EVAL demonstration.
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

/** @defgroup ABOUT
  * @brief    This file includes the about driver for the STM320518-EVAL
  *           demonstration.
  * @{
  */

/** @defgroup ABOUT_Private_Types
  * @{
  */
/**
  * @}
  */


/** @defgroup ABOUT_Private_Defines
  * @{
  */

/**
  * @}
  */

/** @defgroup ABOUT_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup ABOUT_Private_Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup ABOUT_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup ABOUT_Private_Functions
  * @{
  */

/**
  * @brief  Display the About menu.
  * @param  None
  * @retval None
  */
void Menu_AboutFunc(void)
{
  LCD_Clear(LCD_COLOR_WHITE);
  
  Demo_IntExtOnOffCmd(DISABLE);
  
  while (Menu_ReadKey() != NOKEY)
  {}
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_ClearLine(LCD_LINE_0);
  LCD_DisplayStringLine(LCD_LINE_1, DEFINE_0);
  LCD_DisplayStringLine(LCD_LINE_2, DEFINE_1);
  LCD_DisplayStringLine(LCD_LINE_3, DEMO_CONFIG_PART2);
  LCD_DisplayStringLine(LCD_LINE_4, DEFINE_2);
  LCD_DisplayStringLine(LCD_LINE_5, DEFINE_3);
  LCD_ClearLine(LCD_LINE_6);
  LCD_DisplayStringLine(LCD_LINE_7, DEFINE_4);
  LCD_DisplayStringLine(LCD_LINE_8, DEFINE_5);
  LCD_DisplayStringLine(LCD_LINE_9, DEFINE_6);
  LCD_ClearLine(LCD_LINE_9);
  while (Menu_ReadKey() == NOKEY)
  {}
  LCD_Clear(LCD_COLOR_WHITE);
  
  LCD_DisplayStringLine(LCD_LINE_2, DEFINE_7);
  LCD_DisplayStringLine(LCD_LINE_3, DEFINE_8);

#if defined (STM320518_EVAL_DEMO_CONFIG1)
  LCD_DisplayStringLine(LCD_LINE_4, "    DEMO CONFIG 2    ");
#else
  LCD_DisplayStringLine(LCD_LINE_4, "    DEMO CONFIG 1    ");
#endif /*STM320518_EVAL_DEMO_CONFIG1*/
  
  LCD_DisplayStringLine(LCD_LINE_5, DEFINE_9);
  LCD_DisplayStringLine(LCD_LINE_6, DEFINE_10);
  LCD_DisplayStringLine(LCD_LINE_7, DEFINE_11);
  
  while (Menu_ReadKey() == NOKEY)
  {}
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
