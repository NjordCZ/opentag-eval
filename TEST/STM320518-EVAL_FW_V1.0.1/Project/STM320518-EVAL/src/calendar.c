/**
  ******************************************************************************
  * @file    calendar.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file includes the calendar driver for the STM320518-EVAL
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

/** @defgroup CALENDAR
  * @brief    This file includes the calendar driver for the STM320518-EVAL
  *           demonstration.
  * @{
  */

/** @defgroup CALENDAR_Private_Types
  * @{
  */
/* Time Structure definition */
struct time_t
{
  uint8_t sec_l;
  uint8_t sec_h;
  uint8_t min_l;
  uint8_t min_h;
  uint8_t hour_l;
  uint8_t hour_h;
};
struct time_t time_struct;

/* Alarm Structure definition */
struct alarm_t
{
  uint8_t sec_l;
  uint8_t sec_h;
  uint8_t min_l;
  uint8_t min_h;
  uint8_t hour_l;
  uint8_t hour_h;
};
struct alarm_t alarm_struct;

/* Date Structure definition */
struct date_t
{
  uint8_t month;
  uint8_t day;
  uint16_t year;
};
struct date_t date_s;

/**
  * @}
  */

/** @defgroup CALENDAR_Private_Defines
  * @{
  */

  
/**
  * @}
  */

/** @defgroup CALENDAR_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup CALENDAR_Private_Variables
  * @{
  */
static uint32_t wn = 0, dn = 0, dc = 0;
static uint16_t daycolumn = 0, dayline = 0;

uint8_t MonLen[12] =
  {
    32, 29, 32, 31, 32, 31, 32, 32, 31, 32, 31, 32
  };
uint8_t MonthNames[12][20] =
  {"  JANUARY           ", "  FEBRUARY          ", "  MARCH             ",
   "  APRIL             ", "  MAY               ", "  JUNE              ",
   "  JULY              ", "  AUGUST            ", "  SEPTEMBER         ",
   "  OCTOBER           ", "  NOVEMBER          ", "  DECEMBER          "
  };
  
extern __IO uint32_t LCDType;
/**
  * @}
  */


/** @defgroup CALENDAR_Private_FunctionPrototypes
  * @{
  */
static uint8_t Calendar_IsLeapYear(uint16_t nYear);
static void Calendar_WeekDayNum(uint32_t nyear, uint8_t nmonth, uint8_t nday);
static void Calendar_RegulateYear(void);
static void Calendar_RegulateMonth(void);
static void Calendar_RegulateDay(void);
static void Calendar_TimePreAdjust(void);
static void Calendar_DatePreAdjust(void);
/**
  * @}
  */

/** @defgroup CALENDAR_Private_Functions
  * @{
  */

/**
  * @brief  Reads digit entered by user, using menu navigation keys.
  * @param  None
  * @retval Digit value
  */
uint8_t Calendar_ReadDigit(uint8_t ColBegin, uint8_t CountBegin, uint8_t ValueMax, uint8_t ValueMin)
{
  uint32_t MyKey = 0, tmpValue = 0;

  

  /* Initialize tmpValue */
  tmpValue = CountBegin;
  
  }

  

/**
  * @brief  Initializes calendar application.
  * @param  None
  * @retval None
  */
void Calendar_Init(void)
{
  //uint32_t pressedkey = 0;
  RTC_DateTypeDef   RTC_DateStructure;

  /* Initialize Date structure */
  date_s.month = 01;
  date_s.day = 01;
  date_s.year = 2012;

  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5AA5)
  {
    
    
  }
  else
  {
    /* PWR and BKP clocks selection ------------------------------------------*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
    date_s.year  = ((uint16_t)RTC_DateStructure.RTC_Year + (uint16_t)2000);
    date_s.month =  RTC_DateStructure.RTC_Month;
    date_s.day   =  RTC_DateStructure.RTC_Date;

    daycolumn = (uint16_t)((uint32_t)RTC_ReadBackupRegister(RTC_BKP_DR4) >> 16);
    dayline = (uint16_t)RTC_ReadBackupRegister(RTC_BKP_DR4);
  }
}

/**
  * @brief  Sets the time entered by user, using menu vavigation keys.
  * @param  None
  * @retval None
  */
void Calendar_TimeRegulate(void)
{
  RTC_TimeTypeDef   RTC_TimeStructure;
  uint32_t Tmp_HH = 0, Tmp_MM = 0, Tmp_SS = 0;

  /* Read time hours */
  Tmp_HH = Calendar_ReadDigit(244, time_struct.hour_h, 0x2, 0x0);

  if (Tmp_HH == 2)
  {
    if (time_struct.hour_l > 3)
    {
      time_struct.hour_l = 0;
    }
    Tmp_HH = Tmp_HH * 10 + Calendar_ReadDigit(228, time_struct.hour_l, 0x3, 0x0);
  }
  else
  {
    Tmp_HH = Tmp_HH * 10 + Calendar_ReadDigit(228, time_struct.hour_l, 0x9, 0x0);
  }
  /* Read time  minutes */
  Tmp_MM = Calendar_ReadDigit(196, time_struct.min_h, 5, 0x0);
  Tmp_MM = Tmp_MM * 10 + Calendar_ReadDigit(182, time_struct.min_l, 0x9, 0x0);
  /* Read time seconds */
  Tmp_SS = Calendar_ReadDigit(150, time_struct.sec_h, 5, 0x0);
  Tmp_SS = Tmp_SS * 10 + Calendar_ReadDigit(134, time_struct.sec_l, 0x9, 0x0);

  /* Set the Time */
  RTC_TimeStructure.RTC_Hours   = Tmp_HH;
  RTC_TimeStructure.RTC_Minutes = Tmp_MM;
  RTC_TimeStructure.RTC_Seconds = Tmp_SS;
  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
}

/**
  * @brief  Displays information on LCD for the time configuration
  * @param  None
  * @retval None
  */
static void Calendar_TimePreAdjust(void)
{
  
  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5AA5)
  {
    

    /* RTC Configuration */
    Calendar_RTC_Configuration();

    

    /* Display the current time */
    Calendar_TimeDisplay();

    /* Change the current time */
    Calendar_TimeRegulate();

    RTC_WriteBackupRegister(RTC_BKP_DR0, 0x5AA5);

    

    /* Adjust Date */
    Calendar_DatePreAdjust();
  }
  else
  {
    

    /* Display the current time */
    Calendar_TimeDisplay();

    /* Change the current time */
    Calendar_TimeRegulate();
  }
}

/**
  * @brief  Adjusts time.
  * @param  None
  * @retval None
  */
void Calendar_TimeAdjust(void)
{
  
  /* PreAdjust Time */
  Calendar_TimePreAdjust();
  
}

/**
  * @brief  Displays the current time.
  * @param  None
  * @retval None
  */
void Calendar_TimeDisplay(void)
{
  RTC_TimeTypeDef   RTC_TimeStructure;

  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);

  /* Display time hours */
  time_struct.hour_h = (uint8_t)RTC_TimeStructure.RTC_Hours / 10;
  time_struct.hour_l = (uint8_t)(RTC_TimeStructure.RTC_Hours % 10);

    
  
   // LCD_DisplayChar(LCD_LINE_8, 76, (time_struct.hour_h + 0x30));
   // LCD_DisplayChar(LCD_LINE_8, 92, (time_struct.hour_l + 0x30));
  

  /* Display time minutes */
  time_struct.min_h = (uint8_t) RTC_TimeStructure.RTC_Minutes / 10;
  time_struct.min_l = (uint8_t) RTC_TimeStructure.RTC_Minutes % 10;

    
  //  LCD_DisplayChar(LCD_LINE_8, 124, (time_struct.min_h + 0x30));
  //  LCD_DisplayChar(LCD_LINE_8, 138, (time_struct.min_l + 0x30));
  
  /* Display time seconds */
  time_struct.sec_h = (uint8_t)RTC_TimeStructure.RTC_Seconds / 10;
  time_struct.sec_l = (uint8_t)RTC_TimeStructure.RTC_Seconds % 10;
  /* Check if the LCD is HX8347D Controller */
  
  //  LCD_DisplayChar(LCD_LINE_8, 170, (time_struct.sec_h + 0x30));
 //  LCD_DisplayChar(LCD_LINE_8, 186, (time_struct.sec_l + 0x30));
  
  
}

/**
  * @brief  Shows the current time (HH/MM/SS) on LCD.
  * @param  None
  * @retval None
  */
void Calendar_TimeShow(void)
{
  uint32_t testvalue = 0, pressedkey = 0;

  

  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5AA5)
  {
    

    /* Wait a press on any JoyStick pushbuttons */
    
  }
  else
  {
    
    
    
  }
  
}

/**
  * @brief  Sets the date entered by user, using menu navigation keys.
  * @param  None
  * @retval None
  */
void Calendar_DateRegulate(void)
{
  RTC_DateTypeDef   RTC_DateStructure;
  
  
  /*Freeze DR */
  (void)RTC->DR;
  /* Regulate year */
  Calendar_RegulateYear();
  RTC_DateStructure.RTC_Year  = (uint8_t)((uint16_t)date_s.year - (uint16_t)2000);
  RTC_DateStructure.RTC_Month = date_s.month;
  RTC_DateStructure.RTC_Date  = date_s.day;
  RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Monday;
  RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);

  

  /* Regulate the month */
  Calendar_RegulateMonth();
  RTC_DateStructure.RTC_Year  = (uint8_t)((uint16_t)date_s.year - (uint16_t)2000);
  RTC_DateStructure.RTC_Month = date_s.month;
  RTC_DateStructure.RTC_Date  = date_s.day;
  RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Monday;
  RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);

  
  /* Regulate day */
  Calendar_RegulateDay();
    
  RTC_DateStructure.RTC_Year  = (uint8_t)((uint16_t)date_s.year - (uint16_t)2000);
  RTC_DateStructure.RTC_Month = date_s.month;
  RTC_DateStructure.RTC_Date  = date_s.day;
  RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Monday;
  RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);  
}

/**
  * @brief  Pre-Adjusts the current date (MM/DD/YYYY).
  * @param  None
  * @retval None
  */
static void Calendar_DatePreAdjust(void)
{
  uint32_t tmp = 0, tmp1 = 0, pressedkey = 0;

  

  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5AA5)
  {
    
    
  }
  else
  {
    /* Display the current date */
    Calendar_DateDisplay(date_s.year, date_s.month, date_s.day);

    /* Change the current date */
    Calendar_DateRegulate();

    RTC_WriteBackupRegister(RTC_BKP_DR2, date_s.year);

    tmp = date_s.month << 8;
    tmp |= date_s.day;
    RTC_WriteBackupRegister(RTC_BKP_DR3, tmp);
    tmp1 = (uint32_t)(((uint32_t)daycolumn << 16) | (uint32_t)dayline);
    RTC_WriteBackupRegister(RTC_BKP_DR4, tmp1);
  }
}

/**
  * @brief  Adjusts the current date (MM/DD/YYYY).
  * @param  None
  * @retval None
  */
void Calendar_DateAdjust(void)
{
  
  /* Preadjust the date */
  Calendar_DatePreAdjust();

  
}

/**
  * @brief  Displays the date in graphic mode.
  * @param  None
  * @retval None
  */
void Calendar_DateDisplay(uint16_t nYear, uint8_t nMonth, uint8_t nDay)
{
  __IO uint32_t mline = 0, mcolumn = 319, month = 0;
  __IO uint32_t monthlength = 0;
  
   if (nMonth == 2)
  {
    if (Calendar_IsLeapYear(nYear))
    {
      monthlength = 30;
    }
    else
    {
      monthlength = MonLen[nMonth - 1];
    }
  }
  else
  {
    monthlength = MonLen[nMonth - 1];
  }
  

  // LCD_DisplayStringLine(LCD_LINE_0, MonthNames[nMonth - 1]);
  
  
  //  LCD_DisplayChar(LCD_LINE_0, 225, ((nYear / 1000) + 0x30));
  //  LCD_DisplayChar(LCD_LINE_0, 241, (((nYear % 1000) / 100) + 0x30));
  //  LCD_DisplayChar(LCD_LINE_0, 257, ((((nYear % 1000) % 100) / 10) + 0x30));
  //  LCD_DisplayChar(LCD_LINE_0, 273, ((((nYear % 1000) % 100) % 10) + 0x30));
 
  Calendar_WeekDayNum(nYear, nMonth, nDay);
  
  
  
    if (wn / 10)
    {
     // LCD_DisplayChar(LCD_LINE_1, 97, ((wn / 10) + 0x30));
     // LCD_DisplayChar(LCD_LINE_1, 113, ((wn % 10) + 0x30));
    }
    else
    {
     // LCD_DisplayChar(LCD_LINE_1, 97, ((wn % 10) + 0x30));
    }
    
    if (dc / 100)
    {
     // LCD_DisplayChar(LCD_LINE_1, 273, ((dc / 100) + 0x30));
     // LCD_DisplayChar(LCD_LINE_1, 289, (((dc % 100) / 10) + 0x30));
     // LCD_DisplayChar(LCD_LINE_1, 305, (((dc % 100) % 10) + 0x30));
    }
    else if (dc / 10)
    {
     //LCD_DisplayChar(LCD_LINE_1, 273, ((dc / 10) + 0x30));
      //LCD_DisplayChar(LCD_LINE_1, 289, ((dc % 10) + 0x30));
    }
    else
    {
      //LCD_DisplayChar(LCD_LINE_1, 273, ((dc % 10) + 0x30));
    }
  
  
  
  

  /* Determines the week number, day of the week of the selected date */
  Calendar_WeekDayNum(nYear, nMonth, 1);
//  mline = LCD_LINE_3;
  
  
    mcolumn = 0x13F - (0x30 * dn);
    for (month = 1; month < monthlength; month++)
    {
      if (month == nDay)
      {
        daycolumn = mcolumn;
        dayline = mline;
      }
      if (month / 10)
      {
       // LCD_DisplayChar(mline, 320 - mcolumn, ((month / 10) + 0x30));
      }
      else
      {
     //   LCD_DisplayChar(mline, 320 - mcolumn, ' ');
      }
      mcolumn -= 16;
     // LCD_DisplayChar(mline, 320 - mcolumn, ((month % 10) + 0x30));
      if (mcolumn < 31)
      {
        mcolumn = 319;
        mline += 24;
      }
      else
      {
        mcolumn -= 16;
        //LCD_DisplayChar(mline, 320 - mcolumn, ' ');
        mcolumn -= 16;
      }
    
  }
  
  
  
 // LCD_DrawRect(dayline, daycolumn, 24, 32);
}

/**
  * @brief  Shows date in graphic mode on LCD.
  * @param  None
  * @retval None
  */
void Calendar_DateShow(void)
{
  uint32_t tmpValue = 0;
  uint32_t MyKey = 0, ValueMax = 0;
  uint32_t firstdaycolumn = 0, lastdaycolumn = 0, lastdayline = 0;
  uint32_t testvalue = 0, pressedkey = 0;
  RTC_DateTypeDef   RTC_DateStructure;

  

  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5AA5)
  {
    

  }
  

  /* Freeze DR */
  (void)RTC->DR;
  
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
  date_s.year  = ((uint16_t)RTC_DateStructure.RTC_Year + (uint16_t)2000);
  date_s.month =  RTC_DateStructure.RTC_Month;
  date_s.day   =  RTC_DateStructure.RTC_Date;

  /* Display the current date */
  Calendar_DateDisplay(date_s.year, date_s.month, date_s.day);
}

  
/**
  * @brief  Updates date when time is 23:59:59.
  * @param  None
  * @retval None
  */
void Calendar_DateUpdate(void)
{
  if (date_s.month == 1 || date_s.month == 3 || date_s.month == 5 || date_s.month == 7 ||
      date_s.month == 8 || date_s.month == 10 || date_s.month == 12)
  {
    if (date_s.day < 31)
    {
      date_s.day++;
    }
    /* Date structure member: date_s.day = 31 */
    else
    {
      if (date_s.month != 12)
      {
        date_s.month++;
        date_s.day = 1;
      }
      /* Date structure member: date_s.day = 31 & date_s.month =12 */
      else
      {
        date_s.month = 1;
        date_s.day = 1;
        date_s.year++;
      }
    }
  }
  else if (date_s.month == 4 || date_s.month == 6 || date_s.month == 9 ||
           date_s.month == 11)
  {
    if (date_s.day < 30)
    {
      date_s.day++;
    }
    /* Date structure member: date_s.day = 30 */
    else
    {
      date_s.month++;
      date_s.day = 1;
    }
  }
  else if (date_s.month == 2)
  {
    if (date_s.day < 28)
    {
      date_s.day++;
    }
    else if (date_s.day == 28)
    {
      /* Leap year */
      if (Calendar_IsLeapYear(date_s.year))
      {
        date_s.day++;
      }
      else
      {
        date_s.month++;
        date_s.day = 1;
      }
    }
    else if (date_s.day == 29)
    {
      date_s.month++;
      date_s.day = 1;
    }
  }
}

/**
  * @brief  Sets the year.
  * @param  None
  * @retval None
  */
static void Calendar_RegulateYear(void)
{
  __IO uint16_t tmpValue = 0;
  uint32_t MyKey = 0;

  /* Initialize tmpValue */
  tmpValue = date_s.year;

  
      /* Decrease the value of the digit */
      if (tmpValue == 2099)
      {
        tmpValue = 2000;
        /* Display new value */
        Calendar_DateDisplay(tmpValue, date_s.month, date_s.day);
      }
      else
      {
        /* Display new value */
        Calendar_DateDisplay(++tmpValue, date_s.month, date_s.day);
      }
   
      /* Decrease the value of the digit */
      if (tmpValue == 2000)
      {
        tmpValue = 2099;
        /* Display new value */
        Calendar_DateDisplay(tmpValue, date_s.month, date_s.day);
      }
      else
      {
        /* Display new value */
        Calendar_DateDisplay(--tmpValue, date_s.month, date_s.day);
      }
    
      
      /* SET Display new value */
      Calendar_DateDisplay(tmpValue, date_s.month, date_s.day);
      /* Return the digit value and exit */
      date_s.year = tmpValue;
      return;
    
  
}

/**
  * @brief  Sets month.
  * @param  None
  * @retval None
  */
static void Calendar_RegulateMonth(void)
{
  __IO uint8_t tmpValue = 0;
  uint32_t MyKey = 0;
  RTC_DateTypeDef   RTC_DateStructure;

  RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
  date_s.year  = ((uint16_t)RTC_DateStructure.RTC_Year + (uint16_t)2000);
  date_s.month =  RTC_DateStructure.RTC_Month;
  date_s.day   =  RTC_DateStructure.RTC_Date;

  /* Initialize tmpValue */
  tmpValue = date_s.month;

  
      /* Increase the value of the digit */
      if (tmpValue == 12)
      {
        tmpValue = 0;
      }
      
      Calendar_DateDisplay(date_s.year, ++tmpValue, date_s.day);
    
    
      /* Decrease the value of the digit */
      if (tmpValue == 1)
      {
        tmpValue = 13;
      }
     
      /* Display new value */
      Calendar_DateDisplay(date_s.year, --tmpValue, date_s.day);
   
    /* If "SEL" pushbutton is pressed */
    
      /* Return the digit value and exit */
      date_s.month = tmpValue;
      /* Display new value */
      Calendar_DateDisplay(date_s.year, tmpValue, date_s.day);

      return;
    
   
}

/**
  * @brief  Sets day.
  * @param  None
  * @retval None
  */
static void Calendar_RegulateDay(void)
{
  __IO uint8_t tmpValue = 0;
  uint32_t MyKey = 0, ValueMax = 0;
  uint32_t firstdaycolumn = 0, lastdaycolumn = 0, lastdayline = 0;
  RTC_DateTypeDef   RTC_DateStructure;

  RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
  date_s.year  = ((uint16_t)RTC_DateStructure.RTC_Year + (uint16_t)2000);
  date_s.month =  RTC_DateStructure.RTC_Month;
  date_s.day   =  RTC_DateStructure.RTC_Date;

  if (date_s.month == 2)
  {
    if (Calendar_IsLeapYear(date_s.year))
      ValueMax = 29;
    else
      ValueMax = (MonLen[date_s.month - 1] - 1);
  }
  else
  {
    ValueMax = (MonLen[date_s.month - 1] - 1);
  }
  firstdaycolumn = 0x13F - (0x30 * dn);
  lastdaycolumn = ValueMax - (7 - dn);
  lastdayline = lastdaycolumn / 7;
  lastdaycolumn %= 7;
  if (lastdaycolumn == 0)
  {
  //  lastdayline = LCD_LINE_3 + (lastdayline * 24);
    lastdaycolumn = 31;
  }
  else
  {
    //astdayline = LCD_LINE_4 + (lastdayline * 24);
    lastdaycolumn = 0x13F - (0x30 * (lastdaycolumn - 1));
  }

  /* Initialize tmpValue */
  tmpValue = date_s.day;

  
    //  LCD_DrawRect(dayline, daycolumn, 24, 32);

      /* Increase the value of the digit */
      if (tmpValue == ValueMax)
      {
        tmpValue = 0;
     //   dayline = LCD_LINE_3;
        daycolumn = firstdaycolumn + 48;
      }

      if (daycolumn == 31)
      {
        daycolumn = 367;
        dayline += 24;
      }
      daycolumn -= 48;
     
     // LCD_DrawRect(dayline, daycolumn, 24, 32);
      tmpValue++;
    
    
     
      //LCD_DrawRect(dayline, daycolumn, 24, 32);

      /* Decrease the value of the digit */
      if (tmpValue == 1)
      {
        tmpValue = ValueMax + 1;
        dayline = lastdayline;
        daycolumn = lastdaycolumn - 48;
      }

      if (daycolumn == 319)
      {
        daycolumn = 0xFFEF;
        dayline -= 24;
      }
      daycolumn += 48;
     
      //LCD_DrawRect(dayline, daycolumn, 24, 32);
      tmpValue--;
    
      
     // LCD_DrawRect(dayline, daycolumn, 24, 32);

      if (tmpValue == 1)
      {
        dayline = lastdayline;
        daycolumn =  lastdaycolumn;
        tmpValue = ValueMax;
      }
      else if (tmpValue < 8)
      {
        tmpValue = 1;
     //   dayline = LCD_LINE_3;
        daycolumn = firstdaycolumn;
      }
      else
      {
        dayline -= 24;
        tmpValue -= 7;
      }
      
     // LCD_DrawRect(dayline, daycolumn, 24, 32);
    
    /* If "DOWN" pushbutton is pressed */
    
     
    //  LCD_DrawRect(dayline, daycolumn, 24, 32);
      if (tmpValue == ValueMax)
      {
       // dayline = LCD_LINE_3;
        daycolumn =  firstdaycolumn;
        tmpValue = 1;
      }
      else
      {
        dayline += 24;
        tmpValue += 7;
      }
      if (tmpValue > ValueMax)
      {
        tmpValue = ValueMax;
        dayline = lastdayline;
        daycolumn = lastdaycolumn;
      }
      
    //  LCD_DrawRect(dayline, daycolumn, 24, 32);
    
    /* If "SEL" pushbutton is pressed */
    
      /* Return the digit value and exit */
      date_s.day = tmpValue;
      return;
    
  
}

/**
  * @brief  Determines the week number, the day number and the week day number.
  * @param  None
  * @retval None
  */
static void Calendar_WeekDayNum(uint32_t nyear, uint8_t nmonth, uint8_t nday)
{
  uint32_t a = 0, b = 0, c = 0, s = 0, e = 0, f = 0, g = 0, d = 0;
  int32_t n = 0;
  if (nmonth < 3)
  {
    a = nyear - 1;
  }
  else
  {
    a = nyear;
  }

  b = (a / 4) - (a / 100) + (a / 400);
  c = ((a - 1) / 4) - ((a - 1) / 100) + ((a - 1) / 400);
  s = b - c;
  if (nmonth < 3)
  {
    e = 0;
    f =  nday - 1 + 31 * (nmonth - 1);
  }
  else
  {
    e = s + 1;
    f = nday + (153 * (nmonth - 3) + 2) / 5 + 58 + s;
  }
  g = (a + b) % 7;
  d = (f + g - e) % 7;
  n = f + 3 - d;
  if (n < 0)
  {
    wn = 53 - ((g - s) / 5);
  }
  else if (n > (364 + s))
  {
    wn = 1;
  }
  else
  {
    wn = (n / 7) + 1;
  }
  dn = d;
  dc = f + 1;
}

/**
  * @brief  Check whether the passed year is Leap or not.
  * @param  None
  * @retval 1: leap year
  *         0: not leap year
  */
static uint8_t Calendar_IsLeapYear(uint16_t nYear)
{
  if (nYear % 4 != 0) return 0;
  if (nYear % 100 != 0) return 1;
  return (uint8_t)(nYear % 400 == 0);
}

/**
  * @brief  Sets the alarm time entered by user, using demoboard keys.
  * @param  None
  * @retval None
  */
void Calendar_AlarmRegulate_A(void)
{
  uint32_t Alarm_HH = 0, Alarm_MM = 0, Alarm_SS = 0;
  RTC_AlarmTypeDef  RTC_AlarmStructure;

  /* Read alarm hours */
  Alarm_HH = Calendar_ReadDigit(244, alarm_struct.hour_h, 0x2, 0x0);
  if (Alarm_HH == 0x2)
  {
    if (alarm_struct.hour_l > 0x3)
    {
      alarm_struct.hour_l = 0x0;
    }
    Alarm_HH = Alarm_HH * 10 + Calendar_ReadDigit(228, alarm_struct.hour_l, 0x3, 0x0);
  }
  else
  {
    Alarm_HH = Alarm_HH * 10 + Calendar_ReadDigit(228, alarm_struct.hour_l, 0x9, 0x0);
  }
  /* Read alarm minutes */
  Alarm_MM = Calendar_ReadDigit(196, alarm_struct.min_h, 0x5, 0x0);
  Alarm_MM = Alarm_MM * 10 + Calendar_ReadDigit(182, alarm_struct.min_l, 0x9, 0x0);
  /* Read alarm seconds */
  Alarm_SS = Calendar_ReadDigit(150, alarm_struct.sec_h, 0x5, 0x0);
  Alarm_SS = Alarm_SS * 10 + Calendar_ReadDigit(134, alarm_struct.sec_l, 0x9, 0x0);

  /* Disable the alarm A */
  RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

  /* Set the alarm X+5s */
  RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = Alarm_HH;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = Alarm_MM;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = Alarm_SS;
  RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
  RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
  RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
  RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);

  RTC_ITConfig(RTC_IT_ALRA, ENABLE);
  /* Enable the alarm */
  RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
}

/**
  * @brief  Configures an alarm event to occurs within the current day.
  * @param  None
  * @retval None
  */
void Calendar_AlarmPreAdjust_A(void)
{
  

  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5AA5)
  {
    
    return;
  }

  

  /* Display the alarm value */
  Calendar_AlarmDisplay_A();

  /* Store new alarm value */
  Calendar_AlarmRegulate_A();
}

/**
  * @brief  Adjusts an alarm event to occurs within the current day.
  * @param  None
  * @retval None
  */
void Calendar_AlarmAdjust_A(void)
{
  
  Calendar_AlarmPreAdjust_A();
  
}

/**
  * @brief  Displays alarm time.
  * @param  None
  * @retval None
  */
void Calendar_AlarmDisplay_A(void)
{
  RTC_AlarmTypeDef   RTC_AlarmStructure;

  RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);

  /* Display alarm hours */
  alarm_struct.hour_h = (uint8_t)RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours / 10;
  alarm_struct.hour_l = (uint8_t)RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours % 10;
  
   //LCD_DisplayChar(LCD_LINE_8, 76, (alarm_struct.hour_h + 0x30));
   // LCD_DisplayChar(LCD_LINE_8, 92, (alarm_struct.hour_l + 0x30));
  
 
  /* Display alarm minutes */
  alarm_struct.min_h = (uint8_t)RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes / 10;
  alarm_struct.min_l = (uint8_t)RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes % 10;
    
  //  LCD_DisplayChar(LCD_LINE_8, 124, (alarm_struct.min_h + 0x30));
  //  LCD_DisplayChar(LCD_LINE_8, 138, (alarm_struct.min_l + 0x30));
  
  /* Display alarm seconds */
  alarm_struct.sec_h = (uint8_t)RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds / 10;
  alarm_struct.sec_l = (uint8_t)RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds % 10;
    
  //  LCD_DisplayChar(LCD_LINE_8, 170, (alarm_struct.sec_h + 0x30));
  //  LCD_DisplayChar(LCD_LINE_8, 186, (alarm_struct.sec_l + 0x30));
  
}

/**
  * @brief  Shows alarm time (HH/MM/SS) on LCD.
  * @param  None
  * @retval None
  */
void Calendar_AlarmShow_A(void)
{
  
  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5AA5)
  {
    
    return;
  }

  
  /* Display actual alarm value */
  Calendar_AlarmDisplay_A();
  
}

/**
  * @brief  Configures the RTC.
  * @param  None
  * @retval None
  */
void Calendar_RTC_Configuration(void)
{
  RTC_InitTypeDef   RTC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  RCC_BackupResetCmd(ENABLE);
  RCC_BackupResetCmd(DISABLE);

  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5AA5)
  {
    /*!< LSE Enable */
    RCC_LSEConfig(RCC_LSE_ON);

    /*!< Wait till LSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {}

    /*!< LCD Clock Source Selection */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();

    /*Calender Configuartion*/
    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
    RTC_InitStructure.RTC_SynchPrediv =  0xFF;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    RTC_Init(&RTC_InitStructure);
    RTC_WriteBackupRegister(RTC_BKP_DR0, 0x5AA5);
  }
  else
  {
    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();
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
