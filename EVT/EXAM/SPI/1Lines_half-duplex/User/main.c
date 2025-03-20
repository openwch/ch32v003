/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/06/05
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *single wire half duplex mode, master/slave mode, data transceiver:
 *Master:SPI1_SCK(PC5)\SPI1_MOSI(PC6).
 *Slave:SPI1_SCK(PC5)\SPI1_MISO(PC7).
 *
 *This routine demonstrates that Master sends and Slave receives.
 *Note: The two boards download the Master and Slave programs respectively,
 *and power on at the same time.
 *     Hardware connection:PC5 -- PC5
 *           PC7-- PC6
 *When using SPI slave mode to send data:
 *  -the CPOL bit should be set to 1
 *  -the data should be sent using spi mode 2 or spi mode 3.
 */

#include "debug.h"

/* Global define */

/* Global Variable */
volatile u8 Txval=0, Rxval=0;

/* SPI Mode Definition */
#define HOST_MODE   0
#define SLAVE_MODE   1

/* SPI Communication Mode Selection */
//#define SPI_MODE   HOST_MODE
#define SPI_MODE   SLAVE_MODE

#define Size 18

u16 TxData[Size] = { 0x0101, 0x0202, 0x0303, 0x0404, 0x0505, 0x0606,
                     0x1111, 0x1212, 0x1313, 0x1414, 0x1515, 0x1616,
                     0x2121, 0x2222, 0x2323, 0x2424, 0x2525, 0x2626 };
u16 RxData[Size];

/*********************************************************************
 * @fn      SPI_1Lines_HalfDuplex_Init
 *
 * @brief   Configuring the SPI for half-duplex communication.
 *
 * @return  none
 */
void SPI_1Lines_HalfDuplex_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    SPI_InitTypeDef SPI_InitStructure={0};
    NVIC_InitTypeDef NVIC_InitStructure={0};

    RCC_PB2PeriphClockCmd( RCC_PB2Periph_GPIOC | RCC_PB2Periph_SPI1, ENABLE );

#if (SPI_MODE == HOST_MODE)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

#elif (SPI_MODE == SLAVE_MODE)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOC, &GPIO_InitStructure );
#endif


#if (SPI_MODE == HOST_MODE)
    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;

#elif (SPI_MODE == SLAVE_MODE)
    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Rx;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;

#endif

    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init( SPI1, &SPI_InitStructure );

    NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

#if (SPI_MODE == SLAVE_MODE)
    SPI_I2S_ITConfig( SPI1, SPI_I2S_IT_RXNE , ENABLE );

#endif

    SPI_Cmd( SPI1, ENABLE );
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
    u8 i;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(115200);
#endif
    printf("SystemClk:%d\r\n",SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

#if (SPI_MODE == SLAVE_MODE)
    printf("SLAVE Mode\r\n");
    Delay_Ms(1000);

#endif
    SPI_1Lines_HalfDuplex_Init();

#if (SPI_MODE == HOST_MODE)
    printf("HOST Mode\r\n");
    Delay_Ms(2000);
    SPI_I2S_ITConfig( SPI1, SPI_I2S_IT_TXE , ENABLE );

#endif

    while(1)
    {
#if (SPI_MODE == HOST_MODE)
        while( Txval<18 );

        while( SPI_I2S_GetFlagStatus( SPI1, SPI_I2S_FLAG_TXE ) != RESET )
        {
            if(SPI_I2S_GetFlagStatus( SPI1, SPI_I2S_FLAG_BSY ) == RESET)
            {
                SPI_Cmd( SPI1, DISABLE );
                while(1);
            }
        }

#elif (SPI_MODE == SLAVE_MODE)
        while( Rxval<18 );

        for( i=0; i<18; i++ ){
            printf( "Rxdata:%04x\r\n", RxData[i] );
        }

        while(1);

#endif
    }
}


void SPI1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      SPI1_IRQHandler
 *
 * @brief   This function handles SPI1 exception.
 *
 * @return  none
 */
void SPI1_IRQHandler(void)
{
#if (SPI_MODE == HOST_MODE)
    if( SPI_I2S_GetITStatus( SPI1, SPI_I2S_IT_TXE ) != RESET )
    {
        SPI_I2S_SendData( SPI1, TxData[Txval++] );
        if( Txval == 18 )
        {
            SPI_I2S_ITConfig( SPI1, SPI_I2S_IT_TXE , DISABLE );
        }
    }

#elif (SPI_MODE == SLAVE_MODE)
    RxData[Rxval++] = SPI_I2S_ReceiveData( SPI1 );

#endif
}
