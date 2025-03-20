/********************************** (C) COPYRIGHT *******************************
 * File Name          : M9_Appication_Blocks.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
 * Description        : Standard application module declaration

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
/* Exported macro -----------------------------------------------------------*/
/* Exported functions--------------------------------------------------------*/
_iq24 PID_Calculaion_Delta(_iq24 hReference, _iq24 hFeedback, PID_Structure_Type *PID_Struct);
_iq24 PI_Calculaion_Delta(_iq24 hReference, _iq24 hFeedback, PID_Structure_Type *PID_Struct);
_iq24 PID_Calculaion_Abs(_iq24 hReference, _iq24 hFeedback, PID_Structure_Type *PID_Struct);

void TrigonoMetric_Function(Struct_Cossin_Type *Cossin,_iq24 Angle_PU);

Component_2S_Type Clark_TransForm(Component_3S_Type *Input3s);

Component_2R_Type Park_TransForm(Component_2S_Type *Input2s,Struct_Cossin_Type *Struct_Cossin);

Component_2S_Type Rev_Park_TransForm(Component_2R_Type *Input2r,Struct_Cossin_Type *Struct_Cossin);

void ComponentLimit_2Ror2S(Component_2R_Type *Input,_iq24 Maxsquare,uint8_t *LimMark);

_iq24 Round_Circle_Limitation_Rad(_iq24 Input);

_iq24 Round_Circle_Limitation_PU(_iq24 Input);

_iq24 LPF_1st_Process(LPF_First_Order_Type *LPF_1st, _iq24 input);

_iq24 BPF_2nd_Process(BPF_Second_Order_Type *BPF_2nd,_iq24 input);

_iq24 LPF_2nd_Process(LPF_Second_Order_Type *LPF_2nd, _iq24 input);

_iq24 HPF_1st_Process(HPF_First_Order_Type *HPF_1st, _iq24 input);

_iq24 HPF_2nd_Process(HPF_Second_Order_Type *HPF_2nd,_iq24 input);

void Observer_Adaptor_Process(Observer_Adaptor_type *Adaptor,_iq24 Alpha,_iq24 Beta,_iq24 Angspeed);

_iq24 Speedband_Avoid(_iq24 Speedinput,_iq24 Band,_iq24 Bandwidth);

void Dcurr_Process_Init(DCurrProcess_Type * DCurrProcess,uint8_t Stage);

_iq24 Dcurr_Process(DCurrProcess_Type * DCurrProcess, _iq24 Speedact,_iq24 DCurrRefact);

void Speed_Ramp_Process(Speedrampctr_Type *Speed_Ramp);

_iq24 AverValue_Cal(_iq24 Input,_iq24 Output, BuffPara_Type *Buffpara);

void Buffer_Clear(_iq24 *Buffer, BuffPara_Type *BuffPara);

void Buffer_Update(_iq24 *Buffer, BuffPara_Type *BuffPara,_iq24 Updatevalue);

void Buffer_Set(_iq24 *Buffer, BuffPara_Type *BuffPara,_iq24 Setvalue);

_iq24 Speed_PLL_Cal(Speed_PLL_Type *SpeedPLL, _iq24 Theta_Err,int8_t Direction,_iq24 SpeedRef);

_iq24 IQ24divider(_iq24 dividendin, _iq24 divisorin);

void Saturation(_iq24 *input,_iq24 Upper,_iq24 Lower);

#endif
