#ifndef _USART2_H
#define _USART2_H


#define USART2_RX_ENABLE     1      //是否开启接收功能  1：开启  0：关闭
#define USART2_TXBUFF_SIZE   1024   //定义串口2 发送缓冲区大小 1024字节

#if  USART2_RX_ENABLE                          //如果使能接收功能
#define USART2_RXBUFF_SIZE   1024              //定义串口2 接收缓冲区大小 1024字节
extern char Usart2_RxCompleted ;               //外部声明，其他文件可以调用该变量
extern unsigned int Usart2_RxCounter;          //外部声明，其他文件可以调用该变量
extern char Usart2_RxBuff[USART2_RXBUFF_SIZE]; //外部声明，其他文件可以调用该变量
#endif


void USART2_Init(ulong bps);     
void u2_printf(char*,...) ;          

#endif

