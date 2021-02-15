#ifndef _USART_H
#define _USART_H

#define USART1_RX_ENABLE     // 开启即使能串口接收
#define USART1_TXBUFF_SIZE   256    //定义串口1 发送缓冲区大小 256字节


#ifdef  USART1_RX_ENABLE                          //如果使能接收功能
#define USART1_RXBUFF_SIZE   256               //定义串口1 接收缓冲区大小 256字节
extern char Usart1_RxCompleted ;               //外部声明，其他文件可以调用该变量
extern unsigned int Usart1_RxCounter;          //外部声明，其他文件可以调用该变量
extern char Usart1_RxBuff[USART1_RXBUFF_SIZE]; //外部声明，其他文件可以调用该变量
#endif

void USART1_Init(ulong bps);     //串口1初始化函数
void u1_printf(char*,...) ;         //串口1 printf函数
int fputc(int ch,FILE *p);

#endif

