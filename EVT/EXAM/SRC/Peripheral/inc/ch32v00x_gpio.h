/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00X_gpio.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/01/01
 * Description        : This file contains all the functions prototypes for the
 *                      GPIO firmware library.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#ifndef __CH32V00X_GPIO_H
#define __CH32V00X_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ch32v00X.h>

/* Output Maximum frequency selection */
typedef enum
{
    GPIO_Speed_30MHz = 1,
} GPIOSpeed_TypeDef;

/* Configuration Mode enumeration */
typedef enum
{
    GPIO_Mode_AIN = 0x0,
    GPIO_Mode_IN_FLOATING = 0x04,
    GPIO_Mode_IPD = 0x28,
    GPIO_Mode_IPU = 0x48,
    GPIO_Mode_Out_OD = 0x14,
    GPIO_Mode_Out_PP = 0x10,
    GPIO_Mode_AF_OD = 0x1C,
    GPIO_Mode_AF_PP = 0x18
} GPIOMode_TypeDef;

/* GPIO Init structure definition */
typedef struct
{
    uint16_t GPIO_Pin; /* Specifies the GPIO pins to be configured.
                          This parameter can be any value of @ref GPIO_pins_define */

    GPIOSpeed_TypeDef GPIO_Speed; /* Specifies the speed for the selected pins.
                                     This parameter can be a value of @ref GPIOSpeed_TypeDef */

    GPIOMode_TypeDef GPIO_Mode; /* Specifies the operating mode for the selected pins.
                                   This parameter can be a value of @ref GPIOMode_TypeDef */
} GPIO_InitTypeDef;

/* Bit_SET and Bit_RESET enumeration */
typedef enum
{
    Bit_RESET = 0,
    Bit_SET
} BitAction;

/* GPIO_pins_define */
#define GPIO_Pin_0                     ((uint16_t)0x0001) /* Pin 0 selected */
#define GPIO_Pin_1                     ((uint16_t)0x0002) /* Pin 1 selected */
#define GPIO_Pin_2                     ((uint16_t)0x0004) /* Pin 2 selected */
#define GPIO_Pin_3                     ((uint16_t)0x0008) /* Pin 3 selected */
#define GPIO_Pin_4                     ((uint16_t)0x0010) /* Pin 4 selected */
#define GPIO_Pin_5                     ((uint16_t)0x0020) /* Pin 5 selected */
#define GPIO_Pin_6                     ((uint16_t)0x0040) /* Pin 6 selected */
#define GPIO_Pin_7                     ((uint16_t)0x0080) /* Pin 7 selected */
#define GPIO_Pin_All                   ((uint16_t)0x00FF) /* All pins selected */

/* GPIO_Remap_define */
#define GPIO_PartialRemap1_SPI1        ((uint32_t)0x08100001) /* SPI1 Partial Alternate Function mapping */
#define GPIO_PartialRemap2_SPI1        ((uint32_t)0x08100002) /* SPI1 Partia2 Alternate Function mapping */
#define GPIO_PartialRemap3_SPI1        ((uint32_t)0x08100003) /* SPI1 Partia3 Alternate Function mapping */
#define GPIO_PartialRemap4_SPI1        ((uint32_t)0x08100004) /* SPI1 Partia4 Alternate Function mapping */
#define GPIO_PartialRemap5_SPI1        ((uint32_t)0x08100005) /* SPI1 Partia5 Alternate Function mapping */
#define GPIO_FullRemap_SPI1            ((uint32_t)0x08100006) /* SPI1 Full Alternate Function mapping */
#define GPIO_PartialRemap1_I2C1        ((uint32_t)0x08130008) /* I2C1 Partia1 Alternate Function mapping */
#define GPIO_PartialRemap2_I2C1        ((uint32_t)0x08130010) /* I2C1 Partia2 Alternate Function mapping */
#define GPIO_PartialRemap3_I2C1        ((uint32_t)0x08130018) /* I2C1 Partia3 Alternate Function mapping */
#define GPIO_FullRemap4_I2C1           ((uint32_t)0x08130020) /* I2C1 Full Alternate Function mapping */
#define GPIO_PartialRemap1_USART1      ((uint32_t)0x00160040) /* USART1 Partial1 Alternate Function mapping */
#define GPIO_PartialRemap2_USART1      ((uint32_t)0x00160080) /* USART1 Partial2 Alternate Function mapping */
#define GPIO_PartialRemap3_USART1      ((uint32_t)0x001600C0) /* USART1 Partial3 Alternate Function mapping */
#define GPIO_PartialRemap4_USART1      ((uint32_t)0x00160100) /* USART1 Partial4 Alternate Function mapping */
#define GPIO_PartialRemap5_USART1      ((uint32_t)0x00160140) /* USART1 Partial5 Alternate Function mapping */
#define GPIO_PartialRemap6_USART1      ((uint32_t)0x00160180) /* USART1 Partial6 Alternate Function mapping */
#define GPIO_PartialRemap7_USART1      ((uint32_t)0x001601C0) /* USART1 Partial7 Alternate Function mapping */
#define GPIO_PartialRemap8_USART1      ((uint32_t)0x00160200) /* USART1 Partial8 Alternate Function mapping */
#define GPIO_FullRemap_USART1          ((uint32_t)0x00160240) /* USART1 Full Alternate Function mapping */
#define GPIO_PartialRemap1_TIM1        ((uint32_t)0x001A0400) /* TIM1 Partial1 Alternate Function mapping */
#define GPIO_PartialRemap2_TIM1        ((uint32_t)0x001A0800) /* TIM1 Partial2 Alternate Function mapping */
#define GPIO_PartialRemap3_TIM1        ((uint32_t)0x001A0C00) /* TIM1 Partial3 Alternate Function mapping */
#define GPIO_PartialRemap4_TIM1        ((uint32_t)0x001A1000) /* TIM1 Partial4 Alternate Function mapping */
#define GPIO_PartialRemap5_TIM1        ((uint32_t)0x001A1400) /* TIM1 Partial5 Alternate Function mapping */
#define GPIO_PartialRemap6_TIM1        ((uint32_t)0x001A1800) /* TIM1 Partial6 Alternate Function mapping */
#define GPIO_PartialRemap7_TIM1        ((uint32_t)0x001A1C00) /* TIM1 Partial7 Alternate Function mapping */
#define GPIO_PartialRemap8_TIM1        ((uint32_t)0x001A2000) /* TIM1 Partial8 Alternate Function mapping */
#define GPIO_FullRemap_TIM1            ((uint32_t)0x001A2400) /* TIM1 Full Alternate Function mapping */
#define GPIO_PartialRemap1_TIM2        ((uint32_t)0x084E0001) /* TIM2 Partial1 Alternate Function mapping */
#define GPIO_PartialRemap2_TIM2        ((uint32_t)0x084E0002) /* TIM2 Partial2 Alternate Function mapping */
#define GPIO_PartialRemap3_TIM2        ((uint32_t)0x084E0003) /* TIM2 Partial3 Alternate Function mapping */
#define GPIO_PartialRemap4_TIM2        ((uint32_t)0x084E0004) /* TIM2 Partial4 Alternate Function mapping */
#define GPIO_PartialRemap5_TIM2        ((uint32_t)0x084E0005) /* TIM2 Partial5 Alternate Function mapping */
#define GPIO_PartialRemap6_TIM2        ((uint32_t)0x084E0006) /* TIM2 Partial6 Alternate Function mapping */
#define GPIO_FullRemap_TIM2            ((uint32_t)0x084E0007) /* TIM2 Full Alternate Function mapping */
#define GPIO_Remap_PA1_2               ((uint32_t)0x00200002) /* PA1 and PA2 Alternate Function mapping */
#define GPIO_Remap_ADC1_ETRGINJ        ((uint32_t)0x00200004) /* ADC1 External Trigger Injected Conversion remapping */
#define GPIO_Remap_ADC1_ETRGREG        ((uint32_t)0x00200008) /* ADC1 External Trigger Regular Conversion remapping */
#define GPIO_PartialRemap1_USART2      ((uint32_t)0x08240010) /* USART2 Partial1 Alternate Function mapping */
#define GPIO_PartialRemap2_USART2      ((uint32_t)0x08240020) /* USART2 Partial2 Alternate Function mapping */
#define GPIO_PartialRemap3_USART2      ((uint32_t)0x08240030) /* USART2 Partial3 Alternate Function mapping */
#define GPIO_PartialRemap4_USART2      ((uint32_t)0x08240040) /* USART2 Partial4 Alternate Function mapping */
#define GPIO_PartialRemap5_USART2      ((uint32_t)0x08240050) /* USART2 Partial5 Alternate Function mapping */
#define GPIO_FullRemap_USART2          ((uint32_t)0x08240060) /* USART2 Full Alternate Function mapping */
#define GPIO_Remap_LSI_CAL             ((uint32_t)0x00200080) /* LSI calibration Alternate Function mapping */
#define GPIO_Remap_SDI_Disable         ((uint32_t)0x08300400) /* SDI Disabled */

/* GPIO_Port_Sources */
#define GPIO_PortSourceGPIOA           ((uint8_t)0x00)
#define GPIO_PortSourceGPIOB           ((uint8_t)0x01)
#define GPIO_PortSourceGPIOC           ((uint8_t)0x02)
#define GPIO_PortSourceGPIOD           ((uint8_t)0x03)

/* GPIO_Pin_sources */
#define GPIO_PinSource0                ((uint8_t)0x00)
#define GPIO_PinSource1                ((uint8_t)0x01)
#define GPIO_PinSource2                ((uint8_t)0x02)
#define GPIO_PinSource3                ((uint8_t)0x03)
#define GPIO_PinSource4                ((uint8_t)0x04)
#define GPIO_PinSource5                ((uint8_t)0x05)
#define GPIO_PinSource6                ((uint8_t)0x06)
#define GPIO_PinSource7                ((uint8_t)0x07)

void     GPIO_DeInit(GPIO_TypeDef *GPIOx);
void     GPIO_AFIODeInit(void);
void     GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct);
void     GPIO_StructInit(GPIO_InitTypeDef *GPIO_InitStruct);
uint8_t  GPIO_ReadInputDataBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadInputData(GPIO_TypeDef *GPIOx);
uint8_t  GPIO_ReadOutputDataBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadOutputData(GPIO_TypeDef *GPIOx);
void     GPIO_SetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void     GPIO_ResetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void     GPIO_WriteBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, BitAction BitVal);
void     GPIO_Write(GPIO_TypeDef *GPIOx, uint16_t PortVal);
void     GPIO_PinLockConfig(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void     GPIO_EventOutputConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);
void     GPIO_EventOutputCmd(FunctionalState NewState);
void     GPIO_PinRemapConfig(uint32_t GPIO_Remap, FunctionalState NewState);
void     GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);
void     GPIO_IPD_Unused(void);

#ifdef __cplusplus
}
#endif

#endif /* __CH32V00X_GPIO_H */
