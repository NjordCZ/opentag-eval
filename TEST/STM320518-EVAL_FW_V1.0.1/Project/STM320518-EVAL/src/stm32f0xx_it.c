/**
  ******************************************************************************
  * @file    stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   Main Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "main.h"

/** @addtogroup STM320518_EVAL_Demo
  * @{
  */
/** @addtogroup STM32F0XX_IT
  * @brief Interrupts driver modules
  * @{
  */
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MESSAGE4   "Tamper to Reset Backup reg  " 
#define MESSAGE5     " LEFT   |  RIGHT  |  SEL  " 
#define MESSAGE6   " RESET  |  START  |  EXIT   " 
#define MESSAGE7   " RESET  |  PAUSE  |  EXIT   " 

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint8_t Index = 0, Counter1 = 0;
__IO uint32_t WaveCounter = 0;
static __IO uint8_t AlarmStatus = 0;
static __IO uint8_t LedCounter = 0;
extern uint8_t Buffer1[];
extern uint8_t Buffer2[];

extern FIL F;
extern UINT BytesWritten;
extern UINT BytesRead;
extern __IO uint8_t SMbusAlertOccurred;
extern WAVE_FormatTypeDef WAVE_Format;
//extern uint8_t Key_Button_Pressed;

__IO uint32_t RTCAlarmCount = 0;

__IO uint32_t SubSecFrac = 0;
__IO uint32_t SecondNumb = 60;
extern __IO uint8_t StartEvent;
extern __IO uint8_t MenuApplication;
__IO uint32_t CurrentTimeSec = 0;
RTC_TimeTypeDef RTC_StampTimeStruct;

__IO uint32_t BackupIndex = 0;

uint8_t TabIndex = 0x00;                      /* Variable to choice between Buffer1 and Buffer2 */
uint8_t* Tab;                                 /* Pointer on Buffer1 or Buffer2  */
//__IO uint16_t RecordSample = 0x00;            /* Recorded data index in Buffer1 or Buffer2 */
__IO uint8_t WriteOnTheSDCardStatus = 0x00;
__IO uint8_t DOWNBoutonPressed = 0;           /* Stop wave record enable */
//extern uint32_t SelectedMaxRecordDataLength;  /* Selected max SD Card size dedicated */
extern __IO uint8_t  VoiceRecStop;            /* Stop Voice Recorder Flag*/

__IO uint8_t RFDemoStatus = 0x00;
__IO uint32_t ICValue1 = 0;
__IO uint32_t ICValue2 = 0;
//extern uint32_t RC5_FrameManchestarFormat;
//extern uint32_t SIRC12_FramePulseWidthFormat[];
extern __IO uint32_t LCDType;
__IO uint8_t RFProtocol = 0x00;

extern uint32_t MSec;

extern __IO uint32_t CaptureNumber, PeriodValue;
uint32_t TIM14_IC1ReadValue1 = 0, TIM14_IC1ReadValue2 =0;



/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
 
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  
      //Output_Toggle(LED_BLUE_GPIO_PORT, LED_BLUE_PIN);

  
  
  
  static uint8_t MState = 0;

	//Execute the LED PWM handler
	//H_LEDPWM();

	//Execute charge pump handler
	//H_ChargePump();

	//As the Systick handler now runs at 0.5ms
	//interrupts, MSec needs to be incremented
	//every two Systick interrupts, this is done
	//by incrementing whenever MState = 1. By
	//XOR'ing MState with 1, it will toggle
	//0 and 1. Only when it is equal to 1 can MSec
	//be incremented.

	MState^=1;
	if(MState == 1) MSec++;
  
  
  
  /* Decrement the TimingDelay variable */
  Demo_DecrementTimingDelay(); 
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/



/**
  * @brief  This function handles External line 0/1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_1_IRQHandler(void)
{
  if (EXTI_GetITStatus(TRIGGER_BUTTON_EXTI_LINE) != RESET)
  {
    // filtr spinace
    Delay(10);
    if(GPIO_ReadInputDataBit(TRIGGER_BUTTON_GPIO_PORT, TRIGGER_BUTTON_PIN) == SET)
    {
      Trigger_Press();
    }
    
    /* Clear the EXTI Line 0 */
    EXTI_ClearITPendingBit(TRIGGER_BUTTON_EXTI_LINE);
  }

  if (EXTI_GetITStatus(RELOADING_BUTTON_EXTI_LINE) != RESET)
  {
    // filtr spinace
    Delay(10);
    if(GPIO_ReadInputDataBit(RELOADING_BUTTON_GPIO_PORT, RELOADING_BUTTON_PIN) == RESET)
    {
      Reloading_press();
    }
    
    /* Clear the EXTI Line 1*/
    EXTI_ClearITPendingBit(RELOADING_BUTTON_EXTI_LINE);
  }

}

/**
  * @brief  This function handles External lines 9 to 5 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI4_15_IRQHandler(void)
{

  
}




/**
  * @brief  This function handles TIM14 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM14_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM14, TIM_IT_CC1) != RESET)
  {
    /* Clear TIM14 Capture Compare 1 interrupt pending bit */
    TIM_ClearITPendingBit(TIM14, TIM_IT_CC1);
    
    if(CaptureNumber == 0)
    {
      /* Get the Input Capture value */
      TIM14_IC1ReadValue1 = TIM_GetCapture1(TIM14);
      CaptureNumber = 1;
    }
    else if(CaptureNumber == 1)
    {
       /* Get the Input Capture value */
       TIM14_IC1ReadValue2 = TIM_GetCapture1(TIM14); 
       TIM_ITConfig(TIM14, TIM_IT_CC1, DISABLE);

       /* Capture computation */
       if (TIM14_IC1ReadValue2 > TIM14_IC1ReadValue1)
       {
         PeriodValue = (TIM14_IC1ReadValue2 - TIM14_IC1ReadValue1);
       }
       else
       {
         PeriodValue = ((0xFFFF - TIM14_IC1ReadValue1) + TIM14_IC1ReadValue2);
       }
       /* capture of two values is done */
       CaptureNumber = 2;
    }
  }
}

/**
  * @brief  This function handles RTC Wakeup global interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
  
  // generate 6 interripts per Second
  if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
  {
    // Reloading gun timer 
    Reloading_timer();
    
    // LCD update
    //LCD_update_timer();
    
    
    Output_Toggle(LED_GREEN_GPIO_PORT, LED_GREEN_PIN);
    RTC_ClearITPendingBit(RTC_IT_ALRA);
    EXTI_ClearITPendingBit(EXTI_Line17);
  } 
   

}





/**
  * @brief  This function handles TIM15 overflow and update interrupt request.
  * @param  None
  * @retval None
  */
void TIM15_IRQHandler(void)
{
  /* Clear the TIM15 Update pending bit */
  TIM_ClearITPendingBit(TIM15, TIM_IT_Update);
  
  Lasertag_Fire_Timer();
  
  
}


/**
  * @brief  This function handles TIM16 interrupt request, IR encoder.
  * @param  None
  * @retval None
  */
void TIM16_IRQHandler(void)
{
  /* IR Lasertag signal generate */
  IR_Encode_SignalGenerate();
  
  
  /* Clear TIM16 update interrupt */
  TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
}

/**
  * @brief  This function handles TIM2, IR RX.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
    /* Clear the TIM2 Update pending bit */
   // TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  
      /*The Timer interrupt is used to measure the different period between 
    two successive falling edges in order to identify the frame bits.
    
    We measure the low pulse duration:
    - If the period measured is equal to T = 1200 µs and the low pulse 
    duration is equal to T/2 = 600µs => the bit is logic '0'. 
    - If the period measured is equal to 3T/2 = 1800µs and the low pulse 
    duration is equal to T = 1200µs => the bit is logic '1'.
    - If the whole period measured is equal to 3000µs and the low pulse 
    duration is equal to 2400µs => the bit is ‘start bit’.
    
    Update event:InfraRed decoders time out event
    ----------------------------------------------
    It resets the InfraRed decoders packet.                 
    - The Timer Overflow is set to 4 ms.  */
    
  
  if((TIM_GetFlagStatus(IR_TIM, TIM_FLAG_Update) != RESET))
    { 
      // Clears the IR_TIM's pending flags
      TIM_ClearFlag(IR_TIM, TIM_FLAG_Update);
      
      // IR Timer Overflow
      IR_Timer_Overflow();
    } 
   
 
      
 if((TIM_GetFlagStatus(IR_TIM, TIM_FLAG_CC1) != RESET))
 {
    /* Clear TIM2 Capture compare interrupt pending bit */
  TIM_ClearITPendingBit(IR_TIM, TIM_IT_CC1);

  /* Get the Input Capture value */
  //ICValue2 = TIM_GetCapture2(IR_TIM);
  ICValue1 = TIM_GetCapture1(IR_TIM);
  
  if (ICValue1 != 0) //&& ICValue1 < ICValue2 )
  {
    /* Duty cycle computation */
    //ICValue1 = TIM_GetCapture1(IR_TIM);

    //IR_DataSampling(ICValue1, ICValue2);
  }
  else
  {
  ICValue1++;
    //Frequency = 0;
  }   
     
      
 }           
      
      
    
    
    
 if((TIM_GetFlagStatus(IR_TIM, TIM_FLAG_CC2) != RESET))
 {
    /* Clear TIM2 Capture compare interrupt pending bit */
  TIM_ClearITPendingBit(IR_TIM, TIM_IT_CC2);

  /* Get the Input Capture value */
  //ICValue2 = TIM_GetCapture2(IR_TIM);
  //ICValue1 = TIM_GetCapture1(IR_TIM);
  
  IR_DataSampling(TIM_GetCapture1(IR_TIM), TIM_GetCapture2(IR_TIM));
  
  
 // if (ICValue2 != 0)
 // {
    /* Duty cycle computation */
    //ICValue1 = TIM_GetCapture1(IR_TIM);

  //  IR_DataSampling(ICValue1, ICValue2);
  //}
 //   else
  //    {
  
 //    }   
     
      
 }      
      
      
      
    /*
    // IC1 Interrupt 
    if((TIM_GetFlagStatus(IR_TIM, TIM_FLAG_CC2) != RESET))
    {
      TIM_ClearFlag(IR_TIM, TIM_FLAG_CC2);
      // Get the Input Capture value 
      ICValue2 = TIM_GetCapture2(IR_TIM);
      IR_DataSampling(ICValue1, ICValue2);
      
    }  // IC2 Interrupt  
    else  if((TIM_GetFlagStatus(IR_TIM, TIM_FLAG_CC1) != RESET))
    {
      TIM_ClearFlag(IR_TIM, TIM_FLAG_CC1);
      // Get the Input Capture value 
      ICValue1 = TIM_GetCapture1(IR_TIM);
      
    } 
    // Checks whether the IR_TIM flag is set or not. 
    else if ((TIM_GetFlagStatus(IR_TIM, TIM_FLAG_Update) != RESET))
    { 
      // Clears the IR_TIM's pending flags
      TIM_ClearFlag(IR_TIM, TIM_FLAG_Update);
      
      //IR_ResetPacket();
    } */
}



/**
  * @brief  This function handles DMA1_Channel2_3 overflow and update interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel1_IRQHandler(void) 
{
  
  


}

/**
  * @brief  This function handles DMA1_Channel2_3 overflow and update interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel2_3_IRQHandler(void) 
{
  
  if(DMA_GetITStatus(DMA1_IT_TC3)) 
  {
    
    Wave_Player_Play_Handler();
    //DMA_ClearITPendingBit(DMA1_IT_TC1);
    
  }


}




/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
