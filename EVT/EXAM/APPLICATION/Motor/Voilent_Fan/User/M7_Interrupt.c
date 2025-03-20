/********************************** (C) COPYRIGHT *******************************
 * File Name          : M7_Interrupt.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/15
 * Description        : Interrupt_Configuration and some interruption process.
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
void TIM1_UP_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM1_BRK_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/* Private variables --------------------------------------------------------*/
/* Variables ----------------------------------------------------------------*/
static uint8_t Speed_Sample_PID_CNT = SPEED_SAMPLE_PID_PERIOD;

/*********************************************************************
 * @fn      Interrupt_Configuration
 *
 * @brief   Interrupt Configuration
 *
 * @para    none
 *
 * @return  none
 */
void Interrupt_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure={0};
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
    TIM_ITConfig(TIM1, TIM_IT_Break|TIM_IT_Update,ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = SysTicK_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*********************************************************************
 * @fn      TIM1_UP_IRQHandler
 *
 * @brief   TIM1_UP_IRQHandler Process
 *
 * @para    none
 *
 * @return  none
 */
void TIM1_UP_IRQHandler(void)
{

    if(RunningStatus_M==POSITION ||RunningStatus_M==PRESTART ||RunningStatus_M==START)
    {
            ;
    }
    else
    {
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
        ADC_SoftwareStartInjectedConvCmd(ADC1,ENABLE);
    }
    {
            Speed_Sample_PID_CNT --;

           if (Speed_Sample_PID_CNT == 0 )
           {
               Time_Delay_Process();

               if(EMF_Cal_M.Sprechangeflag == FALSE)
                  IDC_Cal(&ADC_M,ADC1);
               ADC_M.DCVoltInv = _IQdiv(_IQ(1.0),ADC_M.DCVoltPres);
#if 0
                Key_Check();
#endif
               Speed_Sample_PID_CNT = SPEED_SAMPLE_PID_PERIOD;

               if(SpeedRamp_M.Preramp == SpeedRamp_M.Speed_Stop)
               {
                   if ((Time_Delay_Elapse(Operation_M.StopwaitCnt) == TRUE)&&(SpeedRamp_M.SpeedStopflag ==TRUE))
                   {
                       SpeedRamp_M.Preramp = 0;
                       RunningStatus_M = STOP;

                   }
               }
               if (RunningStatus_M==PRESTART || RunningStatus_M==START)
               {
                   Speed_Ramp_Process(&SpeedRamp_M);

                   EMF_Cal_M.Speed_Aver2 = AverValue_Cal2(&(Speedbufffer2[0]),&(EMF_Cal_M.SpeedBuffPara2));

                   ADC_M.IDCFilter_2 =  LPF_1st_Process(&(ADC_M.IDCLPF1st_2),ADC_M.IDCPres);

                   Control_Switch_Sloop(&SpeedRamp_M,&EMF_Status_M);

                    Motor_Stall_Verify1((SpeedRamp_M.Aftramp), &EMF_Cal_M,&EMF_Status_M);
#if 0
                    Motor_Stall_Verify3(&EMF_Cal_M,&EMF_Status_M,&PWMcopper_M);
#endif
                   if(EMF_Status_M.Stall==TRUE)
                      Protection_SetFault(MOTOR_STALL_M);

                   if(Phase_Ins_OverCurrent_Verify_DC(ADC_M.IDCFilter_2,&InstPhaseOC_M) == TRUE)
                       Protection_SetFault(MOTOR_OVER_CURR_SOFT_M);
               }
           }
    }
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
}

/*********************************************************************
 * @fn      TIM1_BRK_IRQHandler
 *
 * @brief   TIM1_BRK_IRQHandler Process
 *
 * @para    none
 *
 * @return  none
 */
void TIM1_BRK_IRQHandler(void)
{
    Protection_SetFault(DC_OVER_CURR_HARD_M);
    TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
}

/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   NMI_Handler Process
 *
 * @para    none
 *
 * @return  none
 */
void NMI_Handler(void)
{
    while (1)
    {
    }
}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   HardFault_Handler Process
 *
 * @para    none
 *
 * @return  none
 */
void HardFault_Handler(void)
{
    NVIC_SystemReset();
    while (1)
    {
    }
}
