/**
  ******************************************************************************
  * @file    ir_lasertag.h
  * @author  njord
  * @version V1.0.0
  * @date    26.10.2015
  * @brief   This file contains all the functions prototypes for the lasertag IR 
  *          firmware library.
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IR_LASERTAG_H
#define __IR_LASERTAG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
   


   

/** @defgroup IR_DECODE_Exported_Types
  * @{
  */   
   
typedef enum { NO = 0, YES = !NO} StatusYesOrNo;
/**
  * @}
  */

/** @defgroup IR_DECODE_Exported_Constants
  * @{
  */
   
#define IR_TIM                 TIM2                     /*!< Timer used for IR decoding */
#define TIM_PRESCALER          47                       /* !< TIM prescaler */
#define IR_TIM_CLK             RCC_APB1Periph_TIM2      /*!< Clock of the used timer */
#define IR_TIM_IRQn            TIM2_IRQn                /*!< IR TIM IRQ */
#define IR_TIM_Channel         TIM_Channel_2            /*!< IR TIM Channel */

#define IR_GPIO_PORT           GPIOB                    /*!< Port which IR output is connected */
#define IR_GPIO_PORT_CLK       RCC_AHBPeriph_GPIOB      /*!< IR pin GPIO Clock Port */
#define IR_GPIO_PIN            GPIO_Pin_3               /*!< Pin which IR is connected */
#define IR_GPIO_SOURCE         GPIO_PinSource3
   


/**
  * @}
  */   
   


/**
  * @}
  */

   


/** @defgroup IR_DECODE_Exported_Types
  * @{
  */



/** 
  * @brief  IR packet structure  
  */
typedef struct
{
  uint8_t count;  /*!< Bit count */
  uint8_t status; /*!< Status */
  uint32_t data;  /*!< Data */
} IR_packet;

/**
  * @}
  */

/** @defgroup IR_DECODE_Exported_Constants
  * @{
  */


#define IR_TIME_OUT_US                4000 

#define IR_STATUS_HEADER              1 << 1
#define IR_STATUS_RX                  1 << 0
//#define INITIAL_STATUS                IR_STATUS_HEADER 

#define IR_BIT_ERROR                  0xFF
#define IR_HEADER_ERROR               0xFF
#define IR_HEADER_OK                  0x00

#define IR_BITS_COUNT                 31
#define IR_TOTAL_BITS_COUNT           31

#define IR_ICFILTER                   0xF  // 0x0-0xF  ir timer capture filter, ideal 0x6 - 0xF
    
#define IR_ONTIME_MIN_US              (600 - 200)
#define IR_ONTIME_MAX_US              (1200 + 200)

#define IR_HEADER_LOW_MIN_US          (2400 - 240)  // 2400
#define IR_HEADER_LOW_MAX_US          (2400 + 240)
#define IR_HEADER_WHOLE_MIN_US        (2400 + 600 - 300)
#define IR_HEADER_WHOLE_MAX_US        (2400 + 600 + 300)

#define IR_VALUE_STEP_US              600
#define IR_VALUE_MARGIN_US            200
#define IR_VALUE_00_US                1200

/**
  * @}
  */
  
/** @defgroup IR_DECODE_Exported_Macros
  * @{
  */
/**
  * @}
  */



/** @addtogroup IR_ENCODE_Exported_Functions
  * @{
  */
void IR_Encode_Start(uint32_t IR_Frame, uint8_t IR_NbBits); //odesila IR paket milestag 2
void IR_Encode_SignalGenerate(void); // generuje vystup na IR pinu
void IR_Encode_Init(void); // init hardware for IR encode





/** @defgroup IR_DECODE_Exported_Functions
  * @{
  */
//void Menu_SIRCDecode_Func(void);
//void IR_DeInit(void);
void IR_Decode_Init(void);
//void IR_Decode(IR_Frame_TypeDef *ir_frame);
void IR_ResetPacket(void);
void IR_DataSampling(__IO uint32_t lowPulseLength, __IO uint32_t wholePulseLength);
void IR_Timer_Overflow(void);




#ifdef __cplusplus
}
#endif

#endif  /*__IR_LASERTAG_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE****/    
