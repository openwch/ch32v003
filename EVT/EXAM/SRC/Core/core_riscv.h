/********************************** (C) COPYRIGHT  *******************************
* File Name          : core_riscv.h
* Author             : WCH
* Version            : V1.0.1
* Date               : 2024/10/28
* Description        : RISC-V V2 Core Peripheral Access Layer Header File for CH32V003
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#ifndef __CORE_RISCV_H__
#define __CORE_RISCV_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* IO definitions */
#ifdef __cplusplus
  #define     __I     volatile                /*  defines 'read only' permissions     */
#else
  #define     __I     volatile const          /*  defines 'read only' permissions     */
#endif
#define       __O     volatile                /*  defines 'write only' permissions    */
#define       __IO    volatile                /*  defines 'read / write' permissions  */

/* Standard Peripheral Library old types (maintained for legacy purpose) */
typedef __I uint32_t vuc32;   /* Read Only */
typedef __I uint16_t vuc16;   /* Read Only */
typedef __I uint8_t  vuc8;    /* Read Only */

typedef const uint32_t uc32;  /* Read Only */
typedef const uint16_t uc16;  /* Read Only */
typedef const uint8_t  uc8;   /* Read Only */

typedef __I int32_t vsc32;    /* Read Only */
typedef __I int16_t vsc16;    /* Read Only */
typedef __I int8_t  vsc8;     /* Read Only */

typedef const int32_t sc32;   /* Read Only */
typedef const int16_t sc16;   /* Read Only */
typedef const int8_t  sc8;    /* Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t  vu16;
typedef __IO uint8_t   vu8;

typedef uint32_t  u32;
typedef uint16_t  u16;
typedef uint8_t   u8;

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef int32_t  s32;
typedef int16_t  s16;
typedef int8_t   s8;

typedef enum {NoREADY = 0, READY = !NoREADY} ErrorStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

#define   RV_STATIC_INLINE  static  inline

/* memory mapped structure for Program Fast Interrupt Controller (PFIC) */
typedef struct{
    __I  uint32_t ISR[8];
    __I  uint32_t IPR[8];
    __IO uint32_t ITHRESDR;
    __IO uint32_t RESERVED;
    __IO uint32_t CFGR;
    __I  uint32_t GISR;
    __IO uint8_t VTFIDR[4];
    uint8_t RESERVED0[12];
    __IO uint32_t VTFADDR[4];
    uint8_t RESERVED1[0x90];
    __O  uint32_t IENR[8];
    uint8_t RESERVED2[0x60];
    __O  uint32_t IRER[8];
    uint8_t RESERVED3[0x60];
    __O  uint32_t IPSR[8];
    uint8_t RESERVED4[0x60];
    __O  uint32_t IPRR[8];
    uint8_t RESERVED5[0x60];
    __IO uint32_t IACTR[8];
    uint8_t RESERVED6[0xE0];
    __IO uint8_t IPRIOR[256];
    uint8_t RESERVED7[0x810];
    __IO uint32_t SCTLR;
}PFIC_Type;

/* memory mapped structure for SysTick */
typedef struct
{
    __IO uint32_t CTLR;
    __IO uint32_t SR;
    __IO uint32_t CNT;
    uint32_t RESERVED0;
    __IO uint32_t CMP;
    uint32_t RESERVED1;
}SysTick_Type;


#define PFIC            ((PFIC_Type *) 0xE000E000 )
#define NVIC            PFIC
#define NVIC_KEY1       ((uint32_t)0xFA050000)
#define	NVIC_KEY2	    ((uint32_t)0xBCAF0000)
#define	NVIC_KEY3		((uint32_t)0xBEEF0000)

#define SysTick         ((SysTick_Type *) 0xE000F000)


/*********************************************************************
 * @fn      __enable_irq
 *          This function is only used for Machine mode.
 *
 * @brief   Enable Global Interrupt
 *
 * @return  none
 */
__attribute__( ( always_inline ) ) RV_STATIC_INLINE void __enable_irq()
{
  __asm volatile ("csrs mstatus, %0" : : "r" (0x88) );
}

/*********************************************************************
 * @fn      __disable_irq
 *          This function is only used for Machine mode.
 *
 * @brief   Disable Global Interrupt
 *
 * @return  none
 */
__attribute__( ( always_inline ) ) RV_STATIC_INLINE void __disable_irq()
{
  __asm volatile ("csrc mstatus, %0" : : "r" (0x88) );
}

/*********************************************************************
 * @fn      __NOP
 *
 * @brief   nop
 *
 * @return  none
 */
__attribute__( ( always_inline ) ) RV_STATIC_INLINE void __NOP()
{
  __asm volatile ("nop");
}

/*********************************************************************
 * @fn      NVIC_EnableIRQ
 *
 * @brief   Enable Interrupt
 *
 * @param   IRQn - Interrupt Numbers
 *
 * @return  none
 */
__attribute__( ( always_inline ) ) RV_STATIC_INLINE void NVIC_EnableIRQ(IRQn_Type IRQn)
{
  NVIC->IENR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/*********************************************************************
 * @fn      NVIC_DisableIRQ
 *
 * @brief   Disable Interrupt
 *
 * @param   IRQn - Interrupt Numbers
 *
 * @return  none
 */
__attribute__( ( always_inline ) ) RV_STATIC_INLINE void NVIC_DisableIRQ(IRQn_Type IRQn)
{
  NVIC->IRER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/*********************************************************************
 * @fn      NVIC_GetStatusIRQ
 *
 * @brief   Get Interrupt Enable State
 *
 * @param   IRQn - Interrupt Numbers
 *
 * @return  1 - Interrupt Pending Enable
 *          0 - Interrupt Pending Disable
 */
__attribute__( ( always_inline ) ) RV_STATIC_INLINE uint32_t NVIC_GetStatusIRQ(IRQn_Type IRQn)
{
  return((uint32_t) ((NVIC->ISR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0));
}

/*********************************************************************
 * @fn      NVIC_GetPendingIRQ
 *
 * @brief   Get Interrupt Pending State
 *
 * @param   IRQn - Interrupt Numbers
 *
 * @return  1 - Interrupt Pending Enable
 *          0 - Interrupt Pending Disable
 */
__attribute__( ( always_inline ) ) RV_STATIC_INLINE uint32_t NVIC_GetPendingIRQ(IRQn_Type IRQn)
{
  return((uint32_t) ((NVIC->IPR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0));
}

/*********************************************************************
 * @fn      NVIC_SetPendingIRQ
 *
 * @brief   Set Interrupt Pending
 *
 * @param   IRQn - Interrupt Numbers
 *
 * @return  none
 */
__attribute__( ( always_inline ) ) RV_STATIC_INLINE void NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
  NVIC->IPSR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/*********************************************************************
 * @fn      NVIC_ClearPendingIRQ
 *
 * @brief   Clear Interrupt Pending
 *
 * @param   IRQn - Interrupt Numbers
 *
 * @return  none
 */
__attribute__( ( always_inline ) ) RV_STATIC_INLINE void NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
  NVIC->IPRR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/*********************************************************************
 * @fn      NVIC_GetActive
 *
 * @brief   Get Interrupt Active State
 *
 * @param   IRQn - Interrupt Numbers
 *
 * @return  1 - Interrupt Active
 *          0 - Interrupt No Active
 */
__attribute__( ( always_inline ) ) RV_STATIC_INLINE uint32_t NVIC_GetActive(IRQn_Type IRQn)
{
  return((uint32_t)((NVIC->IACTR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0));
}

/*********************************************************************
 * @fn      NVIC_SetPriority
 *
 * @brief   Set Interrupt Priority
 *
 * @param   IRQn - Interrupt Numbers
 *          interrupt nesting enable(CSR-0x804 bit1 = 1)
 *            priority - bit[7] - Preemption Priority
 *                       bit[6] - Sub priority
 *                       bit[5:0] - Reserve
 *          interrupt nesting disable(CSR-0x804 bit1 = 0)
 *            priority - bit[7:6] - Sub priority
 *                       bit[5:0] - Reserve
 * @return  none
 */
__attribute__( ( always_inline ) ) RV_STATIC_INLINE void NVIC_SetPriority(IRQn_Type IRQn, uint8_t priority)
{
  NVIC->IPRIOR[(uint32_t)(IRQn)] = priority;
}

/*********************************************************************
 * @fn      _SEV
 *
 * @brief   Set Event
 *
 * @return  none
 */
__attribute__( ( always_inline ) ) RV_STATIC_INLINE void _SEV(void)
{
  uint32_t t;

  t = NVIC->SCTLR;
  NVIC->SCTLR |= (1<<3)|(1<<5);
  NVIC->SCTLR = (NVIC->SCTLR & ~(1<<5)) | ( t & (1<<5));
}

/*********************************************************************
 * @fn      WFItoWFE
 *
 * @brief   WFI to WFE
 *
 * @return  none
 */
__attribute__( ( always_inline ) ) RV_STATIC_INLINE void WFItoWFE(void)
{
  NVIC->SCTLR |= (1<<3);
}

__attribute__( (section(".highcode")) ) void WFE(u32 t);

#define _WFE()   WFE(19) //48M
//#define _WFE()   WFE(11) //24M
//#define _WFE()   WFE(8) //16M
//#define _WFE()   WFE(5) //8M

/*********************************************************************
 * @fn      __WFE
 *
 * @brief   Wait for Events
 *
 * @return  none
 */
__attribute__( ( always_inline ) ) RV_STATIC_INLINE void __WFE(void)
{
    NVIC->SCTLR |= (1<<4);
    __disable_irq();
    _SEV();
    WFItoWFE();
    _WFE();
    WFItoWFE();
    _WFE();
    __enable_irq();
}

/*********************************************************************
 * @fn      __WFI
 *
 * @brief   Wait for Interrupt
 *
 * @return  none
 */
__attribute__( ( always_inline ) ) RV_STATIC_INLINE void __WFI(void)
{
  __WFE();
}

/*********************************************************************
 * @fn      SetVTFIRQ
 *
 * @brief   Set VTF Interrupt
 *
 * @param   addr - VTF interrupt service function base address.
 *                 IRQn - Interrupt Numbers
 *                 num - VTF Interrupt Numbers
 *                 NewState -  DISABLE or ENABLE
 *
 * @return  none
 */
__attribute__( ( always_inline ) ) RV_STATIC_INLINE void SetVTFIRQ(uint32_t addr, IRQn_Type IRQn, uint8_t num, FunctionalState NewState)
{
  if(num > 1)  return ;

  if (NewState != DISABLE)
  {
      NVIC->VTFIDR[num] = IRQn;
      NVIC->VTFADDR[num] = ((addr&0xFFFFFFFE)|0x1);
  }
  else
  {
      NVIC->VTFIDR[num] = IRQn;
      NVIC->VTFADDR[num] = ((addr&0xFFFFFFFE)&(~0x1));
  }
}

/*********************************************************************
 * @fn      NVIC_SystemReset
 *
 * @brief   Initiate a system reset request
 *
 * @return  none
 */
__attribute__( ( always_inline ) ) RV_STATIC_INLINE void NVIC_SystemReset(void)
{
  NVIC->CFGR = NVIC_KEY3|(1<<7);
}


/* Core_Exported_Functions */  
extern uint32_t __get_MSTATUS(void);
extern void __set_MSTATUS(uint32_t value);
extern uint32_t __get_MISA(void);
extern void __set_MISA(uint32_t value);
extern uint32_t __get_MTVEC(void);
extern void __set_MTVEC(uint32_t value);
extern uint32_t __get_MSCRATCH(void);
extern void __set_MSCRATCH(uint32_t value);
extern uint32_t __get_MEPC(void);
extern void __set_MEPC(uint32_t value);
extern uint32_t __get_MCAUSE(void);
extern void __set_MCAUSE(uint32_t value);
extern uint32_t __get_MVENDORID(void);
extern uint32_t __get_MARCHID(void);
extern uint32_t __get_MIMPID(void);
extern uint32_t __get_MHARTID(void);
extern uint32_t __get_SP(void);

#ifdef __cplusplus
}
#endif

#endif/* __CORE_RISCV_H__ */





