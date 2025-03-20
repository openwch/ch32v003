/********************************** (C) COPYRIGHT *******************************
 * File Name          : M7_Interrupt.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
 * Description        : Interrupt configuration

*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Includes -----------------------------------------------------------------*/
#include "M0_Control_Library.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private functions --------------------------------------------------------*/
void TIM1_UP_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM1_BRK_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//void Waveform_Display (void);

/* Private variables --------------------------------------------------------*/
/* Variables ----------------------------------------------------------------*/
/*********************************************************************
 * @fn      Interrupt_Configuration
 *
 * @brief   Interrupt priority configuration
 *
 * @return  none
 */
void Interrupt_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);             //中断优先级分组配置

    //ADC注入中断配置
    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;              //中断通道ADC_IRQn
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          //从优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //中断使能
    NVIC_Init(&NVIC_InitStructure);                             //配置寄存器实体

    //Timer1中断配置
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;          //中断通道TIM1_UP_IRQn
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          //从优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //中断使能
    NVIC_Init(&NVIC_InitStructure);                             //配置寄存器实体

    //刹车中断配置
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_IRQn;         //中断通道TIM1_BRK_IRQn
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //从优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //中断使能
    NVIC_Init(&NVIC_InitStructure);                             //配置寄存器实体
    TIM_ClearITPendingBit(TIM1, TIM_IT_Break);                  //清除刹车中断标志清除
    TIM_ITConfig(TIM1, TIM_IT_Break,ENABLE);                    //使能刹车中断

    //SysTicK中断配置
    NVIC_InitStructure.NVIC_IRQChannel = SysTicK_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级为1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //从优先级为2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*********************************************************************
 * @fn      TIM1_UP_IRQHandler
 *
 * @brief   Timer 1 underflow interrupt
 *
 * @return  none
 */
__attribute__((section(".highcode")))
void TIM1_UP_IRQHandler(void)
{
    if( RunningStatus_M == PRESTART)//电机启动处理
    {
        TIM1->CH4CVR = 48;//ADC触发时刻
        ADC_Start(ENABLE);//ADC启动
        RunningStatus_M=DIRCHECK;
    }

    if((Flystart_M.Process_mark==TRUE)&&(RunningStatus_M == DIRCHECK))
     {
         Flystart_Switch(&Flystart_M,&SpeedRamp_M);
     }

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);//母线电压采样触发
    DCBUS_Volt_Cal(&ADC_M,ADC1);//母线电压计算

//    Waveform_Display();//虚拟示波器观测
    TIM1->INTFR = (uint16_t)~TIM_FLAG_Update;//清除更新事件中断标志
}
/*********************************************************************
 * @fn      TIM1_BRK_IRQHandler
 *
 * @brief   Brake interrupt
 *
 * @return  none
 */
void TIM1_BRK_IRQHandler(void)
{
    Protection_SetFault(DC_OVER_CURR_HARD_M);
    TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
}
/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void)
{
    while (1)
    {
    }
}
/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void)
{
    NVIC_SystemReset();
    while (1)
    {
    }
}
