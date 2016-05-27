/**
  ******************************************************************************
  * @file    waverecord.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file contains all the functions prototypes for the wave recorder
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
#ifndef __WAVERECORD_H
#define __WAVERECORD_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/** @addtogroup STM320518_EVAL_Demo
  * @{
  */

/** @addtogroup WAVERECORD
  * @{
  */

/** @defgroup WAVERECORD_Exported_Types
  * @{
  */

/**
  * @}
  */

/** @defgroup WAVEPLAYER_Exported_Constants
  * @{
  */
/**
  * @}
  */

/** @defgroup WAVEPLAYER_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup WAVEPLAYER_Exported_functions
  * @{
  */
void Menu_RecordedWavePlayFunc(void);  
void Menu_WaveRecorderFunc(void);

uint8_t PlayWaveRecorded_Start(void);
void WaveRec_HeaderInit(void);
void WaveRec_Recording(void);
void WaveRec_Start(void);
void WaveRec_Init(void);

#ifdef __cplusplus
}
#endif

#endif /*__WAVEPLAYER_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
  
/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE******/
