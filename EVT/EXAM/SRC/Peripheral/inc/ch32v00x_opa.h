/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00X_opa.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/01/01
 * Description        : This file contains all the functions prototypes for the
 *                      OPA firmware library.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#ifndef __CH32V00X_OPA_H
#define __CH32V00X_OPA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ch32v00X.h"

/* OPA_PSEL_POLL_enumeration */
typedef enum
{
    CHP_OPA_POLL_OFF = 0,
    CHP_OPA_POLL_ON,
} OPA_PSEL_POLL_TypeDef;

/* OPA_PSEL_POLL_NUM_enumeration */
typedef enum
{
    CHP_POLL_NUM_1 = 0,
    CHP_POLL_NUM_2,
    CHP_POLL_NUM_3
} OPA_PSEL_POLL_NUM_TypeDef;

/* OPA_RST_EN_enumeration */
typedef enum
{
    RST_OPA_OFF = 0,
    RST_OPA_ON,
} OPA_RST_EN_TypeDef;

/* OPA_SETUP_CFG_enumeration */
typedef enum
{
    OPA_SETUP_CFG_0 = 0,
    OPA_SETUP_CFG_1,
    OPA_SETUP_CFG_2 = 3,
} OPA_SETUP_CFG_TypeDef;

/* OPA_POLL_AUTO_enumeration */
typedef enum
{
    OPA_POLL_AUTO_OFF = 0,
    OPA_POLL_AUTO_ON,
} OPA_POLL_AUTO_TypeDef;

/* OPA_OUT_IE_enumeration */
typedef enum
{
    OUT_IE_OFF= 0,
    OUT_IE_ON,
} OPA_OUT_IE_TypeDef;

/* OPA_NMI_IE_enumeration */
typedef enum
{
    NMI_IE_OFF = 0,
    NMI_IE_ON,
} OPA_NMI_IE_TypeDef;

/* OPA_POLL_CH1_definition */
typedef enum
{
    OPA_POLL_CH1_PA2 = 0,
    OPA_POLL_CH1_PD7,
    OPA_POLL_CH1_PD3,
    OPA_POLL_CH1_PD1
} OPA_POLL_CH1_TypeDef;

/* OPA_POLL_CH2_definition */
typedef enum
{
    OPA_POLL_CH2_PA2 = 0,
    OPA_POLL_CH2_PD7,
    OPA_POLL_CH2_PD3,
    OPA_POLL_CH2_PD1
} OPA_POLL_CH2_TypeDef;

/* OPA_POLL_CH3_definition */
typedef enum
{
    OPA_POLL_CH3_PA2 = 0,
    OPA_POLL_CH3_PD7,
    OPA_POLL_CH3_PD3,
    OPA_POLL_CH3_PD1
} OPA_POLL_CH3_TypeDef;

/* OPA_POLL_SEL_enumeration */
typedef enum
{
    OPA_POLL_SEL_SOFT = 0,
    OPA_POLL_SEL_TIM1_CH4,
    OPA_POLL_SEL_TIM2_CH4,
    OPA_POLL_SEL_TIM3_CH1,
    OPA_POLL_SEL_TIM3_CH2
} OPA_POLL_SEL_TypeDef;

/* OPA_out_channel_enumeration */
typedef enum
{
    OUT_IO_OUT0 = 0,
    OUT_IO_OUT1,
    OUT_CMP2_ONLY = 3
} OPA_Mode_TypeDef;

/* OPA_PSEL_enumeration */
typedef enum
{
    CHP0 = 0,
    CHP1,
    CHP2,
    CHP3,
} OPA_PSEL_TypeDef;

/* OPA_NSEL_enumeration */
typedef enum
{
    CHN0 = 0,
    CHN1,
    CHN_PGA_4xIN = 3,
    CHN_PGA_8xIN,
    CHN_PGA_16xIN,
    CHN_PGA_32xIN,
    CHN_OFF,
} OPA_NSEL_TypeDef;

/* OPA_FB_enumeration */
typedef enum
{
    FB_OFF = 0,
    FB_ON
} OPA_FB_TypeDef;

/* OPA_PGADIF_enumeration */
typedef enum
{
    PGADIF_OFF = 0,
    PGADIF_ON
} OPA_PGADIF_TypeDef;

/* OPA_PGA_VBEN_enumeration */
typedef enum
{
    PGA_VBEN_OFF = 0,
    PGA_VBEN_ON,
} OPA_PGA_VBEN_TypeDef;

/* OPA_PGA_VBSEL_enumeration */
typedef enum
{
    PGA_VBSEL_VDD_DIV2 = 0,
    PGA_VBSEL_VDD_DIV4,
} OPA_PGA_VBSEL_TypeDef;

/* OPA_VBCMPSEL_enumeration */
typedef enum
{
    VBCMPSEL_Mode_0 = 0,
    VBCMPSEL_Mode_1,
    VBCMPSEL_Mode_2,
    VBCMPSEL_OFF
} OPA_VBCMPSEL_TypeDef;

/* OPA_HIGH_SPEED_enumeration */
typedef enum
{
    HS_OFF = 0,
    HS_ON
} OPA_HIGH_SPEED_TypeDef;

/* OPA Init Structure definition */
typedef struct
{
    OPA_PSEL_POLL_TypeDef     PSEL_POLL;/* Specifies the positive channel poll of OPA */
    OPA_PSEL_POLL_NUM_TypeDef POLL_NUM; /* Specifies the number of OPA poll */
    OPA_RST_EN_TypeDef        RST_EN;   /* Specifies the reset source of OPA */
    OPA_SETUP_CFG_TypeDef     SETUP_CFG;/* Specifies the ADC conversion interval for the OPA */
    OPA_POLL_AUTO_TypeDef     POLL_AT;  /* specifies auto Poll of OPA */
    OPA_OUT_IE_TypeDef        OUT_IE;   /* Specifies the out interrupt of OPA */
    OPA_NMI_IE_TypeDef        NMI_IE;   /* Specifies the out NMI interrupt of OPA */
    OPA_POLL_CH1_TypeDef      POLL_CH1; /* Specifies the poll channel 1 of OPA */
    OPA_POLL_CH2_TypeDef      POLL_CH2; /* Specifies the poll channel 2 of OPA */
    OPA_POLL_CH3_TypeDef      POLL_CH3; /* Specifies the poll channel 3 of OPA */
    OPA_POLL_SEL_TypeDef      POLL_SEL; /* specifies Poll Trigger Event of OPA */
    OPA_Mode_TypeDef          Mode;     /* Specifies the mode of OPA */
    OPA_PSEL_TypeDef          PSEL;     /* Specifies the positive channel of OPA */
    OPA_NSEL_TypeDef          NSEL;     /* Specifies the negative channel of OPA */
    OPA_FB_TypeDef            FB;       /* Specifies the internal feedback resistor of OPA */
    OPA_PGADIF_TypeDef        PGADIF;   /* OPA with NSEL1 for use as a PGA */
    OPA_PGA_VBEN_TypeDef      PGA_VBEN; /* Enable PGA mode positive reference voltage */
    OPA_PGA_VBSEL_TypeDef     PGA_VBSEL;/* Specifies the positive reference voltage for PGA mode */
    OPA_VBCMPSEL_TypeDef      VBCMPSEL; /* Specifies the CMP2 negative reference voltage */
    OPA_HIGH_SPEED_TypeDef    OPA_HS;   /* specifies high speed mode enable of OPA */
} OPA_InitTypeDef;

/* CMP_member_enumeration */
typedef enum
{
    CMP1 = 0,
    CMP2
} CMP_Num_TypeDef;

/* CMP_PSEL_POLL_enumeration */
typedef enum
{
    CHP_CMP1_POLL_OFF = 0,
    CHP_CMP1_POLL_ON,
} CMP_PSEL_POLL_TypeDef;

/* OPA_PSEL_POLL_NUM_enumeration */
typedef enum
{
    CMP_POLL_NUM_1 = 0,
    CMP_POLL_NUM_2,
    CMP_POLL_NUM_3
} CMP_PSEL_POLL_NUM_TypeDef;

/* CMP_OUT_IE_enumeration */
typedef enum
{
    CMP_OUT_IE_OFF= 0,
    CMP_OUT_IE_ON
} CMP_OUT_IE_TypeDef;

/* CMP_CNT_IE_enumeration */
typedef enum
{
    CMP_CNT_IE_OFF = 0,
    CMP_CNT_IE_ON,
} CMP_CNT_IE_TypeDef;

/* CMP_POLL_CH1_definition */
typedef enum
{
    CMP_POLL_CH1_PC5 = 0,
    CMP_POLL_CH1_PB3,
    CMP_POLL_CH1_PD2
} CMP_POLL_CH1_TypeDef;

/* CMP_POLL_CH2_definition */
typedef enum
{
    CMP_POLL_CH2_PC5 = 0,
    CMP_POLL_CH2_PB3,
    CMP_POLL_CH2_PD2
} CMP_POLL_CH2_TypeDef;

/* CMP_POLL_CH3_definition */
typedef enum
{
    CMP_POLL_CH3_PC5 = 0,
    CMP_POLL_CH3_PB3,
    CMP_POLL_CH3_PD2
} CMP_POLL_CH3_TypeDef;

/* CMP_out_mode_enumeration */
typedef enum
{
    OUT_IO0 = 0,
    OUT_IO_TIM1_CH4,
    OUT_IO_TIM2_CH4
} CMP_OutMode_TypeDef;

/* CMP_NSEL_enumeration */
typedef enum
{
    CMP_CHN0 = 0,
    CMP_CHN1,
    CMP_CHN2
} CMP_NSEL_TypeDef;

/* CMP_PSEL_enumeration */
typedef enum
{
    CMP_CHP0 = 0,
    CMP_CHP1,
    CMP_CHP2
} CMP_PSEL_TypeDef;

/* CMP_HYEN_enumeration */
typedef enum
{
    CMP_HYEN_OFF = 0,
    CMP_HYEN_ON,
} CMP_HYEN_TypeDef;

/* CMP_RMID_enumeration */
typedef enum
{
    CMP_RMID_OFF = 0,
    CMP_RMID_ON
} CMP_RMID_TypeDef;

/* CMP1 Init Structure definition */
typedef struct
{
    uint16_t                    CMP_POLL_Interval; /* CMP polling interval = (CMP_POLL_Interval+1)*1us
                                                   This parameter must range from 0 to 0x1FF.*/
    CMP_PSEL_POLL_TypeDef       PSEL_POLL;         /* Specifies the positive channel poll of CMP */
    CMP_PSEL_POLL_NUM_TypeDef   POLL_NUM;          /* Specifies the poll members of CMP */
    CMP_OUT_IE_TypeDef          OUT_IE;            /* Specifies the out interrupt of CMP */
    CMP_CNT_IE_TypeDef          CNT_IE;            /* Specifies the interrupt at the end of the CMP1 polling interval */
    CMP_POLL_CH1_TypeDef        POLL_CH1;          /* Specifies the poll channel 1 of CMP */
    CMP_POLL_CH2_TypeDef        POLL_CH2;          /* Specifies the poll channel 2 of CMP */
    CMP_POLL_CH3_TypeDef        POLL_CH3;          /* Specifies the poll channel 3 of CMP */
    CMP_OutMode_TypeDef         CMP_Out_Mode;      /* Specifies the out mode of CMP */
    CMP_NSEL_TypeDef            NSEL;              /* Specifies the negative channel of CMP */
    CMP_PSEL_TypeDef            PSEL;              /* Specifies the positive channel of CMP */
    CMP_HYEN_TypeDef            HYEN;              /* Specifies the hysteresis comparator of CMP */
    CMP_RMID_TypeDef            RMID;              /* Specifies the virtual center point of the CMP1 positive input channel */
} CMP1_InitTypeDef;

/* CMP_FILT_Length_definition */
#define CMP_FILT_Len_0                      ((uint32_t)0x00000000)
#define CMP_FILT_Len_1                      ((uint32_t)0x02000000)

/* TIM1_brake_source_definition */
#define TIM1_Brake_Source_IO                ((uint32_t)0x00000000)
#define TIM1_Brake_Source_CMP1              ((uint32_t)0x04000000)
#define TIM1_Brake_Source_CMP2              ((uint32_t)0x08000000)
#define TIM1_Brake_Source_OPA               ((uint32_t)0x0C000000)

/* CMP_OUT_POLL_FLAG */
#define CMP_FLAG_OUT_POLL_CH_1              ((uint32_t)0x00001000)
#define CMP_FLAG_OUT_POLL_CH_2              ((uint32_t)0x00002000)
#define CMP_FLAG_OUT_POLL_CH_3              ((uint32_t)0x00004000)
#define CMP_FLAG_POLL_END                   ((uint32_t)0x00008000)

/* OPA_OUT_POLL_FLAG */
#define OPA_FLAG_OUT_POLL_CH_1              ((uint32_t)0x00001000)
#define OPA_FLAG_OUT_POLL_CH_2              ((uint32_t)0x00002000)
#define OPA_FLAG_OUT_POLL_CH_3              ((uint32_t)0x00004000)


void OPA_Unlock(void);
void OPA_Lock(void);
void OPA_CMP_POLL_Lock(void);
void OPA_CMP_Unlock(void);
void OPA_CMP_Lock(void);
void OPA_Init(OPA_InitTypeDef *OPA_InitStruct);
void OPA_StructInit(OPA_InitTypeDef *OPA_InitStruct);
void OPA_CMP1_Init(CMP1_InitTypeDef *CMP_InitStruct);
void OPA_CMP1_StructInit(CMP1_InitTypeDef *CMP_InitStruct);
void OPA_Cmd(FunctionalState NewState);
void OPA_CMP_Cmd(CMP_Num_TypeDef CMP_NUM, FunctionalState NewState);
void OPA_SystemReset_Cmd(FunctionalState NewState);
void OPA_CMP_SystemReset_Cmd(CMP_Num_TypeDef CMP_NUM, FunctionalState NewState);
void OPA_CMP_FILT_Cmd(FunctionalState NewState);
void OPA_CMP_FILT_LEN_Config(uint32_t FILT_Len);
void OPA_CMP_TIM1_BKINConfig(uint32_t Brake_Source);
FlagStatus OPA_GetFlagStatus(uint32_t OPA_FLAG);
FlagStatus OPA_CMP_GetFlagStatus(uint32_t CMP_FLAG);
void OPA_ClearFlag(uint32_t OPA_FLAG);
void OPA_CMP_ClearFlag(uint32_t CMP_FLAG);
void OPA_SoftwareStartPollCmd(FunctionalState NewState);

#ifdef __cplusplus
}
#endif

#endif
