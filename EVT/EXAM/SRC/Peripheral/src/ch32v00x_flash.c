/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00X_flash.c
 * Author             : WCH
 * Version            : V1.0.1
 * Date               : 2024/12/11
 * Description        : This file provides all the FLASH firmware functions.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#include <ch32v00X_flash.h>

/* Flash Access Control Register bits */
#define ACR_LATENCY_Mask           ((uint32_t)0xFFFFFFFC)

/* Flash Control Register bits */
#define CR_PER_Set                 ((uint32_t)0x00000002)
#define CR_PER_Reset               ((uint32_t)0xFFFFFFFD)
#define CR_MER_Set                 ((uint32_t)0x00000004)
#define CR_MER_Reset               ((uint32_t)0xFFFFFFFB)
#define CR_OPTER_Set               ((uint32_t)0x00000020)
#define CR_OPTER_Reset             ((uint32_t)0xFFFFFFDF)
#define CR_STRT_Set                ((uint32_t)0x00000040)
#define CR_LOCK_Set                ((uint32_t)0x00000080)
#define CR_FLOCK_Set               ((uint32_t)0x00008000)
#define CR_PAGE_PG                 ((uint32_t)0x00010000)
#define CR_PAGE_ER                 ((uint32_t)0x00020000)
#define CR_PAGE_ER_Reset           ((uint32_t)0xFFFDFFFF)
#define CR_BUF_LOAD                ((uint32_t)0x00040000)
#define CR_BUF_RST                 ((uint32_t)0x00080000)
#define CR_BER32                   ((uint32_t)0x00800000)

/* FLASH Status Register bits */
#define SR_BSY                     ((uint32_t)0x00000001)
#define SR_WRPRTERR                ((uint32_t)0x00000010)
#define SR_EOP                     ((uint32_t)0x00000020)

/* FLASH Mask */
#define RDPRT_Mask                 ((uint32_t)0x00000002)
#define WRP0_Mask                  ((uint32_t)0x000000FF)
#define WRP1_Mask                  ((uint32_t)0x0000FF00)
#define WRP2_Mask                  ((uint32_t)0x00FF0000)
#define WRP3_Mask                  ((uint32_t)0xFF000000)

/* FLASH Keys */
#define RDP_Key                    ((uint16_t)0x00A5)
#define FLASH_KEY1                 ((uint32_t)0x45670123)
#define FLASH_KEY2                 ((uint32_t)0xCDEF89AB)

/* Delay definition */
#define EraseTimeout               ((uint32_t)0x000B0000)
#define ProgramTimeout             ((uint32_t)0x00002000)

/* Flash Program Valid Address */
#define ValidAddrStart             (FLASH_BASE)

#if defined(CH32V002)
#define ValidAddrEnd               (FLASH_BASE + 0x4000)

#elif defined(CH32V004) || defined(CH32V005)
#define ValidAddrEnd               (FLASH_BASE + 0x8000)

#elif defined(CH32V006) || defined(CH32V007_M007)
#define ValidAddrEnd               (FLASH_BASE + 0xF800)

#endif

/* FLASH Size */
#define Size_256B                  0x100
#define Size_1KB                   0x400
#define Size_32KB                  0x8000

/********************************************************************************
 * @fn      FLASH_SetLatency
 *
 * @brief   Sets the code latency value.
 *
 * @param   FLASH_Latency - specifies the FLASH Latency value.
 *          FLASH_Latency_0 - FLASH Zero Latency cycle
 *          FLASH_Latency_1 - FLASH One Latency cycle
 *          FLASH_Latency_2 - FLASH Two Latency cycles
 *
 * @return  None
 */
void FLASH_SetLatency(uint32_t FLASH_Latency)
{
    uint32_t tmpreg = 0;

    tmpreg = FLASH->ACTLR;
    tmpreg &= ACR_LATENCY_Mask;
    tmpreg |= FLASH_Latency;
    FLASH->ACTLR = tmpreg;
}

/********************************************************************************
 * @fn      FLASH_Unlock
 *
 * @brief   Unlocks the FLASH Program Erase Controller.
 *
 * @return  None
 */
void FLASH_Unlock(void)
{
    /* Authorize the FPEC of Bank1 Access */
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
}

/********************************************************************************
 * @fn      FLASH_Lock
 *
 * @brief   Locks the FLASH Program Erase Controller.
 *
 * @return  None
 */
void FLASH_Lock(void)
{
    FLASH->CTLR |= CR_LOCK_Set;
}

/********************************************************************************
 * @fn      FLASH_ErasePage
 *
 * @brief   Erases a specified FLASH page(1KB).
 *
 * @param   Page_Address - The page address to be erased.
 *
 * @return  FLASH Status - The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
 *          FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
 */
FLASH_Status FLASH_ErasePage(uint32_t Page_Address)
{
    FLASH_Status status = FLASH_COMPLETE;

    status = FLASH_WaitForLastOperation(EraseTimeout);

    if(status == FLASH_COMPLETE)
    {
        FLASH->CTLR &= (CR_OPTER_Reset & CR_PAGE_ER_Reset);
        FLASH->CTLR |= CR_PER_Set;
        FLASH->ADDR = Page_Address;
        FLASH->CTLR |= CR_STRT_Set;

        status = FLASH_WaitForLastOperation(EraseTimeout);

        FLASH->CTLR &= CR_PER_Reset;
    }

    return status;
}

/********************************************************************************
 * @fn      FLASH_EraseAllPages
 *
 * @brief   Erases all FLASH pages.
 *
 * @return  FLASH Status - The returned value can be:FLASH_BUSY, FLASH_ERROR_PG,
 *          FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
 */
FLASH_Status FLASH_EraseAllPages(void)
{
    FLASH_Status status = FLASH_COMPLETE;

    status = FLASH_WaitForLastOperation(EraseTimeout);
    if(status == FLASH_COMPLETE)
    {
        FLASH->CTLR &= (CR_OPTER_Reset & CR_PAGE_ER_Reset);
        FLASH->CTLR |= CR_MER_Set;
        FLASH->CTLR |= CR_STRT_Set;

        status = FLASH_WaitForLastOperation(EraseTimeout);

        FLASH->CTLR &= CR_MER_Reset;
    }

    return status;
}

/********************************************************************************
 * @fn      FLASH_EraseOptionBytes
 *
 * @brief   Erases the FLASH option bytes.
 *
 * @return  FLASH Status - The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
 *          FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
 */
FLASH_Status FLASH_EraseOptionBytes(void)
{
    FLASH_Status status = FLASH_COMPLETE;

    status = FLASH_WaitForLastOperation(EraseTimeout);
    if(status == FLASH_COMPLETE)
    {
        FLASH_Unlock();

        FLASH->OBKEYR = FLASH_KEY1;
        FLASH->OBKEYR = FLASH_KEY2;

        FLASH->CTLR &= (CR_OPTER_Reset & CR_PAGE_ER_Reset);
        FLASH->CTLR |= CR_OPTER_Set;
        FLASH->CTLR |= CR_STRT_Set;
        status = FLASH_WaitForLastOperation(EraseTimeout);

        FLASH->CTLR &= CR_OPTER_Reset;

        FLASH_Lock();
    }
    return status;
}

/*********************************************************************
 * @fn      FLASH_OptionBytePR
 *
 * @brief   Programs option bytes.
 *
 * @param   pbuf - data.
 *
 * @return  none
 */
void FLASH_OptionBytePR(u32* pbuf)
{
    uint8_t i;

    FLASH_EraseOptionBytes();
    FLASH_Unlock_Fast();
    FLASH_BufReset();

    for(i=0; i<4; i++)
    {
        FLASH_BufLoad((OB_BASE + 4*i), *pbuf++);
    }

    FLASH_ProgramPage_Fast(OB_BASE);
    FLASH_Lock_Fast();
}

/*********************************************************************
 * @fn      FLASH_EnableWriteProtection
 *
 * @brief   Write protects the desired sectors
 *
 * @param   FLASH_Pages - specifies the address of the pages to be write protected.
 *
 * @return  FLASH Status - The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
 *          FLASH_ERROR_WRP, FLASH_COMPLETE , FLASH_TIMEOUT or FLASH_RDP.
 */
FLASH_Status FLASH_EnableWriteProtection(uint32_t FLASH_Pages)
{
    uint8_t     WRP0_Data = 0xFF, WRP1_Data = 0xFF, WRP2_Data = 0xFF, WRP3_Data = 0xFF;
    uint32_t buf[4];
    uint8_t i;
    FLASH_Status status = FLASH_COMPLETE;

    if((FLASH->OBR & RDPRT_Mask) != (uint32_t)RESET)
    {
        status = FLASH_RDP;
    }
    else{
        FLASH_Pages = (uint32_t)(~FLASH_Pages);
        WRP0_Data = (uint8_t)(FLASH_Pages & WRP0_Mask);
        WRP1_Data = (uint8_t)((FLASH_Pages & WRP1_Mask) >> 8);
        WRP2_Data = (uint8_t)((FLASH_Pages & WRP2_Mask) >> 16);
        WRP3_Data = (uint8_t)((FLASH_Pages & WRP3_Mask) >> 24);

        status = FLASH_WaitForLastOperation(ProgramTimeout);

        if(status == FLASH_COMPLETE)
        {
            for(i=0; i<4; i++){
                buf[i] = *(uint32_t*)(OB_BASE + 4*i);
            }

            buf[2] = ((uint32_t)(((uint32_t)(WRP0_Data) & 0x00FF) + (((uint32_t)(~WRP0_Data) & 0x00FF) << 8) \
                   + (((uint32_t)(WRP1_Data) & 0x00FF) << 16) + (((uint32_t)(~WRP1_Data) & 0x00FF) << 24)));
            buf[3] = ((uint32_t)(((uint32_t)(WRP2_Data) & 0x00FF) + (((uint32_t)(~WRP2_Data) & 0x00FF) << 8) \
                   + (((uint32_t)(WRP3_Data) & 0x00FF) << 16) + (((uint32_t)(~WRP3_Data) & 0x00FF) << 24)));

            FLASH_OptionBytePR(buf);
        }
    }

    return status;
}

/*********************************************************************
 * @fn      FLASH_EnableReadOutProtection
 *
 * @brief   Enables the read out protection.
 *
 * @return  FLASH Status - The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
 *          FLASH_ERROR_WRP, FLASH_COMPLETE, FLASH_TIMEOUT or FLASH_RDP.
 */
FLASH_Status FLASH_EnableReadOutProtection(void)
{
    FLASH_Status status = FLASH_COMPLETE;
    uint32_t buf[4];
    uint8_t i;

    if((FLASH->OBR & RDPRT_Mask) != (uint32_t)RESET)
    {
        status = FLASH_RDP;
    }
    else{
        status = FLASH_WaitForLastOperation(EraseTimeout);
        if(status == FLASH_COMPLETE)
        {
            for(i=0; i<4; i++){
                buf[i] = *(uint32_t*)(OB_BASE + 4*i);
            }

            buf[0] = 0x000000FF + (buf[0] & 0xFFFF0000);
            FLASH_OptionBytePR(buf);
        }
    }

    return status;
}

/*********************************************************************
 * @fn      FLASH_UserOptionByteConfig
 *
 * @brief   Programs the FLASH User Option Byte - IWDG_SW / RST_STOP /
 *        RST_STDBY / OB_PowerON_Start_Mode.
 *
 * @param   OB_IWDG - Selects the IWDG mode
 *            OB_IWDG_SW - Software IWDG selected
 *            OB_IWDG_HW - Hardware IWDG selected
 *          OB_STDBY - Reset event when entering Standby mode.
 *            OB_STDBY_NoRST - No reset generated when entering in STANDBY.
 *            OB_STDBY_RST - Reset generated when entering in STANDBY.
 *          OB_RST - Selects the reset IO mode and Ignore delay time.
 *            OB_RST_NoEN - Reset IO disable.
 *            OB_RST_EN_DT12ms - Reset IO enable and  Ignore delay time 12ms.
 *            OB_RST_EN_DT1ms - Reset IO enable and  Ignore delay time 1ms.
 *            OB_RST_EN_DT128us - Reset IO enable and  Ignore delay time 128us.
 *          OB_PowerON_Start_Mode - Selects start mode after power on.
 *            OB_PowerON_Start_Mode_BOOT - Boot start after power on.
 *            OB_PowerON_Start_Mode_USER - User start after power on.
 *
 * @return  FLASH Status - The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
 *        FLASH_ERROR_WRP, FLASH_COMPLETE, FLASH_TIMEOUT or FLASH_RDP.
 */
FLASH_Status FLASH_UserOptionByteConfig(uint16_t OB_IWDG, uint16_t OB_STDBY, uint16_t OB_RST, uint16_t OB_PowerON_Start_Mode)
{
    FLASH_Status status = FLASH_COMPLETE;
    uint8_t UserByte;
    uint32_t buf[4];
    uint8_t i;

    if((FLASH->OBR & RDPRT_Mask) != (uint32_t)RESET)
    {
        status = FLASH_RDP;
    }
    else{
        UserByte = OB_IWDG | (uint16_t)(OB_STDBY | (uint16_t)(OB_RST | (uint16_t)(OB_PowerON_Start_Mode | 0xC2)));

        for(i=0; i<4; i++){
            buf[i] = *(uint32_t*)(OB_BASE + 4*i);
        }
        buf[0] = ((uint32_t)((((uint32_t)(UserByte) & 0x00FF) << 16) + (((uint32_t)(~UserByte) & 0x00FF) << 24))) + 0x00005AA5;

        FLASH_OptionBytePR(buf);
    }

    return status;
}

/*********************************************************************
 * @fn      FLASH_GetUserOptionByte
 *
 * @brief   Returns the FLASH User Option Bytes values.
 *
 * @return  The FLASH User Option Bytes values:IWDG_SW(Bit0), RST_STDBY(Bit2)
 *          , RST_MODE(Bit[3:4]) and START_MODE(Bit5).
 */
uint32_t FLASH_GetUserOptionByte(void)
{
    return (uint32_t)(FLASH->OBR >> 2);
}

/*********************************************************************
 * @fn      FLASH_GetWriteProtectionOptionByte
 *
 * @brief   Returns the FLASH Write Protection Option Bytes Register value.
 *
 * @return  The FLASH Write Protection Option Bytes Register value.
 */
uint32_t FLASH_GetWriteProtectionOptionByte(void)
{
    return (uint32_t)(FLASH->WPR);
}

/*********************************************************************
 * @fn      FLASH_GetReadOutProtectionStatus
 *
 * @brief   Checks whether the FLASH Read Out Protection Status is set or not.
 *
 * @return  FLASH ReadOut Protection Status(SET or RESET)
 */
FlagStatus FLASH_GetReadOutProtectionStatus(void)
{
    FlagStatus readoutstatus = RESET;
    if((FLASH->OBR & RDPRT_Mask) != (uint32_t)RESET)
    {
        readoutstatus = SET;
    }
    else
    {
        readoutstatus = RESET;
    }
    return readoutstatus;
}

/*********************************************************************
 * @fn      FLASH_ITConfig
 *
 * @brief   Enables or disables the specified FLASH interrupts.
 *
 * @param   FLASH_IT - specifies the FLASH interrupt sources to be enabled or disabled.
 *            FLASH_IT_ERROR - FLASH Error Interrupt.
 *            FLASH_IT_EOP - FLASH end of operation Interrupt.
 *            FLASH_IT_FWAKE - FLASH Wake Up Interrupt.
 *          NewState - new state of the specified Flash interrupts(ENABLE or DISABLE).
 *
 * @return  FLASH Prefetch Buffer Status (SET or RESET).
 */
void FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        FLASH->CTLR |= FLASH_IT;
    }
    else
    {
        FLASH->CTLR &= ~(uint32_t)FLASH_IT;
    }
}

/*********************************************************************
 * @fn      FLASH_GetFlagStatus
 *
 * @brief   Checks whether the specified FLASH flag is set or not.
 *
 * @param   FLASH_FLAG - specifies the FLASH flag to check.
 *            FLASH_FLAG_BSY - FLASH Busy flag
 *            FLASH_FLAG_WRPRTERR - FLASH Write protected error flag
 *            FLASH_FLAG_EOP - FLASH End of Operation flag
 *            FLASH_FLAG_FWAKE - FLASH Wake Up flag
 *            FLASH_FLAG_OPTERR - FLASH Option Byte error flag
 *
 * @return  The new state of FLASH_FLAG (SET or RESET).
 */
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG)
{
    FlagStatus bitstatus = RESET;

    if(FLASH_FLAG == FLASH_FLAG_OPTERR)
    {
        if((FLASH->OBR & FLASH_FLAG_OPTERR) != (uint32_t)RESET)
        {
            bitstatus = SET;
        }
        else
        {
            bitstatus = RESET;
        }
    }
    else
    {
        if((FLASH->STATR & FLASH_FLAG) != (uint32_t)RESET)
        {
            bitstatus = SET;
        }
        else
        {
            bitstatus = RESET;
        }
    }
    return bitstatus;
}

/*********************************************************************
 * @fn      FLASH_ClearFlag
 *
 * @brief   Clears the FLASH's pending flags.
 *
 * @param   FLASH_FLAG - specifies the FLASH flags to clear.
 *            FLASH_FLAG_WRPRTERR - FLASH Write protected error flag
 *            FLASH_FLAG_EOP - FLASH End of Operation flag
 *            FLASH_FLAG_FWAKE - FLASH Wake Up flag
 *
 * @return  none
 */
void FLASH_ClearFlag(uint32_t FLASH_FLAG)
{
    if(FLASH_FLAG == FLASH_FLAG_FWAKE)
    {
        FLASH->STATR &= ~FLASH_FLAG;
    }
    else
    {
        FLASH->STATR = FLASH_FLAG;
    }
}

/*********************************************************************
 * @fn      FLASH_GetStatus
 *
 * @brief   Returns the FLASH Status.
 *
 * @return  FLASH Status - The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
 *          FLASH_ERROR_WRP or FLASH_COMPLETE.
 */
FLASH_Status FLASH_GetStatus(void)
{
    FLASH_Status flashstatus = FLASH_COMPLETE;

    if((FLASH->STATR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
    {
        flashstatus = FLASH_BUSY;
    }
    else
    {
        if((FLASH->STATR & FLASH_FLAG_WRPRTERR) != 0)
        {
            flashstatus = FLASH_ERROR_WRP;
        }
        else
        {
            flashstatus = FLASH_COMPLETE;
        }
    }
    return flashstatus;
}

/*********************************************************************
 * @fn      FLASH_GetBank1Status
 *
 * @brief   Returns the FLASH Bank1 Status.
 *
 * @return  FLASH Status - The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
 *          FLASH_ERROR_WRP or FLASH_COMPLETE.
 */
FLASH_Status FLASH_GetBank1Status(void)
{
    FLASH_Status flashstatus = FLASH_COMPLETE;

    if((FLASH->STATR & FLASH_FLAG_BANK1_BSY) == FLASH_FLAG_BSY)
    {
        flashstatus = FLASH_BUSY;
    }
    else
    {
        if((FLASH->STATR & FLASH_FLAG_BANK1_WRPRTERR) != 0)
        {
            flashstatus = FLASH_ERROR_WRP;
        }
        else
        {
            flashstatus = FLASH_COMPLETE;
        }
    }
    return flashstatus;
}

/*********************************************************************
 * @fn      FLASH_WaitForLastOperation
 *
 * @brief   Waits for a Flash operation to complete or a TIMEOUT to occur.
 *
 * @param   Timeout - FLASH programming Timeout
 *
 * @return  FLASH Status - The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
 *          FLASH_ERROR_WRP or FLASH_COMPLETE.
 */
FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout)
{
    FLASH_Status status = FLASH_COMPLETE;

    status = FLASH_GetBank1Status();
    while((status == FLASH_BUSY) && (Timeout != 0x00))
    {
        status = FLASH_GetBank1Status();
        Timeout--;
    }
    if(Timeout == 0x00)
    {
        status = FLASH_TIMEOUT;
    }
    return status;
}

/*********************************************************************
 * @fn      FLASH_WaitForLastBank1Operation
 *
 * @brief   Waits for a Flash operation on Bank1 to complete or a TIMEOUT to occur.
 *
 * @param   Timeout - FLASH programming Timeout
 *
 * @return  FLASH Status - The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
 *          FLASH_ERROR_WRP or FLASH_COMPLETE.
 */
FLASH_Status FLASH_WaitForLastBank1Operation(uint32_t Timeout)
{
    FLASH_Status status = FLASH_COMPLETE;

    status = FLASH_GetBank1Status();
    while((status == FLASH_FLAG_BANK1_BSY) && (Timeout != 0x00))
    {
        status = FLASH_GetBank1Status();
        Timeout--;
    }
    if(Timeout == 0x00)
    {
        status = FLASH_TIMEOUT;
    }
    return status;
}

/*********************************************************************
 * @fn      FLASH_Unlock_Fast
 *
 * @brief   Unlocks the Fast Program Erase Mode.
 *
 * @return  none
 */
void FLASH_Unlock_Fast(void)
{
    /* Authorize the FPEC of Bank1 Access */
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;

    /* Fast program mode unlock */
    FLASH->MODEKEYR = FLASH_KEY1;
    FLASH->MODEKEYR = FLASH_KEY2;
}

/*********************************************************************
 * @fn      FLASH_Lock_Fast
 *
 * @brief   Locks the Fast Program Erase Mode.
 *
 * @return  none
 */
void FLASH_Lock_Fast(void)
{
    FLASH->CTLR |= CR_FLOCK_Set;
}

/*********************************************************************
 * @fn      FLASH_BufReset
 *
 * @brief   Flash Buffer reset.
 *
 * @return  none
 */
void FLASH_BufReset(void)
{
    FLASH->CTLR &= (CR_OPTER_Reset & CR_PAGE_ER_Reset);

    FLASH->CTLR |= CR_PAGE_PG;
    FLASH->CTLR |= CR_BUF_RST;
    while(FLASH->STATR & SR_BSY)
        ;
    FLASH->CTLR &= ~CR_PAGE_PG;
}

/*********************************************************************
 * @fn      FLASH_BufLoad
 *
 * @brief   Flash Buffer load(4Byte).
 *
 * @param   Address - specifies the address to be programmed.
 *          Data0 - specifies the data0 to be programmed.
 *
 * @return  none
 */
void FLASH_BufLoad(uint32_t Address, uint32_t Data0)
{
    if(((Address >= ValidAddrStart) && (Address < ValidAddrEnd)) || ((Address >= OB_BASE) && (Address < OB_BASE+0x100)))
    {
        FLASH->CTLR &= (CR_OPTER_Reset & CR_PAGE_ER_Reset);

        FLASH->CTLR |= CR_PAGE_PG;
        *(__IO uint32_t *)(Address) = Data0;
        FLASH->CTLR |= CR_BUF_LOAD;
        while(FLASH->STATR & SR_BSY)
            ;
        FLASH->CTLR &= ~CR_PAGE_PG;
    }
}

/*********************************************************************
 * @fn      FLASH_ErasePage_Fast
 *
 * @brief   Erases a specified FLASH page (1page = 256Byte).
 *
 * @param   Page_Address - The page address to be erased.
 *
 * @return  none
 */
void FLASH_ErasePage_Fast(uint32_t Page_Address)
{
    if((Page_Address >= ValidAddrStart) && (Page_Address < ValidAddrEnd))
    {
        FLASH->CTLR &= (CR_OPTER_Reset & CR_PAGE_ER_Reset);

        FLASH->CTLR |= CR_PAGE_ER;
        FLASH->ADDR = Page_Address;
        FLASH->CTLR |= CR_STRT_Set;
        while(FLASH->STATR & SR_BSY)
            ;
        FLASH->CTLR &= ~CR_PAGE_ER;
    }
}

/*********************************************************************
 * @fn      FLASH_EraseBlock_32K_Fast
 *
 * @brief   Erases a specified FLASH Block (1Block = 32KByte).
 *
 * @param   Block_Address - The block address to be erased.
 *          This function is only capable of erasing addresses
 *          in the range of 0x08000000~0x08008000.
 *
 * @return  none
 */
void FLASH_EraseBlock_32K_Fast(uint32_t Block_Address)
{
    FLASH->CTLR &= (CR_OPTER_Reset & CR_PAGE_ER_Reset);

    Block_Address &= 0xFFFF8000;

    FLASH->CTLR |= CR_BER32;
    FLASH->ADDR = Block_Address;
    FLASH->CTLR |= CR_STRT_Set;
    while(FLASH->STATR & SR_BSY)
        ;
    FLASH->CTLR &= ~CR_BER32;
}

/*********************************************************************
 * @fn      FLASH_ProgramPage_Fast
 *
 * @brief   Program a specified FLASH page (1page = 256Byte).
 *
 * @param   Page_Address - The page address to be programed.
 *
 * @return  none
 */
void FLASH_ProgramPage_Fast(uint32_t Page_Address)
{
    if(((Page_Address >= ValidAddrStart) && (Page_Address < ValidAddrEnd)) || (Page_Address == OB_BASE))
    {
        FLASH->CTLR &= (CR_OPTER_Reset & CR_PAGE_ER_Reset);

        FLASH->CTLR |= CR_PAGE_PG;
        FLASH->ADDR = Page_Address;
        FLASH->CTLR |= CR_STRT_Set;
        while(FLASH->STATR & SR_BSY)
            ;
        FLASH->CTLR &= ~CR_PAGE_PG;
    }
}

/*********************************************************************
 * @fn      SystemReset_StartMode
 *
 * @brief   Start mode after system reset.
 *
 * @param   Mode - Start mode.
 *            Start_Mode_USER - USER start after system reset
 *            Start_Mode_BOOT - Boot start after system reset
 *
 * @return  none
 */
void SystemReset_StartMode(uint32_t Mode)
{
    FLASH_Unlock();

    FLASH->BOOT_MODEKEYR = FLASH_KEY1;
    FLASH->BOOT_MODEKEYR = FLASH_KEY2;

    FLASH->STATR &= ~(1<<14);
    if(Mode == Start_Mode_BOOT){
        FLASH->STATR |= (1<<14);
    }

    FLASH_Lock();
}

/*********************************************************************
 * @fn      ROM_ERASE
 *
 * @brief   Select erases a specified FLASH .
 *
 * @param   StartAddr - Erases Flash start address(StartAddr%256 == 0).
 *          Cnt - Erases count.
 *          Erase_Size - Erases size select.The returned value can be:
 *            Size_32KB, Size_1KB, Size_256B.
 *
 * @return  none.
 */
static void ROM_ERASE(uint32_t StartAddr, uint32_t Cnt, uint32_t Erase_Size)
{
    FLASH->CTLR &= (CR_OPTER_Reset & CR_PAGE_ER_Reset);

    do{
        if(Erase_Size == Size_32KB)
        {
            FLASH->CTLR |= CR_BER32;
        }
        else if(Erase_Size == Size_1KB)
        {
            FLASH->CTLR |= CR_PER_Set;
        }
        else if(Erase_Size == Size_256B)
        {
            FLASH->CTLR |= CR_PAGE_ER;
        }

        FLASH->ADDR = StartAddr;
        FLASH->CTLR |= CR_STRT_Set;
        while(FLASH->STATR & SR_BSY)
            ;

        if(Erase_Size == Size_32KB)
        {
            FLASH->CTLR &= ~CR_BER32;
            StartAddr += Size_32KB;
        }
        else if(Erase_Size == Size_1KB)
        {
            FLASH->CTLR &= ~CR_PER_Set;
            StartAddr += Size_1KB;
        }
        else if(Erase_Size == Size_256B)
        {
            FLASH->CTLR &= ~CR_PAGE_ER;
            StartAddr += Size_256B;
        }
    }while(--Cnt);
}

/*********************************************************************
 * @fn      FLASH_ROM_ERASE
 *
 * @brief   Erases a specified FLASH .
 *
 * @param   StartAddr - Erases Flash start address(StartAddr%256 == 0).
 *          Length - Erases Flash start Length(Length%256 == 0).
 *
 * @return  FLASH Status - The returned value can be: FLASH_ADR_RANGE_ERROR,
 *        FLASH_ALIGN_ERROR, FLASH_OP_RANGE_ERROR or FLASH_COMPLETE.
 */
FLASH_Status FLASH_ROM_ERASE( uint32_t StartAddr, uint32_t Length )
{
    uint32_t Addr0 = 0, Addr1 = 0, Length0 = 0, Length1 = 0;

    FLASH_Status status = FLASH_COMPLETE;

    if((StartAddr < ValidAddrStart) || (StartAddr >= ValidAddrEnd))
    {
        return FLASH_ADR_RANGE_ERROR;
    }

    if((StartAddr + Length) > ValidAddrEnd)
    {
        return FLASH_OP_RANGE_ERROR;
    }

    if((StartAddr & (Size_256B-1)) || (Length & (Size_256B-1)) || (Length == 0))
    {
        return FLASH_ALIGN_ERROR;
    }

    /* Authorize the FPEC of Bank1 Access */
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;

    /* Fast program mode unlock */
    FLASH->MODEKEYR = FLASH_KEY1;
    FLASH->MODEKEYR = FLASH_KEY2;

    Addr0 = StartAddr;

    if(Length >= Size_32KB)
    {
        Length0 = Size_32KB - (Addr0 & (Size_32KB - 1));
        Addr1 = StartAddr + Length0;
        Length1 = Length - Length0;
    }
    else if(Length >= Size_1KB)
    {
        Length0 = Size_1KB - (Addr0 & (Size_1KB - 1));
        Addr1 = StartAddr + Length0;
        Length1 = Length - Length0;
    }
    else if(Length >= Size_256B)
    {
        Length0 = Length;
    }

    /* Erase 32KB */
    if(Length0 >= Size_32KB)//front
    {
        Length = Length0;
        if(Addr0 & (Size_32KB - 1))
        {
            Length0 = Size_32KB - (Addr0 & (Size_32KB - 1));
        }
        else
        {
            Length0 = 0;
        }

        ROM_ERASE((Addr0 + Length0), ((Length - Length0) >> 15), Size_32KB);
    }

    if(Length1 >= Size_32KB)//back
    {
        StartAddr = Addr1;
        Length = Length1;

        if((Addr1 + Length1) & (Size_32KB - 1))
        {
            Addr1 = ((StartAddr + Length1) & (~(Size_32KB - 1)));
            Length1 = (StartAddr + Length1) & (Size_32KB - 1);
        }
        else
        {
            Length1 = 0;
        }

        ROM_ERASE(StartAddr, ((Length - Length1) >> 15), Size_32KB);
    }

    /* Erase 1KB */
    if(Length0 >= Size_1KB) //front
    {
        Length = Length0;
        if(Addr0 & (Size_1KB - 1))
        {
            Length0 = Size_1KB - (Addr0 & (Size_1KB - 1));
        }
        else
        {
            Length0 = 0;
        }

        ROM_ERASE((Addr0 + Length0), ((Length - Length0) >> 10), Size_1KB);
    }

    if(Length1 >= Size_1KB) //back
    {
        StartAddr = Addr1;
        Length = Length1;

        if((Addr1 + Length1) & (Size_1KB - 1))
        {
            Addr1 = ((StartAddr + Length1) & (~(Size_1KB - 1)));
            Length1 = (StartAddr + Length1) & (Size_1KB - 1);
        }
        else
        {
            Length1 = 0;
        }

        ROM_ERASE(StartAddr, ((Length - Length1) >> 10), Size_1KB);
    }

    /* Erase 256B */
    if(Length0)//front
    {
        ROM_ERASE(Addr0, (Length0 >> 8), Size_256B);
    }

    if(Length1)//back
    {
        ROM_ERASE(Addr1, (Length1 >> 8), Size_256B);
    }

    FLASH->CTLR |= CR_FLOCK_Set;
    FLASH->CTLR |= CR_LOCK_Set;

    return status;
}

/*********************************************************************
 * @fn      FLASH_ROM_WRITE
 *
 * @brief   Writes a specified FLASH .
 *
 * @param   StartAddr - Writes Flash start address(StartAddr%256 == 0).
 *          Length - Writes Flash start Length(Length%256 == 0).
 *          pbuf - Writes Flash value buffer.
 *
 * @return  FLASH Status - The returned value can be: FLASH_ADR_RANGE_ERROR,
 *        FLASH_ALIGN_ERROR, FLASH_OP_RANGE_ERROR or FLASH_COMPLETE.
 */
FLASH_Status FLASH_ROM_WRITE( uint32_t StartAddr, uint32_t *pbuf, uint32_t Length )
{
    uint32_t i, adr;
    uint8_t size;

    FLASH_Status status = FLASH_COMPLETE;

    if((StartAddr < ValidAddrStart) || (StartAddr >= ValidAddrEnd))
    {
        return FLASH_ADR_RANGE_ERROR;
    }

    if((StartAddr + Length) > ValidAddrEnd)
    {
        return FLASH_OP_RANGE_ERROR;
    }

    if((StartAddr & (Size_256B-1)) || (Length & (Size_256B-1)) || (Length == 0))
    {
        return FLASH_ALIGN_ERROR;
    }
    adr = StartAddr;
    i = Length >> 8;

    /* Authorize the FPEC of Bank1 Access */
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;

    /* Fast program mode unlock */
    FLASH->MODEKEYR = FLASH_KEY1;
    FLASH->MODEKEYR = FLASH_KEY2;

    FLASH->CTLR &= (CR_OPTER_Reset & CR_PAGE_ER_Reset);

    do{
        FLASH->CTLR |= CR_PAGE_PG;
        FLASH->CTLR |= CR_BUF_RST;
        while(FLASH->STATR & SR_BSY)
            ;
        size = 64;
        while(size)
        {
            *(uint32_t *)StartAddr = *(uint32_t *)pbuf;
            FLASH->CTLR |= CR_BUF_LOAD;
            while(FLASH->STATR & SR_BSY)
                ;
            StartAddr += 4;
            pbuf += 1;
            size -= 1;
        }

        FLASH->ADDR = adr;
        FLASH->CTLR |= CR_STRT_Set;
        while(FLASH->STATR & SR_BSY)
            ;
        FLASH->CTLR &= ~CR_PAGE_PG;
        adr += 256;
    }while(--i);

    FLASH->CTLR |= CR_FLOCK_Set;
    FLASH->CTLR |= CR_LOCK_Set;

    return status;
}
