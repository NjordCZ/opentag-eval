/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm320518_eval.h"
//#include "stm320518_eval_lcd.h"
#include "stm320518_eval_spi_sd.h"
// #include "stm320518_eval_i2c_tsensor.h"
// #include "stm320518_eval_cec.h"
// #include "stm320518_eval_i2c_ee.h"
// #include "cec_display.h"
#include "lowpower.h"
#include "waveplayer.h"
#include "time_date.h"
#include "bluetooth.h"
  
#include "i2c_ssd1306.h"
#include "lcd_fonts.h"

//#include "ir_decode.h"
//#include "IR_decode.h"
//#include "IR_encode.h"
//#include "i2c.h"
#include "io.h"
//#include "thermometer.h"
//#include "rf_eeprom.h"
//#include "about.h"
//#include "help.h"
#include "ir_lasertag.h"
#include "lasertag.h"
#include "storage.h"
#include "ff.h"
#include <stdio.h>
#include <stdlib.h>
#include "stm32f0xx_it.h"
#include <string.h>
#include "HD44780LIB.h"

  


/** @addtogroup STM320518_EVAL_Demo
  * @{
  */

/** @addtogroup MAIN
  * @{
  */

/** @defgroup MAIN_Exported_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup MAIN_Exported_Constants
  * @{
  */
#define MAX_FILES              4  

#define DEFINE_0    "   STM320518-EVAL   " 
#define DEFINE_1    "   Demonstration    "
#define DEFINE_2    "   Version 1.0.1    "
#define DEFINE_3    "    29-May-2012     "
#define DEFINE_4    "MCD Application Team"
#define DEFINE_5    "   COPYRIGHT 2012   "
#define DEFINE_6    " STMicroelectronics "
  
#define DEFINE_7     "   An additional    "
#define DEFINE_8     "   application      "
#define DEFINE_9     " is available for   "  
#define DEFINE_10    "   download from    "  
#define DEFINE_11    "    ST internet     "
  

  
  
  
  
  
/* Uncomment the line below according to the STM320518-EVAL Demo configuration 
   used in your application. 
  */
#if !defined (STM320518_EVAL_DEMO_CONFIG1) && !defined (STM320518_EVAL_DEMO_CONFIG2)
//#define STM320518_EVAL_DEMO_CONFIG1 
//#define STM320518_EVAL_DEMO_CONFIG2
#endif
  
#if !defined (STM320518_EVAL_DEMO_CONFIG1) && !defined (STM320518_EVAL_DEMO_CONFIG2)
 #error "Please select first the STM320518-EVAL demo configuration used in your application "
#endif 
  
/* The following lines defines the Sub-Demos used in the STM320518-EVAL Demo Config 1 */  
#ifdef  STM320518_EVAL_DEMO_CONFIG1
 #define DEMO_CONFIG_PART     "   DEMO CONFIG 1    "
 #define DEMO_CONFIG_PART2    "      CONFIG 1      "

 #if defined   (__GNUC__)        /* GNU Compiler */
  /* For TrueSTUDIO toolchain, the number of application is limited due to code size
     constraint */
  #define NUMBER_OF_ICONS         7
 #else
  #define NUMBER_OF_ICONS         8
  #define HELP_SUBDEMO  
 #endif  /* __GNUC__ */
  
 #define CALENDAR_SUBDEMO  
 #define LOWPOWERMODE_SUBDEMO
 #define RFEEPROM_SUBDEMO
 #define APPLICATION_SUBDEMO
 #define WAVERECORD_SUBDEMO 
 #define THERMOMETER_SUBDEMO  
#endif
 /* The following lines defines the Sub-Demos used in the STM320518-EVAL Demo Config 2 */  
 #ifdef  STM320518_EVAL_DEMO_CONFIG2

 #define DEMO_CONFIG_PART    "   DEMO CONFIG 2    "
 #define DEMO_CONFIG_PART2   "      CONFIG 2      "
  
 #define NUMBER_OF_ICONS       6  
 
 #define IMAGEVIEWER_SUBDEMO 
 #define AUDIO_SUBDEMO
 #define HDMICEC_SUBDEMO
 #define INFRALED_SUBDEMO
 #define HELP_SUBDEMO
#endif /* STM320518_EVAL_DEMO_CONFIG1 */


/* The following lines defines the Sub-Demos used in the STM320518-EVAL Demo Config 1 */  
#ifdef  STM320518_EVAL_REVB
  #define BOARD_REV    "       REV B        "
#else
  #define BOARD_REV    "       REV A        "
#endif /* STM320518_EVAL_REVB */

/* Used to know the version of the LCD used on the board */
#define LCD_HX8347D        0x0047
  
/**
  * @}
  */

/** @defgroup MAIN_Exported_Macros
  * @{
  */
  
/**
  * @}
  */

/** @defgroup MAIN_Exported_Functions
  * @{
  */
void Error(char* error); 
uint32_t getTrueRandomNumber(void);  
void Demo_Init(void);
void Demo_Interrupt_Init(void);
void SysTick_Init(void);
void Demo_EVAL_Init(void);
void Demo_IntExtOnOffCmd(FunctionalState NewState);
void Demo_Delay(__IO uint32_t nCount);
uint32_t Demo_DelayJoyStick(__IO uint32_t nTime);
void Demo_DecrementTimingDelay(void);
void Delay_Decrement(__IO uint32_t nCount);
void Demo_LedShow(FunctionalState NewState);
uint32_t Demo_Get_LedShowStatus(void);
void Demo_CheckBitmapFilesStatus(void);
void Set_DOWNStatus(void);
void Set_UPStatus(void);
void Set_SELStatus(void);
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
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
