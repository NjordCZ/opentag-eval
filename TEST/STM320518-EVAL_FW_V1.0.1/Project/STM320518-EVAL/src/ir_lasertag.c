/**
  ******************************************************************************
  * @file    ir_lasertag.c
  * @author  njord
  * @version V1.0.0
  * @date    26.10.2015
  * @brief   This file provides all the lasertag IR firmware functions
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/        
#include "main.h"



GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
TIM_ICInitTypeDef TIM_ICInitStructure;

TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
//GPIO_InitTypeDef GPIO_InitStructure;
//NVIC_InitTypeDef NVIC_InitStructure;   
  
  
/** @addtogroup STM320518_EVAL_Demo
  * @{
  */

/** @addtogroup IR_ENCODE
  * @brief IR_ENCODE driver modules
  * @{
  */

/** @defgroup IR_ENCODE_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup IR_ENCODE_Private_Defines
  * @{
  */
//#define  SIRC12_HIGH_STATE_NB_SYMBOL     ((uint8_t )3)       /* Nb high state symbol definition*/
//#define  SIRC12_LOW_STATE_NB_SYMBOL      ((uint8_t )2)       /* Nb low state symbol definition*/
//#define  SIRC12_ADDRESS_BITS             ((uint8_t )5)       /* Nb of data bits definition*/
//#define  SIRC12_INSTRUCTION_BITS         ((uint8_t )7)       /* Nb of data bits definition*/
//#define  SIRC12_HIGH_STATE_CODE          ((uint8_t )0x03)    /* SIRC12 high level definition*/
//#define  SIRC12_LOW_STATE_CODE           ((uint8_t )0x01)    /* SIRC12 low level definition*/
//#define  SIRC12_MAIN_FRAME_LENGTH        ((uint8_t )12)      /* Main frame length*/
//#define  SIRC12_BIT_FORMAT_MASK          ((uint16_t)1)       /* Bit mask definition*/
//#define  SIRC12_COUNTER_LIMIT            ((uint16_t)75)      /* main frame + idle time */
//#define  SIRC12_IS_RUNNING               ((uint8_t)4)        /* SIRC12 Protocol number */
//#define  SIRC12_HEADERS                  ((uint16_t)0x0F)    /* SIRC12 Start pulse */
//#define  SIRC12_HEADERS_LENGTH           ((uint8_t)5)        /* Length of the headers */
//#define  SIRC12_CODED_FRAME_TABLE_LENGTH ((uint8_t)2)        /* Coded frame table number of uint32_t word  */


/**
  * @}  
  */


/** @defgroup IR_ENCODE_Private_Macros
  * @{
  */
/**
  * @}  
  */

/** @defgroup IR_ENCODE_Private_Variables
  * @{
  */

   



// ir encode private variables
uint32_t IR_Frame = 0;  /* IR zprava */
uint8_t IR_Encode_NbBits = 32;  /* pocet odeslanach bitu IR_Frame */
uint8_t IR_Send_Operation_Ready_f = RESET;
uint8_t IR_Send_Operation_Completed_f = SET;  
uint8_t IR_BitsSent_Counter = 0; // pocet bitu ktere budou odeslany IR
uint8_t IR_Pulse = 4; // délka ir pulsu, nasobky TIM16 - 600us, 4...header, 2...1bit, 1...0bit


/**
  * @}
  */

/** @defgroup IR_DECODE_Private_FunctionPrototypes
  * @{
  */
 
static uint8_t IR_DecodeHeader(__IO uint32_t lowPulseLength, __IO uint32_t wholePulseLength);
static uint8_t IR_DecodeBit(__IO uint32_t lowPulseLength, __IO uint32_t wholePulseLength);
static uint32_t TIM_GetCounterCLKValue(void);
static uint32_t reversebit(uint32_t data);
 
/**
  * @}
  */





/** @defgroup IR_ENCODE_Private_FunctionPrototypes
  * @{
  */

static uint32_t MSBToLSB_Data(uint32_t Data, uint8_t DataNbBits); // nemazat

/**
  * @}
  */



/** @defgroup IR_DECODE_Private_Variables
  * @{
  */

__IO StatusYesOrNo IRFrameReceived = NO;   /*!< IR frame status */
  
/* IR bit definitions*/
uint16_t IROnTimeMin = 0; 
uint16_t IROnTimeMax = 0; 
uint16_t IRValueStep = 0;
uint16_t IRValueMargin = 0;
uint16_t IRValue00 = 0;

/* Header time definitions*/
uint16_t IRHeaderLowMin = 0;
uint16_t IRHeaderLowMax = 0;
uint16_t IRHeaderWholeMin = 0;
uint16_t IRHeaderWholeMax = 0;
IR_packet IRTmpPacket; /*!< IR packet*/

uint16_t IRTimeOut = 0;
uint32_t TIMCLKValueKHz = 0;/*!< Timer clock */

//IR_Frame_TypeDef IR_FRAME;
//extern __IO uint8_t RFDemoStatus;
/**
  * @}
  */








//////////////////////////////////////////////////////////////////////////////////////
//  ENCODEE IR                                                                       //
//////////////////////////////////////////////////////////////////////////////////////



/**
  * @brief  Init Hardware (IPs used) for IR generation
  * @param  None
  * @retval None
  */
void IR_Encode_Init(void)
{
  
  
  
  
  /* TIM16 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
  
  /* TIM17 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);
  
  /* GPIOB clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  
  /* Pin configuration: input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_0);

  
  
  /* DeInit TIM17 */
  TIM_DeInit(TIM17);
  
  
  /* Time base configuration for timer 2 */
  TIM_TimeBaseStructure.TIM_Period = 857; //857 - 56Khz
  TIM_TimeBaseStructure.TIM_Prescaler = 0x00;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  
  TIM_TimeBaseInit(TIM17, &TIM_TimeBaseStructure);
  
  /* Prescaler configuration */
  TIM_PrescalerConfig(TIM17, 0, TIM_PSCReloadMode_Immediate);
  
  /* Output Compare Timing Mode configuration: Channel 1N */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 429; // 429 Set duty cycle to 50% TIM_Period to be compatible with IR specification
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  //TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; //TIM_OCIdleState_Reset;
  TIM_OC1Init(TIM17, &TIM_OCInitStructure);
  
  /* Timer17 preload enable */
  TIM_OC1PreloadConfig(TIM17, TIM_OCPreload_Enable);
  
  /* Timer 17 Enable */
  TIM_Cmd(TIM17, ENABLE);
  
  /* Enable the TIM16 channel1 output to be connected internly to the IRTIM */
  TIM_CtrlPWMOutputs(TIM17, ENABLE);
  
  /* DeInit TIM16 */
  TIM_DeInit(TIM16);

  /* Time Base = 56Khz */
  /* Time Base configuration for timer 16 */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 28799;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);
  
  
  /* Channel 1 Configuration in Timing mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  //TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 28799;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  //TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  //TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  //TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
  TIM_OC1Init(TIM16, &TIM_OCInitStructure);
   
  /* Enable the TIM16 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM16_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  
  /* TIM16 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM16, ENABLE);
  
  /* TIM IT Disable */
  TIM_ITConfig(TIM16, TIM_IT_Update, DISABLE);
  
  /* TIM Disable */
  TIM_Cmd(TIM16, DISABLE);
 
  
  
  
  //////////////////////////////////
  // IR power timer TIM14
  /////////////////////////////////
  
  /* TIM14 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
  
  /* Pin configuration IR power */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_4);

  
  /* DeInit TIM14 */
  TIM_DeInit(TIM14);

  /* Time Base configuration for timer 14 */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
  
  /* Channel 1 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  //TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 100 - 1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  //TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  //TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  //TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  
  TIM_OC1Init(TIM14, &TIM_OCInitStructure);
  
  /* TIM14 counter enable */
  TIM_Cmd(TIM14, ENABLE);

  /* TIM14 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM14, ENABLE);
  

}

/**
  * @brief  Send the IR frame.
  * @param  uint32_t IR_Pocket
  * @retval None
  */
void IR_Encode_Start(uint32_t IR_Pocket, uint8_t IR_NbBits)
{
  /* Number of bits IR pocket */
  IR_Encode_NbBits = IR_NbBits;  
  /* Transform address and data from MSB first to LSB first */
  IR_Frame = MSBToLSB_Data(IR_Pocket, IR_NbBits);
  
  /* Set the Send operation Ready flag to indicate that the frame is ready to be sent */
  IR_Send_Operation_Ready_f = SET;
  
  
  
  /* Enable all Interrupt */
  TIM_Cmd(TIM16, ENABLE);
  
  /* TIM IT Enable */
  TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);
  
   
}
        
/**
  * @brief  Send by hardware IR frame.
  * @param  None
  * @retval None
  */
void IR_Encode_SignalGenerate(void)
{
  if((IR_Send_Operation_Ready_f != RESET) && (IR_BitsSent_Counter <= (IR_Encode_NbBits)))
  {
    /*Reset send operation complete flag*/
    IR_Send_Operation_Completed_f = RESET;
    if(IR_Pulse == 0)
    {
      /* IR pause */ 
       TIM_ForcedOC1Config(TIM16, TIM_ForcedAction_InActive); //TIM_ForcedAction_InActive
      
      /*Read message next bits*/
      if(((IR_Frame >> IR_BitsSent_Counter) & 1) == SET)
      {
        IR_Pulse = 2;  // IR bit 1
      }
      else
       {
         IR_Pulse = 1;  // IR bit 0
       } 
       
       IR_BitsSent_Counter++;
    }
    else
     {
       /* IR pause generate signal */
       TIM_ForcedOC1Config(TIM16, TIM_ForcedAction_Active); //TIM_ForcedAction_Active
       IR_Pulse--;
     }
   
  }   
  else
  {
    /* IR pause */
    TIM_ForcedOC1Config(TIM16, TIM_ForcedAction_InActive); //TIM_ForcedAction_InActive
    
    /* Reset counters  */
    IR_BitsSent_Counter = 0;
    
    /* Set header */
    IR_Pulse = 4;
    
    /* Reset IR frame */
    IR_Frame = 0;
    
    /* Reset flags   */
    
    IR_Send_Operation_Completed_f = SET;
    IR_Send_Operation_Ready_f = RESET;
    
    
    /* TIM IT Disable */
    TIM_ITConfig(TIM16, TIM_IT_Update, DISABLE);
    TIM_Cmd(TIM16, DISABLE);
        
    
    
  }  
  
 
}

/**
  * @brief  Transform the frame binary form from MSB to LSB.
  * @param  Data: Frame binary format to inverse
  * @param  DataNbBits: number if bits to be inverted
  * @retval Symmetric binary frame form
  */
static uint32_t MSBToLSB_Data(uint32_t Data ,uint8_t DataNbBits)
{
  uint32_t temp = 0;       /* Temporary variable to memorize the converted message */
  uint8_t datacount = 0; /* Counter of bits converted */
  
  /* Shift the temporary variable to the left and add one bit from the Binary frame  */
  for (datacount = 0; datacount < (DataNbBits); datacount++)
  {
    temp=temp << 1;
    temp |= ((Data>>datacount)&1);
  }
  return temp;
}




//////////////////////////////////////////////////////////////////////////////////////
//  DECODE IR                                                                       //
//////////////////////////////////////////////////////////////////////////////////////


/**
  * @brief  Initialize the IR bit time range.
  * @param  None
  * @retval None
  */
void IR_Decode_Init(void)
{ 
  
  
  /*  Clock Configuration for TIMER */
  RCC_APB1PeriphClockCmd(IR_TIM_CLK, ENABLE);

  /* Enable Button GPIO clock */
  RCC_AHBPeriphClockCmd(IR_GPIO_PORT_CLK, ENABLE);
  
  /* Pin configuration: input floating */
  GPIO_InitStructure.GPIO_Pin = IR_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //no
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(IR_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(IR_GPIO_PORT, IR_GPIO_SOURCE, GPIO_AF_2);
     
  /* Enable the TIM global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = IR_TIM_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* TIMER frequency input */
  TIM_PrescalerConfig(IR_TIM, TIM_PRESCALER, TIM_PSCReloadMode_Immediate);

  /* TIM configuration */
  TIM_ICInitStructure.TIM_Channel = IR_TIM_Channel;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling; //TIM_ICPolarity_Falling   / 
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = IR_ICFILTER;  
  TIM_PWMIConfig(IR_TIM, &TIM_ICInitStructure); 

  /* Timer Clock */
  TIMCLKValueKHz = TIM_GetCounterCLKValue()/1000; 

  /* Select the TIM Input Trigger: TI2FP2 */
  TIM_SelectInputTrigger(IR_TIM, TIM_TS_TI2FP2);

  /* Select the slave Mode: Reset Mode */
  TIM_SelectSlaveMode(IR_TIM, TIM_SlaveMode_Reset);

  /* Enable the Master/Slave Mode */
  TIM_SelectMasterSlaveMode(IR_TIM, TIM_MasterSlaveMode_Enable);

  /* Configures the TIM Update Request Interrupt source: counter overflow */
  TIM_UpdateRequestConfig(IR_TIM,  TIM_UpdateSource_Regular); //TIM_UpdateSource_Regular);
   
  IRTimeOut = TIMCLKValueKHz * IR_TIME_OUT_US/1000;

  /* Set the TIM auto-reload register for each IR protocol */
  IR_TIM->ARR = IRTimeOut;
  
  
  
  
  
  

  /* Enable TIM Update Event Interrupt Request */
  TIM_ITConfig(IR_TIM, TIM_IT_Update, ENABLE);

  /* Clear update flag */
  TIM_ClearFlag(IR_TIM, TIM_FLAG_Update);
  
  
  /* Enable the CC2/CC1 Interrupt Request */
  TIM_ITConfig(IR_TIM, TIM_IT_CC2, ENABLE);
  

  /* Clear TIM2 Capture compare interrupt pending bit */
  TIM_ClearITPendingBit(IR_TIM, TIM_IT_CC2);
  
  /* Enable the timer */
  TIM_Cmd(IR_TIM, ENABLE);
  
  
  
  /* Header */	
  IRHeaderLowMin = TIMCLKValueKHz * IR_HEADER_LOW_MIN_US/1000;
  IRHeaderLowMax = TIMCLKValueKHz * IR_HEADER_LOW_MAX_US/1000;
  IRHeaderWholeMin = TIMCLKValueKHz * IR_HEADER_WHOLE_MIN_US/1000;
  IRHeaderWholeMax = TIMCLKValueKHz * IR_HEADER_WHOLE_MAX_US/1000;

  /* Bit time range*/
  IROnTimeMax = TIMCLKValueKHz * IR_ONTIME_MAX_US /1000;
  IROnTimeMin = TIMCLKValueKHz * IR_ONTIME_MIN_US/1000; 
  IRValueStep = TIMCLKValueKHz * IR_VALUE_STEP_US/1000;
  IRValueMargin = TIMCLKValueKHz * IR_VALUE_MARGIN_US/1000;
  IRValue00 = TIMCLKValueKHz * IR_VALUE_00_US/1000;

  /* Default state */
  IR_ResetPacket();
}


/**
  * @brief  Identify the IR data.
  * @param  lowPulseLength: low pulse duration. 
  * @param  wholePulseLength: whole pulse duration.
  * @retval None
  */
void IR_DataSampling(uint32_t lowPulseLength, uint32_t wholePulseLength)
{
  uint8_t  tmpBit;
  
  /* If the pulse timing is correct */
  if ((IRTmpPacket.status == IR_STATUS_RX)) 
  {
    /* Convert raw pulse timing to data value */
    tmpBit = IR_DecodeBit(lowPulseLength, wholePulseLength); 
    if (tmpBit != IR_BIT_ERROR && IRTmpPacket.count < IR_TOTAL_BITS_COUNT) /* If the pulse timing is correct */
    {
      /* This operation fills in the incoming bit to the correct position in
      32 bit word*/
      IRTmpPacket.data |= tmpBit;
      IRTmpPacket.data <<= 1;
      /* Increment the bit count  */
      IRTmpPacket.count++;
    }
    else
    {
      
      /* Frame received*/
      IRFrameReceived = YES;
      
      /* Decode IR data*/
      Lasertag_DecodeData(IRTmpPacket.data, IRTmpPacket.count);
      
      /* Reset IR packet - data, count, status*/
      IR_ResetPacket();
      
    }
    
  }
  else if ((IRTmpPacket.status == IR_STATUS_HEADER))
  {
    /* Convert raw pulse timing to data value */
    tmpBit = IR_DecodeHeader(lowPulseLength, wholePulseLength);
    
    /* If the Header timing is correct */
    if (tmpBit == IR_HEADER_OK)
    { 
      /* Reception Status for the inverted message */
      IRTmpPacket.status = IR_STATUS_RX; 
    }
    else
    {
      /* Wrong header time */
//      IRTmpPacket.status = IR_STATUS_HEADER;  
 //     Wave_Player_Play("/CLICK.WAV"); // play wave from SD card
 //    Delay(100);
 //     Wave_Player_Play("/CLICK.WAV"); // play wave from SD card
    }
  }
}




/**
  * @brief  Check the header pulse if it has correct pulse time.
  * @param  lowPulseLength: low pulse header duration.
  * @param  wholePulseLength: whole pulse header duration.
  * @retval IR_HEADER_OK or IR_HEADER_ERROR
  */
static uint8_t IR_DecodeHeader(uint32_t lowPulseLength, uint32_t wholePulseLength)
{
  /* First check if low pulse time is according to the specs */
  if ((lowPulseLength >= IRHeaderLowMin) && (lowPulseLength <= IRHeaderLowMax)
     && (wholePulseLength >= IRHeaderWholeMin) && (wholePulseLength <= IRHeaderWholeMax)) 
  {
    /* Check if the high pulse is OK */
    //if ((wholePulseLength >= IRHeaderWholeMin) && (wholePulseLength <= IRHeaderWholeMax))
    //{
      return IR_HEADER_OK; /* Valid Header */
   // }
  }	
  return IR_HEADER_ERROR;  /* Wrong Header */
}


/**
  * @brief  Convert raw time to data value.
  * @param  lowPulseLength: low pulse bit duration.
  * @param  wholePulseLength: whole pulse bit duration.
  * @retval BitValue( data0 or data1) or IR_BIT_ERROR
  */
static uint8_t IR_DecodeBit(uint32_t lowPulseLength , uint32_t wholePulseLength)
{
 // uint8_t i = 0;
  
  /* First check if low pulse time is according to the specs */
  if ((lowPulseLength >= IROnTimeMin) && (lowPulseLength <= IROnTimeMax))
  {
   if ((wholePulseLength >= IRValue00 + (IRValueStep * 0) - IRValueMargin) 
          && (wholePulseLength <= IRValue00 + (IRValueStep * 0) + IRValueMargin))
        return 0; /* Return bit value */
   
   if ((wholePulseLength >= IRValue00 + (IRValueStep * 1) - IRValueMargin) 
          && (wholePulseLength <= IRValue00 + (IRValueStep * 1) + IRValueMargin))
        return 1; /* Return bit value */
   
      
  }
  return IR_BIT_ERROR; /* No correct pulse length was found */
}

/**
  * @brief  Identify TIM clock
  * @param  None
  * @retval Timer clock
  */
static uint32_t TIM_GetCounterCLKValue(void)
{
  uint32_t apbprescaler = 0, apbfrequency = 0;
  uint32_t timprescaler = 0;
  __IO RCC_ClocksTypeDef RCC_ClockFreq;
  
  /* This function fills the RCC_ClockFreq structure with the current
  frequencies of different on chip clocks */
  RCC_GetClocksFreq((RCC_ClocksTypeDef*)&RCC_ClockFreq);
  
   /* Get the clock prescaler of APB1 */
    apbprescaler = ((RCC->CFGR >> 8) & 0x7);
    apbfrequency = RCC_ClockFreq.PCLK_Frequency; 
    timprescaler = TIM_PRESCALER;

  
  /* If APBx clock div >= 4 */
  if (apbprescaler >= 4)
  {
    return ((apbfrequency * 2)/(timprescaler + 1));
  }
  else
  {
    return (apbfrequency/(timprescaler+ 1));
  }
}
/**
  * @brief  Reverse bit       
  * @param  data: data to be inversed
  * @retval None
  */
static uint32_t reversebit(uint32_t data) 
{
  uint32_t i = 0;
  uint32_t j = 0;
  
  for(j = i = 0; i < 32; i++) 
  {
    j = (j << 1) + (data & 1); 
    data >>= 1; 
  }
  
  return j;
}

/**
  * @brief  IR timer overflow, end mesage 
  *         
  * @param  None
  * @retval None
  */
void IR_Timer_Overflow(void)
{
  if(IRTmpPacket.status == IR_STATUS_RX)
  {
    /* Frame received*/
    IRFrameReceived = YES;
      
    /* Decode IR data*/
    Lasertag_DecodeData(IRTmpPacket.data, IRTmpPacket.count);
      
    /* Reset IR packet - data, count, status*/
    IR_ResetPacket();
  }
  
}


/**
  * @brief  Put the packet to default state, ready to be filled in by incoming 
  *         data.
  * @param  None
  * @retval None
  */
void IR_ResetPacket(void)
{
  IRTmpPacket.count = 0;
  IRTmpPacket.status = IR_STATUS_HEADER;
  IRTmpPacket.data = 0;
  
  // Clears the IR_TIM's pending flags
  TIM_ClearFlag(IR_TIM, TIM_FLAG_Update);
  /* Clear TIM2 Capture compare interrupt pending bit */
  //TIM_ClearITPendingBit(IR_TIM, TIM_IT_CC1);
  /* Clear TIM2 Capture compare interrupt pending bit */
  TIM_ClearITPendingBit(IR_TIM, TIM_IT_CC2);
  
}

/*****************************END OF FILE****/

