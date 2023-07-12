#include <ioCC2530.h>
#include "./hal_uart.h"

#define uint unsigned int
#define uchar unsigned char
#define LED1 P1_0
#define LED2 P1_1

void Delay(uint n)
{
  uint i,t;
  for(i = 0;i<5;i++)
  for(t = 0;t<n;t++);
}

void LedInit(void)
{
  P1DIR |= 0x03;
  LED1 = 1;
  LED2 = 1;
}

void InitIrda(void)
{
  // P1_2 interrupt io initialized.
  P1DIR &= ~(0x01<<2); //P1_2 input mode
  P1INP &= ~(0x01<<2); //P1_2 Pull up
  P1IEN |= (0x01<<2); //P1_2 int enable
  PICTL &= ~(0x01<<1); //P1_2 rasing edge
  // PICTL |= (0x01<<1); //falling edge
  IEN0 |= 0x80;
  IEN2 |= (0x01<<4);
  P1IFG &=~ (0x01<<2);
}

void main(void)
{
  LedInit();
  InitUart();
  InitIrda();
  prints("http://www.up-tech.com\r\n");
  while(1)
  {
    LED2 = !LED2;
    Delay(50000);
    Delay(50000);
  }
}

#pragma vector = P1INT_VECTOR
__interrupt void P1_ISR(void)
{
  prints("irda interrupt!\r\n");
  if((P1IFG&0X04) > 0) //�ж�
  {
    P1IFG &= ~(0x04);
    LED1 = !LED1;
    Delay(1000);
  }
  P1IF = 0; //���жϱ�־
}