/**
  ******************************************************************************
  * @file    calendar.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file contains all the functions prototypes for the calendar 
  *          firmware driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CALENDAR_H
#define __CALENDAR_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/** @addtogroup STM320518_EVAL_Demo
  * @{
  */

/** @addtogroup CALENDAR
  * @{
  */

/** @defgroup CALENDAR_Exported_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup CALENDAR_Exported_Constants
  * @{
  */
/**
  * @}
  */

/** @defgroup CALENDAR_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup CALENDAR_Exported_Functions
  * @{
  */
uint8_t Calendar_ReadDigit(uint8_t ColBegin, uint8_t CountBegin, uint8_t ValueMax, uint8_t ValueMin);
void Calendar_Init(void);
void Calendar_TimeRegulate(void);
void Calendar_TimeAdjust(void);
void Calendar_TimeShow(void);
void Calendar_TimeDisplay(void);
void Calendar_DateRegulate(void);
void Calendar_DateAdjust(void);
void Calendar_DateDisplay(uint16_t nYear, uint8_t nMonth, uint8_t nDay);
void Calendar_DateShow(void);
void Calendar_DateUpdate(void);
void Calendar_AlarmRegulate_A(void);
void Calendar_AlarmAdjust_A(void);
void Calendar_AlarmPreAdjust_A(void);
void Calendar_AlarmDisplay_A(void);
void Calendar_AlarmShow_A(void);
void Calendar_RTC_Configuration(void);

#ifdef __cplusplus
}
#endif

#endif /* __CALENDAR_H */
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
