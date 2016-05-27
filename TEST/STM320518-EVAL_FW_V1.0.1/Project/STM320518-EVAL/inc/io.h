/**
  ******************************************************************************
  * @file    io.c
  * @author  njord
  * @version V1.0.0
  * @date    26.10.2015
  * @brief   This file provides IO firmware, button, led, etc.
  *   
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IO_H
#define __IO_H

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @brief Trigger push-button - gun fire
 */
#define TRIGGER_BUTTON_PIN                   GPIO_Pin_0
#define TRIGGER_BUTTON_GPIO_PORT             GPIOA
#define TRIGGER_BUTTON_GPIO_CLK              RCC_AHBPeriph_GPIOA
#define TRIGGER_BUTTON_EXTI_LINE             EXTI_Line0
#define TRIGGER_BUTTON_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOA
#define TRIGGER_BUTTON_EXTI_PIN_SOURCE       EXTI_PinSource0
#define TRIGGER_BUTTON_EXTI_IRQn             EXTI0_1_IRQn 

/**
  * @}
  */    
   
/**
 * @brief Reloading push-button - reloading gun
 */
#define RELOADING_BUTTON_PIN                   GPIO_Pin_1
#define RELOADING_BUTTON_GPIO_PORT             GPIOA
#define RELOADING_BUTTON_GPIO_CLK              RCC_AHBPeriph_GPIOA
#define RELOADING_BUTTON_EXTI_LINE             EXTI_Line1
#define RELOADING_BUTTON_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOA
#define RELOADING_BUTTON_EXTI_PIN_SOURCE       EXTI_PinSource1
#define RELOADING_BUTTON_EXTI_IRQn             EXTI0_1_IRQn 

/**
  * @}
  */    
    
    
/**
   * @brief fire mode switch 1 - semi/auto
 */
#define MODE1_BUTTON_PIN                   GPIO_Pin_4
#define MODE1_BUTTON_GPIO_PORT             GPIOB
#define MODE1_BUTTON_GPIO_CLK              RCC_AHBPeriph_GPIOB
//#define MODE1_BUTTON_EXTI_LINE             EXTI_Line0
//#define MODE1_BUTTON_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOA
//#define MODE1_BUTTON_EXTI_PIN_SOURCE       EXTI_PinSource0
//#define MODE1_BUTTON_EXTI_IRQn             EXTI0_1_IRQn 

/**
  * @}
  */       

 /**
   * @brief fire mode switch 2 - fire/safe
 */
#define MODE2_BUTTON_PIN                   GPIO_Pin_5
#define MODE2_BUTTON_GPIO_PORT             GPIOB
#define MODE2_BUTTON_GPIO_CLK              RCC_AHBPeriph_GPIOB
//#define MODE2_BUTTON_EXTI_LINE             EXTI_Line0
//#define MODE2_BUTTON_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOA
//#define MODE2_BUTTON_EXTI_PIN_SOURCE       EXTI_PinSource0
//#define MODE2_BUTTON_EXTI_IRQn             EXTI0_1_IRQn 

/**
  * @}
  */         
   
   
/**
 * @brief LED - output
 */  
   
// FIRE or HIT LED   
#define LED_FIRE_PIN                         GPIO_Pin_7
#define LED_FIRE_GPIO_PORT                   GPIOC
#define LED_FIRE_GPIO_CLK                    RCC_AHBPeriph_GPIOC

// Green team led or status led  
#define LED_GREEN_PIN                        GPIO_Pin_9
#define LED_GREEN_GPIO_PORT                  GPIOC
#define LED_GREEN_GPIO_CLK                   RCC_AHBPeriph_GPIOC

// Blue team led or status    
#define LED_BLUE_PIN                         GPIO_Pin_8
#define LED_BLUE_GPIO_PORT                   GPIOC
#define LED_BLUE_GPIO_CLK                    RCC_AHBPeriph_GPIOC   
   
/**
  * @}
  */    
   
   
   
void Input_Init(void);  
void Output_Init(void);
void Output_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
   
#ifdef __cplusplus
}
#endif

#endif  /*__IO_H */
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
