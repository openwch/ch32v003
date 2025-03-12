/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00X_opa.c
 * Author             : WCH
 * Version            : V1.0.1
 * Date               : 2025/01/07
 * Description        : This file provides all the OPA firmware functions.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#include <ch32v00X_opa.h>


/* FLASH Keys */
#define OPA_KEY1                                ((uint32_t)0x45670123)
#define OPA_KEY2                                ((uint32_t)0xCDEF89AB)

/* TIM1 Brake Source mask definition */
#define TIM1_Brake_Source_Config_MASK           ((uint32_t)0xF3FFFFFF)

/********************************************************************************
 * @fn             OPA_Unlock
 *
 * @brief          Unlocks the OPA Controller.
 *
 * @return         None
 */
void OPA_Unlock(void)
{
    OPA->OPAKEY = OPA_KEY1;
    OPA->OPAKEY = OPA_KEY2;
}

/********************************************************************************
 * @fn             OPA_Lock
 *
 * @brief          Locks the OPA Controller.
 *
 * @return         None
 */
void OPA_Lock(void)
{
    OPA->CTLR1 |= (1<<31);
}

/********************************************************************************
 * @fn             OPA_CMP_POLL_Lock
 *
 * @brief          Locks the OPA and CMP POLL Controller.
 *
 * @return         None
 */
void OPA_CMP_POLL_Lock(void)
{
    OPA->POLLKEY = OPA_KEY1;
    OPA->POLLKEY = OPA_KEY2;
}

/********************************************************************************
 * @fn             OPA_CMP_Unlock
 *
 * @brief          Unlocks the CMP Controller.
 *
 * @return         None
 */
void OPA_CMP_Unlock(void)
{
    OPA->CMPKEY = OPA_KEY1;
    OPA->CMPKEY = OPA_KEY2;
}

/********************************************************************************
 * @fn             OPA_CMP_Lock
 *
 * @brief          Locks the CMP Controller.
 *
 * @return         None
 */
void OPA_CMP_Lock(void)
{
    OPA->CTLR2 |= (1<<31);
}

/*********************************************************************
 * @fn      OPA_Init
 *
 * @brief   Initializes the OPA peripheral according to the specified
 *        parameters in the OPA_InitStruct.
 *
 * @param   OPA_InitStruct - pointer to a OPA_InitTypeDef structure
 *
 * @return  none
 */
void OPA_Init(OPA_InitTypeDef *OPA_InitStruct)
{
    uint32_t tmp0 = 0, tmp1 = 0;

    tmp0 = OPA->CFGR1;
    tmp1 = OPA->CTLR1;

    tmp0 &= 0xF1C0FA02;
    tmp1 &= 0xFFE0E0C9;

    tmp0 |= (OPA_InitStruct->PSEL_POLL) | (OPA_InitStruct->POLL_NUM << 2)
            | (OPA_InitStruct->RST_EN << 4) | (OPA_InitStruct->SETUP_CFG <<5)
            | (OPA_InitStruct->POLL_AT <<7) | (OPA_InitStruct->OUT_IE << 8)
            | (OPA_InitStruct->NMI_IE << 10) | (OPA_InitStruct->POLL_CH1 << 16)
            | (OPA_InitStruct->POLL_CH2 << 18) | (OPA_InitStruct->POLL_CH3 << 20)
            | (OPA_InitStruct->POLL_SEL << 25);
    tmp1 |= (OPA_InitStruct->Mode << 1) | (OPA_InitStruct->PSEL << 4)
            | (OPA_InitStruct->NSEL << 8) | (OPA_InitStruct->FB << 11)
            | (OPA_InitStruct->PGADIF << 12) | (OPA_InitStruct->PGA_VBEN << 16)
            | (OPA_InitStruct->PGA_VBSEL << 17) |(OPA_InitStruct->VBCMPSEL << 18)
            | (OPA_InitStruct->OPA_HS << 20);

    OPA->CFGR1 = tmp0;
    OPA->CTLR1 = tmp1;
}

/*********************************************************************
 * @fn      OPA_StructInit
 *
 * @brief   Fills each OPA_StructInit member with its reset value.
 *
 * @param   OPA_StructInit - pointer to a OPA_InitTypeDef structure
 *
 * @return  none
 */
void OPA_StructInit(OPA_InitTypeDef *OPA_InitStruct)
{
    OPA_InitStruct->PSEL_POLL = CHP_OPA_POLL_OFF;
    OPA_InitStruct->POLL_NUM = CHP_POLL_NUM_1;
    OPA_InitStruct->RST_EN = RST_OPA_OFF;
    OPA_InitStruct->SETUP_CFG = OPA_SETUP_CFG_0;
    OPA_InitStruct->POLL_AT = OPA_POLL_AUTO_OFF;
    OPA_InitStruct->OUT_IE = OUT_IE_OFF;
    OPA_InitStruct->NMI_IE = NMI_IE_OFF;
    OPA_InitStruct->POLL_CH1 = OPA_POLL_CH1_PA2;
    OPA_InitStruct->POLL_CH2 = OPA_POLL_CH2_PA2;
    OPA_InitStruct->POLL_CH3 = OPA_POLL_CH3_PA2;
    OPA_InitStruct->POLL_SEL = OPA_POLL_SEL_SOFT;
    OPA_InitStruct->Mode = OUT_IO_OUT0;
    OPA_InitStruct->PSEL = CHP0;
    OPA_InitStruct->NSEL = CHN_OFF;
    OPA_InitStruct->FB = FB_OFF;
    OPA_InitStruct->PGADIF = PGADIF_OFF;
    OPA_InitStruct->PGA_VBEN = PGA_VBEN_OFF;
    OPA_InitStruct->PGA_VBSEL = PGA_VBSEL_VDD_DIV2;
    OPA_InitStruct->VBCMPSEL = VBCMPSEL_OFF;
    OPA_InitStruct->OPA_HS = HS_OFF;
}

/*********************************************************************
 * @fn      OPA_CMP1_Init
 *
 * @brief   Initializes the CMP1 peripheral according to the specified
 *        parameters in the CMP1_InitTypeDef.
 *
 * @param   CMP1_InitStruct - pointer to a CMP1_InitTypeDef structure
 *
 * @return  none
 */
void OPA_CMP1_Init(CMP1_InitTypeDef *CMP_InitStruct)
{
    uint32_t tmp0 = 0;
    uint32_t tmp1 = 0;

    tmp0 = OPA->CFGR2;
    tmp1 = OPA->CTLR2;

    tmp0 &= 0x8000FCF2;
    tmp1 &= 0xFFFFFE01;

    tmp0 |= (CMP_InitStruct->PSEL_POLL) | (CMP_InitStruct->POLL_NUM << 2)
            | (CMP_InitStruct->OUT_IE << 8) | (CMP_InitStruct->CNT_IE << 9)
            | (CMP_InitStruct->CMP_POLL_Interval << 16) | (CMP_InitStruct->POLL_CH1 << 25)
            | (CMP_InitStruct->POLL_CH2 << 27) | (CMP_InitStruct->POLL_CH3 << 29);

    tmp1 |= (CMP_InitStruct->CMP_Out_Mode << 1) | (CMP_InitStruct->NSEL << 3)
            | (CMP_InitStruct->PSEL <<5) | (CMP_InitStruct->HYEN <<7)
            |(CMP_InitStruct->RMID << 8);

    OPA->CFGR2 = tmp0;
    OPA->CTLR2 = tmp1;
}

/*********************************************************************
 * @fn      OPA_CMP1_StructInit
 *
 * @brief   Fills each OPA_CMP1_StructInit member with its reset value.
 *
 * @param   CMP1_StructInit - pointer to a OPA_CMP1_StructInit structure
 *
 * @return  none
 */
void OPA_CMP1_StructInit(CMP1_InitTypeDef *CMP_InitStruct)
{
    CMP_InitStruct->CMP_POLL_Interval = 0;
    CMP_InitStruct->PSEL_POLL = CHP_CMP1_POLL_OFF;
    CMP_InitStruct->POLL_NUM = CMP_POLL_NUM_1;
    CMP_InitStruct->OUT_IE = CMP_OUT_IE_OFF;
    CMP_InitStruct->CNT_IE = CMP_CNT_IE_OFF;
    CMP_InitStruct->POLL_CH1 = CMP_POLL_CH1_PC5;
    CMP_InitStruct->POLL_CH2 = CMP_POLL_CH2_PC5;
    CMP_InitStruct->POLL_CH3 = CMP_POLL_CH3_PC5;
    CMP_InitStruct->CMP_Out_Mode = OUT_IO0;
    CMP_InitStruct->NSEL = CMP_CHN0;
    CMP_InitStruct->PSEL = CMP_CHP0;
    CMP_InitStruct->HYEN = CMP_HYEN_OFF;
    CMP_InitStruct->RMID = CMP_RMID_OFF;
}

/*********************************************************************
 * @fn      OPA_Cmd
 *
 * @brief   Enables or disables the specified OPA peripheral.
 *
 * @param   NewState - ENABLE or DISABLE.
 *
 * @return  none
 */
void OPA_Cmd(FunctionalState NewState)
{
    if(NewState == ENABLE)
    {
        OPA->CTLR1 |= (uint32_t)(1 << 0);
    }
    else
    {
        OPA->CTLR1 &= ~(uint32_t)(1 << 0);
    }
}

/*********************************************************************
 * @fn      OPA_CMP_Cmd
 *
 * @brief   Enables or disables the specified CMP peripheral.
 *
 * @param   CMP_NUM - Select CMP
 *          NewState - ENABLE or DISABLE.
 *
 * @return  none
 */
void OPA_CMP_Cmd(CMP_Num_TypeDef CMP_NUM, FunctionalState NewState)
{
    if(NewState == ENABLE)
    {
        OPA->CTLR2 |= (uint32_t)(1 << (CMP_NUM <<4));
    }
    else
    {
        OPA->CTLR2 &= ~(uint32_t)(1 << (CMP_NUM <<4));
    }
}

/*********************************************************************
 * @fn      OPA_SystemReset_Cmd
 *
 * @brief   Enables or disables system reset the specified OPA peripheral.
 *
 * @param   NewState - ENABLE or DISABLE.
 *
 * @return  none
 */
void OPA_SystemReset_Cmd(FunctionalState NewState)
{
    if(NewState == ENABLE)
    {
        OPA->CFGR1 |= (uint32_t)(1 << 4);
    }
    else
    {
        OPA->CFGR1 &= ~(uint32_t)(1 << 4);
    }
}

/*********************************************************************
 * @fn      OPA_CMP_SystemReset_Cmd
 *
 * @brief   Enables or disables system reset the specified CMP peripheral.
 *
 * @param   CMP_NUM - Select CMP
 *          NewState - ENABLE or DISABLE.
 *
 * @return  none
 */
void OPA_CMP_SystemReset_Cmd(CMP_Num_TypeDef CMP_NUM, FunctionalState NewState)
{
    if(NewState == ENABLE)
    {
        OPA->CFGR2 |= (uint32_t)(1 << (CMP_NUM + 4));
    }
    else
    {
        OPA->CFGR2 &= ~(uint32_t)(1 << (CMP_NUM + 4));
    }
}

/*********************************************************************
 * @fn      OPA_CMP_FILT_Cmd
 *
 * @brief   Enables or disables digital filtering the specified CMP peripheral.
 *
 * @param   CMP_NUM - Select CMP
 *          NewState - ENABLE or DISABLE.
 *
 * @return  none
 */
void OPA_CMP_FILT_Cmd(FunctionalState NewState)
{
    if(NewState == ENABLE)
    {
        OPA->CTLR2 |= (uint32_t)(1 << 24);
    }
    else
    {
        OPA->CTLR2 &= ~(uint32_t)(1 << 24);
    }
}

/*********************************************************************
 * @fn      OPA_CMP_FILT_LEN_Config
 *
 * @brief   Configures the length of digital filtering for the CMP.
 *
 * @param   FILT_Len - The length of digital filtering.
 *            CMP_FILT_Len_0 - digital filtering(0.33us).
 *            CMP_FILT_Len_1 - digital filtering(0.5us).
 *
 * @return  none
 */
void OPA_CMP_FILT_LEN_Config(uint32_t FILT_Len)
{
    OPA->CTLR2 &= ~CMP_FILT_Len_1;
    OPA->CTLR2 |= FILT_Len;
}

/*********************************************************************
 * @fn      OPA_CMP_TIM1_BKINConfig
 *
 * @brief   Specifies the TIM1 Brake source.
 *
 * @param   Brake_Source - specifies the brake source to clear.
 *            TIM1_Brake_Source_IO - choose GPIO to be the TIM1 brake source
 *            TIM1_Brake_Source_CMP1 - choose CMP1 to be the TIM1 brake source
 *            TIM1_Brake_Source_CMP2 - choose CMP2 to be the TIM1 brake source
 *            TIM1_Brake_Source_OPA - choose OPA to be the TIM1 brake source
 *
 * @return  none
 */
void OPA_CMP_TIM1_BKINConfig(uint32_t Brake_Source)
{
    OPA->CTLR2 &= TIM1_Brake_Source_Config_MASK;
    OPA->CTLR2 |= Brake_Source;
}

/*********************************************************************
 * @fn      OPA_GetFlagStatus
 *
 * @brief   Checks whether the OPA flag is set or not.
 *
 * @param   OPA_FLAG - specifies the OPA flag to check.
 *            OPA_FLAG_OUT_POLL_CH_1 - the poll channel 1 of OPA
 *            OPA_FLAG_OUT_POLL_CH_2 - the poll channel 2 of OPA
 *            OPA_FLAG_OUT_POLL_CH_3 - the poll channel 3 of OPA
 *
 * @return  FlagStatus: SET or RESET.
 */
FlagStatus OPA_GetFlagStatus(uint32_t OPA_FLAG)
{
    FlagStatus bitstatus = RESET;

    if((OPA->CFGR1 & OPA_FLAG) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/*********************************************************************
 * @fn      OPA_CMP_GetFlagStatus
 *
 * @brief   Checks whether the CMP flag is set or not.
 *
 * @param   CMP_FLAG - specifies the CMP flag to check.
 *            CMP_FLAG_OUT_POLL_CH_1 - the poll channel 1 of CMP
 *            CMP_FLAG_OUT_POLL_CH_2 - the poll channel 2 of CMP
 *            CMP_FLAG_OUT_POLL_CH_3 - the poll channel 3 of CMP
 *            CMP_FLAG_POLL_END - poll channel end of CMP
 *
 * @return  FlagStatus: SET or RESET.
 */
FlagStatus OPA_CMP_GetFlagStatus(uint32_t CMP_FLAG)
{
    FlagStatus bitstatus = RESET;

    if((OPA->CFGR2 & CMP_FLAG) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/*********************************************************************
 * @fn      OPA_ClearFlag
 *
 * @brief   Clears the OPA flag.
 *
 * @param   OPA_FLAG - specifies the OPA flag to clear.
 *            OPA_FLAG_OUT_POLL_CH_1 - the poll channel 1 of OPA
 *            OPA_FLAG_OUT_POLL_CH_2 - the poll channel 2 of OPA
 *            OPA_FLAG_OUT_POLL_CH_3 - the poll channel 3 of OPA
 *
 * @return  none
 */
void OPA_ClearFlag(uint32_t OPA_FLAG)
{
    OPA->CFGR1 &= (uint32_t)~OPA_FLAG;
}

/*********************************************************************
 * @fn      OPA_CMP_ClearFlag
 *
 * @brief   Clears the CMP flag.
 *
 * @param   CMP_FLAG - specifies the CMP flag to clear.
 *            CMP_FLAG_OUT_POLL_CH_1 - the poll channel 1 of CMP
 *            CMP_FLAG_OUT_POLL_CH_2 - the poll channel 2 of CMP
 *            CMP_FLAG_OUT_POLL_CH_3 - the poll channel 3 of CMP
 *            CMP_FLAG_POLL_END - poll channel end of CMP
 *
 * @return  none
 */
void OPA_CMP_ClearFlag(uint32_t CMP_FLAG)
{
    OPA->CFGR2 &= (uint32_t)~CMP_FLAG;
}

/*********************************************************************
 * @fn      OPA_SoftwareStartPollCmd
 *
 * @brief   Enables or disables the selected OPA software start POLL.
 *
 * @param   NewState - ENABLE or DISABLE.
 *
 * @return  None
 */
void OPA_SoftwareStartPollCmd(FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        OPA->CFGR1 |= (1<<24);
    }
    else
    {
        OPA->CFGR1 &= ~(1<<24);
    }
}
