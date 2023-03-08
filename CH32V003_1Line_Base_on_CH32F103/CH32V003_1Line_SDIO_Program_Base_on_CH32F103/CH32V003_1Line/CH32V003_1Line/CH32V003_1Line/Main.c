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
#include "flash_prg_algorithm.h"
#include "code.h"


/* Global define */




/* Global Variable */





/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */

int main( void )
{
		uint32_t i=0;
		uint8_t ack=0;
		uint32_t data;
		
		uint32_t len;
	
		NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2 );
		Delay_Init( );
		USART_Printf_Init(115200);
    TimeBase_Init( );    
    debug_log( "SystemClk:%d\r\n", SystemCoreClock );
    debug_log( "CH32V003 SingleLine Test %s-%s\r\n",__DATE__,__TIME__ );
		Delay_Ms(20);  
	
	
	
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
		}
		
		
		//check protect
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
    if(ack){ debug_log("Pause Err\r\n"); while(1);}
		debug_log("Write Code To Ram\r\n");
	  ack = SingleLine_Flash_Init( );
		if(ack) { debug_log("Write Code To Ram Err\r\n"); }
		
		
		{
			debug_log("Check Ram Data:\r\n");	
			for(i=0;i<(512/4);i++)
			{
					if(i%4 == 0) debug_log("\r\n");
					SingleLineReadMEMDirect(0x20000000+4*i,&data,DEG_RWSize_32);
					debug_log("%08x  ",data);
			}
			debug_log("\r\n");
		}
		
		
	  ack = SingleLine_Flash_EraseChip(); //Full erase	
		if(ack)
		{ 
			 debug_log("Erase Fail\r\n");
			 while(1);
		}
		else
		{
			debug_log("Erase Chip Suc\r\n");
		}
		
		len = sizeof(code);
		debug_log("Length:%d\r\n",len);
		
		
		ack = SingleLine_Flash_ProgramPages( 0x08000000,(uint32_t *)code, len );
		
		if(ack) debug_log("Program Fail\r\n");
		else
		{
			debug_log("Program Succ And Lock\r\n");
			ack = SingleLine_Flash_Uninit();
			if(ack) debug_log("Flash Lock Fail\r\n");
			else
			{
					debug_log("Flash Lock Succ And Reset Chip!\r\n");
					SingleLineSoftRest( );
			}
		}

		
		
		
		
		while( 1 )
		{
			;
		}
}



