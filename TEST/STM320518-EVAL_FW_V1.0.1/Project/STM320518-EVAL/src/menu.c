/**
  ******************************************************************************
  * @file    menu.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file includes the menu navigation driver for the STM320518-EVAL
  *          demonstration.
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
#include "main.h"

/** @addtogroup STM320518_EVAL_Demo
  * @{
  */

/** @defgroup MENU
  * @brief    This file includes the menu navigation driver for the STM320518-EVAL
  *           demonstration.
  * @{
  */

/** @defgroup MENU_Private_Types
  * @{
  */
typedef void (* tMenuFunc)(void);
typedef struct sMenuItem * tMenuItem;
typedef struct sMenu * tMenu;

/**
  * @}
  */


/** @defgroup MENU_Private_Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup MENU_Private_Macros
  * @{
  */


/**
  * @}
  */

/** @defgroup MENU_Private_Variables
  * @{
  */

uint8_t MenuItemIndex = 0, nMenuLevel = 0;
uint8_t ItemNumb[MAX_MENU_LEVELS];
uint8_t NumberOfFiles = 0;
uint32_t bmplen = 0;
uint8_t Buffer1[_MAX_SS];
UINT BytesWritten;
UINT BytesRead;
char* DirectoryFiles[MAX_FILES];
tMenuItem psMenuItem, psCurrentMenuItem;
tMenu psPrevMenu[MAX_MENU_LEVELS];
tMenu psCurrentMenu;
FIL F;
FATFS fs;
__IO uint8_t  VoiceRecStop = RESET;                /* Stop Voice Recorder  Flag */

uint8_t Key_Button_Pressed = 0;
__IO uint8_t sEEStatus = sEE_FAIL;

struct sMenuItem
{
  uint8_t* pszTitle;
  tMenuFunc pfMenuFunc;
  tMenuFunc pfUpDownMenuFunc;
  tMenu psSubMenu;
};

struct sMenu
{
  uint8_t* pszTitle;
  tMenuItem psItems;
  uint8_t nItems;
};

const uint8_t* Images1Addr[3] =
  {"STFILES/HELP.BMP", "STFILES/RF4.BMP", "STFILES/RF10.BMP"
  };

const uint8_t* Images2Addr[2] =
  {"STFILES/ALARM.BMP", "STFILES/WATCH.BMP",   
  };

const uint8_t* Images3Addr[10] =
  {"STFILES/ARROWD.BMP","STFILES/ARROWU.BMP","STFILES/WHITE.BMP",
   "STFILES/PROMO.BMP","STFILES/DISCOUNT.BMP","STFILES/MUSIC2.BMP",
   "STFILES/PLAY2.BMP","STFILES/RECC2.BMP", "STFILES/RECC.BMP",
   "STFILES/TV.BMP"
  };

const uint8_t* Images4Addr[11] =
  {"STFILES/LDR1.BMP", "STFILES/LDR1.BMP", "STFILES/LDR2.BMP",
   "STFILES/LDR3.BMP", "STFILES/LDR4.BMP", "STFILES/LDR5.BMP",
   "STFILES/LDR6.BMP", "STFILES/LDR7.BMP", "STFILES/LDR8.BMP",
   "STFILES/LDR9.BMP", "STFILES/LDR10.BMP"
  };

const uint8_t* Images5Addr[1] =
  {"STFILES/STLOGO.BMP"
  };

const uint8_t* IconsAddr[NUMBER_OF_ICONS] =
  {
#ifdef CALENDAR_SUBDEMO
    "STFILES/ICON2.BMP",
#endif /* CALENDAR_SUBDEMO */
#ifdef IMAGEVIEWER_SUBDEMO
    "STFILES/ICON3.BMP",
#endif /* IMAGEVIEWER_SUBDEMO */
#ifdef AUDIO_SUBDEMO
    "STFILES/ICON4.BMP",
#endif /* AUDIO_SUBDEMO */
#ifdef HDMICEC_SUBDEMO
    "STFILES/ICON5.BMP",
#endif /* HDMICEC_SUBDEMO */
#ifdef THERMOMETER_SUBDEMO
    "STFILES/ICON6.BMP",
#endif /* THERMOMETER_SUBDEMO */
#ifdef LOWPOWERMODE_SUBDEMO
    "STFILES/ICON7.BMP",
#endif /* LOWPOWERMODE_SUBDEMO */
#ifdef RFEEPROM_SUBDEMO
    "STFILES/ICON8.BMP",
#endif /* RFEEPROM_SUBDEMO */
#ifdef APPLICATION_SUBDEMO
    "STFILES/ICON9.BMP",
#endif /* APPLICATION_SUBDEMO */
#ifdef WAVERECORD_SUBDEMO
    "STFILES/ICON10.BMP",
#endif /* WAVERECORD_SUBDEMO */
#ifdef INFRALED_SUBDEMO
    "STFILES/ICON11.BMP",
#endif /* INFRALED_SUBDEMO */
#ifdef HELP_SUBDEMO
    "STFILES/ICON12.BMP",
#endif /* HELP_SUBDEMO */ 
    "STFILES/ICON13.BMP",
  };

const uint32_t DelayTiming[14] =
  {
    2650, 3020, 800, 2480, 4950, 2480, 3640, 3600, 1630, 3800, 2520, 3500, 2400, 1770
  };

const uint8_t SlidesCheck[6] =
  {
    0x42, 0x4D, 0x42, 0x58, 0x02, 0x00
  };
const uint8_t Icons64Check[6] =
  {
    0x42, 0x4D, 0x42, 0x20, 0x00, 0x00
  };

const uint8_t Icons128Check[6] =
  {
    0x42, 0x4D, 0x42, 0x80, 0x00, 0x00
  };

const uint8_t Icons80Check[6] =
  {
    0x42, 0x4D, 0x42, 0x32, 0x00, 0x00
  };

const uint8_t Icons170Check[6] =
  {
    0x42, 0x4D, 0x0A, 0xE2, 0x00, 0x00
  };

#define SCROLL_SPEED  2500 /* Low value gives higher speed */
#define SCROLL_NUM    1

/*------------------------------ Menu level 4 -------------------------------*/
/*------------------------------ Menu level 3 -------------------------------*/
#ifdef INFRALED_SUBDEMO
struct sMenuItem RFRECEIVERMenuItems[] =
  {
    {"        SIRC        ", Menu_SIRCDecode_Func, Menu_IdleFunc},
    {"        RC5         ", Menu_RC5Decode_Func, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };
struct sMenu RFRECEIVERMenu =
  {"     IR Receiver    ", RFRECEIVERMenuItems, countof(RFRECEIVERMenuItems)
  };

struct sMenuItem RFTRANSMITTERMenuItems[] =
  {
    {"        SIRC        ", Menu_SIRC12_Encode_Func, Menu_IdleFunc},
    {"        RC5         ", Menu_RC5_Encode_Func, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };
struct sMenu RFTRANSMITTERMenu =
  {"   IR Transmitter   ", RFTRANSMITTERMenuItems, countof(RFTRANSMITTERMenuItems)
  };
#endif /* INFRALED_SUBDEMO */

#ifdef LOWPOWERMODE_SUBDEMO
struct sMenuItem LOWPOWERSTOPMenuItems[] =
  {
    {"    Exit: EXTI      ", LowPower_EnterSTOPMode_EXTI, Menu_IdleFunc},
    {"  Exit: RTC Alarm   ", LowPower_EnterSTOPMode_RTCAlarm, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };
struct sMenu LOWPOWERSTOPMenu =
  {"     STOP Mode      ", LOWPOWERSTOPMenuItems, countof(LOWPOWERSTOPMenuItems)
  };
struct sMenuItem LOWPOWERSTANDBYMenuItems[] =
  {
    {"  Exit: Wakeup Pin  ", LowPower_EnterSTANDBYMode_WAKEUP_1, Menu_IdleFunc},
    {"  Exit: RTC Alarm   ", LowPower_EnterSTANDBYMode_RTCAlarm, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };
struct sMenu LOWPOWERSTANDBYMenu =
  {"   STANDBY Mode     ", LOWPOWERSTANDBYMenuItems, countof(LOWPOWERSTANDBYMenuItems)
  };
#endif /* LOWPOWERMODE_SUBDEMO */

#ifdef CALENDAR_SUBDEMO
struct sMenuItem SettingTimeMenuItems[] =
  {
    {"       Adjust       ", Calendar_TimeAdjust, Menu_IdleFunc},
    {"        Show        ", Calendar_TimeShow, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };

struct sMenu TimeMenu =
  {"        Time        ", SettingTimeMenuItems, countof(SettingTimeMenuItems)
  };

struct sMenuItem SettingDateMenuItems[] =
  {
    {"       Adjust       ", Calendar_DateAdjust, Menu_IdleFunc},
    {"        Show        ", Calendar_DateShow, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };

struct sMenu DateMenu =
  {"        Date        ", SettingDateMenuItems, countof(SettingDateMenuItems)
  };

struct sMenuItem SettingAlarmAMenuItems[] =
  {
    {"       Adjust       ", Calendar_AlarmAdjust_A, Menu_IdleFunc},
    {"        Show        ", Calendar_AlarmShow_A, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };

struct sMenu AlarmAMenu =
  {"       AlarmA       ", SettingAlarmAMenuItems, countof(SettingAlarmAMenuItems)
  };
#endif /* CALENDAR_SUBDEMO */
/*------------------------------ Menu level 2 -------------------------------*/
struct sMenuItem AboutMenuItems[] =
  {
    {"        About       ", Menu_AboutFunc, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };

struct sMenu AboutMenu =
  {"        About       ", AboutMenuItems, countof(AboutMenuItems)
  };

#ifdef HELP_SUBDEMO
struct sMenuItem HelpMenuItems[] =
  {
    {"    Menu Navigation ", Menu_MenuNavigationFunc, Menu_IdleFunc},
    {"    Jumpers Config  ", Menu_JumpConfFunc, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };

struct sMenu HelpMenu =
  {
    "        Help        ", HelpMenuItems, countof(HelpMenuItems)
  };
#endif /* HELP_SUBDEMO */

#ifdef INFRALED_SUBDEMO 
struct sMenuItem InfraRedMenuItems[] =
  {
    {"    IR Transmitter  ", Menu_IdleFunc, Menu_IdleFunc, &RFTRANSMITTERMenu },
    {"     IR Receiver    ", Menu_IdleFunc, Menu_IdleFunc, &RFRECEIVERMenu},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
    
  };

struct sMenu InfraRedMenu =
  {"      InfraRed      ", InfraRedMenuItems, countof(InfraRedMenuItems)
  };
#endif /* INFRALED_SUBDEMO */

#ifdef WAVERECORD_SUBDEMO 
struct sMenuItem WaveRecordMenuItems[] =
  {
    {"       Record       ", Menu_WaveRecorderFunc, Menu_IdleFunc},
    {"       Player       ", Menu_RecordedWavePlayFunc, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };

struct sMenu WaveRecordMenu =
  {"    Wave Record     ", WaveRecordMenuItems, countof(WaveRecordMenuItems)
  };
#endif /* WAVERECORD_SUBDEMO */

#ifdef APPLICATION_SUBDEMO
struct sMenuItem ApplicationsMenuItems[] =
  {
    {"     StopWatch      ", App_StopWatch_Start, Menu_IdleFunc},
    {"       Timer        ", App_LapTimer_Start, Menu_IdleFunc},
    {"        LDR         ", App_LightDependentResistor_Start, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };

struct sMenu ApplicaionsMenu =
  {"    Applications    ", ApplicationsMenuItems, countof(ApplicationsMenuItems)
  };
#endif /* APPLICATION_SUBDEMO */

#ifdef RFEEPROM_SUBDEMO
struct sMenuItem RFEepromMenuItems[] =
  {
    {"        ESL         ", RF_EEprom_ESLFunc, Menu_IdleFunc},
    {"    DataLogger (T)  ", RF_EEprom_DataLogger, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };

struct sMenu RFEepromMenu =
  {"     RF EEPROM      ", RFEepromMenuItems, countof(RFEepromMenuItems)
  };
#endif /* RFEEPROM_SUBDEMO */

#ifdef LOWPOWERMODE_SUBDEMO
struct sMenuItem LowPowerMenuItems[] =
  {
    {"        STOP        ", Menu_IdleFunc, Menu_IdleFunc, &LOWPOWERSTOPMenu
    },
    {"       STANDBY      ", Menu_IdleFunc, Menu_IdleFunc, &LOWPOWERSTANDBYMenu},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };
struct sMenu LowPowerMenu =
  {"    Low Power Mode  ", LowPowerMenuItems, countof(LowPowerMenuItems)
  };
#endif /* LOWPOWERMODE_SUBDEMO */

#ifdef THERMOMETER_SUBDEMO
struct sMenuItem TempSensorMenuItems[] =
  {
    {"    Temperature     ", Menu_ThermometerFun, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };

struct sMenu TempSensorMenu =
  {"    Thermometer     ", TempSensorMenuItems, countof(TempSensorMenuItems)
  };
#endif /* THERMOMETER_SUBDEMO */

#ifdef HDMICEC_SUBDEMO
struct sMenuItem CECDisplayMenuItems[] =
  {
    {"     HDMI CEC       ", Menu_CECDisplayFunc, Menu_IdleFunc
    },
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };

struct sMenu CECDisplayMenu =
  {"     HDMI CEC       ", CECDisplayMenuItems, countof(CECDisplayMenuItems)
  };
#endif /* HDMICEC_SUBDEMO */

#ifdef AUDIO_SUBDEMO
struct sMenuItem AudioMenuItems[] =
  {
    {"      WavePlayer    ", Menu_WavePlayerFunc, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };

struct sMenu AudioMenu =
  {"        Audio       ", AudioMenuItems, countof(AudioMenuItems)
  };
#endif /* AUDIO_SUBDEMO */

#ifdef IMAGEVIEWER_SUBDEMO
struct sMenuItem ImagesViewerMenuItems[] =
  {
    {"    Images Viewer   ", Menu_ImagesViewerFunc, Menu_IdleFunc},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };

struct sMenu ImagesViewerMenu =
  {"    Images Viewer   ", ImagesViewerMenuItems, countof(ImagesViewerMenuItems)
  };
#endif /* IMAGEVIEWER_SUBDEMO */

#ifdef CALENDAR_SUBDEMO
struct sMenuItem CalendarMenuItems[] =
  {
    {"        Time        ", Menu_IdleFunc, Menu_IdleFunc, &TimeMenu},
    {"        Date        ", Menu_IdleFunc, Menu_IdleFunc, &DateMenu},
    {"       AlarmA       ", Menu_IdleFunc, Menu_IdleFunc, &AlarmAMenu},
    {"       Return       ", Menu_ReturnFunc, Menu_IdleFunc}
  };

struct sMenu CalendarMenu =
  {"       Calendar     ", CalendarMenuItems, countof(CalendarMenuItems)
  };
#endif /* CALENDAR_SUBDEMO */

/*------------------------------ Menu level 1 -------------------------------*/
struct sMenuItem MainMenuItems[] =
  {
#ifdef CALENDAR_SUBDEMO
    {"       Calendar     ", Menu_IdleFunc, Menu_IdleFunc, &CalendarMenu},
#endif /* CALENDAR_SUBDEMO */
#ifdef IMAGEVIEWER_SUBDEMO
    {"    Images Viewer   ", Menu_IdleFunc, Menu_IdleFunc, &ImagesViewerMenu},
#endif /* IMAGEVIEWER_SUBDEMO */
#ifdef AUDIO_SUBDEMO
    {"        Audio       ", Menu_IdleFunc, Menu_IdleFunc, &AudioMenu},
#endif /* AUDIO_SUBDEMO */
#ifdef HDMICEC_SUBDEMO
    {"     HDMI CEC       ", Menu_IdleFunc, Menu_IdleFunc, &CECDisplayMenu},
#endif /* HDMICEC_SUBDEMO */
#ifdef THERMOMETER_SUBDEMO
    {"    Thermometer     ", Menu_IdleFunc, Menu_IdleFunc, &TempSensorMenu},
#endif /* THERMOMETER_SUBDEMO */
#ifdef LOWPOWERMODE_SUBDEMO
    {"    Low Power Mode  ", Menu_IdleFunc, Menu_IdleFunc, &LowPowerMenu},
#endif /* LOWPOWERMODE_SUBDEMO */
#ifdef RFEEPROM_SUBDEMO
    {"     RF EEPROM      ", Menu_IdleFunc, Menu_IdleFunc, &RFEepromMenu},
#endif /* RFEEPROM_SUBDEMO */
#ifdef APPLICATION_SUBDEMO
    {"    Applications    ", Menu_IdleFunc, Menu_IdleFunc, &ApplicaionsMenu},
#endif /* APPLICATION_SUBDEMO */
#ifdef WAVERECORD_SUBDEMO
    {"    Wave Record     ", Menu_IdleFunc, Menu_IdleFunc, &WaveRecordMenu},
#endif /* WAVERECORD_SUBDEMO */
#ifdef INFRALED_SUBDEMO
    {"      InfraRed      ", Menu_IdleFunc, Menu_IdleFunc, &InfraRedMenu},
#endif /* INFRALED_SUBDEMO */
#ifdef HELP_SUBDEMO
    {"        Help        ", Menu_IdleFunc, Menu_IdleFunc, &HelpMenu},
#endif /* HELP_SUBDEMO */
    {"        About       ", Menu_IdleFunc, Menu_IdleFunc, &AboutMenu}
  };

struct sMenu MainMenu =
  {"     Main menu      ", MainMenuItems, countof(MainMenuItems)
  };

/**
  * @}
  */

/** @defgroup MENU_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup MENU_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the navigation menu.
  * @param  None
  * @retval None
  */
void Menu_Init(void)
{
  psCurrentMenu = &MainMenu;
  psPrevMenu[nMenuLevel] = psCurrentMenu;
  psMenuItem = MainMenuItems;
}

/**
  * @brief  Displays the current menu.
  * @param  None
  * @retval None
  */
void Menu_DisplayMenu(void)
{
  uint32_t Line = 0, index = 0;

  tMenuItem psMenuItem2;
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  /* Clear the LCD Screen */
  LCD_Clear(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(Line, psCurrentMenu->pszTitle);
  Line += 24;
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLUE);
  while (!(index >= (psCurrentMenu->nItems)))
  {
    psMenuItem2 = &(psCurrentMenu->psItems[index]);
    LCD_DisplayStringLine(Line, psMenuItem2->pszTitle);
    index++;
    Line += 24;
  }
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_GREEN);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  /* Get the current menu */
  psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
  LCD_DisplayStringLine(((MenuItemIndex + 1) * 24), psMenuItem->pszTitle);
}

/**
  * @brief  This function is executed when "SEL" push-buttton is pressed.
  * @param  None
  * @retval None
  */
void Menu_SelFunc(void)
{
  psCurrentMenuItem = psMenuItem;
  if (psMenuItem->psSubMenu != '\0')
  {
    /* Update the current Item by the submenu */
    MenuItemIndex = 0;
    psCurrentMenu = psMenuItem->psSubMenu;
    psMenuItem = &(psCurrentMenu->psItems)[MenuItemIndex];
    Menu_DisplayMenu();
    nMenuLevel++;
    psPrevMenu[nMenuLevel] = psCurrentMenu;
  }
  psCurrentMenuItem->pfMenuFunc();
}

/**
  * @brief  This function is executed when any of "UP" push-butttons is pressed.
  * @param  None
  * @retval None
  */
void Menu_UpFunc(void)
{
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLUE);
  psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
  LCD_DisplayStringLine(((MenuItemIndex + 1) * 24), psMenuItem->pszTitle);
  if (MenuItemIndex > 0)
  {
    MenuItemIndex--;
  }
  else
  {
    MenuItemIndex = psCurrentMenu->nItems - 1;
  }
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_GREEN);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
  LCD_DisplayStringLine(((MenuItemIndex + 1) * 24), psMenuItem->pszTitle);
  ItemNumb[nMenuLevel] = MenuItemIndex;
}

/**
  * @brief  This function is executed when any of "Down" push-butttons is pressed.
  * @param  None
  * @retval None
  */
void Menu_DownFunc(void)
{
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLUE);
  psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
  LCD_DisplayStringLine(((MenuItemIndex + 1) * 24), psMenuItem->pszTitle);

  /* Test on the MenuItemIndex value before incrementing it */
  if (MenuItemIndex >= ((psCurrentMenu->nItems) - 1))
  {
    MenuItemIndex = 0;
  }
  else
  {
    MenuItemIndex++;
  }
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_GREEN);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  /* Get the current menu */
  psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
  LCD_DisplayStringLine(((MenuItemIndex + 1) * 24), psMenuItem->pszTitle);
  ItemNumb[nMenuLevel] = MenuItemIndex;
}

/**
  * @brief  This function is executed when the "RETURN" menu is selected.
  * @param  None
  * @retval None
  */
void Menu_ReturnFunc(void)
{
  psMenuItem->pfUpDownMenuFunc();
  if (nMenuLevel == 0)
  {
    nMenuLevel++;
  }
  psCurrentMenu = psPrevMenu[nMenuLevel-1];
  psMenuItem = &psCurrentMenu->psItems[0];
  ItemNumb[nMenuLevel] = 0;
  MenuItemIndex = 0;
  nMenuLevel--;
  if (nMenuLevel != 0)
  {
    Menu_DisplayMenu();
  }
  else
  {
    Menu_ShowMenuIcons();
  }
}

/**
  * @brief  Reads key from demoboard.
  * @param  None
  * @retval Return RIGHT, LEFT, SEL, UP, DOWN or NOKEY
  */
uint8_t Menu_ReadKey(void)
{
  /* "right" key is pressed */
  if (STM_EVAL_PBGetState(BUTTON_RIGHT))
  {
    while (STM_EVAL_PBGetState(BUTTON_RIGHT) == Bit_SET);
    return RIGHT;
  }
  /* "left" key is pressed */
  if (STM_EVAL_PBGetState(BUTTON_LEFT))
  {
    while (STM_EVAL_PBGetState(BUTTON_LEFT) == Bit_SET);
    return LEFT;
  }
  /* "up" key is pressed */
  if (STM_EVAL_PBGetState(BUTTON_UP))
  {
    while (STM_EVAL_PBGetState(BUTTON_UP) == Bit_SET);
    return UP;
  }
  /* "down" key is pressed */
  if (STM_EVAL_PBGetState(BUTTON_DOWN))
  {
    while (STM_EVAL_PBGetState(BUTTON_DOWN) == Bit_SET);
    return DOWN;
  }
  /* "sel" key is pressed */
  if (STM_EVAL_PBGetState(BUTTON_SEL))
  {
    while (STM_EVAL_PBGetState(BUTTON_SEL) == Bit_SET);
    return SEL;
  }
  /* No key is pressed */
  else
  {
    return NOKEY;
  }
}

/**
  * @brief  Idle function.
  * @param  None
  * @retval None
  */
void Menu_IdleFunc(void)
{
  /* Nothing to execute: return */
  return;
}

/**
  * @brief  Display menu icons.
  * @param  None
  * @retval None
  */
void Menu_DisplayIcons(void)
{
  uint32_t i = 0, j = 0, l = 0,  iconline = 0, iconcolumn = 0;

  iconline = 98;
  iconcolumn = 290 ;

  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 4; j++)
    {
      LCD_SetDisplayWindow(iconline, iconcolumn, 64, 64);
      Storage_OpenReadFile(iconline, iconcolumn, (const char*)IconsAddr[l]);
      iconcolumn -= 65;
      l++;
      if (l == NUMBER_OF_ICONS )
      {
        return;
      }
    }
    iconline += 65;
    iconcolumn = 290;
  }
}

/**
  * @brief  Display the STM32 introduction.
  * @param  None
  * @retval None
  */
void Menu_STM32Intro(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Enable the EXTI4_15_IRQn Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  
  
  /* Enable the EXTI4_15_IRQn Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  LCD_Clear(LCD_COLOR_WHITE);
  
    /* Set Back and Text color */
  LCD_SetBackColor(LCD_COLOR_BLUE2);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  
  LCD_DisplayStringLine(LCD_LINE_0, "   STM320518-EVAL   "); 
  LCD_DisplayStringLine(LCD_LINE_1, DEMO_CONFIG_PART);
    
  
  LCD_SetDisplayWindow(220, 245, 170, 170);
  Storage_OpenReadFile(220, 245, "STFILES/STLOGO.BMP");
  LCD_WindowModeDisable();

  
  /* Insert delay */
  Demo_Delay(170);
  
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLUE);


}

/**
  * @brief  Show the main menu icon.
  * @param  None
  * @retval None
  */
void Menu_ShowMenuIcons(void)
{
  uint32_t pressedkey = 0, i = 0;
  uint16_t IconRect[12][2] = {{34, 290}, {34, 225}, {34, 160}, {34, 95},
                              {99, 290}, {99, 225}, {99, 160}, {99, 95},
                              {164, 290}, {164, 225}, {164, 160}, {164, 95}};

  /* Disable the JoyStick interrupts */
  Demo_IntExtOnOffCmd(DISABLE);

  while (Menu_ReadKey() != NOKEY)
  {}

  /* Initializes the Menu state machine */
  Menu_Init();

  MenuItemIndex = 0;

  /* Clear*/
  LCD_Clear(LCD_COLOR_WHITE);

  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);

  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);

  LCD_DisplayStringLine(LCD_LINE_0, psMenuItem->pszTitle);

  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);

  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLUE);

  /* Displays Icons */
  Menu_DisplayIcons();

  /* Disable LCD Window mode */
  LCD_WindowModeDisable();

  LCD_DrawRect(IconRect[0][0], IconRect[0][1], 64, 65);

  /* Endless loop */
  while (1)
  {
    /* Check which key is pressed */
    pressedkey = Menu_ReadKey();

    /* If "UP" pushbutton is pressed */
    if (pressedkey == UP)
    {
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_WHITE);

      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 64, 65);

      if (i <= 3)
      {
        i += 8;
        if (i >= NUMBER_OF_ICONS)
        {
          i = (NUMBER_OF_ICONS - 1);
        }
      }
      else
      {
        i -= 4;
      }
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_BLUE);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 64, 65);

      /* Set the Back Color */
      LCD_SetBackColor(LCD_COLOR_BLUE);
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_WHITE);
      /* Test on the MenuItemIndex value before incrementing it */
      if (MenuItemIndex <= 3)
      {
        MenuItemIndex += 8;
        if (MenuItemIndex >= NUMBER_OF_ICONS)
        {
          MenuItemIndex = (NUMBER_OF_ICONS - 1);
        }
      }
      else
      {
        MenuItemIndex -= 4;
      }
      /* Get the current menu */
      psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
      LCD_DisplayStringLine(LCD_LINE_0, psMenuItem->pszTitle);
      ItemNumb[nMenuLevel] = MenuItemIndex;
    }
    /* If "DOWN" pushbutton is pressed */
    if (pressedkey == DOWN)
    {
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_WHITE);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 64, 65);
      if (i >= 8)
      {
        i -= 8;
      }
      else
      {
        i += 4;
        if (i >= NUMBER_OF_ICONS)
        {
          i = (NUMBER_OF_ICONS - 1);
        }
      }
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_BLUE);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 64, 65);

      /* Set the Back Color */
      LCD_SetBackColor(LCD_COLOR_BLUE);
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_WHITE);
      /* Test on the MenuItemIndex value before incrementing it */
      if (MenuItemIndex >= 8)
      {
        MenuItemIndex -= 8;
      }
      else
      {
        MenuItemIndex += 4;
        if (MenuItemIndex >= NUMBER_OF_ICONS)
        {
          MenuItemIndex = (NUMBER_OF_ICONS - 1);
        }
      }
      /* Get the current menu */
      psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
      LCD_DisplayStringLine(LCD_LINE_0, psMenuItem->pszTitle);
      ItemNumb[nMenuLevel] = MenuItemIndex;
    }
    /* If "LEFT" pushbutton is pressed */
    if (pressedkey == LEFT)
    {
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_WHITE);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 64, 65);
      if (i == 0)
      {
        i = (NUMBER_OF_ICONS - 1);
      }
      else
      {
        i--;
      }
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_BLUE);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 64, 65);

      /* Set the Back Color */
      LCD_SetBackColor(LCD_COLOR_BLUE);
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_WHITE);
      if (MenuItemIndex > 0)
      {
        MenuItemIndex--;
      }
      else
      {
        MenuItemIndex = psCurrentMenu->nItems - 1;
      }

      psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
      LCD_DisplayStringLine(LCD_LINE_0, psMenuItem->pszTitle);
      ItemNumb[nMenuLevel] = MenuItemIndex;
    }
    /* If "RIGHT" pushbutton is pressed */
    if (pressedkey == RIGHT)
    {
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_WHITE);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 64, 65);
      if (i == (NUMBER_OF_ICONS - 1))
      {
        i = 0x00;
      }
      else
      {
        i++;
      }
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_BLUE);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 64, 65);

      /* Set the Back Color */
      LCD_SetBackColor(LCD_COLOR_BLUE);
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_WHITE);
      /* Test on the MenuItemIndex value before incrementing it */
      if (MenuItemIndex >= ((psCurrentMenu->nItems) - 1))
      {
        MenuItemIndex = 0;
      }
      else
      {
        MenuItemIndex++;
      }
      /* Get the current menu */
      psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
      LCD_DisplayStringLine(LCD_LINE_0, psMenuItem->pszTitle);
      ItemNumb[nMenuLevel] = MenuItemIndex;
    }
    /* If "SEL" pushbutton is pressed */
    if (pressedkey == SEL)
    {
      Menu_SelFunc();
      Demo_IntExtOnOffCmd(ENABLE);
      return;
    }
  }
}

/**
  * @brief  Display the Images stored on the SDcard.
  * @param  None
  * @retval None
  */
void Menu_ImagesViewerFunc(void)
{
  uint8_t str[20], pressedkey = 0;
  int8_t i = 0;

  LCD_Clear(LCD_COLOR_WHITE);
  
  Demo_IntExtOnOffCmd(DISABLE);

  while (Menu_ReadKey() != NOKEY)
  {}

  for (i = 0; i < MAX_FILES; i++)
  {
    DirectoryFiles[i] = malloc(13); /** Initialize the DirectoryFiles pointers (heap) */
  }

  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);

  NumberOfFiles = Storage_GetDirectoryBitmapFiles ("/USER", DirectoryFiles); /** Get the BMP file names on root directory  Maximum 10 files */
  i = 0;

  if (NumberOfFiles == 0)
  {
    for (i = 0; i < MAX_FILES; i++)
    {
      free(DirectoryFiles[i]);
    }
    LCD_Clear(LCD_COLOR_WHITE);
    
    /* Set the Back Color */
    LCD_SetBackColor(LCD_COLOR_BLACK);
    /* Set the Text Color */
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DisplayStringLine(LCD_LINE_0, "No Bitmap files...  ");
    LCD_DisplayStringLine(LCD_LINE_1, "Exit:  Push JoyStick");
    while (Menu_ReadKey() == NOKEY)
    {}
    LCD_Clear(LCD_COLOR_WHITE);
    Menu_DisplayMenu();
    Demo_IntExtOnOffCmd(ENABLE);
    f_mount(0, &fs);
    f_open (&F, (const char*)IconsAddr[0], FA_READ);
    return;
  }
  while (i < NumberOfFiles)
  {
    sprintf ((char*)str, "/USER/%-11.11s", DirectoryFiles[i]);/** Format the string */
    if (Storage_CheckBitmapFile((const char*)str, &bmplen) == 0)
    {
      LCD_SetDisplayWindow(239, 0x13F, 240, 320);
      Storage_OpenReadFile(239, 319, (const char*)str);
      pressedkey = Demo_DelayJoyStick(1000);
      if (pressedkey == LEFT)
      {
        if (i == 0)
        {
          i = NumberOfFiles;
        }
        i--;
      }
      else if (pressedkey == RIGHT)
      {
        if (i == (NumberOfFiles - 1))
        {
          i = -1;
        }
        i++;
      }
      else if (pressedkey == SEL)
      {
        i = NumberOfFiles;
      }
      else if (pressedkey == NOKEY)
      {
        i++;
      }
    }
    else
    {
      LCD_Clear(LCD_COLOR_WHITE);
      /* Set the Back Color */
      LCD_SetBackColor(LCD_COLOR_BLUE);
      /* Set the Text LCD_COLOR_WHITE */
      LCD_SetTextColor(LCD_COLOR_WHITE);
      LCD_ClearLine(LCD_LINE_3);
      LCD_DisplayStringLine(LCD_LINE_3, (uint8_t *) str);
      LCD_DisplayStringLine(LCD_LINE_4, "Bitmap file is not  ");
      LCD_DisplayStringLine(LCD_LINE_5, "supported.          ");
      LCD_DisplayStringLine(LCD_LINE_6, "Press JoyStick to   ");
      LCD_DisplayStringLine(LCD_LINE_7, "continue.           ");
      pressedkey = Demo_DelayJoyStick(100);
      if (pressedkey == LEFT)
      {
        if (i == 0)
        {
          i = NumberOfFiles;
        }
        i--;
      }
      else if (pressedkey == RIGHT)
      {
        if (i == (NumberOfFiles - 1))
        {
          i = -1;
        }
        i++;
      }
      else if (pressedkey == SEL)
      {
        i = NumberOfFiles;
      }
      else if (pressedkey == NOKEY)
      {
        i++;
      }
    }
  }

  for (i = 0; i < MAX_FILES; i++)
  {
    free(DirectoryFiles[i]);
  }

  LCD_Clear(LCD_COLOR_WHITE);
  Menu_DisplayMenu();
  Demo_IntExtOnOffCmd(ENABLE);
}

/**
  * @brief  Checks if the bitmapfiles (slides + icons) are already loaded in the
  *         SD Card FLASH.
  * @param  None
  * @retval None
  */
uint32_t Menu_CheckBitmapFiles(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  uint32_t err = 0;
  uint32_t i = 0x300;
  uint8_t j = 0;

  LCD_Clear(LCD_COLOR_WHITE);
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  
  
  /*------------------- FAT Initialization -------------------------------*/
  do
  {
    SD_LowLevel_DeInit();

    /** SDCard Initialisation for FatFs                            */
    err = Storage_Init();
  }
  while ((err != 0) && (i-- != 0));

  /* SD Card not formatted */
  if (i == 0)
  {
    LCD_DisplayStringLine(LCD_LINE_1, "SDCard not formatted");
    LCD_DisplayStringLine(LCD_LINE_1, "Reprogram your card.");
    while (1)
    {}
  }
  
  /* Check icons if available on SD card under STFILES directory*/
  for (j = 0; j < NUMBER_OF_ICONS; j++)
  {
    if (f_open (&F, (const char*)IconsAddr[j], FA_READ))
    {
      return 1;
    }
    f_read (&F, Buffer1, _MAX_SS, &BytesRead);
    if (Menu_Buffercmp((uint8_t *)Icons64Check,  Buffer1, 6) != 0)
    {
      return 2;
    }
  }
   /* Check required images size 320x240 if available on SD card under STFILES directory */
  for (j = 0; j < 3; j++)
  {
    if (f_open (&F, (const char*)Images1Addr[j], FA_READ))
    {
      return 5;
    }

    f_read (&F, Buffer1, _MAX_SS, &BytesRead);
    if (Menu_Buffercmp((uint8_t *)SlidesCheck,  Buffer1, 6) != 0)
    {
      return 6;
    }
  }
  
  /* Check required images size 128x128 if available on SD card under STFILES directory*/
  for (j = 0; j < 2; j++)
  {
    if (f_open (&F, (const char*)Images2Addr[j], FA_READ))
    {
      return 5;
    }

    f_read (&F, Buffer1, _MAX_SS, &BytesRead);
    if (Menu_Buffercmp((uint8_t *)Icons128Check,  Buffer1, 6) != 0)
    {
      return 6;
    }
  }
  
  /* Check icons (64x64) under STFILES directory: required for COMP demo application */
  for (j = 0; j < 10; j++)
  {
    if (f_open (&F, (const char*)Images3Addr[j], FA_READ))
    {
      return 7;
    }

    f_read (&F, Buffer1, _MAX_SS, &BytesRead);
    if (Menu_Buffercmp((uint8_t *)Icons64Check,  Buffer1, 6) != 0)
    {
      return 9;
    }
  }

  
  /* Check icons (170x170) under STFILES directory */
  for (j = 0; j < 1; j++)
  {
    if (f_open (&F, (const char*)Images5Addr[j], FA_READ))
    {
      return 7;
    }

    f_read (&F, Buffer1, _MAX_SS, &BytesRead);
    if (Menu_Buffercmp((uint8_t *)Icons170Check,  Buffer1, 6) != 0)
    {
      return 9;
    }
  }
  
#ifdef APPLICATION_SUBDEMO
  
  /* Check LDR images (80x80) if available on SD card under STFILES directory: required for COMP demo application */
  for (j = 0; j < 11; j++)
  {
    if (f_open (&F, (const char*)Images4Addr[j], FA_READ))
    {
      return 10;
    }

    f_read (&F, Buffer1, _MAX_SS, &BytesRead);
    if (Menu_Buffercmp((uint8_t *)Icons80Check,  Buffer1, 6) != 0)
    {
      return 11;
    }
  }
#endif /* APPLICATION_SUBDEMO */

  return 0;
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length.
  * @retval  0: pBuffer1 identical to pBuffer2
  *          1: pBuffer1 differs from pBuffer2
  */
uint8_t Menu_Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t sEE_TIMEOUT_UserCallback(void)
{
  sEEStatus = sEE_FAIL;
  
  /* Clear STOP flag */
  I2C_ClearFlag(I2C1, I2C_FLAG_STOPF);
  
  /* Clear Acknowledge failure flag */
  I2C_ClearFlag(I2C1, I2C_FLAG_NACKF);
  
  return 0;
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
