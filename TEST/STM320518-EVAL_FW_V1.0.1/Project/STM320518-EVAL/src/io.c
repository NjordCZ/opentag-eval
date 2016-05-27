/**
  ******************************************************************************
  * @file    io.c
  * @author  njord
  * @version V1.0.0
  * @date    26.10.2015
  * @brief   This file provides IO firmware functions, button, led, etc.
  *   
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/** @addtogroup Lasertag
  * @{
  */


/** @defgroup IO
  * @{
  */

// input init, buton, switch, etc... 
void Input_Init(void)
{
  /* Enable GPIOA, GPIOB and GPIOC clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | \
                        RCC_AHBPeriph_GPIOF, ENABLE);
  
  
  
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the BUTTON Clock */
  
  /* GPIOA clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* GPIOB clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  
  //RCC_AHBPeriphClockCmd(TRIGGER_BUTTON_GPIO_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Configure Button pin as input - trigger*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = TRIGGER_BUTTON_PIN;
  GPIO_Init(TRIGGER_BUTTON_GPIO_PORT, &GPIO_InitStructure);
  
  /* Configure Button pin as input - reloading*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = RELOADING_BUTTON_PIN;
  GPIO_Init(RELOADING_BUTTON_GPIO_PORT, &GPIO_InitStructure);
  
  /* Configure switch pin as input fire mode 1*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = MODE1_BUTTON_PIN;
  GPIO_Init(MODE1_BUTTON_GPIO_PORT, &GPIO_InitStructure); 
  
  /* Configure switch pin as input fire mode 2*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = MODE2_BUTTON_PIN;
  GPIO_Init(MODE2_BUTTON_GPIO_PORT, &GPIO_InitStructure); 
  
  

  
  /* TRIGGER Connect Button EXTI Line to Button GPIO Pin */
  SYSCFG_EXTILineConfig(TRIGGER_BUTTON_EXTI_PORT_SOURCE, TRIGGER_BUTTON_EXTI_PIN_SOURCE);
  
  /* RELOADING Connect Button EXTI Line to Button GPIO Pin */
  SYSCFG_EXTILineConfig(RELOADING_BUTTON_EXTI_PORT_SOURCE, RELOADING_BUTTON_EXTI_PIN_SOURCE);

  /* TRIGGER Configure Button EXTI line */
  EXTI_InitStructure.EXTI_Line = TRIGGER_BUTTON_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* RELOADING Configure Button EXTI line */
  EXTI_InitStructure.EXTI_Line = RELOADING_BUTTON_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  
  
  /* TRIGGER Enable and set Button EXTI Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TRIGGER_BUTTON_EXTI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x03;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
  NVIC_EnableIRQ(TRIGGER_BUTTON_EXTI_IRQn);
  
  
  /* RELOADING Enable and set Button EXTI Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RELOADING_BUTTON_EXTI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x03;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
  NVIC_EnableIRQ(RELOADING_BUTTON_EXTI_IRQn);
  
}

// output init, led, etc...
void Output_Init(void)
{
  
  /* Enable GPIOA, GPIOB and GPIOC clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | \
                        RCC_AHBPeriph_GPIOF, ENABLE);
  
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO_LED Clock */
  //RCC_AHBPeriphClockCmd(GPIO_CLK[Led], ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = LED_FIRE_PIN | LED_GREEN_PIN | LED_BLUE_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LED_FIRE_GPIO_PORT, &GPIO_InitStructure);
  GPIO_Init(LED_GREEN_GPIO_PORT, &GPIO_InitStructure);
  GPIO_Init(LED_BLUE_GPIO_PORT, &GPIO_InitStructure);
  LED_FIRE_GPIO_PORT->BSRR = LED_FIRE_PIN;
  LED_GREEN_GPIO_PORT->BSRR = LED_GREEN_PIN;
  LED_BLUE_GPIO_PORT->BSRR = LED_BLUE_PIN;

}


/**
  * @brief  Toggles the selected output.
  * @param  GPIOx: where x can be (A, B, C, D or F) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bits to be written.
  * @note   This parameter can be GPIO_Pin_x where x can be:(0..15) for GPIOA, 
  *         GPIOB or GPIOC,(0..2) for GPIOD and(0..3) for GPIOF.
  * @retval None
  */
void Output_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  GPIOx->ODR ^= GPIO_Pin;
}







/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE**************************************/
