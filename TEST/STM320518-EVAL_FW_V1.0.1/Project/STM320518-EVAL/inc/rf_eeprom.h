/**
  ******************************************************************************
  * @file    rf_eeprom.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file contains all the functions prototypes for the RF_EEPROM 
  *          navigation firmware driver.
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
#ifndef __RF_EEPROM_H
#define __RF_EEPROM_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/** @addtogroup STM320518_EVAL_Demo
  * @{
  */

/** @addtogroup RF_EEPROM
  * @{
  */

/** @defgroup RF_EEPROM_Exported_Types
  * @{
  */

/**
  * @}
  */

/** @defgroup RF_EEPROM_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @defgroup RF_EEPROM_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup RF_EEPROM_Exported_Functions
  * @{
  */
  
void RF_EEprom_ESLFunc(void);
void RF_EEprom_DataLogger_Init(void);
void RF_EEprom_DataLogger(void);

#ifdef __cplusplus
}
#endif

#endif /* __RF_EEPROM_H */

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
