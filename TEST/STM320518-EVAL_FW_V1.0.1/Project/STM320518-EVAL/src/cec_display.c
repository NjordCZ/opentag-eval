/**
  ******************************************************************************
  * @file    cec_display.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   It displyas the CEC demonstration messages on LCD.
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
#include "cec_display.h"

/** @addtogroup STM320518_EVAL_Demo
  * @{
  */
  
/** @defgroup CEC_DISPLAY 
  * @brief    It displyas the CEC demonstration messages on either LCD or 
  *           Hyperterminal
  * @{
  */ 

/** @defgroup CEC_DISPLAY_Private_Types
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup CEC_DISPLAY_Private_Defines
  * @{
  */ 
#define NUMBER_OF_CMD  12


/**
  * @}
  */ 

/** @defgroup CEC_DISPLAY_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup CEC_DISPLAY_Private_Variables
  * @{
  */ 
extern uint32_t RC5_Data;
extern __IO uint8_t RFDemoStatus;
HDMI_CEC_Error TransmitStatus = HDMI_CEC_OK;
__IO uint8_t FollowerAddress = 0;
__IO uint8_t KeyValue = 0;
extern __IO uint32_t ReceivedFrame, ReceiveStatus;
extern HDMI_CEC_Message HDMI_CEC_RX_MessageStructPrivate;
extern HDMI_CEC_Message HDMI_CEC_TX_MessageStructure;
extern __IO uint8_t MyLogicalAddress;
extern __IO uint16_t MyPhysicalAddress;
extern __IO uint8_t DeviceType;
extern uint8_t* HDMI_CEC_Follower_String[13][2];
extern __IO uint8_t Key_Button_Pressed;
RC5_Frame_TypeDef CEC_RC5_FRAME;
StatusYesOrNo FirstTimeIssued = YES;
__IO uint16_t CEC_RC5_ToggleBit_Previous = 0;
__IO uint8_t CECDemoStatus = 0;
extern __IO uint32_t LCDType;

/* CEC device commands */
uint8_t* HDMI_CEC_CMD_String[NUMBER_OF_CMD] =
{
  "   FEATURE_ABORT    ",
  " GET CEC VERSION    ",
  " GIVE PHYSICAL ADR  ",
  "    GIVE OSD NAME   ",
  "   STANDBY VALUE    ",
  "   IMAGE VIEW ON    ",
  "    TEXT VIEW ON    ",
  "    RECORD OFF      ",
  " GIVE DEV VENDOR ID ",
  "     Volume Up      ",
  "    Volume Down     ",
  "    Volume Mute     "
};

uint8_t HDMI_CEC_Follower_ADD[13] =
{
  0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xF
};

uint8_t HDMI_CEC_CMD_OP[NUMBER_OF_CMD] =
{
  HDMI_CEC_OPCODE_FEATURE_ABORT, HDMI_CEC_OPCODE_GET_CEC_VERSION, 
  HDMI_CEC_OPCODE_GIVE_PHYSICAL_ADDRESS, HDMI_CEC_OPCODE_GIVE_OSD_NAME,
  HDMI_CEC_OPCODE_STANDBY, HDMI_CEC_OPCODE_IMAGE_VIEW_ON, 
  HDMI_CEC_OPCODE_TEXT_VIEW_ON, HDMI_CEC_OPCODE_RECORD_OFF,
  HDMI_CEC_OPCODE_GIVE_DEVICE_VENDOR_ID,
  HDMI_CEC_OPCODE_USER_CONTROL_PRESSED, 0x55, 0x66
}; /* 0x55 = Volume Down, 0x66= Volume Mute */

 /* CEC device commands: Correspondance table between RC5 and CEC Commands */
uint8_t HDMI_CEC_RC5_CMD[128] =
  {
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x0 , 0x0 , 0x40, 0x43, 0x0C, 0x35, 0x41, 0x42, 0x0 ,
    0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x30, 0x31, 0x32, 0x0 , 0x0 , 0x0 ,
    0x54, 0x0 , 0x0 , 0x0 , 0x0 , 0x4B, 0x4C, 0x4F, 0x0B, 0x0 , 0x46, 0x4A, 0x48, 0x0 ,0x0  , 0x44, 0x45, 0x47, 0x0 ,
    0x0 , 0x0E, 0x0 , 0x6B, 0x0 , 0x51, 0x0 , 0xF , 0x10, 0x11, 0x12, 0x13, 0x14, 0x0 , 0x0 , 0x15, 0x16, 0x17, 0x18,
    0x19, 0x0 , 0x0 , 0x33, 0x01, 0x02, 0x0A, 0x0D, 0x69, 0x0 , 0x0 , 0x53, 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 ,
    0x0 , 0x0 , 0x0 ,0x0  , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x75, 0x00, 0x72, 0x73, 0x74, 0x71, 0x0 , 0x0 , 0x32, 
    0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x6E, 0x0 ,0x0  , 0x0 , 0x0 , 0x0 
  };

/**
  * @}
  */ 

/** @defgroup CEC_DISPLAY_Private_Function_Prototypes
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup CEC_DISPLAY_Private_Functions
  * @{
  */ 

 
/**
  * @brief CEC display function
  * @param  None
  * @retval : None
  */
void Menu_CECDisplayFunc(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  /* Disable the JoyStick interrupt */
  Demo_IntExtOnOffCmd(DISABLE);
  
  /* Connect Button EXTI Line to Key Button GPIO Pin */
  SYSCFG_EXTILineConfig(KEY_BUTTON_EXTI_PORT_SOURCE, KEY_BUTTON_EXTI_PIN_SOURCE);
  
  /* Configure Button EXTI line */
  EXTI_InitStructure.EXTI_Line = KEY_BUTTON_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable and set the Key Button EXTI Interrupt to the Highest priority */
  NVIC_InitStructure.NVIC_IRQChannel = KEY_BUTTON_EXTI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  
  NVIC_Init(&NVIC_InitStructure);
  
  Key_Button_Pressed = 0;
  CECDemoStatus = 1;
  
  /* Initialize the CEC Demo */
  CEC_Display_Init();  

  /* Infinite loop */
  while (Key_Button_Pressed == 0)
  {
    CEC_Display_CECAvailableCommands();   
  }
  
  Key_Button_Pressed = 0;
  
  /* Configure Button Key in GPIO Mode */
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_GPIO);
  
  NVIC_InitStructure.NVIC_IRQChannel = CEC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure); 
  
  /* CEC DeInit */
  CEC_DeInit();
  
  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Enable the JoyStick interrupt */
  Demo_IntExtOnOffCmd(ENABLE);
  
  CECDemoStatus = 0;
  /* Display the previous menu */
  Menu_DisplayMenu();
}
 
/**
  * @brief  DeInitialize the CEC Demo.
  * @param  None
  * @retval None
  */
void CEC_Display_DeInit(void)
{
  CEC_DeInit();
  I2C_DeInit(I2C2);
  GPIO_DeInit(GPIOB);
}

/**
  * @brief  Initialize the CEC Demo.
  * @param  None
  * @retval None
  */
void CEC_Display_Init(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the CEC global Interrupt (with higher priority) */
  NVIC_InitStructure.NVIC_IRQChannel = CEC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
  
  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_BLUE);
  LCD_SetBackColor(LCD_COLOR_BLACK);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  
  CEC_Display(LCD_LINE_0, "   Select your CEC  ");
  CEC_Display(LCD_LINE_1, "     Device Type    ");
  LCD_SetBackColor(LCD_COLOR_BLUE);
  
  CEC_Display(LCD_LINE_3, "RIGHT--> Recording  ");
  CEC_Display(LCD_LINE_4, "LEFT --> Tuner      ");
  CEC_Display(LCD_LINE_5, "UP   --> Playback   ");
  CEC_Display(LCD_LINE_6, "DOWN --> AudioSystem");
  
  LCD_SetBackColor(LCD_COLOR_BLACK);
  CEC_Display(LCD_LINE_8, "KEY  --> EXIT       ");
  CEC_Display(LCD_LINE_9, "                    ");
  
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Select the device type */  
  CEC_Display_SelectDeviceType();
  
  if(Key_Button_Pressed != 0x00) return;
  
  /* CEC Initialization */
  HDMI_CEC_Init();
  
  /* for Unregistered Device */  
  if (MyLogicalAddress == 0xF)
  {

    /* Clear the LCD */
    LCD_Clear(LCD_COLOR_BLUE);
    LCD_SetBackColor(LCD_COLOR_BLACK);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    CEC_Display(LCD_LINE_5,"Unregistered Device");
    
    while(Key_Button_Pressed == 0x0);
    CEC_Display_DeInit();
    return;
  }
 
   /* Display initialization */
  LCD_Clear(LCD_COLOR_BLUE);
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  CEC_Display(LCD_LINE_0, "   CEC device is   ");
  CEC_Display(LCD_LINE_1, "   initialized     ");
  CEC_Display(LCD_LINE_2, "   correctly   ");  
  /* Display physical and logical addesses */
  CEC_Display(LCD_LINE_3, "   Your Logical ");
  CEC_Display(LCD_LINE_4, " Address is :");
  LCD_SetTextColor(LCD_COLOR_RED);
  CEC_Display_LCD_DisplayString(LCD_LINE_4, LCD_COLUMN_14, "0x");
  CEC_Display_DisplayHexToChar (MyLogicalAddress, LCD_LINE_4, LCD_COLUMN_16);  
  LCD_SetTextColor(LCD_COLOR_WHITE);
  CEC_Display(LCD_LINE_5, "   Your Physical ");
  CEC_Display(LCD_LINE_6, " Address is :");
  LCD_SetTextColor(LCD_COLOR_RED);
  CEC_Display_LCD_DisplayString(LCD_LINE_6, LCD_COLUMN_14, "0x");  
  CEC_Display_DisplayHexToChar (MyPhysicalAddress >> 8, LCD_LINE_6, LCD_COLUMN_16);
  CEC_Display_DisplayHexToChar (MyPhysicalAddress & 0xFF, LCD_LINE_6, LCD_COLUMN_18);
  
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_SetBackColor(LCD_COLOR_BLACK);
  CEC_Display(LCD_LINE_8, "Press SEL button to ");
  CEC_Display(LCD_LINE_9, "  enter CEC menu    ");

  /* Press SEL button to enter CEC menu */
  while((Menu_ReadKey() != SEL) && (Key_Button_Pressed == 0))
  {}
  if(Key_Button_Pressed)return;
 
  /* Display CEC menu */
  LCD_Clear(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_SetBackColor(LCD_COLOR_BLACK);
  CEC_Display(LCD_LINE_0, "Receive:            ");
  CEC_Display(LCD_LINE_7, "Send Status:        ");
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetBackColor(LCD_COLOR_RED);
}

/**
  * @brief  Select the device type
  * @param  None
  * @retval None
  */
void CEC_Display_SelectDeviceType(void)
{    
  do 
  {
    KeyValue = Menu_ReadKey();
    if(Key_Button_Pressed != 0x00) return;
   } while((!KeyValue) || (KeyValue == SEL));
  
  switch(KeyValue)
  {
  case RIGHT: 
    DeviceType = HDMI_CEC_RECORDING;
    break;
  case LEFT: 
    DeviceType = HDMI_CEC_TUNER;
    break;
  case UP: 
    DeviceType = HDMI_CEC_PLAYBACK;
    break;
  case DOWN: 
    DeviceType = HDMI_CEC_AUDIOSYSTEM;
    break;
  }
}

/**
  * @brief  Display the available CEC commands.
  * @param  None
  * @retval None
  */
void CEC_Display_CECAvailableCommands(void)
{
  uint32_t KeyState = 0, opcode = 0;
  uint8_t sendcount = 0;
  int8_t i = 0;
  
  /* select the follower address */
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLACK);
  
  CEC_Display(LCD_LINE_8, "Select Follower ADDR");
    
  FollowerAddress = CEC_Display_SelectFollowerAddress();
 
  while((Menu_ReadKey() != NOKEY) && (Key_Button_Pressed == 0))
  {}
  if(Key_Button_Pressed)return;
  
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLACK);
  CEC_Display(LCD_LINE_8, " Select CEC Command ");
  
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  CEC_Display(LCD_LINE_9, HDMI_CEC_CMD_String[i]); 
  opcode = HDMI_CEC_CMD_OP[i]; 
  
  /* Initialize the InfraRed application: RC5 */
  RFDemoStatus = RC5DEMO; 
  RC5_Init();
  
  while(KeyState != SEL)
  {
    KeyState = Menu_ReadKey();
    
    if(Key_Button_Pressed)return;
    
    if(KeyState == LEFT)
    {
      if(i == 0)
      {
        i = NUMBER_OF_CMD;
      }
      i--;      
      /* Set the Back Color */
      LCD_SetBackColor(LCD_COLOR_BLUE);
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_WHITE);
      CEC_Display(LCD_LINE_9, HDMI_CEC_CMD_String[i]);       
      opcode = HDMI_CEC_CMD_OP[i];      
    }
    else if (KeyState == RIGHT)
    {
      if(i == (NUMBER_OF_CMD - 1))
      { 
        i = -1;
      }
      i++;   
      /* Set the Back Color */
      LCD_SetBackColor(LCD_COLOR_BLUE);
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_WHITE);
      CEC_Display(LCD_LINE_9, HDMI_CEC_CMD_String[i]); 
      opcode = HDMI_CEC_CMD_OP[i];      
    }
    else if (KeyState == SEL)
    {
      /* Send CEC Message */
      HDMI_CEC_TX_MessageStructure.Header = ((MyLogicalAddress << 4) | FollowerAddress);
      HDMI_CEC_TX_MessageStructure.Opcode = opcode;
      HDMI_CEC_TX_MessageStructure.TxMessageLength = 0x0;
      if (opcode == HDMI_CEC_OPCODE_USER_CONTROL_PRESSED)
      {
        HDMI_CEC_TX_MessageStructure.Operande[0] = 0x41;
        HDMI_CEC_TX_MessageStructure.TxMessageLength = 0x1;  
      }
      if (opcode == 0x55)
      {
        HDMI_CEC_TX_MessageStructure.Opcode = HDMI_CEC_OPCODE_USER_CONTROL_PRESSED;
        HDMI_CEC_TX_MessageStructure.Operande[0] = 0x42;
        HDMI_CEC_TX_MessageStructure.TxMessageLength = 0x1;  
      }
      if (opcode == 0x66)
      {
        HDMI_CEC_TX_MessageStructure.Opcode = HDMI_CEC_OPCODE_USER_CONTROL_PRESSED;
        HDMI_CEC_TX_MessageStructure.Operande[0] = 0x43;
        HDMI_CEC_TX_MessageStructure.TxMessageLength = 0x1;  
      }
      TransmitStatus = HDMI_CEC_TransmitMessage(&HDMI_CEC_TX_MessageStructure);
      
      /* retransmit message until 5 time */
      while((TransmitStatus != HDMI_CEC_OK)&& sendcount <0x5)
      {
        sendcount++;
        TransmitStatus = HDMI_CEC_TransmitMessage(&HDMI_CEC_TX_MessageStructure);
      }
     
      if (TransmitStatus == HDMI_CEC_OK)      
      { 
        LCD_SetBackColor(LCD_COLOR_GREEN);
        CEC_Display(LCD_LINE_7, "Send Status: Succeed");
      }
      else
      {
        LCD_SetBackColor(LCD_COLOR_RED);
        CEC_Display(LCD_LINE_7, "Send Status: Fail   ");
      }   
    }
    else if (KeyState == NOKEY)
    { 
      /* Command from Remote Control: RC5 */
      RC5_Decode(&CEC_RC5_FRAME); 
      
      /* If the first time RC5 frame is received */
      if(FirstTimeIssued == YES)
      {
        /* Get and invert the toggle bit received at the first time to allow 
        to enter the next condition at the first time */
        CEC_RC5_ToggleBit_Previous = ~(CEC_RC5_FRAME.ToggleBit)&0x01;
      }
      
      /* Test on RC5 data and toggle bit */
      if ((CEC_RC5_FRAME.ToggleBit != CEC_RC5_ToggleBit_Previous) && (RC5_Data !=0))
      { 
        FirstTimeIssued = NO;
        HDMI_CEC_TX_MessageStructure.Header = ((MyLogicalAddress << 4) | FollowerAddress);
        HDMI_CEC_TX_MessageStructure.Opcode = HDMI_CEC_OPCODE_USER_CONTROL_PRESSED;
        HDMI_CEC_TX_MessageStructure.Operande[0] = HDMI_CEC_RC5_CMD[CEC_RC5_FRAME.Command];
        HDMI_CEC_TX_MessageStructure.TxMessageLength = 0x1;
        TransmitStatus = HDMI_CEC_TransmitMessage(&HDMI_CEC_TX_MessageStructure);
        /* Retransmit message until 5 time */
        while ((TransmitStatus != HDMI_CEC_OK) && sendcount < 0x5)
        {
          sendcount++;
          TransmitStatus = HDMI_CEC_TransmitMessage(&HDMI_CEC_TX_MessageStructure);
        } 
        if (TransmitStatus == HDMI_CEC_OK)
        {
          LCD_SetBackColor(LCD_COLOR_GREEN);
          CEC_Display(LCD_LINE_7, "Send Status: Succeed");
        }
        else
        {
          LCD_SetBackColor(LCD_COLOR_RED);
          CEC_Display(LCD_LINE_7, "Send Status: Fail   ");
        }  
        
        CEC_RC5_ToggleBit_Previous = CEC_RC5_FRAME.ToggleBit;
      }
    }
    /* Display the received messages */
    while(ReceivedFrame)
    {
      CEC_Display_ReceiveInfoDisplay();
      /* Get the physical and logical address of new connected devices */
      HDMI_CEC_CommandCallBack();
    }          
  }	
}

/**
  * @brief  Select follower address.
  * @param  None
  * @retval The follower address
  */
uint8_t CEC_Display_SelectFollowerAddress(void)
{
  uint32_t KeyState = 0;
  static uint32_t address = 0;
  int8_t i = 0;
  while((Menu_ReadKey() != NOKEY)&& (Key_Button_Pressed == 0))
  {}
  if(Key_Button_Pressed) return 0;
  
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  do
  {
    i++;     
  }while(*(uint8_t *)(HDMI_CEC_Follower_String[i][1]) == '0'); 
  
  CEC_Display(LCD_LINE_9, HDMI_CEC_Follower_String[i][0]); 
  address = HDMI_CEC_Follower_ADD[i];  
  
  while(KeyState != SEL)
  {
    KeyState = Menu_ReadKey();
    if(Key_Button_Pressed)return 0;
    if(KeyState == LEFT)
    {    
      do
      {
        if(i == 0)
        {
          i = 13;
        }
        i--; 
      }while(*(uint8_t *)(HDMI_CEC_Follower_String[i][1]) == '0');
      
      CEC_Display(LCD_LINE_9, HDMI_CEC_Follower_String[i][0]);       
      address = HDMI_CEC_Follower_ADD[i]; 
    }
    else if (KeyState == RIGHT)
    {
    
      do
      {
        if(i == 12)
        { 
          i = -1;
        }
        i++; 
      }while(*(uint8_t *)(HDMI_CEC_Follower_String[i][1]) == '0');
      
      CEC_Display(LCD_LINE_9, HDMI_CEC_Follower_String[i][0]); 
      address = HDMI_CEC_Follower_ADD[i];
    }
    else if (KeyState == SEL)
    {
      address = HDMI_CEC_Follower_ADD[i];
    }
    else if (KeyState == NOKEY)
    {
    }
    /* Display the received messages */
    while(ReceivedFrame)
    {
      CEC_Display_ReceiveInfoDisplay();
      /* Get the physical and logical address  of new connected devices*/
      HDMI_CEC_CommandCallBack();
    }   
  }
  return address;
}

/**
  * @brief  Displays the string on the required row and COLUMN
  * @param  Row: the Line where to display the character shape.
  * @param  Column: start Column address.
  * @param  *ptr: pointer to string to display on LCD.
  * @retval None
  */
void CEC_Display_LCD_DisplayString(uint8_t Row, uint16_t Column, uint8_t *ptr)
{
  uint32_t i = 0;
  uint16_t refcolumn = Column;

  /* Send the string character by character on lCD */
  while ( (*ptr !=0) && (i < 20))
  {
    /* Check if the LCD is HX8347D Controller */
    if(LCDType == LCD_HX8347D)
    {
      /* Display one character on LCD */
      LCD_DisplayChar(Row, 320 - refcolumn, *ptr);
    }
    else
    {
      /* Display one character on LCD */
      LCD_DisplayChar(Row, refcolumn, *ptr);
    }
    
    /* Decrement the Column position by 16 */
    refcolumn -= 16;
    /* Point on the next character */
    ptr++;
    /* Increment the character counter */
    i++;
  }
}

/**
  * @brief  Displays a data byte on the LCD in Hex format.
  * @param  Data: the data to be displayed.
  * @param  Line: the line where the data will be displayed.
  * @param  Column: the Column where the data will be displayed.
  * @retval None
  */
void CEC_Display_DisplayHexToChar(uint8_t Data, uint16_t Line, uint16_t Column)
{
  /* Check if the LCD is HX8347D Controller */
  if(LCDType == LCD_HX8347D)
  {
    LCD_DisplayChar(Line, 320 -  (Column - 16), CEC_Display_HexToChar(Data));
    LCD_DisplayChar(Line, 320 - Column, CEC_Display_HexToChar(Data >> 4));
  }
  else
  {
    LCD_DisplayChar(Line, (Column - 16), CEC_Display_HexToChar(Data));
    LCD_DisplayChar(Line, Column, CEC_Display_HexToChar(Data >> 4));
  }
}

/**
  * @brief  Converts a data (four bits) to a character to be displayed  in Hex format.
  * @param  Data: byte to be converted.
  * @retval None
  */
uint8_t CEC_Display_HexToChar(uint8_t Data)
{
  Data &= 0xF;

  switch(Data)
  {
    case 0xA: return 0x41;
    case 0xB: return 0x42;
    case 0xC: return 0x43;
    case 0xD: return 0x44;
    case 0xE: return 0x45;
    case 0xF: return 0x46;
    default: return (Data + 0x30);
  }
}

/**
  * @brief  Displays the content of a buffer on the LCD in Hex format.
  * @param  DataBuffer: a pointer to the buffer to be displayed.
  * @param  BufferSize: the size of the buffer in byte.
  * @param  Line: the line where the data will be displayed.
  * @retval None
  */
void CEC_Display_DisplayBuffer(uint8_t *DataBuffer, uint8_t BufferSize, uint16_t Line)
{
  uint8_t i;
  uint16_t Column = LCD_COLUMN_5;
  uint16_t line = Line;
 
  CEC_Display_LCD_DisplayString(Line, LCD_COLUMN_0, "                    "); 
  CEC_Display_LCD_DisplayString(Line+24, LCD_COLUMN_0, "                    ");
  CEC_Display_LCD_DisplayString(Line+48, LCD_COLUMN_0, "                    "); 
  CEC_Display_LCD_DisplayString(Line, LCD_COLUMN_0, "Data:"); 

  for (i = 0; i < BufferSize; i++)
  { 
    CEC_Display_DisplayHexToChar(*(DataBuffer++), line, Column);
    Column -= 32;
    /* Check if the LCD is HX8347D Controller */
    if(LCDType == LCD_HX8347D)
    {
      LCD_DisplayChar(line, 320 -  Column, ',');
    }
    else
    {
      LCD_DisplayChar(line, Column, ',');
    }
    
    if (i == 4 || i == 0xA)
      {
      line += 24;
      Column = LCD_COLUMN_0;
    }
    Column -= 16;
  }
}

/**
  * @brief  Displays the CEC messages on either LCD or Hyperterminal.
  * @param  Line: the Line where to display the character shape .
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..9
  * @param  *ptr: pointer to string to display on LCD or Hyperterminal.
  * @retval None
  */
void CEC_Display (uint8_t Line, uint8_t *ptr)
{
  uint8_t *pointer = ptr;
  uint8_t line = Line;
  LCD_DisplayStringLine(line,  pointer);
}

/**
  * @brief  Displays the CEC received messages on the required row and COLUMN
  * @param  None
  * @retval None
  */
void CEC_Display_ReceiveInfoDisplay(void)
{
  /* If a frame has been received */
  if(ReceivedFrame)
  {
    ReceivedFrame = 0;
    LCD_SetBackColor(LCD_COLOR_BLUE);
    
    /* Check if the frame has been received correctly */
    if(ReceiveStatus)
    {
      LCD_SetBackColor(LCD_COLOR_GREEN);
      CEC_Display_LCD_DisplayString(LCD_LINE_0, LCD_COLUMN_10, "Succeeded");
    }
    else /* The receive was failed */
    {
      LCD_SetBackColor(LCD_COLOR_RED);
      CEC_Display_LCD_DisplayString(LCD_LINE_0, LCD_COLUMN_10, " Failed  ");
    }
    /* Display the receive status, the initiator address, the received data 
    frame and  the number of received bytes  */
    LCD_SetBackColor(LCD_COLOR_BLUE);
    CEC_Display_LCD_DisplayString(LCD_LINE_1, LCD_COLUMN_0, "Sender Address = ");
    CEC_Display_DisplayHexToChar ((uint8_t)((HDMI_CEC_RX_MessageStructPrivate.Header >> 0x4) & 0x0F), LCD_LINE_1, LCD_COLUMN_17);
    CEC_Display_LCD_DisplayString(LCD_LINE_2, LCD_COLUMN_0, "Number of bytes:    ");
    CEC_Display_DisplayHexToChar ((uint8_t)(HDMI_CEC_RX_MessageStructPrivate.RxMessageLength + 2), LCD_LINE_2, LCD_COLUMN_17);
    CEC_Display_LCD_DisplayString(LCD_LINE_3, LCD_COLUMN_0, "Message Opcode:     ");
    CEC_Display_DisplayHexToChar ((uint8_t)HDMI_CEC_RX_MessageStructPrivate.Opcode, LCD_LINE_3, LCD_COLUMN_17);    
    CEC_Display_DisplayBuffer((uint8_t*)HDMI_CEC_RX_MessageStructPrivate.Operande, (HDMI_CEC_RX_MessageStructPrivate.RxMessageLength), LCD_LINE_4);
  }
}

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
