/********************************** (C) COPYRIGHT *******************************
 * File Name          : M6_CurrentLoop.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
 * Description        : ADC sampling result reading, current loop processing, FOC calculation, overcurrent phaseloss protection processing

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
void ADC1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
extern void FOC_Cal(Motor_Sructure_Type *MStruc,SVPWM_2R_Type *SVPWM_Cal,Motor_PID_Type *Mpid,\
        ADCStruc_Type *ADCStruc,Motor_Observer_Type *MObserver,int8_t Direction);
/* Private variables --------------------------------------------------------*/
/* Variables ----------------------------------------------------------------*/
/*********************************************************************
 * @fn      ADC_IRQHandler
 *
 * @brief   ADC interrupts the response function to read the current conversion result
 *
 * @return  none
 */
__attribute__((section(".highcode")))
void ADC1_IRQHandler(void)
{
    int16_t ADsample_int1=0;
    int16_t ADsample_int2=0;
    if((ADC1->STATR & ADC_FLAG_JEOC) == ADC_FLAG_JEOC)
    {
        ADsample_int1 = (int32_t)(ADC_M.OffsetValue1) - (int32_t)((ADC1->IDATAR1)<<1);
        ADsample_int2 = (int32_t)(ADC_M.OffsetValue2) - (int32_t)((ADC1->IDATAR2)<<1);

        ADsample_int1=( (ADsample_int1) < (S16_MIN) ? (S16_MIN) : \
                                      ( (ADsample_int1) > (S16_MAX) ? (S16_MAX) : (ADsample_int1)));

        ADsample_int2=( (ADsample_int2) < (S16_MIN) ? (S16_MIN) : \
                                      ( (ADsample_int2) > (S16_MAX) ? (S16_MAX) : (ADsample_int2)));

        MStruc_M.C3S.PhaseC  = (_iq24)((_IQ16MPY_MACRO(_IQ16(ADsample_int1),ADC_M.RatedCurr_ConvInv))<<1);
        MStruc_M.C3S.PhaseB = (_iq24)((_IQ16MPY_MACRO(_IQ16(ADsample_int2),ADC_M.RatedCurr_ConvInv))<<1);
        MStruc_M.C3S.PhaseA= -(MStruc_M.C3S.PhaseC + MStruc_M.C3S.PhaseB);

        switch(RunningStatus_M)
        {
            case DIRCHECK:
                if(Flystart_M.Process_mark==FALSE)
                {
                    Observer_M.Cal_Struc.Inimark=TRUE;
                    Flystart_Process(&Flystart_M,SpeedRamp_M.Direction,&MStruc_M,&SVPWM_Cal_M,&Observer_M,&ADC_M,&Mpid_M);
                }

                if((Flystart_M.Process_mark==TRUE)&&(Flystart_M.Switch_mark==FALSE))
                {
                    Flystart_Switch(&Flystart_M,&SpeedRamp_M);
                }
            break;
                case RUN:
                    FOC_Cal(&MStruc_M,&SVPWM_Cal_M,&Mpid_M,&ADC_M,&Observer_M,SpeedRamp_M.Direction);//FOC¼ÆËã
                    break;
            default:
              break;
        }
            if ( ((RunningStatus_M==RUN))&&(Powerctr_M.Poweron == TRUE) )
            {
                if(Phaseloss_Verify2R3R(&(MStruc_M.C3S),&Phaseloss_M,&MStruc_M,SpeedRamp_M.Direction))
                    Protection_SetFault(PHASELOSS_FALUT);
            }
        }

    if ((RunningStatus_M == DIRCHECK)||(RunningStatus_M == RUN))
    {
        if(Phase_Ins_OverCurrent_Verify(&(MStruc_M.C3S),&InstPhaseOC_M) == TRUE)
            Protection_SetFault(MOTOR_OVER_CURR_SOFT_M);
    }

    TIM1->CH1CVR = SVPWM_Cal_M.cc1Load;
    TIM1->CH2CVR = SVPWM_Cal_M.cc2Load;
    TIM1->CH3CVR = SVPWM_Cal_M.cc3Load;

    ADC1->STATR = ~(uint32_t)ADC_FLAG_JEOC;
}

