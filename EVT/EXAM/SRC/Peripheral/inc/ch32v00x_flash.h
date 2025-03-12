/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00X_flash.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/11/04
 * Description        : This file contains all the functions prototypes for the FLASH
 *                      firmware library.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#ifndef __CH32V00X_FLASH_H
#define __CH32V00X_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ch32v00X.h>

/* FLASH Status */
typedef enum
{
    FLASH_BUSY = 1,
    FLASH_ERROR_PG,
    FLASH_ERROR_WRP,
    FLASH_COMPLETE,
    FLASH_TIMEOUT,
    FLASH_RDP,
    FLASH_OP_RANGE_ERROR = 0xFD,
    FLASH_ALIGN_ERROR = 0xFE,
    FLASH_ADR_RANGE_ERROR = 0xFF,
} FLASH_Status;

/* Flash_Latency */
#define FLASH_Latency_0                  ((uint32_t)0x00000000) /* FLASH Zero Latency cycle */
#define FLASH_Latency_1                  ((uint32_t)0x00000001) /* FLASH One Latency cycle */
#define FLASH_Latency_2                  ((uint32_t)0x00000002) /* FLASH Two Latency cycles */

/* Values to be used with CH32V00X devices (1page = 256Byte) */
#define FLASH_WRProt_Pages0to7           ((uint32_t)0x00000001) /* Write protection of page 0 to 7 */
#define FLASH_WRProt_Pages8to15          ((uint32_t)0x00000002) /* Write protection of page 8 to 15 */
#define FLASH_WRProt_Pages16to23         ((uint32_t)0x00000004) /* Write protection of page 16 to 23 */
#define FLASH_WRProt_Pages24to31         ((uint32_t)0x00000008) /* Write protection of page 24 to 31 */
#define FLASH_WRProt_Pages32to39         ((uint32_t)0x00000010) /* Write protection of page 32 to 39 */
#define FLASH_WRProt_Pages40to47         ((uint32_t)0x00000020) /* Write protection of page 40 to 47 */
#define FLASH_WRProt_Pages48to55         ((uint32_t)0x00000040) /* Write protection of page 48 to 55 */
#define FLASH_WRProt_Pages56to63         ((uint32_t)0x00000080) /* Write protection of page 56 to 63 */
#define FLASH_WRProt_Pages64to71         ((uint32_t)0x00000100) /* Write protection of page 64 to 71 */
#define FLASH_WRProt_Pages72to79         ((uint32_t)0x00000200) /* Write protection of page 72 to 79 */
#define FLASH_WRProt_Pages80to87         ((uint32_t)0x00000400) /* Write protection of page 80 to 87 */
#define FLASH_WRProt_Pages88to95         ((uint32_t)0x00000800) /* Write protection of page 88 to 95 */
#define FLASH_WRProt_Pages96to103        ((uint32_t)0x00001000) /* Write protection of page 96 to 103 */
#define FLASH_WRProt_Pages104to111       ((uint32_t)0x00002000) /* Write protection of page 104 to 111 */
#define FLASH_WRProt_Pages112to119       ((uint32_t)0x00004000) /* Write protection of page 112 to 119 */
#define FLASH_WRProt_Pages120to127       ((uint32_t)0x00008000) /* Write protection of page 120 to 127 */
#define FLASH_WRProt_Pages128to135       ((uint32_t)0x00010000) /* Write protection of page 128 to 135 */
#define FLASH_WRProt_Pages136to143       ((uint32_t)0x00020000) /* Write protection of page 136 to 143 */
#define FLASH_WRProt_Pages144to151       ((uint32_t)0x00040000) /* Write protection of page 144 to 151 */
#define FLASH_WRProt_Pages152to159       ((uint32_t)0x00080000) /* Write protection of page 152 to 159 */
#define FLASH_WRProt_Pages160to167       ((uint32_t)0x00100000) /* Write protection of page 160 to 167 */
#define FLASH_WRProt_Pages168to175       ((uint32_t)0x00200000) /* Write protection of page 168 to 175 */
#define FLASH_WRProt_Pages176to183       ((uint32_t)0x00400000) /* Write protection of page 176 to 183 */
#define FLASH_WRProt_Pages184to191       ((uint32_t)0x00800000) /* Write protection of page 184 to 191 */
#define FLASH_WRProt_Pages192to199       ((uint32_t)0x01000000) /* Write protection of page 192 to 199 */
#define FLASH_WRProt_Pages200to207       ((uint32_t)0x02000000) /* Write protection of page 200 to 207 */
#define FLASH_WRProt_Pages208to215       ((uint32_t)0x04000000) /* Write protection of page 208 to 215 */
#define FLASH_WRProt_Pages216to223       ((uint32_t)0x08000000) /* Write protection of page 216 to 223 */
#define FLASH_WRProt_Pages224to231       ((uint32_t)0x10000000) /* Write protection of page 224 to 231 */
#define FLASH_WRProt_Pages232to239       ((uint32_t)0x20000000) /* Write protection of page 232 to 239 */
#define FLASH_WRProt_Pages240to247       ((uint32_t)0x40000000) /* Write protection of page 240 to 247 */

#define FLASH_WRProt_AllPages            ((uint32_t)0xFFFFFFFF) /* Write protection of all Pages */

/* Option_Bytes_IWatchdog */
#define OB_IWDG_SW                       ((uint16_t)0x0001) /* Software IWDG selected */
#define OB_IWDG_HW                       ((uint16_t)0x0000) /* Hardware IWDG selected */

/* Option_Bytes_nRST_STDBY */
#define OB_STDBY_NoRST                   ((uint16_t)0x0004) /* No reset generated when entering in STANDBY */
#define OB_STDBY_RST                     ((uint16_t)0x0000) /* Reset generated when entering in STANDBY */

/* Option_Bytes_RST_ENandDT */
#define OB_RST_NoEN                      ((uint16_t)0x0018) /* Reset IO disable */
#define OB_RST_EN_DT12ms                 ((uint16_t)0x0010) /* Reset IO enable and  Ignore delay time 12ms */
#define OB_RST_EN_DT1ms                  ((uint16_t)0x0008) /* Reset IO enable and  Ignore delay time 1ms */
#define OB_RST_EN_DT128us                ((uint16_t)0x0000) /* Reset IO enable and  Ignore delay time 128us */

/* Option_Bytes_Power_ON_Start_Mode */
#define OB_PowerON_Start_Mode_BOOT       ((uint16_t)0x0020) /* Boot start after power on */
#define OB_PowerON_Start_Mode_USER       ((uint16_t)0x0000) /* User start after power on */

/* FLASH_Interrupts */
#define FLASH_IT_ERROR                   ((uint32_t)0x00000400) /* FPEC error interrupt source */
#define FLASH_IT_EOP                     ((uint32_t)0x00001000) /* End of FLASH Operation Interrupt source */
#define FLASH_IT_FWAKE                   ((uint32_t)0x00002000) /* FLASH Wake Up Interrupt source */

/* FLASH_Flags */
#define FLASH_FLAG_BSY                   ((uint32_t)0x00000001) /* FLASH Busy flag */
#define FLASH_FLAG_EOP                   ((uint32_t)0x00000020) /* FLASH End of Operation flag */
#define FLASH_FLAG_FWAKE                 ((uint32_t)0x00000040) /* FLASH Wake Up flag */
#define FLASH_FLAG_WRPRTERR              ((uint32_t)0x00000010) /* FLASH Write protected error flag */
#define FLASH_FLAG_OPTERR                ((uint32_t)0x00000001) /* FLASH Option Byte error flag */

#define FLASH_FLAG_BANK1_BSY             FLASH_FLAG_BSY       /* FLASH BANK1 Busy flag*/
#define FLASH_FLAG_BANK1_WRPRTERR        FLASH_FLAG_WRPRTERR  /* FLASH BANK1 Write protected error flag */

/* System_Reset_Start_Mode */
#define Start_Mode_USER                  ((uint32_t)0x00000000)
#define Start_Mode_BOOT                  ((uint32_t)0x00004000)


/*Functions used for all CH32V00X devices*/
void         FLASH_SetLatency(uint32_t FLASH_Latency);
void         FLASH_Unlock(void);
void         FLASH_Lock(void);
FLASH_Status FLASH_ErasePage(uint32_t Page_Address);
FLASH_Status FLASH_EraseAllPages(void);
FLASH_Status FLASH_EraseOptionBytes(void);
FLASH_Status FLASH_EnableWriteProtection(uint32_t FLASH_Pages);
FLASH_Status FLASH_EnableReadOutProtection(void);
FLASH_Status FLASH_UserOptionByteConfig(uint16_t OB_IWDG, uint16_t OB_STDBY, uint16_t OB_RST, uint16_t OB_PowerON_Start_Mode);
uint32_t     FLASH_GetUserOptionByte(void);
uint32_t     FLASH_GetWriteProtectionOptionByte(void);
FlagStatus   FLASH_GetReadOutProtectionStatus(void);
void         FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState);
FlagStatus   FLASH_GetFlagStatus(uint32_t FLASH_FLAG);
void         FLASH_ClearFlag(uint32_t FLASH_FLAG);
FLASH_Status FLASH_GetStatus(void);
FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout);
void         FLASH_Unlock_Fast(void);
void         FLASH_Lock_Fast(void);
void         FLASH_BufReset(void);
void         FLASH_BufLoad(uint32_t Address, uint32_t Data0);
void         FLASH_ErasePage_Fast(uint32_t Page_Address);
void         FLASH_EraseBlock_32K_Fast(uint32_t Block_Address);
void         FLASH_ProgramPage_Fast(uint32_t Page_Address);
void         SystemReset_StartMode(uint32_t Mode);
FLASH_Status FLASH_ROM_ERASE(uint32_t StartAddr, uint32_t Length);
FLASH_Status FLASH_ROM_WRITE(uint32_t StartAddr, uint32_t *pbuf, uint32_t Length);

#ifdef __cplusplus
}
#endif

#endif /* __CH32V00X_FLASH_H */
