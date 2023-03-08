/********************************** (C) COPYRIGHT *******************************
 * File Name          : Singleline.h
 * Author             : RVMaking
 * Version            : V1.0.0
 * Date               : 2022/08/01
 * Description        : single line debug interface for
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#ifndef __SINGLELINE_H
#define __SINGLELINE_H

#include "debug.h"

#define  ONLINE_CAPR_STA           (0x7C<<1)
#define  ONLINE_CFGR_MASK          (0x7D<<1)
#define  ONLINE_CFGR_SHAD          (0x7E<<1)


/* debug modle reg I2C addr , left shift one bit */
#define  DEG_Module_STA           (0x11<<1)
#define  DEG_Module_CTRL          (0x10<<1)
#define  DEG_Hart_INFO            (0x12<<1)
#define  DEG_Abst_STA             (0x16<<1)
#define  DEG_Abst_CMD             (0x17<<1)
#define  DEG_Abst_CMDAUTO         (0x18<<1)
#define  DEG_Abst_DATA0           (0x04<<1)
#define  DEG_Abst_DATA1           (0x05<<1)
#define  DEG_Prog_BUF0            (0x20<<1)
#define  DEG_Prog_BUF1            (0x21<<1)
#define  DEG_Prog_BUF2            (0x22<<1)
#define  DEG_Prog_BUF3            (0x23<<1)
#define  DEG_Prog_BUF4            (0x24<<1)
#define  DEG_Prog_BUF5            (0x25<<1)
#define  DEG_Prog_BUF6            (0x26<<1)
#define  DEG_Prog_BUF7            (0x27<<1)
#define  DEG_Hart_SUM0            (0x40<<1)

//DM size
#define DEG_RWSize_32  2
#define DEG_RWSize_16  1
#define DEG_RWSize_8   0


//SDIO  PB3-SDIO
#define DIO_IN()  {GPIOB->CFGLR&=0XFFFF0FFF;GPIOB->CFGLR|=8<<12;}    //in
#define DIO_OUT() {GPIOB->CFGLR&=0XFFFF0FFF;GPIOB->CFGLR|=3<<12;}    //outpp
#define DIO_OUT_OD() {GPIOB->CFGLR&=0XFFFF0FFF;GPIOB->CFGLR|=7<<12;} //oc

#define SWDIO_Out_1()   {GPIOB->BSHR = GPIO_Pin_3;}
#define SWDIO_Out_0()   {GPIOB->BCR = GPIO_Pin_3; }
#define SWDIO_In()      (GPIOB->INDR & GPIO_Pin_3)                  //DIO  ‰»Î


uint8_t DWordTransm( uint8_t addr, uint32_t dat );
uint8_t DWordRecv( uint8_t addr, uint32_t *pdat );
uint8_t BlockTransm( uint8_t addr, uint32_t *datbuf, uint16_t *psize );
uint8_t BlockRecv( uint8_t addr,  uint32_t *datbuf, uint16_t *psize );
void DebugSpeedMode( uint8_t speed );
void ResetDeviceSWD( void );

void TimeBase_Init(void);
uint8_t SingleLineDebugIint(void);
uint8_t SingleLineCheckDMSTA( uint32_t field,  uint32_t val);
uint8_t SingleLineCheckAbstSTA( void );
uint8_t SingleLineCheckPauseContinue(void);
uint32_t  SingleLineReadPauseS(void);
uint8_t SingleLineGoToPauseMode(void);
uint8_t SingleLineExitPauseMode(void);
uint8_t SingleLineCoreReset(void);
uint8_t SingleLineDebugMReset(void);

uint8_t SingleLineReadGPR( uint8_t gpregnum, uint32_t *pd );
uint8_t SingleLineWriteGPR( uint8_t gpregnum, uint32_t d );
uint8_t SingleLineReadCSR( uint16_t csr, uint32_t *pd );
uint8_t SingleLineWriteCSR( uint16_t csr, uint32_t d );
uint8_t SingleLineReadMEMDirect( uint32_t addr, uint32_t *pd, uint32_t size );
uint8_t SingleLineReadMEMDirectBlock( uint32_t addr, uint16_t *psize, uint32_t *pdbuf);
uint8_t SingleLineWriteMEMDirect( uint32_t addr, uint32_t d, uint32_t size );
uint8_t SingleLineWriteMEMDirectBlock( uint32_t addr, uint16_t *psize, uint32_t *pdbuf );




#endif

