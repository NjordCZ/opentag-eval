/**
  ******************************************************************************
  * @file bluetooth.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file includes the about driver for the STM320518-EVAL demonstration.
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"


/** @addtogroup lasertag
  * @{
  */

/** @defgroup bluetooth
  * @brief BT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
DMA_InitTypeDef  DMA_InitStructure;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t TxBuffer[255] = "USART DMA Example: Communication between two USART using DMA";
uint8_t RxBuffer [255];

uint8_t Buffer_TX_Size = 255; /* {Number of byte to transmit} */
uint8_t Buffer_RX_Size = 255; /* {Number of byte to receive } */

__IO uint32_t TimeOut = 0x0;   
__IO JOYState_TypeDef PressedButton = JOY_NONE;

/* Private function prototypes -----------------------------------------------*/
static void TimeOut_UserCallback(void);
static void USART_Config(void);
static void SysTickConfig(void);
static JOYState_TypeDef Read_Joystick(void);
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
static void Fill_Buffer(uint8_t *pBuffer, uint16_t BufferLength);




/**
* @brief  Init BT.
* @param  None
* @retval None
*/

void Bluetooth_Init(void)
{
  USART_Config();
}




/**
* @brief  Configures the USART Peripheral.
* @param  None
* @retval None
*/
static void USART_Config(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(USARTx_TX_GPIO_CLK | USARTx_RX_GPIO_CLK, ENABLE);
  
  /* Enable USART clock */
  USARTx_APBPERIPHCLOCK(USARTx_CLK, ENABLE);
  
  /* Enable the DMA periph */
  RCC_AHBPeriphClockCmd(DMAx_CLK, ENABLE);
  
  /* Connect PXx to USARTx_Tx */
  GPIO_PinAFConfig(USARTx_TX_GPIO_PORT, USARTx_TX_SOURCE, USARTx_TX_AF);
  
  /* Connect PXx to USARTx_Rx */
  GPIO_PinAFConfig(USARTx_RX_GPIO_PORT, USARTx_RX_SOURCE, USARTx_RX_AF);
  
  /* Configure USART Tx and Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
  GPIO_InitStructure.GPIO_Pin = USARTx_TX_PIN;
  GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;
  GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStructure);
  
  /* USARTx configuration ----------------------------------------------------*/
  /* USARTx configured as follow:
  - BaudRate = 230400 baud  
  - Word Length = 8 Bits
  - one Stop Bit
  - No parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 9600; //230400;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* When using Parity the word length must be configured to 9 bits */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USARTx, &USART_InitStructure);
  
  
  
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* DMA Configuration -------------------------------------------------------*/
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    
  /* Enable USART */
  USART_Cmd(USARTx, ENABLE);
  
  
  
      /* DMA channel Tx of USART Configuration */
      DMA_DeInit(USARTx_TX_DMA_CHANNEL);
      DMA_InitStructure.DMA_PeripheralBaseAddr = USARTx_TDR_ADDRESS;
      DMA_InitStructure.DMA_BufferSize = Buffer_TX_Size;
      DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TxBuffer;
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
      DMA_InitStructure.DMA_Priority = DMA_Priority_High;
      DMA_Init(USARTx_TX_DMA_CHANNEL, &DMA_InitStructure);
      
      USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);
      
      /* Clear the TC bit in the SR register by writing 0 to it */
      USART_ClearFlag(USARTx, USART_FLAG_TC);
      
      /* Enable DMA1 USART Tx Channel */
      DMA_Cmd(USARTx_TX_DMA_CHANNEL, ENABLE);
      
      
      /* The transmitter After receive the ACK command it sends the defined data
         in his TxBuffer */      
      /* DMA channel Rx of USART Configuration */
      DMA_DeInit(USARTx_RX_DMA_CHANNEL);
      DMA_InitStructure.DMA_PeripheralBaseAddr = USARTx_RDR_ADDRESS;
      DMA_InitStructure.DMA_BufferSize = Buffer_RX_Size;
      DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) RxBuffer;
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
      DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
      DMA_Init(USARTx_RX_DMA_CHANNEL, &DMA_InitStructure);
      
      
      
      /* Enable the USART Rx DMA request */
      USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);
    
      /* Enable the DMA channel */
      DMA_Cmd(USARTx_RX_DMA_CHANNEL, ENABLE);
}
 
/**
* @brief  Fills buffer.
* @param  pBuffer: pointer on the Buffer to fill
* @param  BufferLength: size of the buffer to fill
* @retval None
*/
static void Fill_Buffer(uint8_t *pBuffer, uint16_t BufferLength)
{
  uint16_t index = 0;
  
  /* Put in global buffer same values */
  for (index = 0; index < BufferLength; index++ )
  {
    pBuffer[index] = 0x00;
  }
}

/**
  * @}
  */

/**
  * @}
  */

/***************************END OF FILE****/
