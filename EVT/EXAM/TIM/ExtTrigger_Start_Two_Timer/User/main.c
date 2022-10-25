/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/08
 * Description        : Main program body.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/*
 *@Note
 外部触发同步启动两个定时器例程：
 TIM1_CH1(PD2)
 本例程演示 TIM_CH1(PD2) 引脚上拉输入，该引脚检测到上升沿，则同时启动 TIM1 和
 TIM2。

*/

#include "debug.h"

/*********************************************************************
 * @fn      ExtTrigger_Start_Two_TIM
 *
 * @brief   Starting 2 timers synchronously in response to an external trigger.
 *
 * @param   arr - the period value.
 *          psc - the prescaler value.
 *
 * @return  none
 */
void ExtTrigger_Start_Two_TIM(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    TIM_ICInitTypeDef TIM_ICInitStructure={0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD | RCC_APB2Periph_TIM1, ENABLE );
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );

    TIM_CounterModeConfig( TIM2, TIM_CounterMode_Up );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init( GPIOD, &GPIO_InitStructure);

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter =  0x00;
    TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x00;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInit( TIM1, &TIM_ICInitStructure );

    TIM_SelectInputTrigger( TIM1, TIM_TS_TI1FP1 );
    TIM_SelectSlaveMode( TIM1, TIM_SlaveMode_Trigger );
    TIM_SelectMasterSlaveMode( TIM1, TIM_MasterSlaveMode_Enable );
    TIM_SelectOutputTrigger( TIM1, TIM_TRGOSource_Enable );
    TIM_SelectInputTrigger( TIM2, TIM_TS_ITR0 );
    TIM_SelectSlaveMode( TIM2, TIM_SlaveMode_Trigger );
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
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n",SystemCoreClock);

    ExtTrigger_Start_Two_TIM( 0xFFFF, 48000-1);

    while(1)
    {
        printf("TIM1 cnt:%d\r\n", TIM1->CNT);
        printf("TIM2 cnt:%d\r\n", TIM2->CNT);
        Delay_Ms(100);
    }
}
