/**
  ******************************************************************************
  * @file    waveplayer.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file contains all the functions prototypes for the wave player
  *          firmware driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WAVEPLAYER_H
#define __WAVEPLAYER_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
@code 
  .WAV file format :
 Endian      Offset      Length      Contents
  big         0           4 bytes     'RIFF'             // 0x52494646
  little      4           4 bytes     <file length - 8>
  big         8           4 bytes     'WAVE'             // 0x57415645
Next, the fmt chunk describes the sample format:
  big         12          4 bytes     'fmt '          // 0x666D7420
  little      16          4 bytes     0x00000010      // Length of the fmt data (16 bytes)
  little      20          2 bytes     0x0001          // Format tag: 1 = PCM
  little      22          2 bytes     <channels>      // Channels: 1 = mono, 2 = stereo
  little      24          4 bytes     <sample rate>   // Samples per second: e.g., 22050
  little      28          4 bytes     <bytes/second>  // sample rate * block align
  little      32          2 bytes     <block align>   // channels * bits/sample / 8
  little      34          2 bytes     <bits/sample>   // 8 or 16
Finally, the data chunk contains the sample data:
  big         36          4 bytes     'data'        // 0x64617461
  little      40          4 bytes     <length of the data block>
  little      44          *           <sample data>
@endcode
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"


/** @addtogroup STM320518_EVAL_Demo
  * @{
  */

/** @addtogroup WAVEPLAYER
  * @{
  */

/** @defgroup WAVEPLAYER_Exported_Types
  * @{
  */

typedef enum
{
  LittleEndian,
  BigEndian
}Endianness;

typedef struct
{
  uint32_t  RIFFchunksize;
  uint16_t  FormatTag;
  uint16_t  NumChannels;
  uint32_t  SampleRate;
  uint32_t  ByteRate;
  uint16_t  BlockAlign;
  uint16_t  BitsPerSample;
  uint32_t  DataSize;
}
WAVE_FormatTypeDef;
typedef enum
{
  Valid_WAVE_File = 0,
  Unvalid_RIFF_ID,
  Unvalid_WAVE_Format,
  Unvalid_FormatChunk_ID,
  Unsupporetd_FormatTag,
  Unsupporetd_Number_Of_Channel,
  Unsupporetd_Sample_Rate,
  Unsupporetd_Bits_Per_Sample,
  Unvalid_DataChunk_ID,
  Unsupporetd_ExtraFormatBytes,
  Unvalid_FactChunk_ID
} ErrorCode;

typedef struct
{
  __IO uint8_t wavelist[14];
} WavePlayList;
/**
  * @}
  */



/** @defgroup Lasertag_Constants
  * @{
  */
#define Wave_Click              "/empty.WAV"
#define Wave_Shot               "/shot2.wav"
//#define Wave_Reloading          "/CLICK.WAV"
#define Wave_Reloading_Click1   "/empty.WAV"
#define Wave_Reloading_Click2   "/reload.WAV"
#define Wave_Reloading_No       "/empty.WAV"
#define Wave_Trigger_Press_No   "/empty.WAV"
#define Wave_System_On          "/buzz.WAV"



/**
  * @}
  */




/** @defgroup WAVEPLAYER_Exported_Constants
  * @{
  */
#define SpeechReadAddr         0x0  /* Speech wave start read address */


/* Audio Play STATUS */
#define AudioPlayStatus_STOPPED       0
#define AudioPlayStatus_PLAYING	      1
#define AudioPlayStatus_PAUSED        2	 

#define  CHUNK_ID                            0x52494646  /* correspond to the letters 'RIFF' */
#define  FILE_FORMAT                         0x57415645  /* correspond to the letters 'WAVE' */
#define  FORMAT_ID                           0x666D7420  /* correspond to the letters 'fmt ' */
#define  DATA_ID                             0x64617461  /* correspond to the letters 'data' */
#define  FACT_ID                             0x66616374  /* correspond to the letters 'fact' */
#define  WAVE_FORMAT_PCM                     0x01
#define  FORMAT_CHNUK_SIZE                   0x10
#define  CHANNEL_MONO                        0x01
#define  SAMPLE_RATE_8000                    8000
#define  SAMPLE_RATE_11025                   11025
#define  SAMPLE_RATE_22050                   22050
#define  SAMPLE_RATE_44100                   44100
#define  BITS_PER_SAMPLE_8                   8
#define  WAVE_DUMMY_BYTE                     0xA5   
#define  DAC_DHR8R1_Address                  0x40007410
#define  DAC_DHR12R1_Address                 0x40007408
#define  DAC_DHR12L1_Address                 0x4000740C
/**
  * @}
  */


/** @defgroup WAVEPLAYER_Exported_Macros
  * @{
  */
/**
  * @}
  */



/** @defgroup WAVEPLAYER_Exported_functions
  * @{
  */

void Wave_Player_Play_Handler(void);
void Menu_WavePlayerFunc(void);
void WavePlayer_Init(void);
uint8_t WavePlayer_Start(void);
void WavePlayer_Stop(void);
void WavePlayer_RePlay(void);
void WavePlayer_Pause(void);
void WavePointerUpdate(uint32_t value);
uint32_t Decrement_WaveDataLength(void);
void Set_WaveDataLength(uint32_t value);
ErrorCode Get_WaveFileStatus(void);
void Wave_Player_Play(char* FileName);
uint8_t LCD_Update(void);
uint32_t ReadUnit(uint8_t *buffer, uint8_t idx, uint8_t NbrOfBytes, Endianness BytesFormat);
void Media_List_Display(WavePlayList WaveTable[], uint8_t NumbWaves, uint8_t SelectedWaveIndex);
void Media_Visualization(uint8_t *buffer,uint16_t RGB_Code);
void ExtraCode_StateMachine(void) ;
void ExtraCode_Init(void);

#ifdef __cplusplus
}
#endif

#endif /*__WAVEPLAYER_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
  
/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE******/
