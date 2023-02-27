/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : RVMaking
 * Version            : V1.0.0
 * Date               : 2019/10/15
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#include "debug.h"
#include "singleline.h"
#include "flashop.h"
/* Global define */


/* Global Variable */

uint16_t size = 64;
uint32_t data = 0;
uint32_t testdata[64],readbuf[64];




/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main( void )
{
	uint8_t i=0;
	uint8_t ack=0;
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2 );
	Delay_Init( );
    USART_Printf_Init( 115200 );
    TimeBase_Init( );    
    debug_log( "SystemClk:%d\r\n", SystemCoreClock );
	
    debug_log( "CH32V003 SingleLine Test %s-%s\r\n",__DATE__,__TIME__ );
	Delay_Ms(1000); //003 is powered  after the debugger is powered 
	
	for(i=0;i<size;i++)
	{
	    testdata[i] = 0xAA55AA00+i;
	}		
	
	ack = SingleLineDebugIint( );
	if( ack )
	{
	    debug_log("Debug interface init err\r\n");
	}
	else
	{
		debug_log("Init Suc \r\n");
		ack = SingleLineGoToPauseMode( );
		if(ack) {debug_log("Pause Err\r\n");while(1);}
		debug_log("Pause Suc \r\n");
	    debug_log("===============RAM Test====================\r\n");
		ack = SingleLineWriteMEMDirectBlock(0x20000000,&size,testdata);
		if(ack) debug_log("Write Ram Err\r\n");
		ack = SingleLineReadMEMDirectBlock(0x20000000,&size,readbuf);
		if(!ack)
        {
            for(i=0;i<size;i++)
            {
            if(i%8 == 0) debug_log("\r\n");
            debug_log("%08x ",readbuf[i]);
            }
            debug_log("\r\n");
        }
		
	}			
	debug_log("===============Flash Test====================\r\n");
    ack = SingleLineReadMEMDirect(FLASH_OBR_Addr, &data, DEG_RWSize_32);
    if(!ack) debug_log("OBR:%08x\r\n",data);
    if(data&0x3)
    {
        debug_log("Read protect enable\r\n");
        SingleLine_Remove_ReadProtect();
        Delay_Ms(10);
        SingleLineExitPauseMode();

        SingleLineGoToPauseMode();
        SingleLineSoftRest();
        Delay_Ms(10);
        SingleLineExitPauseMode();
   }
	ack=SingleLineGoToPauseMode();
    if(ack){ debug_log("Pause err\r\n"); while(1);}


    ack= FLASH_FAST_MODE_UNLOCK();
    if(ack)
    {
        debug_log("Err unlock:%x\r\n",ack);
    }
    else
    {
        debug_log("Unlock suc\r\n");
        ack = FLASH_ERASE_FAST(0x08000000);
        if(ack)
        {
            debug_log("Err erase:%x\r\n",ack);
        }
        else
        {
            debug_log("Erase suc\r\n");
            for(i=0;i<16;i++)
            {
                if(i%8 == 0) debug_log("\r\n");
                SingleLineReadMEMDirect(0x08000000+i*4,&data,DEG_RWSize_32);
                debug_log("%08x ",data);
            }
            debug_log("\r\n");

            ack = FLASH_BUF_RESET();
            if(ack) debug_log("Err bufload:%x\r\n",ack);
            else
            {
                debug_log("Bufreset suc\r\n");
                for(i = 0; i < 16; i++)
                {
                    testdata[i] = 0xCCCCCCC0+i;
                }
                for(i=0; i<16; i++)
                {
                    FLASH_BUF_LOAD(0x08000000+4*i, testdata[i]);
                }

                ack = FLASH_PAGE_PG_FAST(0x08000000);

                if(ack) debug_log("Err page pg:%x\r\n",ack);
                else
                {
                    debug_log("Page pg suc\r\n");
                    for(i=0;i<16;i++)
                    {
                        if(i%8 == 0) debug_log("\r\n");
                        SingleLineReadMEMDirect(0x08000000+i*4,&data,DEG_RWSize_32);
                        debug_log("%08x ",data);
                    }
                    debug_log("\r\n");

                    debug_log("Erase again\r\n");
                    ack = FLASH_ERASE_FAST(0x08000000);
                    if(ack) debug_log("Err Page pg:%x\r\n",ack);
                    else
                    {
                        for(i=0;i<16;i++)
                        {
                            if(i%8 == 0) debug_log("\r\n");
                            SingleLineReadMEMDirect(0x08000000+i*4,&data,DEG_RWSize_32);
                            debug_log("%08x ",data);
                        }
                        debug_log("\r\n");
                    }


                }

            }

        }
    }
	
	
	
  while( 1 )
  {
		;
  }
}



