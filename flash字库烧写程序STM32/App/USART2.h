#ifndef _USART2_H
#define _USART2_H


#define USART2_RX_ENABLE     1      //�Ƿ������չ���  1������  0���ر�
#define USART2_TXBUFF_SIZE   1024   //���崮��2 ���ͻ�������С 1024�ֽ�

#if  USART2_RX_ENABLE                          //���ʹ�ܽ��չ���
#define USART2_RXBUFF_SIZE   1024              //���崮��2 ���ջ�������С 1024�ֽ�
extern char Usart2_RxCompleted ;               //�ⲿ�����������ļ����Ե��øñ���
extern unsigned int Usart2_RxCounter;          //�ⲿ�����������ļ����Ե��øñ���
extern char Usart2_RxBuff[USART2_RXBUFF_SIZE]; //�ⲿ�����������ļ����Ե��øñ���
#endif


void USART2_Init(ulong bps);     
void u2_printf(char*,...) ;          

#endif

