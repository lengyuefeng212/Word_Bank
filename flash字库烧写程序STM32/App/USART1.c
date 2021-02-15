#include "All.h"

#ifdef  USART1_RX_ENABLE                   //���ʹ�ܽ��չ���
char Usart1_RxCompleted = 0;            //����һ������ 0����ʾ����δ��� 1����ʾ������� 
unsigned int Usart1_RxCounter = 0;      //����һ����������¼����1�ܹ������˶����ֽڵ�����
char Usart1_RxBuff[USART1_RXBUFF_SIZE]; //����һ�����飬���ڱ��洮��1���յ�������   	
#endif

/********************************************
@����:����1��ʼ��
@˵��:
@����: bps ������
@����ֵ: ��
*********************************************/
void USART1_Init(ulong bps)
{  	 	
  GPIO_InitTypeDef GPIO_InitStructure;     //����һ������GPIO���ܵı���
	USART_InitTypeDef USART_InitStructure;   //����һ�����ô��ڹ��ܵı���
#ifdef USART1_RX_ENABLE                         //���ʹ�ܽ��չ���
	NVIC_InitTypeDef NVIC_InitStructure;     //���ʹ�ܽ��չ��ܣ�����һ�������жϵı���
#endif

#ifdef USART1_RX_ENABLE                                 //���ʹ�ܽ��չ���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3
#endif	
      
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);  //ʹ�ܴ���1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //ʹ��GPIOAʱ��
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;              //׼������PA9
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO����50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	       //����������������ڴ���1�ķ���
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //����PA9
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;             //׼������PA10 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //�������룬���ڴ���1�Ľ���
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //����PA10
	
	USART_InitStructure.USART_BaudRate = bps;                                    //����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8������λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;                            //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
#ifdef USART1_RX_ENABLE               												   //���ʹ�ܽ���ģʽ
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //�շ�ģʽ
#else                                                                              //�����ʹ�ܽ���ģʽ
	USART_InitStructure.USART_Mode = USART_Mode_Tx ;	                           //ֻ��ģʽ
#endif        
    USART_Init(USART1, &USART_InitStructure);                                      //���ô���1	

#ifdef USART1_RX_ENABLE  	         					        //���ʹ�ܽ���ģʽ
	USART_ClearFlag(USART1, USART_FLAG_RXNE);	            //������ձ�־λ
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);          //���������ж�
	// Ҫ�ֿ����� �����û�һ����������
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);          //���������ж�
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;       //���ô���1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                        //���ô���1�ж�
#endif  

	USART_Cmd(USART1, ENABLE);                              //ʹ�ܴ���1
}

/********************************************
@����:����1��������
@˵��:
@����: ��ʽ������ַ����Ͳ���
@����ֵ: ��
*********************************************/
__align(8) char Usart1_TxBuff[USART1_TXBUFF_SIZE];  

void u1_printf(char* fmt,...) 
{  
	uint i,length;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf(Usart1_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)Usart1_TxBuff);//���㷢�͵������е����ݵĳ���
	while((USART1->SR&0X40)==0); //�жϴ���1��״̬ �����Է�������
	for(i = 0;i < length;i ++)
	{			
		USART1->DR = Usart1_TxBuff[i]; //��ֵ
		while((USART1->SR&0X40)==0);	//�ȴ��������
	}	
}

/********************************************
@����:����1�����ж�
@˵��: 
@����: ��
@����ֵ: ��
*********************************************/
void USART1_IRQHandler(void)   
{          
  
	uint8_t res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){  //���USART_IT_RXNE��־��λ����ʾ�����ݵ��ˣ�����if��֧
		/* �Ѷ�Ӧ��λ�������Զ�����ж� */
		//if(USART1->DR){                                     //����ֵ�ű��浽������	
			Usart1_RxBuff[Usart1_RxCounter ++] = USART1->DR;     //���浽������	
	   // }
	}
	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) // ���յ�һ֡����
	{
		/* ����Ĵ����ķ��� */
		USART1->SR; // �ȶ�SR
        USART1->DR; // �ٶ�DR
		Usart1_RxCompleted = 1;  // һ֡���ݽ������
	}
}  

/* ��ʽ����ʽ�������� */
int fputc(int ch,FILE *p)
{
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);//�ȴ��������
  return ch;
}



