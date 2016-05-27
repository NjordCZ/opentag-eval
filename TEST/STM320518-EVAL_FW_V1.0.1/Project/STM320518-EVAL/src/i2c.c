/**
  ******************************************************************************
  * I2C
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/        
#include "main.h"
#include "i2c.h"

#include "stm32f0xx.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_i2c.h"
#include <stdio.h>


USART_InitTypeDef USART_InitStructure;


uint8_t sec = 0;


uint8_t ReadReg(uint8_t RegAdd)
{
  uint8_t mpu6050_BufferRX[2] ={0,0};
  uint8_t temp;
  uint32_t DataNum = 0;
  int mpu6050_Timeout = 0;

  /* Test on BUSY Flag */
#if 1
  mpu6050_Timeout = mpu6050_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_BUSY) != RESET)
  {
    if((mpu6050_Timeout--) == 0) return 0;
  }
#endif
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(I2C2, RTC_address, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

  /* Wait until TXIS flag is set */
  mpu6050_Timeout = mpu6050_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS) == RESET)
  {
    if((mpu6050_Timeout--) == 0) return 0;
  }

  /* Send Register address */
  I2C_SendData(I2C2, (uint8_t)RegAdd);

  /* Wait until TC flag is set */
  mpu6050_Timeout = mpu6050_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_TC) == RESET)
  {
    if((mpu6050_Timeout--) == 0) return 0;
  }

  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(I2C2, RTC_address, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

  /* Reset local variable */
  DataNum = 0;

  /* Wait until all data are received */
  while (DataNum != 1)
  {
    /* Wait until RXNE flag is set */
      mpu6050_Timeout = mpu6050_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(I2C2, I2C_ISR_RXNE) == RESET)
    {
      if((mpu6050_Timeout--) == 0) return 0;
    }

    /* Read data from RXDR */
    mpu6050_BufferRX[DataNum]= I2C_ReceiveData(I2C2);

    /* Update number of received data */
    DataNum++;
  }

  /* Wait until STOPF flag is set */
  mpu6050_Timeout = mpu6050_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_STOPF) == RESET)
  {
    if((mpu6050_Timeout--) == 0) return 0;
  }

  /* Clear STOPF flag */
  I2C_ClearFlag(I2C2, I2C_ICR_STOPCF);


  // !< Store LM75_I2C received data
  //tmp = (uint16_t)(LM75_BufferRX[0] << 8);
  //tmp |= LM75_BufferRX[0];
  temp = mpu6050_BufferRX[0];
  // return a Reg value
  return (uint8_t)temp;



}


uint8_t WRiteReg(uint8_t data,uint8_t RegAdd)
{
  uint8_t mpu6050_BufferRX[2] ={0,0};
  uint8_t temp;
  uint32_t DataNum = 0;
  int mpu6050_Timeout = 0;

  /* Test on BUSY Flag */

  mpu6050_Timeout = mpu6050_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_BUSY) != RESET)
  {
    if((mpu6050_Timeout--) == 0) return 0;
  }

  /* Send Start*/
  I2C_GenerateSTART(I2C2, ENABLE);



  /* Send Device address address of the DS1307*/
  I2C_SendData(I2C1, RTC_address);


  /* Send the address of register within device DS1307*/
I2C_SendData(I2C1, (uint8_t)RegAdd);
/* Wait until TC flag is set */
 mpu6050_Timeout = mpu6050_LONG_TIMEOUT;
 while(I2C_GetFlagStatus(I2C2, I2C_ISR_TC) == RESET)
 {
   if((mpu6050_Timeout--) == 0) return 0;
 }
/* Send the data to write in internal register*/

 I2C_SendData(I2C2, (uint8_t)data);
 /* Wait until TC flag is set */
  mpu6050_Timeout = mpu6050_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_TC) == RESET)
  {
    if((mpu6050_Timeout--) == 0) return 0;
  }


  I2C_GenerateSTOP(I2C2, ENABLE);
     /*stop bit flag*/
     while(I2C_GetFlagStatus(I2C2, I2C_FLAG_STOPF));
}








void I2C_init(void){

    

}
