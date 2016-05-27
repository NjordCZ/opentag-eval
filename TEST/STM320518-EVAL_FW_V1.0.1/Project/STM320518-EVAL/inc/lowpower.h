/**
  ******************************************************************************
  * @file    lowpower.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file contains all the functions prototypes for the low power
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
#ifndef __LOWPOWER_H
#define __LOWPOWER_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/** @addtogroup STM320518_EVAL_Demo
  * @{
  */

/** @addtogroup LOWPOWER
  * @{
  */

/** @defgroup LOWPOWER_Exported_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup LOWPOWER_Exported_Constants
  * @{
  */
/**
  * @}
  */

/** @defgroup LOWPOWER_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup LOWPOWER_Exported_Functions
  * @{
  */
void LowPower_Init(void);
void LowPower_SYSCLKConfig_STOP(void);
void LowPower_EnterSTOPMode_EXTI(void);
void LowPower_EnterSTOPMode_RTCAlarm(void);
void LowPower_EnterSTANDBYMode_WAKEUP_1(void);
void LowPower_EnterSTANDBYMode_RTCAlarm(void);
void LowPower_Set_STOPModeStatus(void);
void LowPower_Reset_STOPModeStatus(void);
void LowPower_SaveGPIOConfig(void);
void LowPower_RestoreGPIOConfig(void);

#ifdef __cplusplus
}
#endif

#endif /* __LOWPOWER_H */
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
