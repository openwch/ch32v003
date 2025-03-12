/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2025/01/14
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *   Comprehensive demonstration of touch keys:
 *1. Spaced sensing: hand close to the expansion board, the corresponding LED on, away from the LED off.
 *2. Touch button: finger press, corresponding LED on, release LED off.
 *3. Touch Slider: LED follows the finger to light up, which position the finger slides to,
 *   the corresponding position LED lights up, and the rest position LED goes out.
 *4. Touch Wheel: same as touching the slide bar, LED follows the finger sliding position to light up.
 */

#include "debug.h"
#include "Touch.h"
#include "app.h"
/* Global define */

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
	Delay_Ms(1000);
	RCC_PB2PeriphClockCmd( RCC_PB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SDI_Disable, ENABLE);
#ifdef DEBUG
    USART_Printf_Init(115200);
#endif
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID());
	printf("Touch Utility\n");
	TKY_Init();
	while(1)
	{
	    TKY_dataProcess();
	}
}
