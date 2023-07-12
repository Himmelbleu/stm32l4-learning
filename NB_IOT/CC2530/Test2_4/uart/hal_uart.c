#include "hal_board_cfg.h"
#include <stdio.h>
#include <string.h>
#include "hal_uart.h"

/*********************************************************************
 * �������ƣ�uartGetkey
 * ��    �ܣ��Ӵ��ڻ�ȡPC���̰���ֵ
 * ��ڲ�������
 * ���ڲ�������
 * �� �� ֵ��c    �õ��ļ�ֵ
 ********************************************************************/
char uartGetkey(void)
{
  char c;
  uint8 status;

  status = U0CSR;
  U0CSR |= UART_ENABLE_RECEIVE; // ���ڽ���ʹ��

  while (!URX0IF );
  c = U0DBUF;
  URX0IF = FALSE;
 
  U0CSR = status;     // �洢״̬

  return c;
}


/*********************************************************************
 * �������ƣ�initUART
 * ��    �ܣ���ʼ������UART
 * ��ڲ�������
 * ���ڲ�������
 * �� �� ֵ����
 ********************************************************************/
void InitUart(void)
{
  U0CSR = 0xC0;
  
  IO_PER_LOC_USART0_AT_PORT0_PIN2345();// ����UART IO�˿�����

  HAL_BOARD_INIT();//�弶��ʼ��

  UART_SETUP(0, 57600, HIGH_STOP);  // ��������
  UTX0IF = 1;
}


/*********************************************************************
 * �������ƣ�send_a_byte
 * ��    �ܣ�����һ���ַ�
 * ��ڲ�����c  ���͵��ַ�
 * ���ڲ�������
 * �� �� ֵ����
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
 * �������ƣ�prints
 * ��    �ܣ�����һ���ַ�
 * ��ڲ�����s         �ַ���ָ��
 * ���ڲ�������
 * �� �� ֵ����
 ********************************************************************/
void prints(char *s)
{
  uint8 i = 0;
 
  while(s[i] != 0)    // �ж��ַ����Ƿ����
  {
    send_a_byte(s[i]);// ����һ�ֽ�
    i++;
  }
}