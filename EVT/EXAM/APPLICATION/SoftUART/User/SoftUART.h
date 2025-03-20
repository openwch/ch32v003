/********************************** (C) COPYRIGHT *******************************
 * File Name          : SoftUART.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/01/01
 * Description        : Simulate UART with software
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#ifndef USER_SOFTUART_H_
#define USER_SOFTUART_H_

#include "ch32v00X.h"
#include "malloc.h"
#include "string.h"

#define BUFFER_SIZE 1024

#define USER_SOFTUART_COSTOM 1

#if !USER_SOFTUART_COSTOM
#define DEFAULT_RXTIM TIM1
#define DEFAULT_RXPORT GPIOD
#define DEFAULT_RXPIN GPIO_Pin_2
#define DEFAULT_TXTIM TIM2
#define DEFAULT_TXPORT GPIOD
#define DEFAULT_TXPIN GPIO_Pin_4
#define DEFAULT_TXDMA DMA1_Channel2
#define DEFAULT_BAUDRATE 9600
#define DEFAULT_BITNUM bit_8
#define DEFAULT_FLOWCTRL NoFlowControl
#define DEFAULT_VERYFY NoVeryfy
#define DEFAULT_STOPBITS Stopbit1
void Init();
#else
void Init(u32 baudrate);
#endif // !USER_SOFTUART_COSTOM

typedef enum
{
    bit_6 = 6,
    bit_7,
    bit_8
} Bitnum_e;

typedef enum
{
    NoVeryfy=0,
    Odd,
    Even
} Veryfy_e;

/******************************
 * @enum FlowCTRL_e
 * @brief DO NOT USE IT
 * @details This is banned
 */
typedef enum
{
    NoFlowControl,
    RTS_CTS,
    DTS
} FlowCTRL_e;

typedef enum
{
    Stopbit1 = 2,
    Stopbit1_5,
    Stopbit2
} Stopbits_e;

typedef struct
{
#if !USER_SOFTUART_COSTOM
    void (*Init)(); // initialization function  pointer
#else
    void (*Init)(u32 baudrate); // initialization function  pointer
#endif
    void (*TransmitBytes)(vu8 *data, u16 len);  // transmit function pointer
    u8 (*ReceiveBytes)(vu8 *data, u16 timeout); // receive function pointer
} SoftUARTFunc_t;

/*
    The para in structs below can be changed whenever the code is running or not,but after changing the para.
    You should use the "SoftUARTFunc.Init" function to update the parameters,and abandon the peripheral used before.
*/
typedef struct
{
    volatile unsigned long baud_rate; // frequency in Hz used for SoftUART,the range of baud rate is from 2400 to 138000
    volatile Bitnum_e bitnum;         // receive bit's quantity
    volatile Veryfy_e veryfy;         // veryfy way
    volatile FlowCTRL_e flowctrl;     // flow control way
    volatile Stopbits_e stopbits;     // stop bits' size
} SoftUARTPara_t;

typedef struct
{
    GPIO_TypeDef *rxPort;
    volatile uint16_t rxPin;
    TIM_TypeDef *rxTIM;
    DMA_Channel_TypeDef *rxDMA;

    GPIO_TypeDef *txPort;
    volatile uint16_t txPin;
    TIM_TypeDef *txTIM;
    DMA_Channel_TypeDef *txDMA;

    GPIO_TypeDef *rtsPort;
    volatile uint16_t rtsPin;
    GPIO_TypeDef *ctsPort;
    volatile uint16_t ctsPin;
} SoftUARTHardware_t;

extern SoftUARTHardware_t SoftUARTHardware;
extern SoftUARTFunc_t SoftUARTFunc;
extern SoftUARTPara_t SoftUARTPara;

#endif /* USER_SOFTUART_H_ */
