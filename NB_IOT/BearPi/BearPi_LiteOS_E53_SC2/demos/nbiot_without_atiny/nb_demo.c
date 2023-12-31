/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/
 

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "nb_iot/los_nb_api.h"
#include "nb_cmd_ioctl.h"
#include "lcd.h"
#include "E53_SC2.h"


#define OCEAN_IP "119.3.250.80"
#define SECURITY_PORT "5684"
#define NON_SECURITY_PORT "5683"
#define DEV_PSKID  "868744031131026"
#define DEV_PSK  "d1e1be0c05ac5b8c78ce196412f0cdb0"


#define cn_buf_len    256   //may be bigger enough
static char s_report_buf[cn_buf_len];
char E53_SC2_send[24]={0};

void nb_iot_entry(void);

void demo_nbiot_only(void)
{
#if defined(WITH_AT_FRAMEWORK) && defined(USE_NB_NEUL95_NO_ATINY)
    #define AT_DTLS 0
    #if AT_DTLS
    sec_param_s sec;
    sec.setpsk = 1;
    sec.pskid = DEV_PSKID;
    sec.psk = DEV_PSK;
    #endif
    printf("\r\n=====================================================");
    printf("\r\nSTEP1: Init NB Module( NB Init )");
    printf("\r\n=====================================================\r\n");

#if AT_DTLS
    los_nb_init((const int8_t *)OCEAN_IP, (const int8_t *)SECURITY_PORT, &sec);
    LCD_Clear(WHITE);		   
    POINT_COLOR = RED;			
    LCD_ShowString(40, 10, 200, 16, 24, "IoTCluB BearPi");
    LCD_ShowString(50, 50, 200, 16, 24, "E53_SC2_Demo");
    LCD_ShowString(10, 100, 200, 16, 16, "NCDP_IP:");
    LCD_ShowString(80, 100, 200, 16, 16, OCEAN_IP);
    LCD_ShowString(10, 150, 200, 16, 16, "NCDP_PORT:");
    LCD_ShowString(100, 150, 200, 16, 16, SECURITY_PORT);
#else
    los_nb_init((const int8_t *)OCEAN_IP, (const int8_t *)NON_SECURITY_PORT, NULL);
    LCD_Clear(WHITE);		   	
    POINT_COLOR = RED;			
    LCD_ShowString(40, 10, 200, 16, 24, "IoTCluB BearPi");
    LCD_ShowString(50, 50, 200, 16, 24, "E53_SC2_Demo");
    LCD_ShowString(10, 100, 200, 16, 16, "NCDP_IP:");
    LCD_ShowString(80, 100, 200, 16, 16, OCEAN_IP);
    LCD_ShowString(10, 150, 200, 16, 16, "NCDP_PORT:");
    LCD_ShowString(100, 150, 200, 16, 16, NON_SECURITY_PORT);
#endif

#if defined(WITH_SOTA)
    extern void nb_sota_demo(void);
    nb_sota_demo();
#endif
//    printf("\r\n=====================================================");
//    printf("\r\nSTEP2: Register Command( NB Notify )");
//    printf("\r\n=====================================================\r\n");
//    los_nb_notify("+NNMI:",strlen("+NNMI:"),nb_cmd_data_ioctl,OC_cmd_match);
    printf("\r\n=====================================================");
    printf("\r\nSTEP3: Report Data to Server( NB Report )");
    printf("\r\n=====================================================\r\n");
    nb_iot_entry();

#else
    printf("Please checkout if open WITH_AT_FRAMEWORK and USE_NB_NEUL95_NO_ATINY\n");
#endif

}



VOID data_collection_task(VOID)
{


	UINT32 uwRet = LOS_OK;
	int X = 0,Y = 0,Z = 0;
	Init_E53_SC2();
	while (1)
	{
			E53_SC2_Read_Data();
			printf("\r\n******************************Temperature      is  %d\r\n", (int)E53_SC2_Data.Temperature);
			printf("\r\n******************************Accel[0]         is  %d\r\n", (int)E53_SC2_Data.Accel[0]);
			printf("\r\n******************************Accel[1]         is  %d\r\n", (int)E53_SC2_Data.Accel[1]);
			printf("\r\n******************************Accel[2]         is  %d\r\n", (int)E53_SC2_Data.Accel[2]);
			sprintf(E53_SC2_send,"%02X%04X%04X%04X",E53_SC2_Data.Temperature,E53_SC2_Data.Accel[0]& 0xFFFF, E53_SC2_Data.Accel[1]&0xFFFF, E53_SC2_Data.Accel[2]&0xFFFF);
			if( X == 0 && Y == 0 && Z == 0)
			{
					X = (int)E53_SC2_Data.Accel[0];
					Y = (int)E53_SC2_Data.Accel[1];
					Z = (int)E53_SC2_Data.Accel[2];
			}
			else
			{
					if(X+100<E53_SC2_Data.Accel[0]||X-100>E53_SC2_Data.Accel[0]||Y+100<E53_SC2_Data.Accel[1]||Y-100>E53_SC2_Data.Accel[1]||Z+100<E53_SC2_Data.Accel[2]||Z-100>E53_SC2_Data.Accel[2])
					{
							HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
							HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);								
							memcpy(E53_SC2_send + 14, "2054696c74", 10);//Tilt
					}
					else
					{
							HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
							HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
							memcpy(E53_SC2_send + 14, "6c6576656c", 10); //Level
							
					}

				}
			uwRet=LOS_TaskDelay(3000);
			if(uwRet !=LOS_OK)
			return;
		}
	}

UINT32 creat_data_collection_task()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;
		UINT32 TskHandle;
    task_init_param.usTaskPrio = 0;
    task_init_param.pcName = "data_collection_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)data_collection_task;
    task_init_param.uwStackSize = 0x1000;

    uwRet = LOS_TaskCreate(&TskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}

VOID data_report_task(VOID)

{
	UINT32 uwRet = LOS_OK;
	UINT32 msglen = 0;
	UINT32 msgid;
	while(1)
		{
			UINT32 msgid =0;
			sprintf(s_report_buf,"%02d", msgid);
			memcpy(s_report_buf + 2, &E53_SC2_send, sizeof(E53_SC2_send));
			msglen = sizeof(E53_SC2_send); 
			los_nb_report((const char*)(&s_report_buf), (msglen+2) / 2);		//发送数据到平台	
			memset(s_report_buf, 0, sizeof(s_report_buf));
			uwRet=LOS_TaskDelay(3000);
		  if(uwRet !=LOS_OK)
		return;
	}
}
UINT32 creat_data_report_task()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;
	  UINT32 TskHandle;

    task_init_param.usTaskPrio = 1;
    task_init_param.pcName = "data_report_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)data_report_task;
    task_init_param.uwStackSize = 0x1000;
    uwRet = LOS_TaskCreate(&TskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}


void nb_iot_entry(void)
{
    UINT32 uwRet = LOS_OK;

    uwRet = creat_data_collection_task();
    if (uwRet != LOS_OK)
    {
        return ;
    }
		
		uwRet = creat_data_report_task();
    if (uwRet != LOS_OK)
    {
        return ;
    }

}
