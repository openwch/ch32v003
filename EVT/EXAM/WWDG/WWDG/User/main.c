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
 *Window watchdog routine:
 *This routine demonstrates that the watchdog is fed between the window 0x40 - 0x5f
 *to prevent the watchdog from being reset.
 *
*/

#include "debug.h"

/* Global define */
#define WWDG_CNT    0X7F

/* Global Variable */

/*********************************************************************
 * @fn      WWDG_NVIC_Config
 *
 * @brief   WWDG INT init.
 *
 * @return  none
 */
static void WWDG_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*********************************************************************
 * @fn      WWDG_Config
 *
 * @brief   Configure WWDG.
 *
 * @param   tr - The value of the decrement counter(0x7f~0x40)
 *          wr - Window value(0x7f~0x40)
 *          prv - Prescaler value
 *            WWDG_Prescaler_1
 *            WWDG_Prescaler_2
 *            WWDG_Prescaler_4
 *            WWDG_Prescaler_8
 *
 * @return  none
 */
void WWDG_Config(uint8_t tr, uint8_t wr, uint32_t prv)
{
    RCC_PB1PeriphClockCmd(RCC_PB1Periph_WWDG, ENABLE);

    WWDG_SetCounter(tr);
    WWDG_SetPrescaler(prv);
    WWDG_SetWindowValue(wr);
    WWDG_Enable(WWDG_CNT);
    WWDG_ClearFlag();
    WWDG_NVIC_Config();
    WWDG_EnableIT();
}

/*********************************************************************
 * @fn      WWDG_Feed
 *
 * @brief   Feed WWDG.
 *
 * @return  none
 */
void WWDG_Feed(void)
{
    WWDG_SetCounter(WWDG_CNT);
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
    u8 wwdg_tr, wwdg_wr;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(115200);
#endif
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("WWDG Test\r\n");

    WWDG_Config(0x7f, 0x5f, WWDG_Prescaler_8); /* 48M/8/4096 */
    wwdg_wr = WWDG->CFGR & 0x7F;
    while(1)
    {
        Delay_Ms(10);

        printf("**********\r\n");
        wwdg_tr = WWDG->CTLR & 0x7F;
        if(wwdg_tr < wwdg_wr)
        {
            WWDG_Feed();
        }
        printf("##########\r\n");
    }
}
