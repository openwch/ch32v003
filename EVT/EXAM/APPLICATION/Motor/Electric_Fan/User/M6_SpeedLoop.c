/********************************** (C) COPYRIGHT *******************************
 * File Name          : M6_SpeedLoop.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
 * Description        : Speed loop processing, related protection treatment, power up and down control processing

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
void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/* Private variables --------------------------------------------------------*/
/* Variables ----------------------------------------------------------------*/
/*********************************************************************
 * @fn      SysTick_Handler
 *
 * @brief  Speed loop processing.
 *
 * @return  none
 */
void SysTick_Handler(void)
{
    Time_Delay_Process();

    Poweroperation_Process(&SpeedRamp_M,&ADC_M,&Powerctr_M,&RunningStatus_M,Observer_M.Cal_Struc.Angspeed_Aver1);

    if(Powerctr_M.Poweron==TRUE)
    {
        KeyCheck_Process(&Key_M,&SpeedRamp_M);
    }

    switch(RunningStatus_M)
    {
        case RUN:
            Buffer_Update(&(Observer_SpeedBuffer_M[0]), &(Observer_M.SpeedBuffPara), Observer_M.Cal_Struc.Angspeed_Inst);

            Observer_M.Cal_Struc.Angspeed_Aver1 = LPF_1st_Process(&(Observer_M.Cal_Struc.SpeedLPF1st_M),\
                    Observer_M.Cal_Struc.Angspeed_Inst);

            Observer_Status_M.Reliable =  Speed_Reliable_Verify(&(Observer_M.SpeedBuffPara),&Observer_SpeedBuffer_M[0],\
                    Observer_M.Cal_Struc.Angspeed_Aver1,Observer_Status_M.VarianceThresh);

            Speed_Ramp_Process(&SpeedRamp_M);

            Speed_Stable_Verify(&Observer_Status_M, Observer_M.Cal_Struc.Angspeed_Aver1);
            if(Observer_Status_M.Stable==FALSE)
                Protection_SetFault(SPEED_ABNORMAL_M);

            if((SpeedRamp_M.Decemark==FALSE))
            {
                Motor_Stall_Verify(SpeedRamp_M.Aftramp, &Observer_M, MStruc_M.CRef2R.cq,&Observer_Status_M);
                if(Observer_Status_M.Stall==TRUE)
                 Protection_SetFault(MOTOR_STALL_M);
             }

            MStruc_M.CRef2R.cq = PI_Calculaion_Delta (SpeedRamp_M.Aftramp, Observer_M.Cal_Struc.Angspeed_Aver1, &(Mpid_M.Speed));

            if((DCurrProcess_M.Stage == 1)&&(_IQabs(MStruc_M.CRef2R.cd) <= DCurrProcess_M.Target1)&&(Observer_M.Cal_Struc.Angspeed_Aver1>=(_IQ(0.1))))
                Dcurr_Process_Init(&DCurrProcess_M,2);

            MStruc_M.CRef2R.cd = Dcurr_Process(&DCurrProcess_M, Observer_M.Cal_Struc.Angspeed_Aver1, MStruc_M.CRef2R.cd);

            break;
        default:
                break;
    }

//    DCOvervolt_Protection(ADC_M.DCVoltAver, &DCBusProtecion_M);
//
//    DCUndervolt_Protection(ADC_M.DCVoltAver, &DCBusProtecion_M);

    SysTick->SR &= ~(1 << 0);
}
/*********************************************************************
 * @fn      Systick_Init
 *
 * @brief   Systick timer initialization
 *
 * @return  none
 */
void Systick_Init(void)
{
    SysTick->CTLR  =0;
    SysTick->CMP   =0;
    SysTick->SR   &= ~(1 << 0);
    SysTick->CNT   = 0;
    SysTick->CMP   = TIMER_PERIDO_SYSTICK;
    SysTick->CTLR |=(1 << 1)|(1 << 3)|(1 << 5);
}

/*********************************************************************
 * @fn      SYS_TIM_Cmd
 *
 * @brief   Systick timer enabled
 *
 * @param   NewState - ENABLE or DISABLE.
 *
 * @return  none
 */
void SYS_TIM_Cmd(FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        SysTick->CTLR |= TIM_CEN;
    }
    else
    {
        SysTick->CTLR &= (uint16_t)(~((uint16_t)TIM_CEN));
    }
}
