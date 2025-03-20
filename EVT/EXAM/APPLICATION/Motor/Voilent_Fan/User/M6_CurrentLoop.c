/********************************** (C) COPYRIGHT *******************************
 * File Name          : M6_Currentloop.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/15
 * Description        : Currentloop operation process.
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

/*********************************************************************
 * @fn      ADC1_IRQHandler
 *
 * @brief   ADC1_IRQHandler Process
 *
 * @para    none
 *
 * @return  none
 */
__attribute__((section(".highcode")))
void ADC1_IRQHandler(void)
{

    if((ADC1->STATR & ADC_FLAG_JEOC) == ADC_FLAG_JEOC)
    {
        {
            ADC_M.BEMFPres=_IQ18toIQ(_IQ18MPY_MACRO((_IQ18(ADC1->IDATAR2)),ADC_M.EMFRated_ConvInv));

            ADC_M.DCVoltPres_ADvalue =ADC1->IDATAR1;

            ADC_M.DCVoltPres=_IQ18toIQ(_IQ18MPY_MACRO(_IQ18(ADC_M.DCVoltPres_ADvalue),ADC_M.RatedDCVoltADCInv));

            ADC_M.DCVoltHalf= ADC_M.DCVoltPres>>1;
        }

            ADC1->CTLR2 |= 0x00500000;
            switch (RunningStatus_M)
            {
                case POSITION:
                    Captimer_Clear();

                    EMF_Cal_M.Timer_Overflow_Cnt =0;

                    RunningStatus_M = PRESTART;
                    break;

                case PRESTART:
                    PWM_Switch(&PWMcopper_M);

                    PWM_Update(&PWMcopper_M);

                    RunningStatus_M = START;

                    break;

                case START:
                    {
                       if (EMF_Cal_M.Schangedelaycnt != 0)
                                EMF_Cal_M.Schangedelaycnt --;

                       EMF_Schange_Check(&EMF_Cal_M,&ADC_M, &EMF_Status_M);

                    }
                    break;

                default:
                    break;
            }

            TIM1->CH4CVR = PWMcopper_M.Duty>>1;

           ADC1->STATR = ~(uint32_t)ADC_FLAG_JEOC;
    }
}
