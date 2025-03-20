/********************************** (C) COPYRIGHT *******************************
 * File Name          : M8_Protection.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
 * Description        : Protection parameter initialization, protection state setting

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
InstPhaseOC_Type  InstPhaseOC_M;        // Instantaneous over current protection structure
DCbusProtection_Type DCBusProtecion_M;  // Bus voltage protection structure
Phaseloss_Type Phaseloss_M;             // Phase loss protection structure
/*********************************************************************
 * @fn      Protection_and_Emergency_Parameter_Define
 *
 * @brief   Protection parameters are initialization
 *
 * @return  none
 */
void Protection_and_Emergency_Init ()
{
    InstPhaseOC_M.Cnt = 0;
    InstPhaseOC_M.CurrThreshold = INST_PHASEOC_THRE_M;
    InstPhaseOC_M.CntThreshold = INST_PHASEOC_CNT_THRE_M;

    DCBusProtecion_M.OverThresh = DCBUS_OVER_VOLT_THRE;
    DCBusProtecion_M.UnderThresh = DCBUS_UNDER_VOLT_THRE;
    DCBusProtecion_M.OverCntThresh = DCBUS_OVER_VOLT_CNT;
    DCBusProtecion_M.UnderCntThresh = DCBUS_UNDER_VOLT_CNT;
    DCBusProtecion_M.OverCnt = 0;
    DCBusProtecion_M.UnderCnt = 0;
    DCBusProtecion_M.Overvolt = FALSE;
    DCBusProtecion_M.Undervolt = FALSE;

    Phaseloss_M.CurrThresh = PHASELOSS_CURR_THRE;
    Phaseloss_M.CntThresh = (uint16_t)(PHASELOSS_TIME_THRE * PHASELOSS_CAL_FREQ);
    Phaseloss_M.MultiphaseCnt = 0;
    Phaseloss_M.SiglephaseCntA = 0;
    Phaseloss_M.SiglephaseCntB = 0;
    Phaseloss_M.SiglephaseCntC = 0;
    Phaseloss_M.CurrBuffPara.Length = PHASELOSS_CAL_BUFFSIZE;
    Phaseloss_M.CurrBuffPara.Inv1 = _IQ(1.0)/PHASELOSS_CAL_BUFFSIZE;
    Phaseloss_M.CurrBuffPara.Inv2 = _IQ(1.0) - Phaseloss_M.CurrBuffPara.Inv1;
    Phaseloss_M.Curr_LowerThresh= PHASELOSS_CURR_LOWERTHRE;
}

/*********************************************************************
 * @fn      Protection_SetFault
 *
 * @brief   Fault set
 *
 *  @para   Fault_type: error types
 *
 * @return  none
 */
void Protection_SetFault(uint16_t Fault_type)
{
    Time_Delay_Set(Operation_M.FaultHoldingSet,&(Operation_M.FaultHoldingCnt));

    TIM_CtrlPWMOutputs(TIM1, DISABLE);

    ADC_Start(DISABLE);

    Operation_M.Checkmode =0;

    RunningStatus_M = FAULT;

    System_Status_Global |= Fault_type;
}

/*********************************************************************
 * @fn      DCOvervolt_Protection
 *
 * @brief   DC bus over voltage verification
 *
 *  @para   DCVolt: DC bus voltage
 *          DCbusProtec: Bus voltage protection structure pointer
 *
 * @return  none
 */
void DCOvervolt_Protection(_iq24 DCVolt, DCbusProtection_Type *DCbusProtec)
{
    if (DCVolt >= DCbusProtec->OverThresh)
        DCbusProtec->OverCnt++;
    else
        DCbusProtec->OverCnt = 0;

    if(DCbusProtec->OverCnt > DCbusProtec->OverCntThresh)
    {
        Protection_SetFault(DC_OVER_VOLT);
        DCBusProtecion_M.Overvolt = TRUE;
    }
}

/*********************************************************************
 * @fn      DCUndervolt_Protection
 *
 * @brief   DC bus under voltage protection
 *
 *  @para   DCVolt: DC bus voltage
 *          DCbusProtec: Bus voltage protection structure pointer
 *
 * @return  none
 */
void DCUndervolt_Protection(_iq24 DCVolt, DCbusProtection_Type *DCbusProtec)
{
    if (DCVolt <= DCbusProtec->UnderThresh)
         DCbusProtec->UnderCnt++;
    else
        DCbusProtec->UnderCnt = 0;

    if(DCbusProtec->UnderCnt > DCbusProtec->UnderCntThresh)
    {
        Protection_SetFault(DC_UNDER_VOLT);
        DCBusProtecion_M.Overvolt = TRUE;
    }
}
