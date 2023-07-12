#include <ioCC2530.h>
#include "./uart/hal_uart.h"
#include "./sht11/sht11.h"
#include <stdio.h>

#define uint unsigned int
#define uchar unsigned char

#define LED1 P1_0
#define LED2 P1_1

void Delay(uint n) {
    uint i, t;
    for (i = 0; i < 5; i++)
        for (t = 0; t < n; t++);
}

void LedInit(void) {
    P1DIR |= 0x03; //���� LED
    LED1 = 1;
    LED2 = 1;
}

int main(void) {
    char temp_buf[10];
    char humi_buf[10];
    float humi, temp;

    LED1 = 0; //LED1
    InitUart(); //bautrate:57600
    Sht11Init();

    while (1) {
        if (GetHumiAndTemp(&humi, &temp) == 0) {
            sprintf(humi_buf, (char *) "%f", humi);
            sprintf(temp_buf, (char *) "%f", temp);

            prints("temp:");
            prints(temp_buf);
            prints(" humi:");
            prints(humi_buf);
            prints("\r\n");

            Sht11Delay(50000);
            Sht11Delay(50000);
            Sht11Delay(50000);

            LED2 = !LED2; //LED2
        }
    }
}
