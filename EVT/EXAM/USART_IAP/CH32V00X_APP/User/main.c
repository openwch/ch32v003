/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.1
 * Date               : 2025/01/13
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 * APP routine: this routine support only UART mode,
 * and you can choose the command method to jump to the IAP .
 * Key  parameters: CalAddr - address in flash(same in IAP), note that this address needs to be unused.
 *                  CheckNum - The value of 'CalAddr' that needs to be modified.
 * Tips :the routine need IAP software version 1.50.
 */
/*
 * note: The serial port baud rate values and BRR register values for different clock master frequencies are as follows.
 *              baud rate = 460800 ------- USARTx->BRR = 0X34 ------- System clock = 24MHz
 *              baud rate = 460800 ------- USARTx->BRR = 0X68 ------- System clock = 48MHz
 *              ...
 *       Set the value of the BRR register at different clock frequencies in order to select
 *       the desired serial communication frequency.
 */
#include "debug.h"
#include "iap.h"
/* Global define */

/* Global Variable */

/*********************************************************************
 * @fn      GPIO_Toggle_INIT
 *
 * @brief   Initializes GPIOD.0
 *
 * @return  none
 */
void GPIO_Toggle_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/*********************************************************************
 * @fn      APP_2_IAP
 *
 * @brief   APP_2_IAP program.
 *
 * @return  none
 */
void APP_2_IAP(void)
{
    RCC_ClearFlag();
    SystemReset_StartMode(Start_Mode_BOOT);
    NVIC_SystemReset();
}

/*********************************************************************
 * @fn      USART1_IT_CFG
 *
 * @brief   USART1 IT configuration.
 *
 * @return  none
 */
void USART1_IT_CFG(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_USART1, ENABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);
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
    u8 i = 0;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    RCC->PB2PCENR |= RCC_PB2Periph_GPIOD| RCC_PB2Periph_USART1;/* Enable USART1, GPIOC  clock */
    USART1_CFG();
    USART1_IT_CFG();
    GPIO_Toggle_INIT();

    while(1)
    {
        Delay_Ms(250);
        GPIO_WriteBit(GPIOD, GPIO_Pin_0, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
        if(*(uint32_t*)CalAddr != CheckNum)
        {
            APP_2_IAP();
            while(1);
        }
    }
}
void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      USART1_IRQHandler
 *
 * @brief   This function handles USART3 global interrupt request.
 *
 * @return  none
 */
void USART1_IRQHandler(void)
{
    if((USART1->STATR & USART_FLAG_RXNE) != (uint16_t)RESET)
    {
        UART_Rx_Deal();
    }
}
