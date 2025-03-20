/********************************** (C) COPYRIGHT *******************************
 * File Name          : SoftUART.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/01/01
 * Description        : Simulate UART with software
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#include "SoftUART.h"
static uint8_t ByteSize;

static vu8 RxBuffer[BUFFER_SIZE] = {0};
static vu8 rec_stat = 0, rec_len = 0, ReciveCP = 0;
static vu32 rec_buff = 0;
static vu16 recout = 0;

SoftUARTHardware_t SoftUARTHardware;
SoftUARTFunc_t SoftUARTFunc = {Init};
SoftUARTPara_t SoftUARTPara;

EXTI_InitTypeDef ExtiN={0};

/*********************************************************************
 * @fn      isOdd
 *
 * @brief   judge whether the data is odd or not
 *
 * @param   data - data to be checked
 *
 * @return  none
 */
static u8 isOdd(u16 data)
{
    u8 CNT = 0,tmp = 0;
    tmp = SoftUARTPara.bitnum + 1;
    if (SoftUARTPara.veryfy) {
        tmp ++;
    }
    for (u8 i = 0; i <  tmp; i++)
    {
        if (data & (1 << i))
        {
            CNT++;
        }
    }

    return CNT % 2;
}

void TIM1_UP_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast"))); /* Interrupt handler function */
/*********************************************************************
 * @fn      TIM1_UP_IRQHandler
 *
 * @brief   The function `TIM1_UP_IRQHandler` handles the interrupt for the update event of TIM1 and performs
 *        operations related to receiving data in a SoftUART communication protocol.
 *
 * @return  none
 */
void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetFlagStatus(SoftUARTHardware.rxTIM, TIM_FLAG_Update) != RESET)
    {
        TIM_ClearITPendingBit(SoftUARTHardware.rxTIM, TIM_IT_Update);

        if (GPIO_ReadInputDataBit(SoftUARTHardware.rxPort, SoftUARTHardware.rxPin) == SET)
            rec_buff |= 0x0001 << rec_stat;
        else
            rec_buff &= ~(0x0001 << rec_stat);

        rec_stat++;
        if (rec_stat >= ByteSize / 2)
        {
            if (rec_buff & 0x01)
            {
                TIM_Cmd(SoftUARTHardware.rxTIM, DISABLE);
                ExtiN.EXTI_LineCmd = ENABLE;
                EXTI_Init(&ExtiN);
                ReciveCP = 1;
            }
            else
            {

                if (SoftUARTPara.veryfy)
                {
                    switch (SoftUARTPara.veryfy)
                    {
                    case Odd:
                        if (isOdd(rec_buff))
                        {
                            RxBuffer[rec_len] = (rec_buff >> 1) & 0xff;
                            rec_len = (rec_len + 1) % 127;
                        }
                        break;
                    case Even:
                        if (!isOdd(rec_buff))
                        {
                            RxBuffer[rec_len] = (rec_buff >> 1) & 0xff;
                            rec_len = (rec_len + 1) % 127;
                        }
                        break;

                    default:
                        break;
                    }
                }
                else
                {
                    RxBuffer[rec_len] = (rec_buff >> 1) & 0xff;
                    rec_len = (rec_len + 1) % BUFFER_SIZE;
                }

                if (SoftUARTPara.stopbits % 2)
                {
                    SoftUARTHardware.rxTIM->CNT = 0;
                    while (SoftUARTHardware.rxTIM->CNT < 9)
                        ;
                    SoftUARTHardware.rxTIM->CNT = 0;
                }
                rec_stat = 0;
                ExtiN.EXTI_LineCmd = ENABLE;
                EXTI_Init(&ExtiN);
            }
            return;
        }
    }
}

void EXTI7_0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast"))); /* Interrupt handler function */
/*********************************************************************
 * @fn      EXTI7_0_IRQHandler
 *
 * @brief   The function EXTI7_0_IRQHandler handles the interrupt for EXTI lines 7 to 0 and performs certain
 *        actions based on the status of the interrupt.
 *
 * @return  none
 */
void EXTI7_0_IRQHandler(void)
{
    if (EXTI_GetFlagStatus(EXTI_Line2) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line2);

        if (rec_stat >= ByteSize / 2 && ReciveCP == 0)
        {
            TIM_Cmd(SoftUARTHardware.rxTIM, ENABLE);
            rec_stat = 0;
            SoftUARTHardware.rxTIM->CNT = 0;
            TIM_GenerateEvent(SoftUARTHardware.rxTIM, TIM_IT_Update);
            ExtiN.EXTI_LineCmd = DISABLE;
            EXTI_Init(&ExtiN);
        }
        else if (rec_stat == 0)
        {
            SoftUARTHardware.rxTIM->CNT = 0;
            TIM_GenerateEvent(SoftUARTHardware.rxTIM, TIM_IT_Update);
            ExtiN.EXTI_LineCmd = DISABLE;
            EXTI_Init(&ExtiN);
        }
    }
}

/*********************************************************************
 * @fn      RxInit
 *
 * @brief   The function initializes the receive functionality for a SoftUART communication protocol.
 *
 * @return  none
 */
static void RxInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = SoftUARTHardware.rxPin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(SoftUARTHardware.rxPort, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource2);
    EXTI_InitTypeDef EXTI_InitStructure={0};
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_ClearITPendingBit(EXTI_Line2);
    ExtiN = EXTI_InitStructure;

    NVIC_InitTypeDef NVIC_InitStructure={0};
    NVIC_InitStructure.NVIC_IRQChannel = EXTI7_0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);

    TIM_TimeBaseInitTypeDef TIM_TimeInitStructure={0};
    TIM_TimeInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeInitStructure.TIM_Period = 10 - 1;
    TIM_TimeInitStructure.TIM_Prescaler = (SystemCoreClock / (SoftUARTPara.baud_rate * 10) - 1);
    TIM_TimeInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(SoftUARTHardware.rxTIM, &TIM_TimeInitStructure);
    TIM_ClearITPendingBit(SoftUARTHardware.rxTIM, TIM_IT_Update);
    TIM_ITConfig(SoftUARTHardware.rxTIM, TIM_IT_Update, ENABLE);
    TIM_ARRPreloadConfig(SoftUARTHardware.rxTIM, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
}

/*********************************************************************
 * @fn      TxInit
 *
 * @brief   function initializes the necessary GPIO pins, timers, and DMA for transmitting data
 *        using a software UART.
 *
 * @return  none
 */
static void TxInit()
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = SoftUARTHardware.txPin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(SoftUARTHardware.txPort, &GPIO_InitStructure);

    TIM_TimeBaseInitTypeDef TIM_TimeInitStructure={0};
    TIM_TimeInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeInitStructure.TIM_Period = 5 - 1;
    TIM_TimeInitStructure.TIM_Prescaler = (SystemCoreClock / (SoftUARTPara.baud_rate * 10) - 1);
    TIM_TimeInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(SoftUARTHardware.txTIM, &TIM_TimeInitStructure);

    TIM_OCInitTypeDef TIM_OCInitstructure={0};
    TIM_OCInitstructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitstructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitstructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitstructure.TIM_Pulse = 0x5;
    TIM_OC1Init(SoftUARTHardware.txTIM, &TIM_OCInitstructure);

    TIM_OC1PreloadConfig(SoftUARTHardware.txTIM, TIM_OCPreload_Disable);
    TIM_ARRPreloadConfig(SoftUARTHardware.txTIM, ENABLE);

    DMA_InitTypeDef DMA_InitStruct={0};
    DMA_InitStruct.DMA_BufferSize = 0;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStruct.DMA_MemoryBaseAddr = (u32)NULL;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&SoftUARTHardware.txTIM->CH1CVR;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_Init(SoftUARTHardware.txDMA, &DMA_InitStruct);
    TIM_DMACmd(SoftUARTHardware.txTIM, TIM_DMA_Update, ENABLE);
    TIM_Cmd(SoftUARTHardware.txTIM, ENABLE);
    TIM_CtrlPWMOutputs(SoftUARTHardware.txTIM, ENABLE);
}

/*********************************************************************
 * @fn      TransmitBytes
 *
 * @brief   Transmit bytes from SoftUART in timeout ms
 *
 * @param   data - Transmit buffer's pointer
 *          len - Transmit buffer's length
 *
 * @return  none
 */
static void TransmitBytes(vu8 *data, vu16 len)
{
    u16 length = strnlen((u8 *)data, 127);
    u16 *buffer = malloc(ByteSize);
    if (buffer != NULL)
    {
        for (u16 i = 0; i < (len > 0 ? len : length); i++)
        {
            buffer[0] = buffer[1] = 0x00;
            for (u8 j = 0; j < 8; j++)
            {
                if ((data[i] >> j) & 0x01)
                {
                    buffer[(j + 1) * 2] = 0x5;
                    buffer[(j + 1) * 2 + 1] = 0x5;
                }
                else
                {
                    buffer[(j + 1) * 2] = 0x0;
                    buffer[(j + 1) * 2 + 1] = 0x0;
                }
            }
            if (SoftUARTPara.veryfy)
            {
                switch (SoftUARTPara.veryfy)
                {
                case Odd:
                    if (isOdd(data[i]))
                    {
                        buffer[2 * (SoftUARTPara.bitnum + 1)] = 0x0;

                        buffer[2 * (SoftUARTPara.bitnum + 1) + 1] = 0x0;
                    }
                    else
                    {
                        buffer[2 * (SoftUARTPara.bitnum + 1)] = 0x5;

                        buffer[2 * (SoftUARTPara.bitnum + 1) + 1] = 0x5;
                    }
                    break;

                case Even:
                    if (isOdd(data[i]))
                    {
                        buffer[2 * (SoftUARTPara.bitnum + 1)] = 0x5;

                        buffer[2 * (SoftUARTPara.bitnum + 1) + 1] = 0x5;
                    }
                    else
                    {
                        buffer[2 * (SoftUARTPara.bitnum + 1)] = 0x0;

                        buffer[2 * (SoftUARTPara.bitnum + 1) + 1] = 0x0;
                    }
                    break;

                default:
                    break;
                }
            }

            for (u8 q = 0; q < SoftUARTPara.stopbits; q++)
            {
                buffer[ByteSize - q] = 0x5;
            }
            SoftUARTHardware.txDMA->MADDR = (u32)buffer;
            SoftUARTHardware.txDMA->CNTR = ByteSize;
            SoftUARTHardware.txTIM->CNT = 0;
            DMA_Cmd(SoftUARTHardware.txDMA, ENABLE);
            while (DMA_GetFlagStatus(DMA1_FLAG_TC2) == RESET)
                ;
            DMA_ClearFlag(DMA1_FLAG_TC2);
        }
    }
    free(buffer);
}

/*********************************************************************
 * @fn      ReceiveBytes
 *
 * @brief   Receive bytes from SoftUART in timeout ms
 *
 * @param   data - Receive buffer's pointer
 *         timeout - Return anyway after timeout ms
 *
 * @return  0 - on unsuccess,
 *          other - return the received bytes quantity
 */
static u8 ReceiveBytes(vu8 *data, u16 timeout)
{
    while (ReciveCP == 0 && --timeout)
    {
        Delay_Ms(1);
    }
    if (timeout == 0)
    {
        return 0;
    }
    else
    {
        recout = rec_len;
        memcpy((u8 *)data, (u8 *)RxBuffer, rec_len);
        memset((u8 *)RxBuffer, 0, rec_len + 1);
        ReciveCP = 0;
        rec_len = 0;
        return recout;
    }
}
#if !USER_SOFTUART_COSTOM

/*********************************************************************
 * @fn      Init
 *
 * @brief   The Init function initializes the SoftUART hardware and parameters for communication.
 *
 * @return  none
 */
void Init(void)
{
    // Configure the RCC used
    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOA | RCC_PB2Periph_TIM1 | RCC_PB2Periph_AFIO | RCC_PB2Periph_GPIOD, ENABLE);
    RCC_PB1PeriphClockCmd(RCC_PB1Periph_TIM2, ENABLE);
    RCC_HBPeriphClockCmd(RCC_HBPeriph_DMA1, ENABLE);
    // Select the peripheral used for the SoftUart
    SoftUARTHardware.rxTIM = DEFAULT_RXTIM;
    SoftUARTHardware.rxPort = DEFAULT_RXPORT;
    SoftUARTHardware.rxPin = DEFAULT_RXPIN;
    SoftUARTHardware.txTIM = DEFAULT_TXTIM;
    SoftUARTHardware.txPort = DEFAULT_TXPORT;
    SoftUARTHardware.txPin = DEFAULT_TXPIN;
    SoftUARTHardware.txDMA = DEFAULT_TXDMA;
    // Configure the periph's para
    SoftUARTPara.baud_rate = DEFAULT_BAUDRATE;
    SoftUARTPara.bitnum = DEFAULT_BITNUM;
    SoftUARTPara.flowctrl = DEFAULT_FLOWCTRL;
    SoftUARTPara.veryfy = DEFAULT_VERYFY;
    SoftUARTPara.stopbits = DEFAULT_STOPBITS;
    // Calculate the number of bit to receive and transmit
    ByteSize = (1 + SoftUARTPara.bitnum) * 2 + (SoftUARTPara.stopbits);
    if (SoftUARTPara.veryfy)
    {
        ByteSize += 2;
    }
    // Initial the receive state
    rec_stat = ByteSize;
    // Redirect the functhon
    SoftUARTFunc.TransmitBytes = TransmitBytes;
    SoftUARTFunc.ReceiveBytes = ReceiveBytes;
    // Periph initialize
    RxInit();
    TxInit();
}
#else

/*********************************************************************
 * @fn      Init
 *
 * @brief   The Init function initializes the SoftUART hardware and parameters for communication.
 *
 * @param   baudrate - The baudrate parameter is an unsigned 32-bit integer that represents the desired
 *        baud rate for the SoftUart communication.
 *
 * @return  none
 */
void Init(u32 baudrate)
{
    // Configure the RCC used
    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOA | RCC_PB2Periph_TIM1 | RCC_PB2Periph_AFIO | RCC_PB2Periph_GPIOD, ENABLE);
    RCC_PB1PeriphClockCmd(RCC_PB1Periph_TIM2, ENABLE);
    RCC_HBPeriphClockCmd(RCC_HBPeriph_DMA1, ENABLE);

    // Select the peripheral used for the SoftUart
    SoftUARTHardware.rxTIM = TIM1;
    SoftUARTHardware.rxPort = GPIOD;
    SoftUARTHardware.rxPin = GPIO_Pin_2;
    SoftUARTHardware.txTIM = TIM2;
    SoftUARTHardware.txPort = GPIOD;
    SoftUARTHardware.txPin = GPIO_Pin_4;
    SoftUARTHardware.txDMA = DMA1_Channel2;
    // Configure the periph's para
    SoftUARTPara.baud_rate = baudrate;
    SoftUARTPara.bitnum = bit_8;
    SoftUARTPara.flowctrl = NoFlowControl;
    SoftUARTPara.veryfy = Odd
            ;
    SoftUARTPara.stopbits = Stopbit1;
    // Calculate the number of bit to receive and transmit
    ByteSize = (1 + SoftUARTPara.bitnum) * 2 + (SoftUARTPara.stopbits);
    if (SoftUARTPara.veryfy)
    {
        ByteSize += 2;
    }
    // Initial the receive state
    rec_stat = ByteSize;
    // Redirect the functhon
    SoftUARTFunc.TransmitBytes = TransmitBytes;
    SoftUARTFunc.ReceiveBytes = ReceiveBytes;
    // Periph initialize
    RxInit();
    TxInit();
}
#endif
