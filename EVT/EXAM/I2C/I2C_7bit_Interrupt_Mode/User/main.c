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
 7-bit addressing mode, master/slave mode, transceiver routine:
 I2C1_SCL(PC2)\I2C1_SDA(PC1).
 This routine demonstrates that Master sends and Slave receives using interrupt.
Note: The two boards download the Master and Slave programs respectively,
and power on at the same time.
     Hardware connection:PC2 -- PC2
                         PC1 -- PC1

*/

#include "debug.h"

/* I2C Mode Definition */
#define HOST_MODE   0
#define SLAVE_MODE   1

/* I2C Communication Mode Selection */
#define I2C_MODE   HOST_MODE
//#define I2C_MODE   SLAVE_MODE

/* Global define */
#define Size   6
#define RXAdderss   0x02
#define TxAdderss   0x02

/* Global Variable */
u8 TxData[Size] = { 0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6 };
u8 RxData[Size] = {0};

/*
 * 0 - start
 * 1 - After sending the start signal
 * 2 - After sending the write address
 * 3 - End of send data
 * 4 - After sending the repeat start signal
 * 5 - After sending the read address
 * 0xff - end
 * */
volatile uint8_t master_sate = 0;
volatile uint16_t master_recv_len = 0;
volatile uint16_t master_send_len = 0;

/*
 * 0 - start
 * 0xff - end
 * */
volatile uint8_t slave_state = 0;
volatile uint16_t slave_recv_len = 0;
volatile uint16_t slave_send_len = 0;

void I2C1_EV_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void I2C1_ER_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));


/*********************************************************************
 * @fn      IIC_Init
 *
 * @brief   Initializes the IIC peripheral.
 *
 * @return  none
 */
void IIC_Init(u32 bound, u16 address)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    I2C_InitTypeDef I2C_InitTSturcture={0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_PB2PeriphClockCmd( RCC_PB2Periph_GPIOC | RCC_PB2Periph_AFIO, ENABLE );
    RCC_PB1PeriphClockCmd( RCC_PB1Periph_I2C1, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    I2C_InitTSturcture.I2C_ClockSpeed = bound;
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitTSturcture.I2C_OwnAddress1 = address;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init( I2C1, &I2C_InitTSturcture );

    I2C_Cmd( I2C1, ENABLE );


    NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

    NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

    I2C_ITConfig( I2C1, I2C_IT_BUF, ENABLE );
    I2C_ITConfig( I2C1, I2C_IT_EVT, ENABLE );
    I2C_ITConfig( I2C1, I2C_IT_ERR, ENABLE );

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
    u8 i = 0,j = 0;
    Delay_Init();
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init( 460800 );
#endif
    printf( "SystemClk:%d\r\n", SystemCoreClock );

#if (I2C_MODE == HOST_MODE)
    printf("IIC Host mode\r\n");

    IIC_Init( 80000, TxAdderss);
    Delay_Ms(1000);
        for (j = 0; j < 3; ++j) {
            while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );
            I2C_GenerateSTART(I2C1, ENABLE);

            while(master_sate != 0xff);
              printf( "RxData:\r\n" );
              for( i = 0; i < 6; i++ )
              {
                 printf( "%02x ", RxData[i] );
              }
              printf("\n");

            // Reset the host state machine
            master_sate = 0;
            master_recv_len = 0;
            master_send_len = 0;
            Delay_Ms(500);
        }





#elif (I2C_MODE == SLAVE_MODE)
    printf("IIC Slave mode\r\n");
    IIC_Init( 80000, RXAdderss);

    i = 0;
    while(1){
       while(slave_state != 0xff);
       printf( "RxData:\r\n" );
       for( i = 0; i < 6; i++ )
       {
           printf( "%02x ", RxData[i] );
       }
       printf("\n");
       // Reset the slave state machine
       slave_state = 0;
       slave_recv_len = 0;
       slave_send_len = 0;
    }


#endif

    while(1);
}


void I2C1_EV_IRQHandler(void )
{
#if (I2C_MODE == HOST_MODE)
    if( I2C_GetITStatus( I2C1, I2C_IT_SB ) != RESET )
      {
          if(master_sate == 0 ){
              master_sate = 1;
              I2C_Send7bitAddress( I2C1, 0x02, I2C_Direction_Transmitter );
          }else{
              master_sate = 4;
              I2C_Send7bitAddress( I2C1, 0x02, I2C_Direction_Receiver );
          }
      }

    else if( I2C_GetITStatus( I2C1, I2C_IT_ADDR ) != RESET )
    {
        if(master_sate == 1){
            master_sate = 2;
        }
        if(master_sate == 4){
            master_sate = 5;
        }
        ((void)I2C_ReadRegister(I2C1, I2C_Register_STAR2));
    }
    else if( I2C_GetITStatus( I2C1, I2C_IT_TXE ) != RESET )
      {
        if(master_sate == 2){
            if(master_send_len<6){
                I2C_SendData( I2C1, TxData[master_send_len] );
                master_send_len++;
            }else{
                master_sate = 3;
                I2C_GenerateSTART(I2C1, ENABLE);
                I2C_SendData( I2C1, 0xff ); // dummy byte, to prevent next TxE
            }
        }
      }
    else if( I2C_GetITStatus( I2C1, I2C_IT_RXNE ) != RESET )
       {

           if(master_sate == 5)
           {
               if(master_recv_len<6){
                   RxData[master_recv_len] = I2C_ReceiveData(I2C1);
                   master_recv_len++;
                   if(master_recv_len==5){
                        I2C_NACKPositionConfig(I2C1,I2C_NACKPosition_Next); // clear ack
                        I2C_GenerateSTOP( I2C1, ENABLE );
                    }
                   if(master_recv_len==6){
                       master_sate = 0xff;
                   }
               }else{

               }
           }
       }
#elif (I2C_MODE == SLAVE_MODE)
    if( I2C_GetITStatus( I2C1, I2C_IT_ADDR ) != RESET )
    {
        if(I2C_GetFlagStatus( I2C1, I2C_FLAG_TRA )&&I2C_GetFlagStatus( I2C1, I2C_FLAG_TXE ))
        {
            //write mode
        }else
        {
            //read mode
            ((void)I2C_ReadRegister(I2C1, I2C_Register_STAR2));
        }
    }
    else if( I2C_GetITStatus( I2C1, I2C_IT_RXNE ) != RESET )
   {
        RxData[slave_recv_len] = I2C_ReceiveData(I2C1);
        slave_recv_len++;
   }else if( I2C_GetITStatus( I2C1, I2C_IT_STOPF ) != RESET )
   {
       I2C1->CTLR1 &= I2C1->CTLR1;
       ((void)(I2C1->STAR1));
   }

    else if( I2C_GetITStatus( I2C1, I2C_IT_BTF ) != RESET )
   {
        ((void)I2C_ReadRegister( I2C1, I2C_Register_STAR1));
        ((void)I2C_ReceiveData(I2C1));
   }
   else if( I2C_GetITStatus( I2C1, I2C_IT_SB ) != RESET )
  {
       ((void)I2C_ReadRegister( I2C1, I2C_Register_STAR1));
      ((void)I2C_ReceiveData(I2C1));
  }
   else if( I2C_GetITStatus( I2C1, I2C_IT_TXE ) != RESET )
  {
       I2C_SendData(I2C1,  RxData[slave_send_len]);
      slave_send_len++;

  }
#endif
   else{
       printf( "unknown i2c event \n" );
       printf("sr1 %x \nsr2 %x \n",I2C1->STAR1,I2C1->STAR2);
   }
}



void I2C1_ER_IRQHandler(void)
{
//err
#if (I2C_MODE == HOST_MODE)


#elif (I2C_MODE == SLAVE_MODE)
    if( I2C_GetITStatus( I2C1, I2C_IT_AF ) )
    {
        I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
        slave_state = 0xff;
    }else{
        //err
    }
#endif
}
