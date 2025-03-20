/********************************** (C) COPYRIGHT *******************************
 * File Name          : M5_Waveform_Configure.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
 * Description        : Waveform display channel configuration

*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Includes -----------------------------------------------------------------*/
#include "M0_Control_Library.h"
/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private functions --------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Variables ----------------------------------------------------------------*/
/*********************************************************************
 * @fn      Waveform_Display
 *
 * @brief   Waveform displays variable Settings
 *
 * @return  none
 */
__attribute__((section(".highcode")))
void Waveform_Display (void)
{
//    sendUsbData.sendDataBuf[sendUsbData.head+3] = (uint16_t)(Observer_M.Cal_Struc.Angspeed_Inst>>10);
    sendUsbData.sendDataBuf[sendUsbData.head+0] = (uint16_t)(Observer_M.Cal_Struc.PosiElecPU>>10);
    sendUsbData.sendDataBuf[sendUsbData.head+1] = (uint16_t)(MStruc_M.C2R.cd>>10);
    sendUsbData.sendDataBuf[sendUsbData.head+2] = (uint16_t)(Observer_M.Cal_Struc.CalphaNew>>10);
    sendUsbData.sendDataBuf[sendUsbData.head+3] = (uint16_t)(MStruc_M.C3S.PhaseA>>10);
    sendUsbData.sendDataBuf[sendUsbData.head+4] = (uint16_t)(MStruc_M.VRef2R.cq>>10);
    sendUsbData.sendDataBuf[sendUsbData.head+5] = (uint16_t)(MStruc_M.C2R.cq>>10);

    sendUsbData.head += MaxUsbDataLen;
}
