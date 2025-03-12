/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00X_rcc.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/01/01
 * Description        : This file provides all the RCC firmware functions.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#ifndef __CH32V00X_RCC_H
#define __CH32V00X_RCC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ch32v00X.h>

/* RCC_Exported_Types */
typedef struct
{
    uint32_t SYSCLK_Frequency; /* returns SYSCLK clock frequency expressed in Hz */
    uint32_t HCLK_Frequency;   /* returns HCLK clock frequency expressed in Hz */
    uint32_t PCLK1_Frequency;  /* returns PCLK1 clock frequency expressed in Hz */
    uint32_t PCLK2_Frequency;  /* returns PCLK2 clock frequency expressed in Hz */
    uint32_t ADCCLK_Frequency; /* returns ADCCLK clock frequency expressed in Hz */
} RCC_ClocksTypeDef;

/* HSE_configuration */
#define RCC_HSE_OFF                      ((uint32_t)0x00000000)
#define RCC_HSE_ON                       ((uint32_t)0x00010000)
#define RCC_HSE_Bypass                   ((uint32_t)0x00040000)

/* PLL_entry_clock_source */
#define RCC_PLLSource_HSI_MUL2           ((uint32_t)0x00000000)
#define RCC_PLLSource_HSE_MUL2           ((uint32_t)0x00010000)

/* System_clock_source */
#define RCC_SYSCLKSource_HSI             ((uint32_t)0x00000000)
#define RCC_SYSCLKSource_HSE             ((uint32_t)0x00000001)
#define RCC_SYSCLKSource_PLLCLK          ((uint32_t)0x00000002)

/* HB_clock_source */
#define RCC_SYSCLK_Div1                  ((uint32_t)0x00000000)
#define RCC_SYSCLK_Div2                  ((uint32_t)0x00000010)
#define RCC_SYSCLK_Div3                  ((uint32_t)0x00000020)
#define RCC_SYSCLK_Div4                  ((uint32_t)0x00000030)
#define RCC_SYSCLK_Div5                  ((uint32_t)0x00000040)
#define RCC_SYSCLK_Div6                  ((uint32_t)0x00000050)
#define RCC_SYSCLK_Div7                  ((uint32_t)0x00000060)
#define RCC_SYSCLK_Div8                  ((uint32_t)0x00000070)
#define RCC_SYSCLK_Div16                 ((uint32_t)0x000000B0)
#define RCC_SYSCLK_Div32                 ((uint32_t)0x000000C0)
#define RCC_SYSCLK_Div64                 ((uint32_t)0x000000D0)
#define RCC_SYSCLK_Div128                ((uint32_t)0x000000E0)
#define RCC_SYSCLK_Div256                ((uint32_t)0x000000F0)

/* RCC_Interrupt_source */
#define RCC_IT_LSIRDY                    ((uint8_t)0x01)
#define RCC_IT_HSIRDY                    ((uint8_t)0x04)
#define RCC_IT_HSERDY                    ((uint8_t)0x08)
#define RCC_IT_PLLRDY                    ((uint8_t)0x10)
#define RCC_IT_CSS                       ((uint8_t)0x80)
#define RCC_IT_SYSCLK_FAIL               ((uint8_t)0x02)

/* ADC_clock_source */
#define RCC_PCLK2_Div1                   ((uint32_t)0x80000000)
#define RCC_PCLK2_Div2                   ((uint32_t)0x00000000)
#define RCC_PCLK2_Div4                   ((uint32_t)0x00004000)
#define RCC_PCLK2_Div6                   ((uint32_t)0x00008000)
#define RCC_PCLK2_Div8                   ((uint32_t)0x0000C000)
#define RCC_PCLK2_Div12                  ((uint32_t)0x0000A000)
#define RCC_PCLK2_Div16                  ((uint32_t)0x0000E000)
#define RCC_PCLK2_Div24                  ((uint32_t)0x0000A800)
#define RCC_PCLK2_Div32                  ((uint32_t)0x0000E800)
#define RCC_PCLK2_Div48                  ((uint32_t)0x0000B000)
#define RCC_PCLK2_Div64                  ((uint32_t)0x0000F000)
#define RCC_PCLK2_Div96                  ((uint32_t)0x0000B800)
#define RCC_PCLK2_Div128                 ((uint32_t)0x0000F800)

/* HB_peripheral */
#define RCC_HBPeriph_DMA1               ((uint32_t)0x00000001)
#define RCC_HBPeriph_SRAM               ((uint32_t)0x00000004)

/* PB2_peripheral */
#define RCC_PB2Periph_AFIO              ((uint32_t)0x00000001)
#define RCC_PB2Periph_GPIOA             ((uint32_t)0x00000004)
#define RCC_PB2Periph_GPIOB             ((uint32_t)0x00000008)
#define RCC_PB2Periph_GPIOC             ((uint32_t)0x00000010)
#define RCC_PB2Periph_GPIOD             ((uint32_t)0x00000020)
#define RCC_PB2Periph_ADC1              ((uint32_t)0x00000200)
#define RCC_PB2Periph_TIM1              ((uint32_t)0x00000800)
#define RCC_PB2Periph_SPI1              ((uint32_t)0x00001000)
#define RCC_PB2Periph_USART2            ((uint32_t)0x00002000)
#define RCC_PB2Periph_USART1            ((uint32_t)0x00004000)

/* PB1_peripheral */
#define RCC_PB1Periph_TIM2              ((uint32_t)0x00000001)
#define RCC_PB1Periph_TIM3              ((uint32_t)0x00000004)
#define RCC_PB1Periph_WWDG              ((uint32_t)0x00000800)
#define RCC_PB1Periph_I2C1              ((uint32_t)0x00200000)
#define RCC_PB1Periph_PWR               ((uint32_t)0x10000000)

/* Clock_source_to_output_on_MCO_pin */
#define RCC_MCO_NoClock                  ((uint8_t)0x00)
#define RCC_MCO_SYSCLK                   ((uint8_t)0x04)
#define RCC_MCO_HSI                      ((uint8_t)0x05)
#define RCC_MCO_HSE                      ((uint8_t)0x06)
#define RCC_MCO_PLLCLK                   ((uint8_t)0x07)

/* RCC_Flag */
#define RCC_FLAG_HSIRDY                  ((uint8_t)0x21)
#define RCC_FLAG_HSERDY                  ((uint8_t)0x31)
#define RCC_FLAG_PLLRDY                  ((uint8_t)0x39)
#define RCC_FLAG_LSIRDY                  ((uint8_t)0x61)
#define RCC_FLAG_SYSCFAL                 ((uint8_t)0x68)
#define RCC_FLAG_ADCRST                  ((uint8_t)0x77)
#define RCC_FLAG_OPCMRST                 ((uint8_t)0x79)
#define RCC_FLAG_PINRST                  ((uint8_t)0x7A)
#define RCC_FLAG_PORRST                  ((uint8_t)0x7B)
#define RCC_FLAG_SFTRST                  ((uint8_t)0x7C)
#define RCC_FLAG_IWDGRST                 ((uint8_t)0x7D)
#define RCC_FLAG_WWDGRST                 ((uint8_t)0x7E)

/* SysTick_clock_source */
#define SysTick_CLKSource_HCLK_Div8      ((uint32_t)0xFFFFFFFB)
#define SysTick_CLKSource_HCLK           ((uint32_t)0x00000004)

/* RCC_HSE_Current_Level */
#define RCC_HSE_C_Level0                 ((uint32_t)0x00000000)
#define RCC_HSE_C_Level1                 ((uint32_t)0x00400000)
#define RCC_HSE_C_Level2                 ((uint32_t)0x00800000)
#define RCC_HSE_C_Level3                 ((uint32_t)0x00C00000)

/* ADC_clock_H_Level_Duty_Cycle */
#define RCC_ADC_H_Level_Mode0   ((uint32_t)0x00000000)
#define RCC_ADC_H_Level_Mode1   ((uint32_t)0x10000000)


void        RCC_DeInit(void);
void        RCC_HSEConfig(uint32_t RCC_HSE);
ErrorStatus RCC_WaitForHSEStartUp(void);
void        RCC_AdjustHSICalibrationValue(uint8_t HSICalibrationValue);
void        RCC_HSICmd(FunctionalState NewState);
void        RCC_PLLConfig(uint32_t RCC_PLLSource);
void        RCC_PLLCmd(FunctionalState NewState);
void        RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource);
uint8_t     RCC_GetSYSCLKSource(void);
void        RCC_HCLKConfig(uint32_t RCC_SYSCLK);
void        RCC_ITConfig(uint8_t RCC_IT, FunctionalState NewState);
void        RCC_ADCCLKConfig(uint32_t RCC_PCLK2);
void        RCC_LSICmd(FunctionalState NewState);
void        RCC_GetClocksFreq(RCC_ClocksTypeDef *RCC_Clocks);
void        RCC_HBPeriphClockCmd(uint32_t RCC_HBPeriph, FunctionalState NewState);
void        RCC_PB2PeriphClockCmd(uint32_t RCC_PB2Periph, FunctionalState NewState);
void        RCC_PB1PeriphClockCmd(uint32_t RCC_PB1Periph, FunctionalState NewState);
void        RCC_PB2PeriphResetCmd(uint32_t RCC_PB2Periph, FunctionalState NewState);
void        RCC_PB1PeriphResetCmd(uint32_t RCC_PB1Periph, FunctionalState NewState);
void        RCC_ClockSecuritySystemCmd(FunctionalState NewState);
void        RCC_MCOConfig(uint8_t RCC_MCO);
FlagStatus  RCC_GetFlagStatus(uint8_t RCC_FLAG);
void        RCC_ClearFlag(void);
ITStatus    RCC_GetITStatus(uint8_t RCC_IT);
void        RCC_ClearITPendingBit(uint8_t RCC_IT);
void        RCC_ClockMonitorCmd(FunctionalState NewState);
void        RCC_HSE_LP_Cmd(FunctionalState NewState);
void        RCC_HSI_LP_Cmd(FunctionalState NewState);
void        RCC_HSECurrentConfig(uint32_t RCC_HSECurrent);
void        RCC_ADCCLKDutyCycleConfig(uint32_t RCC_DutyCycle);

#ifdef __cplusplus
}
#endif

#endif /* __CH32V00X_RCC_H */
