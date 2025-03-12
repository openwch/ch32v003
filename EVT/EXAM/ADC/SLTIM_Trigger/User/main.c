/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.1
* Date               : 2025/01/09
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 *SLTIM trigger ADC conversion routine:
 *ADC channel 2 (PC4) - injection group channel, TIM3 updata trigger,
 *
 */

#include "debug.h"

/* Global Variable */

/*********************************************************************
 * @fn      SLTIM_Init
 *
 * @brief   SLTIM Init.
 *
 * @return  none
 */
void SLTIM_Init(void)
{
    RCC_PB2PeriphClockCmd(RCC_PB2Periph_TIM1, ENABLE);
    RCC_PB1PeriphClockCmd(RCC_PB1Periph_TIM3, ENABLE);

    TIM_CounterModeConfig(TIM1, TIM_CounterMode_Up);
    TIM_CounterModeConfig(TIM3, TIM_CounterMode_Down);
    TIM_SetAutoreload(TIM1, 0x3);
    TIM_SetAutoreload(TIM3, 0xFD);
    TIM_SetCompare2(TIM3, 0xFA);

    TIM_PrescalerConfig(TIM1, 48000 - 1, TIM_PSCReloadMode_Immediate);
    TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_External1);

    TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

/*********************************************************************
 * @fn      ADC_Function_Init
 *
 * @brief   Initializes ADC collection.
 *
 * @return  none
 */
void ADC_Function_Init(void)
{
    ADC_InitTypeDef  ADC_InitStructure = {0};
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOC, ENABLE);
    RCC_PB2PeriphClockCmd(RCC_PB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_ExternalTrigInjectedConvConfig(ADC1,ADC_ExternalTrigInjecConv_T3_CC2); 

    ADC_InjectedSequencerLengthConfig(ADC1, 1);
    ADC_InjectedChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_CyclesMode7);
    ADC_ExternalTrigInjectedConvCmd(ADC1, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    ADC_BufferCmd(ADC1, DISABLE); //disable buffer
}

void ADC1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
/*********************************************************************
 * @fn      ADC1_IRQHandler
 *
 * @brief   ADC1_2 Interrupt Service Function.
 *
 * @return  none
 */
void ADC1_IRQHandler(void)
{
    u16 ADC_val;

    if(ADC_GetITStatus(ADC1, ADC_IT_JEOC))
    {
        printf("ADC Extline trigger conversion...\r\n");
        ADC_val = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1);
        printf("JADC-%04d\r\n", ADC_val);
    }

    ADC_ClearITPendingBit(ADC1, ADC_IT_JEOC);
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    SystemCoreClockUpdate();
    Delay_Init();

#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(115200);
#endif

    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    ADC_Function_Init();
    SLTIM_Init();
    while(1)
    {
    }
}
