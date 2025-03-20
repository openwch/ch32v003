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
 *Memory to memory mode routine:
 *Transfer SRC_BUF[Buf_Size] to DST_BUF[Buf_Size] via DMA.
 *
 */

#include "debug.h"

/* Global define */
#define Buf_Size    32

/* Global Variable */
u32 SRC_BUF[Buf_Size] = {0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
                         0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
                         0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
                         0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
                         0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
                         0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
                         0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
                         0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80};

u32 DST_BUF[Buf_Size] = {0};
u8  Flag = 0;
/*********************************************************************
 * @fn      BufCmp
 *
 * @brief   Compare the  buf
 *
 * @param   buf1 - pointer of buf1
 *          buf2 - pointer of buf1
 *          buflength - length of buf
 *
 * @return  1 - Two arrays are identical
 *          0 - Two arrays are inconsistent
 */
u8 BufCmp(u32 *buf1, u32 *buf2, u16 buflength)
{
    while(buflength--)
    {
        if(*buf1 != *buf2)
        {
            return 0;
        }
        buf1++;
        buf2++;
    }
    return 1;
}

/*********************************************************************
 * @fn      DMA1_CH3_Init
 *
 * @brief   Initializes Channel3 of DMA1 collection.
 *
 * @return  none
 */
void DMA1_CH3_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure = {0};
    RCC_HBPeriphClockCmd(RCC_HBPeriph_DMA1, ENABLE);

    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(SRC_BUF);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DST_BUF;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = Buf_Size * 4;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);
    DMA_ClearFlag(DMA1_FLAG_TC3);

    DMA_Cmd(DMA1_Channel3, ENABLE);
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
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(115200);
#endif
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    printf("DMA MEM2MEM TEST\r\n");
    DMA1_CH3_Init();

    while(DMA_GetFlagStatus(DMA1_FLAG_TC3) == RESET)
    {
    }

    Flag = BufCmp(SRC_BUF, DST_BUF, Buf_Size);
    if(Flag == 0)
    {
        printf("DMA Transfer Fail\r\n");
    }
    else
    {
        printf("DMA Transfer Success\r\n");
    }

    printf("SRC_BUF:\r\n");
    for(i = 0; i < Buf_Size; i++) {
        printf("0x%08x\r\n", SRC_BUF[i]);
    }

    printf("DST_BUF:\r\n");
    for(i = 0; i < Buf_Size; i++){
        printf("0x%08x\r\n", DST_BUF[i]);
    }
    while(1)
    {
    }
}
