/********************************** (C) COPYRIGHT *******************************
 * File Name          : M9_Appication_Blocks.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/15
 * Description        : Appication funtion head file .
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __M9_APPLICATION_BLOCKS_H
#define __M9_APPLICATION_BLOCKS_H

/* Includes -----------------------------------------------------------------*/
/* Exported types -----------------------------------------------------------*/
/* Exported constants -------------------------------------------------------*/
/* Exported variables -------------------------------------------------------*/
/* Exported macro -----------------------------------------------------------*/
/* Exported functions--------------------------------------------------------*/
_iq24 LPF_1st_Process(LPF_First_Order_Type *LPF_1st, _iq24 input);

_iq24 HPF_1st_Process(HPF_First_Order_Type *HPF_1st, _iq24 input);

_iq24 Speedband_Avoid(_iq24 Speedinput,_iq24 Band,_iq24 Bandwidth);

void Speed_Ramp_Process(Speedrampctr_Type *Speed_Ramp);

_iq24 AverValue_Cal(_iq24 Input,_iq24 Output, BuffPara_Type *Buffpara);

_iq24 AverValue_Cal2(_iq24 *Buffer, BuffPara_Type *Buffpara);

void Buffer_Clear(_iq24 *Buffer, BuffPara_Type *BuffPara);

void Buffer_Update(_iq24 *Buffer, BuffPara_Type *BuffPara,_iq24 Updatevalue);

void Buffer_Set(_iq24 *Buffer, BuffPara_Type *BuffPara,_iq24 Setvalue);

void Saturation(_iq24 *input,_iq24 Upper,_iq24 Lower);

#endif
