/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/01/01
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *Clock source selection routine:
 *This example demonstrates two external clock source modes of TIM1.
 *External clock source mode 1: PD2 is used as clock input pin,
 *External clock source mode 2: PC5 is used as clock input pin.
 *
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
    RCC_PB2PeriphClockCmd(RCC_PB2Periph_TIM1 | RCC_PB2Periph_GPIOD, ENABLE);

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
    RCC_PB2PeriphClockCmd(RCC_PB2Periph_TIM1 | RCC_PB2Periph_GPIOC, ENABLE);

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
    SystemCoreClockUpdate();
    Delay_Init();
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(115200);
#endif
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

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
