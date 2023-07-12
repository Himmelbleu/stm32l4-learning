#include <iocc2530.h>
#include <stdio.h>
#include <string.h>
#include "./hal_uart.h"

#define uchar unsigned char
#define uint unsigned int
#define uint8 uchar
#define uint16 uint
#define TRUE 1
#define FALSE 0
#define LED P1_3 //定义 LED 为 P10 口控制
#define BEEP P1_2

//char arr1[] = "led on";
//char arr2[] = "led off";
//char arr3[] = "beef on";
//char arr4[] = "beef off";
uchar temp;

/****************************
 UP-CUP IOT-4412-II 物联网教学系统实验指导书 96
 延时函数
*****************************/
void Delay(uint n) {
    uint i, t;
    for (i = 0; i < 5; i++)
        for (t = 0; t < n; t++);
}

void InitLed(void) {
    P1DIR |= 0X0F; //P1_0、P1_1 定义为输出
    LED = 1; //LED 灯熄灭
    BEEP = 0;
}

int main(void) {
    char receive_buf[9];
    uchar counter = 0;
    uchar RT_flag = 1;

    InitUart(); //baudrate:57600
    InitLed();

    while (1) {
        if (RT_flag == 1) //接收
        {
            if (temp != 0) {
                if ((temp != '\r') && (counter < 9)) //'\r?回车键为结束字符 最多能接收 3 个字符
                {
                    receive_buf[counter++] = temp;
                } else {
                    RT_flag = 3; //进入 led 设置状态
                }
                if (counter == 8) RT_flag = 3;
                temp = 0;
            }
        }

        if (RT_flag == 3) //led 状态设置
        {
            U0CSR &= ~0x40; //禁止接收
            receive_buf[8] = '\0';
            //prints(receive_buf); prints("\r\n");
            if (receive_buf[0] == '1') {
                if (receive_buf[1] == '1') {
                    LED = 1;
                    BEEP = 0;
                    prints("led on beep off\r\n");
                } else if (receive_buf[1] == '0') {
                    LED = 0;
                    BEEP = 1;
                    prints("led off beep on\r\n");
                }
            } else if (receive_buf[0] == '2') {
                if (receive_buf[1] == '1') {
                    LED = 1;
                    BEEP = 1;
                    prints("led beep on\r\n");
                } else if (receive_buf[1] == '0') {
                    LED = 0;
                    BEEP = 0;
                    prints("led beep off\r\n");
                }
            } else if (strncmp(receive_buf, "led on", 6) == 0) {
                LED = 1;
                prints("led on\r\n");
            } else if (strncmp(receive_buf, "led off", 6) == 0) {
                LED = 0;
                prints("led off\r\n");
            } else if (strncmp(receive_buf, "beep on", 7) == 0) {
                BEEP = 1;
                prints("beep on\r\n");
            } else if (strncmp(receive_buf, "beep off", 8) == 0) {
                BEEP = 0;
                prints("beep off\r\n");
            } else {
                prints(receive_buf);
            }

            U0CSR |= 0x40; //允许接收
            RT_flag = 1; //恢复到接收状态
            counter = 0; //指针归 0
        }
    }
}

/****************************************************************
*函数功能 ：串口接收一个字符
*入口参数 : 无
*返 回 值 ：无
*说 明 ：接收完成后打开接收
****************************************************************/
#pragma vector = URX0_VECTOR

__interrupt void UART0_ISR(void) {
    URX0IF = 0; //清中断标志
    temp = U0DBUF;
}
