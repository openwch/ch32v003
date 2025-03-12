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
 *8 Button Touch History:
 *Press your finger, the corresponding LED will light up, release it and the LED will go out.
 *
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
    RCC_PB2PeriphClockCmd(RCC_PB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SDI_Disable, ENABLE);
#ifdef DEBUG
    USART_Printf_Init(115200);
#endif
    printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID());
	printf("Touch 8 Keys\n");
	TKY_Init();

	while(1)
	{
	    TKY_dataProcess();
	}
}
