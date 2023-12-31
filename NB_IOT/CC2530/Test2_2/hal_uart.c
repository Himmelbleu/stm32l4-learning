#include "hal_board_cfg.h"
#include <stdio.h>
#include <string.h>
#include "hal_uart.h"

/*********************************************************************
 * 函数名称：uartGetkey
 * 功    能：从串口获取PC键盘按键值
 * 入口参数：无
 * 出口参数：无
 * 返 回 值：c    得到的键值
 ********************************************************************/
char uartGetkey(void)
{
  char c;
  uint8 status;

  status = U0CSR;
  U0CSR |= UART_ENABLE_RECEIVE; // 串口接收使能

  while (!URX0IF );
  c = U0DBUF;
  URX0IF = FALSE;
 
  U0CSR = status;     // 存储状态

  return c;
}


/*********************************************************************
 * 函数名称：initUART
 * 功    能：初始化串口UART
 * 入口参数：无
 * 出口参数：无
 * 返 回 值：无
 ********************************************************************/
void InitUart(void)
{
  U0CSR = 0xC0;
  U0CSR |=0X40;
  IEN0 |=0X84;

  
  IO_PER_LOC_USART0_AT_PORT0_PIN2345();// 串行UART IO端口配置

  HAL_BOARD_INIT();//板级初始化

  UART_SETUP(0, 57600, HIGH_STOP);  // 串口设置
  UTX0IF = 1;
}


/*********************************************************************
 * 函数名称：send_a_byte
 * 功    能：发送一个字符
 * 入口参数：c  发送的字符
 * 出口参数：无
 * 返 回 值：无
 ********************************************************************/
uint8 send_a_byte(char c)  
{
  if(c == '\n')  
 {
    while(!UTX0IF);
   U0DBUF = 0x0d;   
 }
  
   while (!UTX0IF);
   UTX0IF = 0;
   return (U0DBUF = c);
}


/*********************************************************************
 * 函数名称：prints
 * 功    能：发送一串字符
 * 入口参数：s         字符串指针
 * 出口参数：无
 * 返 回 值：无
 ********************************************************************/
void prints(char *s)
{
  uint8 i = 0;
 
  while(s[i] != 0)    // 判断字符串是否结束
  {
    send_a_byte(s[i]);// 发送一字节
    i++;
  }
}