#include <ioCC2530.h>
#define uint unsigned int
#define uchar unsigned char
#define LED1 P1_0
#define LED2 P1_1

void Delay(uint);
void Initial(void);
void Delay(uint n)

{
  uint i,t;
  for(i = 0;i<5;i++)
  for(t = 0;t<n;t++);
}

void Initial(void)
{
  P1DIR |= 0x03;
  LED1 = 1; //LED1
  LED2 = 1; //LED2
}

void main(void)
{
  Initial();
  LED1 = 0; //LED1
  LED2 = 0; //LED2
  while(1)
  {
    LED2 = !LED2; //LED2
    Delay(50000);
  }
}
