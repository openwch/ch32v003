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
 时钟源选择例程：
 本例程演示 TIM1 两种外部时钟源模式。
 外部时钟源模式1：PD2作为时钟输入引脚，
 外部时钟源模式2：PC5作为时钟输入引脚。

*/

#include "debug.h"

/* External Clock Mode Definition */
#define ETR_CLK_MODE1    0
#define ETR_CLK_MODE2    1

/*  External Clock Mode Selection */
#define ETR_CLK          ETR_CLK_MODE1
//#define ETR_CLK ETR_CLK_MODE2

/*********************************************************************
 * @fn      TIM1_ETRClockMode1_Init
 *
 * @brief   Configures the External clock Mode1.
 *
 * @return  none
 */
void TIM1_ETRClockMode1_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOD, ENABLE);

    TIM_CounterModeConfig(TIM1, TIM_CounterMode_Up);
    TIM_SetAutoreload(TIM1, 0x3EB);
    TIM_ETRClockMode1Config(TIM1, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_Inverted, 0x0);
    /* GPIOD2 Input as TIM Clock Source */
    TIM_TIxExternalClockConfig(TIM1, TIM_TIxExternalCLK1Source_TI1, TIM_ICPolarity_Rising, 0x00);
    TIM_Cmd(TIM1, ENABLE);
}

/*********************************************************************
 * @fn      TIM1_ETRClockMode2_Init
 *
 * @brief   Configures the External clock Mode2.
 *
 * @return  none
 */
void TIM1_ETRClockMode2_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOC, ENABLE);

    TIM_CounterModeConfig(TIM1, TIM_CounterMode_Up);
    TIM_SetAutoreload(TIM1, 0x3EB);
    /* GPIOC5 Input as TIM Clock Source */
    TIM_ETRClockMode2Config(TIM1, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_Inverted, 0x0);
    TIM_SelectInputTrigger(TIM1, TIM_TS_ETRF);
    TIM_Cmd(TIM1, ENABLE);
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
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);

#if(ETR_CLK == ETR_CLK_MODE1)
    TIM1_ETRClockMode1_Init();

#elif(ETR_CLK == ETR_CLK_MODE2)
    TIM1_ETRClockMode2_Init();

#endif

    while(1)
    {
        if(TIM_GetFlagStatus(TIM1, TIM_FLAG_CC1) != RESET)
        {
            TIM_ClearFlag(TIM1, TIM_FLAG_CC1);
        }

        if(TIM_GetFlagStatus(TIM1, TIM_FLAG_Update) != RESET)
        {
            printf("Count Update\r\n");

            TIM_ClearFlag(TIM1, TIM_FLAG_Update);
        }
    }
}
