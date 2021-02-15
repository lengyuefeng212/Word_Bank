#ifndef _USART_H
#define _USART_H

#define USART1_RX_ENABLE     // ������ʹ�ܴ��ڽ���
#define USART1_TXBUFF_SIZE   256    //���崮��1 ���ͻ�������С 256�ֽ�


#ifdef  USART1_RX_ENABLE                          //���ʹ�ܽ��չ���
#define USART1_RXBUFF_SIZE   256               //���崮��1 ���ջ�������С 256�ֽ�
extern char Usart1_RxCompleted ;               //�ⲿ�����������ļ����Ե��øñ���
extern unsigned int Usart1_RxCounter;          //�ⲿ�����������ļ����Ե��øñ���
extern char Usart1_RxBuff[USART1_RXBUFF_SIZE]; //�ⲿ�����������ļ����Ե��øñ���
#endif

void USART1_Init(ulong bps);     //����1��ʼ������
void u1_printf(char*,...) ;         //����1 printf����
int fputc(int ch,FILE *p);

#endif

