/********************************** (C) COPYRIGHT *******************************
 * File Name          : LED.h
 * Author             : WCH
 * Version            : V1.1
 * Date               : 2025/01/14
 * Description        :
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/******************************************************************************/
#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C" {
#endif
#include "ch32v00X.h"
/*********************************************************************
 * CONSTANTS
 */
#define TKY_BACKLIGHT_NUM   20
 
#define LED_SEG1_PIN	GPIO_Pin_0	//GPIOA
#define LED_SEG2_PIN	GPIO_Pin_3	//GPIOB
#define LED_SEG3_PIN	GPIO_Pin_1	//GPIOB
#define LED_SEG4_PIN	GPIO_Pin_0	//GPIOB

#define LED_SEG1_PORT	GPIOA
#define LED_SEG2_PORT	GPIOB
#define LED_SEG3_PORT	GPIOB
#define LED_SEG4_PORT	GPIOB	

#define LED_COM1_PIN	GPIO_Pin_0	//GPIOD
#define LED_COM2_PIN	GPIO_Pin_7	//GPIOD
#define LED_COM3_PIN	GPIO_Pin_1	//GPIOD
#define LED_COM4_PIN	GPIO_Pin_2	//GPIOC
#define LED_COM5_PIN	GPIO_Pin_1	//GPIOC

#define LED_COM1_PORT	GPIOD
#define LED_COM2_PORT	GPIOD
#define LED_COM3_PORT	GPIOD
#define LED_COM4_PORT	GPIOC
#define LED_COM5_PORT	GPIOC



#define LED_SEG1_ON()	(LED_SEG1_PORT->BSHR = LED_SEG1_PIN)                      	// Set clock line high
#define LED_SEG1_OFF()	(LED_SEG1_PORT->BCR  = LED_SEG1_PIN)                        // Set clock line low
#define LED_SEG2_ON()	(LED_SEG2_PORT->BSHR = LED_SEG2_PIN)                      	// Set clock line high
#define LED_SEG2_OFF()	(LED_SEG2_PORT->BCR  = LED_SEG2_PIN)                        // Set clock line low
#define LED_SEG3_ON()	(LED_SEG3_PORT->BSHR = LED_SEG3_PIN)                      	// Set clock line high
#define LED_SEG3_OFF()	(LED_SEG3_PORT->BCR  = LED_SEG3_PIN)                        // Set clock line low
#define LED_SEG4_ON()	(LED_SEG4_PORT->BSHR = LED_SEG4_PIN)                      	// Set clock line high
#define LED_SEG4_OFF()	(LED_SEG4_PORT->BCR  = LED_SEG4_PIN)                        // Set clock line low

#define LED_COM1_OFF()	(LED_COM1_PORT->BSHR = LED_COM1_PIN)                       // Set clock line high
#define LED_COM1_ON()	(LED_COM1_PORT->BCR  = LED_COM1_PIN)                       // Set clock line low
#define LED_COM2_OFF()	(LED_COM2_PORT->BSHR = LED_COM2_PIN)                       // Set clock line high
#define LED_COM2_ON()	(LED_COM2_PORT->BCR  = LED_COM2_PIN)                       // Set clock line low
#define LED_COM3_OFF()	(LED_COM3_PORT->BSHR = LED_COM3_PIN)                       // Set clock line high
#define LED_COM3_ON()	(LED_COM3_PORT->BCR  = LED_COM3_PIN)                       // Set clock line low
#define LED_COM4_OFF()	(LED_COM4_PORT->BSHR = LED_COM4_PIN)                       // Set clock line high
#define LED_COM4_ON()	(LED_COM4_PORT->BCR  = LED_COM4_PIN)                       // Set clock line low
#define LED_COM5_OFF()	(LED_COM5_PORT->BSHR = LED_COM5_PIN)                       // Set clock line high
#define LED_COM5_ON()	(LED_COM5_PORT->BCR  = LED_COM5_PIN)                       // Set clock line low

typedef enum
{
	Backlight_1,
	Backlight_2,
	Backlight_3,
	Backlight_4,
	Backlight_5,
	Backlight_6,
	Backlight_7,
	Backlight_8,
	Backlight_9,
	Backlight_0,
	Backlight_H,
	Backlight_X,
} BacklightTypeDef;

typedef enum
{
	Normal,
	Breathing,
} BacklightWorkTypeDef;

typedef struct
{
    uint16_t TKY_BacklightOnTime;
    uint8_t BacklightStates;
    uint8_t RFU;
} TKY_Backlight_S;
/*********************************************************************
*********************************************************************/
// Initialize backlight and buzzer
extern void TKY_BacklightInit(void);
extern void TKY_BacklightTaskStart();
extern void TKY_BacklightTaskStop();
extern void TKY_BacklightOn (void);
extern void TKY_BacklightOff (void);
extern void TKY_KeyBacklightOut(uint8_t key, FunctionalState s);
extern void TKY_BacklightProcess (void);

#ifdef __cplusplus
}
#endif

#endif
