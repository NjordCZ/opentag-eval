/**
  ******************************************************************************
  * @file    applications.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file contains all the functions prototypes for the application
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
#ifndef __APPLICATIONS_H
#define __APPLICATIONS_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/** @addtogroup STM320518_EVAL_Demo
  * @{
  */

/** @addtogroup APPLICATIONS
  * @{
  */

/** @defgroup APPLICATIONS_Exported_Types
  * @{
  */
typedef struct{
  uint8_t Tab[12];
}Table_TypeDef;

/**
  * @}
  */

/** @defgroup APPLICATIONS_Exported_Constants
  * @{
  */
/**
  * @}
  */

/** @defgroup APPLICATIONS_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup APPLICATIONS_Exported_Functions
  * @{
  */

void App_StopWatch_Start(void);
void App_LapTimer_Start(void);
void App_LightDependentResistor_Start(void);
void RTC_Time_Display(uint8_t Line, __IO uint16_t Color_x, Table_TypeDef Table);
Table_TypeDef RTC_Get_Time(uint32_t Second_fraction , RTC_TimeTypeDef* RTC_TimeStructure);
void RTC_Time_InitDisplay(void);
#ifdef __cplusplus
}
#endif

#endif /* __APPLICATIONS_H */
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
