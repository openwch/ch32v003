/********************************** (C) COPYRIGHT *******************************
 * File Name          : flashop.c
 * Author             : RVMaking
 * Version            : V1.0.0
 * Date               : 2022/08/01
 * Description        : single line debug interface
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#include "flashop.h"


uint8_t FLASH_FAST_MODE_UNLOCK(void)
{
    uint8_t s;
    s = SingleLineWriteMEMDirect(FLASH_KEYR_Addr, UNLOCK_KEY1,DEG_RWSize_32);
    if(s != 0) return (s);
    s = SingleLineWriteMEMDirect(FLASH_KEYR_Addr, UNLOCK_KEY2,DEG_RWSize_32);
    if(s != 0) return (s);
    s = SingleLineWriteMEMDirect(FLASH_MODEKEYR_Addr, UNLOCK_KEY1,DEG_RWSize_32);
    if(s != 0) return (s);
    s = SingleLineWriteMEMDirect(FLASH_MODEKEYR_Addr, UNLOCK_KEY2,DEG_RWSize_32);
    if(s != 0) return (s);
    return s;
}


//buf reset
uint8_t FLASH_BUF_RESET(void)
{
    uint8_t s;
    u32* P_data;
    uint32_t p=0;
    P_data = &p;

    *P_data = CR_PAGE_PG;    //write PG
    s = SingleLineWriteMEMDirect(FLASH_CR_Addr,*P_data ,DEG_RWSize_32);
    if(s != 0) return (s);

    *P_data |= CR_BUF_RST;    //write RST
    s = SingleLineWriteMEMDirect(FLASH_CR_Addr,*P_data ,DEG_RWSize_32);
    if(s != 0) return (s);

    while(1)
    {   //BSY
        s = SingleLineReadMEMDirect(FLASH_SR_Addr, P_data , DEG_RWSize_32);
        if((((u32)*P_data) & (SR_BSY)) == 0)
        {
            break;
        }
    }

    s = SingleLineReadMEMDirect(FLASH_CR_Addr, P_data , DEG_RWSize_32);
    if(s != 0) return (s);

    *P_data &= ~CR_PAGE_PG;
    s = SingleLineWriteMEMDirect(FLASH_CR_Addr,*P_data ,DEG_RWSize_32);
    if(s != 0) return (s);

    return s;
}


//buf load
uint8_t FLASH_BUF_LOAD(uint32_t addr, uint32_t data)
{
   uint8_t s;
   u32* P_data;

   uint32_t p=0;
   P_data = &p;


   *P_data = CR_PAGE_PG;
   s = SingleLineWriteMEMDirect(FLASH_CR_Addr,*P_data , DEG_RWSize_32);
   if(s != 0) return (s);

   s = SingleLineWriteMEMDirect(addr, data , DEG_RWSize_32);
   if(s != 0) return (s);

   *P_data |= CR_BUF_LOAD;
   s = SingleLineWriteMEMDirect(FLASH_CR_Addr,*P_data ,DEG_RWSize_32);
   if(s != 0) return (s);


   while(1)
   {   //BSY
       s = SingleLineReadMEMDirect(FLASH_SR_Addr, P_data , DEG_RWSize_32);
       if((((u32)*P_data) & (SR_BSY)) == 0)
       {
           break;
       }
   }


   s = SingleLineReadMEMDirect(FLASH_CR_Addr, P_data , DEG_RWSize_32);
   if(s != 0) return (s);
   *P_data &= ~CR_PAGE_PG;
   s = SingleLineWriteMEMDirect(FLASH_CR_Addr,*P_data ,DEG_RWSize_32);
   if(s != 0) return (s);

   return s;


}

//erase page fast
uint8_t FLASH_ERASE_FAST(uint32_t addr)
{
   uint8_t s;
   u32* P_data;
   uint32_t p=0;
   P_data = &p;

   *P_data = CR_PAGE_ER;
   s = SingleLineWriteMEMDirect(FLASH_CR_Addr,*P_data , DEG_RWSize_32);
   if(s != 0) return (s);

   s = SingleLineWriteMEMDirect(FLASH_AR_Addr, addr , DEG_RWSize_32); //write addr
   if(s != 0) return (s);

   *P_data |= CR_STRT_Set;
   s = SingleLineWriteMEMDirect(FLASH_CR_Addr,*P_data , DEG_RWSize_32);
   if(s != 0) return (s);

   while(1)
   {   //BSY
       s = SingleLineReadMEMDirect(FLASH_SR_Addr, P_data , DEG_RWSize_32);
       if((((u32)*P_data) & (SR_BSY)) == 0)
       {
           break;
       }
   }

   s = SingleLineReadMEMDirect(FLASH_CR_Addr, P_data , DEG_RWSize_32);
   if(s != 0) return (s);
   *P_data &= ~CR_PAGE_ER;
   s = SingleLineWriteMEMDirect(FLASH_CR_Addr,*P_data ,DEG_RWSize_32);
   if(s != 0) return (s);

   return s;
}


uint8_t FLASH_PAGE_PG_FAST(uint32_t addr)
{
   uint8_t s;
   u32* P_data;

   uint32_t p=0;
   P_data = &p;

   *P_data = CR_PAGE_PG;
   s = SingleLineWriteMEMDirect(FLASH_CR_Addr,*P_data , DEG_RWSize_32);
   if(s != 0) return (s);


   s = SingleLineWriteMEMDirect(FLASH_AR_Addr, addr , DEG_RWSize_32); //write addr
   if(s != 0) return (s);

   *P_data |= CR_STRT_Set;
   s = SingleLineWriteMEMDirect(FLASH_CR_Addr,*P_data , DEG_RWSize_32);
   if(s != 0) return (s);

   while(1)
   {   //BSY
       s = SingleLineReadMEMDirect(FLASH_SR_Addr, P_data , DEG_RWSize_32);
       if((((u32)*P_data) & (SR_BSY)) == 0)
       {
           break;
       }
   }


   s = SingleLineReadMEMDirect(FLASH_CR_Addr, P_data , DEG_RWSize_32);
   if(s != 0) return (s);
   *P_data &= ~CR_PAGE_PG;
   s = SingleLineWriteMEMDirect(FLASH_CR_Addr,*P_data ,DEG_RWSize_32);
   if(s != 0) return (s);

   return s;


}




//OptionByte Erase
uint8_t SingleLine_OptionByte_PageErase(void)
{
    uint8_t s;
    uint32_t pdata;
    s=FLASH_FAST_MODE_UNLOCK();
    if(s) return s;

    s=SingleLineWriteMEMDirect(FLASH_OPTKEYR_Addr,UNLOCK_KEY1,DEG_RWSize_32);
    if(s) return s;
    s=SingleLineWriteMEMDirect(FLASH_OPTKEYR_Addr,UNLOCK_KEY2,DEG_RWSize_32);
    if(s) return s;

    s=SingleLineReadMEMDirect(FLASH_CR_Addr,&pdata,DEG_RWSize_32);
    if(s) return s;
    s=SingleLineWriteMEMDirect(FLASH_CR_Addr,(pdata|(1<<5)),DEG_RWSize_32);
    if(s) return s;

    s=SingleLineReadMEMDirect(FLASH_CR_Addr,&pdata,DEG_RWSize_32);
    if(s) return s;
    s=SingleLineWriteMEMDirect(FLASH_CR_Addr,(pdata | (1<<6)),DEG_RWSize_32);
    if(s) return s;

    while(1)
    {
        s=SingleLineReadMEMDirect(FLASH_SR_Addr,&pdata,DEG_RWSize_32);
        if(s)
        {
            debug_log("flash busy %x\n",s);
            return s;
        }
        if( (pdata&(1<<0)) == 0 )
        {
            break;
        }
    }

    s=SingleLineReadMEMDirect(FLASH_SR_Addr,&pdata,DEG_RWSize_32);
    if(s) return s;
    s=SingleLineWriteMEMDirect(FLASH_SR_Addr,(pdata|(1<<5)),DEG_RWSize_32);
    if(s) return s;

    s=SingleLineReadMEMDirect(FLASH_CR_Addr,&pdata,DEG_RWSize_32);
    if(s) return s;
    s=SingleLineWriteMEMDirect(FLASH_CR_Addr,(pdata & ~(1<<5)),DEG_RWSize_32);
    if(s) return s;

    s=SingleLineReadMEMDirect(FLASH_CR_Addr,&pdata,DEG_RWSize_32);
    if(s) return s;
    s=SingleLineWriteMEMDirect(FLASH_CR_Addr,(pdata & ~(1<<9)),DEG_RWSize_32);
    if(s) return s;

    return s;
}


#define RVFNIC_CTRLR   0xE000E048
#define NVIC_KEY1       ((uint32_t)0xFA050000)
#define	NVIC_KEY2				((uint32_t)0xBCAF0000)
#define	NVIC_KEY3				((uint32_t)0xBEEF0000)


uint8_t SingleLineSoftRest(void)
{
    uint8_t s;
    //System soft reset
    s=SingleLineWriteMEMDirect(RVFNIC_CTRLR,NVIC_KEY3|(1<<7),DEG_RWSize_32);

    return s;
}

//Unprotect Read
uint8_t SingleLine_Remove_ReadProtect(void)
{
    uint32_t data=0;
    uint8_t s=0,i=0;
    SingleLine_OptionByte_PageErase();

remove_protect:
    FLASH_FAST_MODE_UNLOCK();
    s=SingleLineWriteMEMDirect(FLASH_OPTKEYR_Addr,UNLOCK_KEY1,DEG_RWSize_32);
    if(s) return s;
    s=SingleLineWriteMEMDirect(FLASH_OPTKEYR_Addr,UNLOCK_KEY2,DEG_RWSize_32);
    if(s) return s;
    s=SingleLineReadMEMDirect(FLASH_CR_Addr,&data,DEG_RWSize_32);
    if(s) return s;
    s=SingleLineWriteMEMDirect(FLASH_CR_Addr,data|(1<<4),DEG_RWSize_32);

    if(s) return s;

    s=SingleLineWriteMEMDirect(0x1FFFF800,0x5aa5,DEG_RWSize_16);

    s=SingleLineCheckAbstSTA();
    debug_log("--s:%x\r\n",s);
    if(s)
    {
        if(i<5)
        {
            i++;
            Delay_Ms(1);
            goto remove_protect;
        }
        else
        {
            return s;
        }
    }
    while(1)
    {
        s=SingleLineReadMEMDirect(FLASH_SR_Addr,&data,DEG_RWSize_32);

        if( (data&(1<<0)) == 0 )
        {
            break;
        }
    }

#if 0
    s=SingleLineWriteMEMDirect(0x1FFFF802,*(UINT16*)(&config_buf[1]),DEG_RWSize_16);
    while(1)
    {
        s=SingleLineReadMEMDirect(FLASH_SR_Addr,&data,DEG_RWSize_32);

        if( (data&(1<<0)) == 0 )
        {
            break;
        }
    }
#if 0
    s=SingleLineWriteMEMDirect(0x1FFFF804,*(UINT16*)(&config_buf[2]),DEG_RWSize_16);
    while(1)
    {
        s=SingleLineReadMEMDirect(FLASH_SR_Addr,&data,DEG_RWSize_32);

        if( (data&(1<<0)) == 0 )
        {
            break;
        }
    }

    s=SingleLineWriteMEMDirect(0x1FFFF806,*(UINT16*)(&config_buf[3]),DEG_RWSize_16);
    while(1)
    {
        s=SingleLineReadMEMDirect(FLASH_SR_Addr,&data,DEG_RWSize_32);

        if( (data&(1<<0)) == 0 )
        {
            break;
        }
    }
#endif

    s=SingleLineWriteMEMDirect(0x1FFFF808,*(UINT16*)(&config_buf[4]),DEG_RWSize_16);
    while(1)
    {
        s=SingleLineReadMEMDirect(FLASH_SR_Addr,&data,DEG_RWSize_32);

        if( (data&(1<<0)) == 0 )
        {
            break;
        }
    }
#endif

    //clear EOP
    s=SingleLineReadMEMDirect(FLASH_SR_Addr,&data,DEG_RWSize_32);
    s=SingleLineWriteMEMDirect(FLASH_SR_Addr,data|(1<<5),DEG_RWSize_32);

    //clear OBPG
    s=SingleLineReadMEMDirect(FLASH_CR_Addr,&data,DEG_RWSize_32);
    s=SingleLineWriteMEMDirect(FLASH_CR_Addr,data& ~(1<<4),DEG_RWSize_32);

    //LOCK OPT
    s=SingleLineReadMEMDirect(FLASH_CR_Addr,&data,DEG_RWSize_32);
    s=SingleLineWriteMEMDirect(FLASH_CR_Addr,data& ~(1<<9),DEG_RWSize_32);



    return 0;

}

