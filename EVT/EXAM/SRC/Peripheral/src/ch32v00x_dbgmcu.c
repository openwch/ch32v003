/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00X_dbgmcu.c
 * Author             : WCH
 * Version            : V1.0.1
 * Date               : 2025/02/27
 * Description        : This file provides all the DBGMCU firmware functions.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#include <ch32v00X_dbgmcu.h>


#define IDCODE_DEVID_MASK    ((uint32_t)0x0000FFFF)

/*********************************************************************
 * @fn      DBGMCU_GetREVID
 *
 * @brief   Returns the device revision identifier.
 *
 * @return  Revision identifier.
 */
uint32_t DBGMCU_GetREVID(void)
{
    return ((*(uint32_t *)0x1FFFF704) >> 16);
}

/*********************************************************************
 * @fn      DBGMCU_GetDEVID
 *
 * @brief   Returns the device identifier.
 *
 * @return  Device identifier.
 */
uint32_t DBGMCU_GetDEVID(void)
{
    return ((*(uint32_t *)0x1FFFF704) & IDCODE_DEVID_MASK);
}

/*********************************************************************
 * @fn      __get_DEBUG_CR
 *
 * @brief   Return the DEBUGE Control Register
 *
 * @return  DEBUGE Control value
 */
uint32_t __get_DEBUG_CR(void)
{
    uint32_t result;

    __asm volatile("csrr %0,""0x7C0" : "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __set_DEBUG_CR
 *
 * @brief   Set the DEBUGE Control Register
 *
 * @param   value  - set DEBUGE Control value
 *
 * @return  none
 */
void __set_DEBUG_CR(uint32_t value)
{
    __asm volatile("csrw 0x7C0, %0" : : "r"(value));
}


/*********************************************************************
 * @fn      DBGMCU_Config
 *
 * @brief   Configures the specified peripheral and low power mode behavior
 *        when the MCU under Debug mode.
 *
 * @param   DBGMCU_Periph - specifies the peripheral and low power mode.
 *            DBGMCU_SLEEP -  Keep debugger connection during SLEEP mode
 *            DBGMCU_STANDBY -  Keep debugger connection during STANDBY mode
 *            DBGMCU_IWDG_STOP - Debug IWDG stopped when Core is halted
 *            DBGMCU_WWDG_STOP - Debug WWDG stopped when Core is halted
 *            DBGMCU_TIM1_STOP - TIM1 counter stopped when Core is halted
 *            DBGMCU_TIM2_STOP - TIM2 counter stopped when Core is halted
 *            DBGMCU_TIM3_STOP - TIM3 counter stopped when Core is halted
 *          NewState - ENABLE or DISABLE.
 *
 * @return  none
 */
void DBGMCU_Config(uint32_t DBGMCU_Periph, FunctionalState NewState)
{
    uint32_t val;

    if(NewState != DISABLE)
    {
        __set_DEBUG_CR(DBGMCU_Periph);
    }
    else
    {
        val = __get_DEBUG_CR();
        val &= ~(uint32_t)DBGMCU_Periph;
        __set_DEBUG_CR(val);
    }
}
/*********************************************************************
 * @fn      DBGMCU_GetCHIPID
 *
 * @brief   Returns the CHIP identifier.
 *
 * @return Device identifier.
 *          ChipID List-
 *    CH32V006K8U6-0x006006x0
 *    CH32V006E8R6-0x006106x0
 *    CH32V006F8U6-0x006206x0
 *    CH32V006F8P6-0x006306x0
 *    CH32V006F4U6-0x006406x0
 *
 *    CH32M007G8R6-0x007008x0
 *    CH32M007E8R6-0x007308x0
 *    CH32M007E8U6-0x007408x0
 *    CH32V007E8R6-0x007106x0
 *    CH32V007K8U6-0x007206x0
 *
 *    CH32V005E6R6-0x005006x0
 *    CH32V005F6U6-0x005106x0
 *    CH32V005F6P6-0x005206x0
 *    CH32V005D6U6-0x005306x0
 *
 *    CH32V002F4P6-0x002006x0
 *    CH32V002F4U6-0x002106x0
 *    CH32V002A4M6-0x002206x0
 *    CH32V002D4U6-0x002306x0
 *    CH32V002J4M6-0x002406x0
 *
 *    CH32V004F6P1-0x004006x0
 *    CH32V004F6U1-0x00410600
 */
uint32_t DBGMCU_GetCHIPID( void )
{
    return( *( uint32_t * )0x1FFFF704 );
}
