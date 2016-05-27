/**
  ******************************************************************************
  * @file    lasertag.c
  * @author  njord
  * @version V1.0.0
  * @date    15.10.2015
  * @brief   Lasertag game system.
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lasertag.h"

// Lasertag Private Variables
player_typedef player; // vlastnosti hrace
gun_typedef gun; // vlastnosti zbrane


uint8_t Fire_Redy = SET; // kontrola fire rate pri semi modu
uint8_t LCD_update_tmp; // kontrola fire rate pri semi modu

static uint8_t tmp_player_id = 0;
static uint8_t tmp_player_id2 = 0;
static uint8_t tmp_team = 0;
static uint8_t tmp_damage = 0;
static uint8_t tmp_reloading = 0;

void Add_Ammo(uint8_t add);
void Add_Health(uint8_t add);
void Command(uint8_t command);
void Admin_Kill(void);
void New_Game(void);

extern RTC_TimeTypeDef Time_Date_Time;

/**
  * @brief  Init lasertag seting.
  * @param  uint8_t FireRate
  * @retval None
  */
void Lasertag_Init(uint32_t FireRate)
{
  /////////////////////////
  // lasertag fire timer
  ////////////////////////
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  /* TIM15 clocks enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE); 
  
  /* Time Base configuration for lasertag fire timer*/
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = (60000/FireRate);
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 48000;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseInit(TIM15, &TIM_TimeBaseStructure);
  
  /* Enable the tim Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM15_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x03;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* TIM IT */
  TIM_ITConfig(TIM15, TIM_IT_Update, ENABLE);
  /* Clear the TIM15 Update pending bit */
  TIM_ClearITPendingBit(TIM15, TIM_IT_Update);
  /* TIM Disable */
  TIM_Cmd(TIM15, DISABLE);
  
  
  
}




/**
  * @brief  Init default lasertag seting.
  * @param  None
  * @retval None
  */
void Lasertag_Init_default(void)
{
  player.Armor = 0;
  player.AutoRespawn = 0;
  player.FrendlyFire = 1;
  player.Health = 100;
  player.HitInvulnerability = 0;
  player.HitStun = 0;
  player.PlayerID = 6;
  player.PlayerID2 = 10;
  player.PlayerTeam = RED;
  player.SpawnInvulnerability = 0;
  player.State = Live;
  
  gun.ClipsAmmoMax = 30;
  gun.ClipsAmmo = gun.ClipsAmmoMax;
  gun.ClipsAmountMax = 4;
  gun.ClipsAmount = gun.ClipsAmountMax;
  gun.Damage = Damage25;
  gun.FireRate = 600;
  gun.GunSystem = AutoSemi;
  gun.Reloading = 1;

  IR_ShotPocket_Calc(&player, &gun);


}

/**
  * @brief  Calc gun IR shot packet.
  * @param  &player_typedef  
  * @param  &gun_typedef
  * @retval None
  */

// vypocet a sestaveni IR paketu vystrelu hrace
void IR_ShotPocket_Calc(player_typedef *player, gun_typedef *gun)
{
  gun->ShotIR = 0;
  gun->ShotIR = (gun->ShotIR | player->PlayerID) << 2;
  gun->ShotIR = (gun->ShotIR | player->PlayerTeam) << 4;
  gun->ShotIR = (gun->ShotIR | gun->Damage) << 8;
  gun->ShotIR = (gun->ShotIR | player->PlayerID2); //<< 10;
  
  gun->ShotIR_NbBits = 22;
  
}


/**
  * @brief  Lasertag fire trigger - navesti
  * @param  none
  * @retval none
  */
void Trigger_Press(void)
{
  
  if(gun.ClipsAmmo > 0 && GPIO_ReadInputDataBit(MODE2_BUTTON_GPIO_PORT, MODE2_BUTTON_PIN) == SET 
     && player.State == Live && Fire_Redy == SET)
  {
  
  
    IR_Encode_Start(gun.ShotIR, gun.ShotIR_NbBits);
  
    gun.ClipsAmmo--;
  
    Fire_Redy = RESET;
    
    
    /* lasertag fire TIM Enable */
    TIM_Cmd(TIM15, ENABLE);
    
    
    Wave_Player_Play(Wave_Shot); // play wave from SD card, shot
    
  }
  else
  {
    Wave_Player_Play(Wave_Trigger_Press_No);  
  }
  
}


/**
  * @brief  Lasertag reloading button press
  * @param  none
  * @retval none
  */
void Reloading_press(void)
{
  if(gun.ClipsAmount > 0 && player.State == Live)
  {
    Wave_Player_Play(Wave_Reloading_Click1); // play wave from SD card
    tmp_reloading = gun.Reloading * 6;
    
    player.State = Reload;
  }
  else
  {
    Wave_Player_Play(Wave_Reloading_No); // play wave from SD card
  }
}

/**
  * @brief  Lasertag reloading timer
  * @param  none
  * @retval none
  */
void Reloading_timer(void)
{
  if(player.State == Reload)
  {
    if(tmp_reloading > 0)
    {
      tmp_reloading--;
    }
    else
    {
      player.State = Live;
      gun.ClipsAmount--;
      gun.ClipsAmmo = gun.ClipsAmmoMax;
      Wave_Player_Play(Wave_Reloading_Click2); // play wave from SD card
    }
  } 
}



/**
  * @brief  Lasertag fire timer - navesti
  * @param  none
  * @retval none
  */
void Lasertag_Fire_Timer(void)
{
  if (gun.ClipsAmmo > 0 && (GPIO_ReadInputDataBit(TRIGGER_BUTTON_GPIO_PORT, TRIGGER_BUTTON_PIN) == SET) 
      &&((gun.GunSystem == Auto) || (gun.GunSystem == AutoSemi 
      && GPIO_ReadInputDataBit(MODE1_BUTTON_GPIO_PORT, MODE1_BUTTON_PIN) == SET)))
  {
    
    IR_Encode_Start(gun.ShotIR, gun.ShotIR_NbBits);
  
    gun.ClipsAmmo--;
    
    Wave_Player_Play(Wave_Shot); // play wave from SD card, shot
    
    
  }
  else
  {
    
    TIM_Cmd(TIM15, DISABLE);
    
    Fire_Redy = SET;
    
    
  }
}




/**
  * @brief  Decode Lasertag data.
  * @param  data: lasertag data - ir, ....
  * @param  count: data lengh, bite count.
  * @retval none
  */
void Lasertag_DecodeData(uint32_t data, uint8_t count)
{
   tmp_player_id = 0;
   tmp_player_id2 = 0;
   tmp_team = 0;
   tmp_damage = 0;
  
  
  if((data >> (count)) == 0) // 0 = vystrel 
  {
    tmp_player_id = data >> (count - 7);
    
    tmp_team = (uint8_t)(data >> (count - 9));
    tmp_team &= 0x03;
  
    tmp_damage = (uint8_t)(data >> (count - 13));
    tmp_damage &= 0x0F;
  
    if(count >= 21)
    {
      tmp_player_id2 = (uint8_t)(data >> (count - 21));
      tmp_player_id2 &= 0xFF; 
    }
    else
    {
      tmp_player_id2 = 0; 
    }
    
    if(((tmp_team == (uint8_t)player.PlayerTeam) && (tmp_player_id == player.PlayerID))
       && ((tmp_player_id2 == 0) || (tmp_player_id2 == player.PlayerID2)))
    {
      // vlastni strelba
      return;
    }
    
    if(player.FrendlyFire == 0 && tmp_team == (uint8_t)player.PlayerTeam)
    {
      // strelba od vlastniho ktera je neskodna
      return;
    }
    
/*    
    // zasah vybuchem
    if(tmp_damage == DamageExplode)
    {
      
      if(player.Armor == 255)
      {
        // vybuch ale hrac je neznicitelny
        //Wave_Player_Play(Wave_Click);
        return;
      }
      else
      {
        // vybuch a zniceni hrace
        //Wave_Player_Play(Wave_Click);
        player.Health = 0;
        player.State = Death;
        return;
      }
      
    }
    
 */    
    
    // prevod damage
    switch((uint8_t)tmp_damage)
    {
      case Damage1 : tmp_damage = 1; break;
      case Damage2 : tmp_damage = 2; break;
      case Damage4 : tmp_damage = 4; break;
      case Damage5 : tmp_damage = 5; break;
      case Damage7 : tmp_damage = 7; break;
      case Damage10 : tmp_damage = 10; break;
      case Damage15 : tmp_damage = 15; break;
      case Damage17 : tmp_damage = 17; break;
      case Damage20 : tmp_damage = 20; break;
      case Damage25 : tmp_damage = 25; break;
      case Damage30 : tmp_damage = 30; break;
      case Damage35 : tmp_damage = 35; break;
      case Damage40 : tmp_damage = 40; break;
      case Damage50 : tmp_damage = 50; break;
      case Damage75 : tmp_damage = 75; break;
      case Damage100 : tmp_damage = 100; break;
      // case DamageExplode : tmp_damage = 0xFF; break; vyhodnoceni drive
      default : tmp_damage = 0; break;
    }
    
    
    if((player.Health - tmp_damage) <= 0)
    {
      // zniceni hrace, zivot 0
      //Wave_Player_Play(Wave_Click);
      player.Health = 0;
      player.State = Death;
      return;
    }
    else
    {
      
      // zasah ktery hrac prezil
      //Wave_Player_Play(Wave_Click);
      player.Health -= tmp_damage;
      return;
    }  
    
    
    
  }
  else // neni to vystrel
  {
    if(VALIDATION == (uint8_t)((data >> (count - 23)) & 0xFF)) // kontrola 3 bytu
    {
      switch((uint8_t)((data >> (count - 7)) & 0xFF))
      {
      case ADD_HEALTH : Add_Health((uint8_t)((data >> (count - 15)) & 0xFF));break;
      case ADD_AMMO : Add_Ammo((uint8_t)((data >> (count - 15)) & 0xFF));break;
      case SYSTEM : break;
      case CLIPS_PICKUP : Wave_Player_Play(Wave_Click); break;
      case HEALTH_PICKUP : Wave_Player_Play(Wave_Click); break;
      case FLAG_PICKUP : Wave_Player_Play(Wave_Click); break;
      case COMMAND : Command((uint8_t)((data >> (count - 15)) & 0xFF)); break;
      case TEAM_ID : Wave_Player_Play(Wave_Click); break;
      default : break; // neznama zprava
      }
                
    }
    else
    {
      // vadna zprava
    }
    
    
  }
  
  
  
    
    
    
    
    Wave_Player_Play(Wave_Click); 
  
}

////////////////////
// command decode
///////////////////
void Command(uint8_t command)
{
  switch(command)
  {
      case ADMIN_KILL : Admin_Kill(); break;
      case PAUSE : break;
      case START : break;
      case RESPAWN : break;
      case NEW_GAME : New_Game(); break;
      case FULL_AMMO :  break;
      case END_GAME : break;
      case EXPLODE :  break;
      case FULL_HEALTH :  break;
      case TEST :  break;
      case STUN :  break;
      case DISARM :  break;
      default : break; // neznama zprava
  }
}

//////////////////////////
// admin kill
//////////////////////////
void Admin_Kill(void)
{
  player.Health = 0;
  player.State = GameOver;
  Lasertag_LCD_Update();
  
}


//////////////////////////
// new game
//////////////////////////
void New_Game(void)
{
  
  player.State = Live;
  Lasertag_Init_default();
  Lasertag_LCD_Update();
}



//////////////////////////
// add player ammo clips
//////////////////////////
void Add_Ammo(uint8_t add)
{
  if((gun.ClipsAmount + add) > gun.ClipsAmountMax)
  {
    gun.ClipsAmount = gun.ClipsAmountMax;
  }
  else
  {
    gun.ClipsAmount =+ add;
  }

  Lasertag_LCD_Update();
}

///////////////////////
// add player health
//////////////////////
void Add_Health(uint8_t add)
{
  if((player.Health + add) > 100)
  {
    player.Health = 100;
  }
  else
  {
    player.Health =+ add;
  }

  Lasertag_LCD_Update();
}


/**
  * @brief  Lasertag LCD update.
  * @param  none
  * @retval none
  */
void Lasertag_LCD_Update(void)
{
    I2C_SSD1306_Fill(SSD1306_COLOR_BLACK);
  
    /* Go to location X = stred 5 znaku, Y = 4 */
    //I2C_SSD1306_GotoXY((128-(5*7))/2, 4); // stred horizontalne 
    
    
    RTC_GetTime(RTC_Format_BCD, &Time_Date_Time);
    I2C_SSD1306_GotoXY((127-(3*7)), 4);
    I2C_SSD1306_Putd(Time_Date_Time.RTC_Seconds, 3, &TM_Font_7x10, SSD1306_COLOR_WHITE);  
      
    I2C_SSD1306_GotoXY((127-(2*7)), 15);
    I2C_SSD1306_Putd(player.State, 2, &TM_Font_7x10, SSD1306_COLOR_WHITE);
           
    I2C_SSD1306_GotoXY((127-(9*11))/2, 25);
    
    I2C_SSD1306_Puts("{", &TM_Font_11x18, SSD1306_COLOR_WHITE);
    I2C_SSD1306_Putd(player.Health, 3, &TM_Font_11x18, SSD1306_COLOR_WHITE);
    I2C_SSD1306_Puts(" }", &TM_Font_11x18, SSD1306_COLOR_WHITE);
    I2C_SSD1306_Putd(gun.ClipsAmmo, 3, &TM_Font_11x18, SSD1306_COLOR_WHITE);
    
    I2C_SSD1306_GotoXY((127-(9*11))/2, 45);
    
    I2C_SSD1306_Puts("|", &TM_Font_11x18, SSD1306_COLOR_WHITE);
    I2C_SSD1306_Putd(player.Armor, 3, &TM_Font_11x18, SSD1306_COLOR_WHITE);
    I2C_SSD1306_Puts(" ~", &TM_Font_11x18, SSD1306_COLOR_WHITE);
    I2C_SSD1306_Putd(gun.ClipsAmount, 3, &TM_Font_11x18, SSD1306_COLOR_WHITE);
    
    /* Update screen, send changes to LCD */
    I2C_SSD1306_UpdateScreen();
}


/**
  * @brief  Lasertag update LCD - timer
  * @param  none
  * @retval none
  */
void LCD_update_timer(void)
{
  if(LCD_update_tmp >= 3)
  {
    Lasertag_LCD_Update();
    LCD_update_tmp = 0;
  }
  else
  {
     LCD_update_tmp++;
    
  } 
}


/**
  * @brief  Lasertag LCD init screen.
  * @param  none
  * @retval none
  */
void Lasertag_LCD_Init_Screen(char* str)
{
  I2C_SSD1306_Fill(SSD1306_COLOR_BLACK);
  I2C_SSD1306_GotoXY(10, 25);
  I2C_SSD1306_Puts(str, &TM_Font_11x18, SSD1306_COLOR_WHITE);
  /* Update screen, send changes to LCD */
  I2C_SSD1306_UpdateScreen();
  
  Delay(1000);
  Wave_Player_Play(Wave_System_On); // play wave from SD card
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

/****************************END OF FILE****/
