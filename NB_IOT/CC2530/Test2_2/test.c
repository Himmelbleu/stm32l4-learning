#include <iocc2530.h>
#include <stdio.h>
#include "./hal_uart.h"

#define uchar unsigned char
#define uint unsigned int 
#define uint8 uchar
#define uint16 uint
#define TRUE 1
#define FALSE 0
#define LED1 P1_0
#define LED2 P1_1
uchar temp;

void Delay(uint n)
{
  uint i,t;
  for(i = 0;i<5;i++)
  for(t = 0;t<n;t++);
}

void InitLed(void)
{
  P1DIR |= 0x03; //P1_0
  LED1 = 1; //LED1
  LED2 = 1; //LED2
}

void main(void)
{
  char receive_buf[3];
  uchar counter =0;
  uchar RT_flag=1;

  InitUart(); // baudrate:57600
  InitLed();
 
  prints("input: 11----->led1 on 10----->led1 off 21----->led2 on 20----->led2 off\r\n");
  while(1)
  {
    if(RT_flag == 1)
    {
      if( temp != 0)
      {
        if((temp!='\r')&&(counter<3))
        { 
          receive_buf[counter++] = temp;
        }
        else
        {
          RT_flag = 3;
        }
        if(counter == 3) RT_flag = 3;
        temp = 0;
      }
    }

    if(RT_flag == 3)
    {
      U0CSR &= ~0x40;
      receive_buf[2] = '\0';
      // prints(receive_buf); prints("\r\n");
      if(receive_buf[0] == '1')
      {
        if(receive_buf[1] == '1') { LED1 = 0; prints("led1 on\r\n"); } 
        else if(receive_buf[1] == '0') { LED1 = 1; prints("led1 off\r\n"); }
      }
      else if(receive_buf[0] == '2')
      { 
        if(receive_buf[1] == '1') { LED2 = 0; prints("led2 on\r\n"); } 
        else if(receive_buf[1] == '0') { LED2 = 1; prints("led2 off\r\n"); }
      }
      U0CSR |= 0x40;
      RT_flag = 1;
      counter = 0;
    }
  }
}

#pragma vector = URX0_VECTOR
__interrupt void UART0_ISR(void)
{
  URX0IF = 0;
  temp = U0DBUF;
}