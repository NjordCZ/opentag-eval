/**
  ******************************************************************************
  * @file    bluetooth.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file contains all the functions prototypes for the about
  *          firmware driver.
  ******************************************************************************
  * @attention
  *
  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ABOUT_H
#define __ABOUT_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/** @addtogroup lasertag
  * @{
  */

/** @addtogroup bluetooth
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm320518_eval.h"


/* Exported typedef ----------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Exported define -----------------------------------------------------------*/

/* USER_TIMEOUT value for waiting loops. This timeout is just guarantee that the
   application will not remain stuck if the USART communication is corrupted. 
   You may modify this timeout value depending on CPU frequency and application
   conditions (interrupts routines, number of data to transfer, baudrate, CPU
   frequency...). */ 
#define USER_TIMEOUT                    ((uint32_t)0x64) /* Waiting 1s */

/* Communication boards USART Interface */

  #define USARTx_TDR_ADDRESS                0x40004428  //ADR+28
  #define USARTx_RDR_ADDRESS                0x40004424  //ADR+24

  #define USARTx_TX_DMA_CHANNEL            DMA1_Channel4
  #define USARTx_TX_DMA_FLAG_TC            DMA1_FLAG_TC4
  #define USARTx_TX_DMA_FLAG_GL            DMA1_FLAG_GL4
  #define USARTx_RX_DMA_CHANNEL            DMA1_Channel5
  #define USARTx_RX_DMA_FLAG_TC            DMA1_FLAG_TC5
  #define USARTx_RX_DMA_FLAG_GL            DMA1_FLAG_GL5


#define DMAx_CLK                         RCC_AHBPeriph_DMA1

/* USART Communication boards Interface */

  #define USARTx                           USART2
  #define USARTx_CLK                       RCC_APB1Periph_USART2
  #define USARTx_APBPERIPHCLOCK            RCC_APB1PeriphClockCmd
  #define USARTx_IRQn                      USART2_IRQn
  #define USARTx_IRQHandler                USART2_IRQHandler

  #define USARTx_TX_PIN                    GPIO_Pin_2
  #define USARTx_TX_GPIO_PORT              GPIOA
  #define USARTx_TX_GPIO_CLK               RCC_AHBPeriph_GPIOA
  #define USARTx_TX_SOURCE                 GPIO_PinSource2
  #define USARTx_TX_AF                     GPIO_AF_1

  #define USARTx_RX_PIN                    GPIO_Pin_3          
  #define USARTx_RX_GPIO_PORT              GPIOA            
  #define USARTx_RX_GPIO_CLK               RCC_AHBPeriph_GPIOA
  #define USARTx_RX_SOURCE                 GPIO_PinSource3
  #define USARTx_RX_AF                     GPIO_AF_1


#define TXBUFFERSIZE                     (countof(TxBuffer) - 1)
#define RXBUFFERSIZE                     TXBUFFERSIZE


#define CMD_ACK                          0x66 

/* Define numbers of bytes to transmit from TxBuffer */
#define CMD_RIGHT_SIZE                   0x01
#define CMD_LEFT_SIZE                    0x05
#define CMD_UP_SIZE                      0x14
#define CMD_DOWN_SIZE                    0x1E
#define CMD_SEL_SIZE                     TXBUFFERSIZE

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Bluetooth_Init(void);
  
  
  
  

#ifdef __cplusplus
}
#endif

#endif /* __ABOUT_H */
/**
  * @}
  */


/**
  * @}
  */

/***************************END OF FILE****/
