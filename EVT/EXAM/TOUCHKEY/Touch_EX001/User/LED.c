/********************************** (C) COPYRIGHT *******************************
 * File Name          : LED.c
 * Author             : WCH
 * Version            : V1.3
 * Date               : 2025/01/14
 * Description        :
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/******************************************************************************/

#include "LED.h"

TKY_Backlight_S BacklightArray[ TKY_BACKLIGHT_NUM ];
void TKY_BacklightOff (void);

void TKY_BacklightTaskStart();
void TKY_BacklightTaskStop();
void TKY_KeyBacklightOut (uint8_t key, FunctionalState s);

volatile uint8_t led_scanflag = 0;

/*********************************************************************
 * @fn      TKY_BacklightOff
 *
 * @brief   Backlight fully off.
 *
 * @param   none
 *
 * @return  none
 */
void TKY_BacklightOff (void)
{
    LED_SEG1_OFF();
    LED_SEG2_OFF();
    LED_SEG3_OFF();
    LED_SEG4_OFF();

    LED_COM1_OFF();
    LED_COM2_OFF();
    LED_COM3_OFF();
    LED_COM4_OFF();
    LED_COM5_OFF();    
}

/*********************************************************************
 * @fn      TKY_BacklightOn
 *
 * @brief   Backlight fully open.
 *
 * @param   none
 *
 * @return  none
 */
void TKY_BacklightOn (void)
{
    LED_SEG1_ON();
    LED_SEG2_ON();
    LED_SEG3_ON();
    LED_SEG4_ON();

    LED_COM1_ON();
    LED_COM2_ON();
    LED_COM3_ON();
    LED_COM4_ON();
    LED_COM5_ON();
}

/*********************************************************************
 * @fn      TKY_BacklightInit
 *
 * @brief   Initialization of backlight-related peripherals and parameters.
 *
 * @param   none
 *
 * @return  none
 */
void TKY_BacklightInit (void)
{
    for (uint8_t i = 0; i < TKY_BACKLIGHT_NUM; i++)
    {
        BacklightArray[ i ].TKY_BacklightOnTime = 0;
        BacklightArray[ i ].BacklightStates = 0;
    }

    //--------Backlight---------
    {
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_PB2PeriphClockCmd (RCC_PB2Periph_GPIOA | RCC_PB2Periph_GPIOB | RCC_PB2Periph_GPIOC | RCC_PB2Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_InitStructure.GPIO_Pin = LED_SEG1_PIN;
    GPIO_Init (LED_SEG1_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = LED_SEG2_PIN;
    GPIO_Init (LED_SEG2_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = LED_SEG3_PIN;
    GPIO_Init (LED_SEG3_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = LED_SEG4_PIN;
    GPIO_Init (LED_SEG4_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED_COM1_PIN;
    GPIO_Init (LED_COM1_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = LED_COM2_PIN;
    GPIO_Init (LED_COM2_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = LED_COM3_PIN;
    GPIO_Init (LED_COM3_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = LED_COM4_PIN;
    GPIO_Init (LED_COM4_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = LED_COM5_PIN;
    GPIO_Init (LED_COM5_PORT, &GPIO_InitStructure);
    }

    TKY_BacklightTaskStart();
}

/*********************************************************************
 * @fn      TKY_KeyBacklightOut
 *
 * @brief   Initialization of backlight-related peripherals and parameters.
 *
 * @param   key
 *          s - ENABLE or DISBALE
 *
 * @return  none
 */
void TKY_KeyBacklightOut (uint8_t key, FunctionalState s)
{
    if (DISABLE == s)
    {
        BacklightArray[ key ].BacklightStates = 0;
    }
    else
    {
        BacklightArray[ key ].BacklightStates = 1;
    }
}

/*********************************************************************
 * @fn      TKY_KeyBacklightOut
 *
 * @brief   Setting the key backlight status.
 *
 * @param   Idx - port
 *          state - ENABLE or DISBALE
 *
 * @return  none
 */
void setBacklightState (uint8_t Idx, uint8_t state)
{
    switch (Idx)
    {
        case 0 :
            if (state)
                LED_SEG1_ON();
            else
                LED_SEG1_OFF();
            break;
        case 1 :
            if (state)
                LED_SEG2_ON();
            else
                LED_SEG2_OFF();
            break;
        case 2 :
            if (state)
                LED_SEG3_ON();
            else
                LED_SEG3_OFF();
            break;
        case 3 :
            if (state)
                LED_SEG4_ON();
            else
                LED_SEG4_OFF();
            break;

        default :
            break;
    }
}

/*********************************************************************
 * @fn      TKY_BacklightTaskStart
 *
 * @brief   backlight start.
 *
 * @param   none
 *
 * @return  none
 */
void TKY_BacklightTaskStart ()
{
    TKY_BacklightOn();
    led_scanflag = 1;
}

/*********************************************************************
 * @fn      TKY_BacklightTaskStop
 *
 * @brief   backlight stop.
 *
 * @param   none
 *
 * @return  none
 */
void TKY_BacklightTaskStop ()
{
    TKY_BacklightOff();
    led_scanflag = 0;
}

/*********************************************************************
 * @fn      TKY_BacklightProcess
 *
 * @brief   backlight process.
 *
 * @param   none
 *
 * @return  none
 */
void TKY_BacklightProcess (void)
{
    uint8_t i;
    static uint8_t g_BacklightIdleState = 1;
    static uint8_t backLightScanState = 0;

    if (backLightScanState)
    {
        for (i = 4 * (backLightScanState - 1); i < 4 * backLightScanState; i++)
        {
            setBacklightState (i - 4 * (backLightScanState - 1), BacklightArray[ i ].BacklightStates);
        }
    }
    switch (backLightScanState)
    {
        case 0 :
            LED_COM1_OFF();
            LED_COM2_OFF();
            LED_COM3_OFF();
            LED_COM4_OFF();
            LED_COM5_OFF();
            backLightScanState = g_BacklightIdleState;
            break;
        case 1 :
            LED_COM1_ON();
            backLightScanState = 0;
            g_BacklightIdleState = 2;
            break;
        case 2 :
            LED_COM2_ON();
            backLightScanState = 0;
            g_BacklightIdleState = 3;
            break;
        case 3 :
            LED_COM3_ON();
            backLightScanState = 0;
            g_BacklightIdleState = 4;
            break;
        case 4 :
            LED_COM4_ON();
            backLightScanState = 0;
            g_BacklightIdleState = 5;
            break;
        case 5 :
            LED_COM5_ON();
            backLightScanState = 0;
            g_BacklightIdleState = 1;
            break;           
        default :
            break;
    }
}


/******************************** endfile @ led ******************************/
