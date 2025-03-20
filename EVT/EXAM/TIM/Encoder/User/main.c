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

/**
 * @note
 * Encoder rourine
 * The main function initializes and tests an encoder, and continuously prints the encoder position and
 * speed.
 */
#include "debug.h"

/* Global define */

/* The `#define SpeedSampleTimeMs 20` line is defining a constant macro `SpeedSampleTimeMs` with a
value of 20. This constant is used in the code to specify the time interval (in milliseconds) at
which the speed of the encoder is sampled and calculated. */
#define SpeedSampleTimeMs 20
/* Global Variable */

volatile int circle = 0, precircle = 0;
volatile uint16_t precnt = 0;
volatile uint32_t time = 0;

void TIM1_UP_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
/*********************************************************************
 * @fn TIM1_UP_IRQHandler
 *
 * @brief TIM1_UP_IRQHandler function handles the interrupt for TIM1 and updates the "circle" variable based
 * on the current count and auto-reload values of TIM1.
 */
void TIM1_UP_IRQHandler()
{

    volatile uint16_t tempcnt = TIM1->CNT, temparr = TIM1->ATRLR;
    if (TIM_GetITStatus(TIM1, TIM_IT_Update))
    {

        if (tempcnt < temparr / 2)
        {
            circle += 1;
        }
        else
        {
            circle -= 1;
        }
    }
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
}

/*********************************************************************
 * @fn TIM1_Encoder_Init
 *
 * @brief function initializes the TIM1 timer as an encoder with specific settings.
 *
 * @return none
 */
void TIM1_Encoder_Init()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOD|RCC_PB2Periph_GPIOA, ENABLE);
    RCC_PB2PeriphClockCmd(RCC_PB2Periph_TIM1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
    TIM_TimeBaseStructure.TIM_Period = 80;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 10;
    TIM_ICInit(TIM1, &TIM_ICInitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

    NVIC_Init(&NVIC_InitStructure);

    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    TIM_SetCounter(TIM1, 0);
    TIM_Cmd(TIM1, ENABLE);
}

/*********************************************************************
 * @fn TIM2_EncoderSignalGenerator_Init
 *
 * @brief The function initializes the TIM2 timer as an encoder signal generator with the specified prescaler,
 *      auto-reload value, and capture/compare value.
 *
 * @param psc The "psc" parameter stands for Prescaler value. It is used to divide the timer clock
 *          frequency before it is fed into the counter. This helps in adjusting the timer resolution and the
 *          frequency at which the counter increments.
 *        arr The "arr" parameter in the function TIM2_EncoderSignalGenerator_Init is used to set the
 *          auto-reload value of the timer. It determines the period of the timer, i.e., the number of timer
 *          counts before the timer resets and starts counting again.
 *        ccr The "ccr" parameter in the function TIM2_EncoderSignalGenerator_Init is used to set the
 *          initial value of the Capture/Compare register (CCR) for the Timer 2 output channels. The CCR
 *          determines the duty cycle or pulse width of the generated signal.
 *
 * @return none
 */
void TIM2_EncoderSignalGenerator_Init(uint16_t psc, uint16_t arr, uint16_t ccr)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOD, ENABLE);
    RCC_PB1PeriphClockCmd(RCC_PB1Periph_TIM2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;

    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = ccr;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_Pulse = ccr - 200;
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM2, ENABLE);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
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
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init( 115200 );
#endif
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("ChipID:%08x\r\n", DBGMCU_GetCHIPID());

    printf("Encoder TEST\r\n");
    TIM2_EncoderSignalGenerator_Init(SystemCoreClock / 1000000 - 1, 1000 - 1, 500);
    TIM1_Encoder_Init();

    while (1)
    {
        /* The code block you provided is checking if the encoder position or count has changed. If
        there is a change, it prints the new encoder position and calculates the encoder speed. */
        if (precircle != circle || (precnt != TIM1->CNT && TIM1->CNT % 4 == 0))
        {
            printf("Encoder position= %d circle %d step\r\n", circle, TIM1->CNT);
            if (time != 0)
//                printf("Encoder speed= %f\r\n", -(float)(precircle * 80 + precnt - (circle * 80 + TIM2->CNT)) / (float)time * 1000.0/(float)SpeedSampleTimeMs / 80.0);
                printf("Encoder speed null!!\r\n");
            else
                printf("Encoder speed null!!\r\n");
            time = 0;
            precircle = circle;
            precnt = TIM1->CNT;
        }
        time++;
        Delay_Ms(SpeedSampleTimeMs);
    }
}
