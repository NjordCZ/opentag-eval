/**
  @page STM320518_EVAL_Demo UM1520 STM320518-EVAL demonstration firmware Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    STM320518-EVAL/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   Description of the UM1520 "STM320518-EVAL demonstration firmware".
  ******************************************************************************
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
   @endverbatim

@par Description

This directory contains a set of sources files and pre-configured projects that
describes the demonstration firmware running on the STM320518-EVAL evaluation 
board, which can be used to evaluate the capabilities of the STM32F0xx 
microcontroller and on-board peripherals.

For more details about this demonstration, refer to UM1520 "STM320518-EVAL demonstration firmware".

       
@par Hardware and Software environment  

  - This firmware runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM320518-EVAL (STM32F0xx)
    evaluation board RevB.

  - STM320518-EVAL Set-up
     - Before you execute the demonstration, make sure that all EVAL board jumpers 
       are well configured. For more details, refer to 
       Section "1.10.12 STM320518-EVAL board jumper configuration" in UM1520.
     
       
@par How to use it ?       
      
In order to load the demo code, you have do the following:
 - EWARM
    - Open the STM320518-EVAL_Demo.eww workspace
    - In the workspace toolbar select the project config:
      - STM320518-EVAL-CONFIG1: to load the demonstration CONFIG1
      - STM320518-EVAL-CONFIG2: to load the demonstration CONFIG2
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 - MDK-ARM 
    - Open the Project.Uv2 project
    - In the build toolbar select the project config:
      - STM320518-EVAL-CONFIG1: to load the demonstration CONFIG1
      - STM320518-EVAL-CONFIG2: to load the demonstration CONFIG2
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)

 - TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to  TASKING workspace directory and select the project: 
      - STM320518-EVAL-CONFIG1: to load the demonstration CONFIG1
      - STM320518-EVAL-CONFIG2: to load the demonstration CONFIG2      
     - Rebuild all project files: Select the project in the "Project explorer" 
       window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)

 - TrueSTUDIO   
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace 
      directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory and select the project: 
      - STM320518-EVAL-CONFIG1: to load the demonstration CONFIG1
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

