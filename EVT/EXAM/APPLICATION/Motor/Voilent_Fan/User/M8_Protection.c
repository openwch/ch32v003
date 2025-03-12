/********************************** (C) COPYRIGHT *******************************
 * File Name          : M8_Protection.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/15
 * Description        : Protection_and emergency parameter initialization.
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
InstPhaseOC_Type        InstPhaseOC_M;
DCbusProtection_Type    DCBusProtecion_M;

/*********************************************************************
 * @fn      Protection_and_Emergency_Init
 *
 * @brief   Protection_and_Emergency_Init
 *
 * @para    InstPhaseOC_M:Transient overcurrent protection structure
 *
 * @return  none
 */
void Protection_and_Emergency_Init ()
{
    InstPhaseOC_M.Cnt = 0;
    InstPhaseOC_M.CurrThreshold = INST_PHASEOC_THRE;
    InstPhaseOC_M.CntThreshold = (uint16_t)(INST_PHASEOC_CAL_FREQ *INST_PHASEOC_TIME_THRE);

    DCBusProtecion_M.OverThresh = DCBUS_OVER_THRE;
    DCBusProtecion_M.UnderThresh = DCBUS_UNDER_THRE;
    DCBusProtecion_M.OverCntThresh = DCBUS_PROTEC_CALFREQ*DCBUS_OVER_TIME;
    DCBusProtecion_M.UnderCntThresh = DCBUS_PROTEC_CALFREQ*DCBUS_UNDER_TIME;
    DCBusProtecion_M.OverCnt = 0;
    DCBusProtecion_M.UnderCnt = 0;
}

/*********************************************************************
 * @fn      Protection_SetFault
 *
 * @brief   Protection_SetFault
 *
 * @para    SpeedRamp_M: Acceleration and deceleration control structure
 *          RunningStatus_M: System running status enumeration
 *
 * @return  none
 */
__attribute__((section(".highcode")))
void Protection_SetFault(uint16_t Fault_type)
{
    Time_Delay_Set(Operation_M.FaultHoldingSet,&Operation_M.FaultHoldingCnt);

    TIM_CtrlPWMOutputs(TIM1, DISABLE);

    ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_None);

    SpeedRamp_M.Speed_adncnt = 0;

    RunningStatus_M = FAULT;
    System_Status_Global |= Fault_type;
}

/*********************************************************************
 * @fn      Protction_Fault_Reset_M
 *
 * @brief   Reset Protction Fault
 *
 * @para    InstPhaseOC_M:    Transient overcurrent protection structure
 *          EMF_Status_M:     Observer state structure
 *          DCBusProtecion_M: Bus voltage protection structure
 *          RunningStatus_M:  System running status enumeration
 *
 * @return  none
 */
void Protction_Fault_Reset_M(void)
{
    if (Time_Delay_Elapse(Operation_M.FaultHoldingCnt))
    {
        System_Status_Global &= ~MOTOR_OVER_CURR_SOFT_M;
        InstPhaseOC_M.Cnt = 0;

        System_Status_Global &= ~MOTOR_OVERLOAD_M;

        if (GPIO_ReadInputDataBit(BRK_GPIO_M, BRK_PIN_M))
            System_Status_Global &= ~DC_OVER_CURR_HARD_M;

        System_Status_Global &= ~MOTOR_STALL_M;
        EMF_Status_M.Speedstall_CNT = 0;
        EMF_Status_M.Currover_CNT = 0;

        System_Status_Global&= ~DC_OVER_VOLT;
        DCBusProtecion_M.OverCnt = 0;

        System_Status_Global&= ~DC_UNDER_VOLT;
        DCBusProtecion_M.UnderCnt = 0;

        RunningStatus_M = IDLE;
        System_Status_Global |= FIRST_START_M;
    }
}
