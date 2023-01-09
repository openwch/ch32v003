/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/11/21
 * Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/*
 *@Note
IAP upgrade routine:
Support serial port for FLASH burning

1. Use the IAP download tool to realize the download PA0 floating (default pull-up input)
2. After downloading the APP, connect PC0 to ground (low level input), and press the
reset button to run the APP program.
3. use WCH-LinkUtility.exe download to BOOT(adr-0x1FFFF000)

*/

#include "debug.h"
#include "string.h"
#include "iap.h"

/*********************************************************************
 * @fn      IAP_2_APP
 *
 * @brief   IAP_2_APP program.
 *
 * @return  none
 */
void IAP_2_APP(void)
{
    RCC_ClearFlag();
    SystemReset_StartMode(Start_Mode_USER);
    NVIC_SystemReset();
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
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOD| RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOC;/* Enable GPIOD��USART1�� GPIOC  clock */
      USART1_CFG(460800);

    if(PC0_Check() == 0)
    {
            IAP_2_APP();
            while(1);
    }

    while(1)
    {
        if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
        {
            UART_Rx_Deal();

        }
    }
}
