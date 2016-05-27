/**
  ******************************************************************************
  * @file    lasertag.h
  * @author  njord
  * @version V1.0.0
  * @date    15.10.2015
  * @brief   Lasertag game system
  *          
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LASERTAG_H
#define __LASERTAG_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
//#include "stm32f0xx.h"

  
// milestag 2 team  
typedef enum {
  RED = 0,
  BLUE = 1,
  YELLOW = 2,
  GREEN = 3
} TEAM;



// player state
typedef enum {
  GameOver,
  Death,
  Stun,
  Live,
  Spawn,
  Reload,
  AutoRespawn  
} PLAYERSTATE;  


// Gun system
typedef enum {
  Auto,
  AutoSemi,
  Semi,
  Manual
} GUNSYSTEM;  

// milestag 2 demage
typedef enum {
	Damage1 = 0,
	Damage2 = 1,
	Damage4 = 2,
	Damage5 = 3,
	Damage7 = 4,
	Damage10 = 5,
	Damage15 = 6,
	Damage17 = 7,
	Damage20 = 8,
	Damage25 = 9,
	Damage30 = 10,
	Damage35 = 11,
	Damage40 = 12,
	Damage50 = 13,
	Damage75 = 14,
	Damage100 = 15,
}DAMAGE;  
  
// milestag 2, 2+, player  
typedef struct 
{
  uint8_t PlayerID; // lasertag milestag 2 player id 0 + 7bit, 0-127 ID
  uint8_t PlayerID2; // milestag2+  8bit, 1-255 0 = not use PlayerID2, ID+ID2 0-32767 ID 
  
  PLAYERSTATE State; // stav hrace
  TEAM PlayerTeam; //milestag team
  uint8_t Health; // lasertag 0-100 procent zdravi
  uint8_t Armor; // lasertag 0-100 procent armor, milestag2+ 100 - znicitelny jen vybuchem 255 - neznicitelny
  
  uint8_t FrendlyFire; // 1 - on, 0 - off
  uint8_t SpawnInvulnerability; // sec nesmrtenlost
  uint16_t HitInvulnerability; // milisec nesmrtelnost
  uint16_t HitStun; // milisec omraceni
  uint16_t AutoRespawn; // sec automaticke oziveni, 0 - off
  
  uint8_t TypeID; // prednastavene schemata hracu a struktur 
  
} player_typedef; 

// milestag 2 gun structure
typedef struct 
{
  uint32_t ShotIR;
  uint8_t  ShotIR_NbBits; //
  
  DAMAGE Damage; // poskozeni zpusobene vystrelem
  uint16_t ClipsAmmoMax; // kapacita zasobniku
  uint16_t ClipsAmountMax; // pocet zasobniku maximalni
  uint16_t ClipsAmount; // aktualni pocet zasobniku
  uint16_t ClipsAmmo; // pocet naboju v zasobniku
  uint16_t FireRate; // vystrelu za minutu
  uint16_t Reloading; // rychlost prebijeni sec.
  GUNSYSTEM GunSystem; // system zbrane
  
  uint8_t IrPower; // výkon IR záøièe - dostøel
  uint8_t TypeID; // prednastavene schemata zbrani 
  
} gun_typedef;





typedef struct 
{
  uint8_t MedicBoxAmount; // pocet zasobniku
} medicbox_typedef; 
  
typedef struct 
{
  uint8_t ClipsAmount; // pocet zasobniku
} ammobox_typedef;   
  
 
///////////////////////////
// command message defines
///////////////////////////

// 1. byte packet
#define COMMAND  0x83
#define ADD_HEALTH  0x80
#define ADD_AMMO  0x81
#define SYSTEM  0x87
#define TEAM_ID  0x88    // milestag2+ zmena tymu
#define CLIPS_PICKUP  0x8A
#define HEALTH_PICKUP  0x8B	
#define FLAG_PICKUP  0x8C

// 2. byte command packet
#define ADMIN_KILL  0x00
#define PAUSE  0x01
#define START  0x02
#define RESPAWN  0x04
#define NEW_GAME  0x05
#define FULL_AMMO  0x06
#define END_GAME  0x07
#define EXPLODE  0x0B
#define FULL_HEALTH  0x0D
#define TEST  0x15
#define STUN  0x16
#define DISARM  0x17

// 3. byte validation
#define VALIDATION 0xE8



/** @defgroup Lasertag_Exported_Functions
  * @{
  */



void Lasertag_Init(uint32_t FireRate);
void Lasertag_Init_default(void);
void IR_ShotPocket_Calc(player_typedef *player, gun_typedef *gun);
void Trigger_Press(void);
void Lasertag_DecodeData(uint32_t data, uint8_t count);
void Lasertag_LCD_Update(void);
void Lasertag_Fire_Timer(void);
void Reloading_press(void);
void Reloading_timer(void);
void Lasertag_LCD_Init_Screen(char* str);
void LCD_update_timer(void);

#ifdef __cplusplus
}
#endif

#endif /* __LASERTAG_H */
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
