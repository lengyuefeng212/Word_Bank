#include "All.h"

/********************************************
@����:��ʼ�������˿�(PB9)(PB8)
@˵��:
@����: ��
@����ֵ: ��
*********************************************/
void KEY_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;                       //����һ������IO�Ľṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);    //ʹ��PB�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;                 //׼������PB4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   		   //�������룬Ĭ��״̬�Ǹߵ�ƽ
	GPIO_Init(GPIOB, &GPIO_InitStructure);            		   //����PB4
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;                 //׼������PB3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   		   //�������룬Ĭ��״̬�Ǹߵ�ƽ
	GPIO_Init(GPIOB, &GPIO_InitStructure);            		   //����PB3
} 
/********************************************
@����:����ɨ��
@˵��:
@����: ��
@����ֵ: ��
*********************************************/
uchar KEY_Scan(void)
{	 	  
	if(KEY1_IN_STA==0){                  //�жϿ���KEY1��IO�����ƽ���Ƿ��ǵ͵�ƽ������ǽ���if	
		delay_ms(5);                     //��ʱ5ms����������������
		if(KEY1_IN_STA==0){              //����������Ӱ����ٴ��жϿ���KEY1��IO�����ƽ���Ƿ��ǵ͵�ƽ������ǽ���if��˵������������	
			while(1){                    //�ȴ�����̧��
				if(KEY1_IN_STA==1){      //�жϿ���KEY1��IO�����ƽ���Ƿ��Ǹߵ�ƽ������ǽ���if	
					delay_ms(5);         //��ʱ5ms����������������
					if(KEY1_IN_STA==1){  //����������Ӱ����ٴ��жϿ���KEY1��IO�����ƽ���Ƿ��Ǹߵ�ƽ������ǽ���if��˵������̧����
						return 1;        //����1����ʾKEY1������
					}
				}
			}
		}
	}
	//else 
//	if(KEY2_IN_STA==0){            //�жϿ���KEY2��IO�����ƽ���Ƿ��ǵ͵�ƽ������ǽ���if	
//		delay_ms(5);                     //��ʱ5ms����������������
//		if(KEY2_IN_STA==0){              //����������Ӱ����ٴ��жϿ���KEY2��IO�����ƽ���Ƿ��ǵ͵�ƽ������ǽ���if��˵������������	
//			while(1){                    //�ȴ�����̧��
//				if(KEY2_IN_STA==1){      //�жϿ���KEY2��IO�����ƽ���Ƿ��Ǹߵ�ƽ������ǽ���if
//					delay_ms(5);         //��ʱ5ms����������������
//					if(KEY2_IN_STA==1){  //����������Ӱ����ٴ��жϿ���KEY2��IO�����ƽ���Ƿ��Ǹߵ�ƽ������ǽ���if��˵������̧����
//						return 2;        //����2����ʾKEY2������
//					}
//				}
//			}
//		}
	//}	
	return 0;                            //���û�а������£�����0
}
///********************************************
//@����:�����ⲿ�ж�
//@˵��:
//@����: ��
//@����ֵ: ��
//*********************************************/  
//void KEY_Exti_Init(void)
//{
//	 NVIC_InitTypeDef   NVIC_InitStructure; //����һ������IO�Ľṹ��
//	 EXTI_InitTypeDef   EXTI_InitStructure; //����һ�������жϵĽṹ�� 
//   //ʱ��ʹ��
//  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	          //ʹ��GPIO���ù���ʱ��
//  	//GPIO�ж�ӳ��
//	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9);    //����KEY1-PB9�ж���
//  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8);    //����KEY2-PB8�ж���
//    //�жϳ�ʼ������ (�ж��� �жϷ�ʽ)
//  	EXTI_InitStructure.EXTI_Line = EXTI_Line9|EXTI_Line8;	         //׼������KEY2-PA1�ⲿ9.8�ж���
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //�жϷ�ʽ
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //�½��ش����ж�
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //ʹ��
//  	EXTI_Init(&EXTI_InitStructure);	 	                          //�����ⲿ�ж�1��
//    //�ж����ȼ�  ����
//	  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			  //׼�������ⲿ�ж�9
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //��ռ���ȼ�3 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		      //�����ȼ�0
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			      //ʹ���ⲿ�ж�0ͨ��
//  	NVIC_Init(&NVIC_InitStructure);                               //�����ⲿ�ж�0
//                     
//}
///********************************************
//@����:�жϷ�����
//@˵��:
//@����: ��
//@����ֵ: ��
//*********************************************/  
//void EXTI9_5_IRQHandler(void)
//{
//  if(EXTI_GetITStatus(EXTI_Line9) == SET) //��ȡ�ж�9��״̬ ȷ���ǽ����ж�
//	{
//	   delay_ms(10);//�ӳ٣�����
//		if(KEY1_IN_STA == 0)//ȷ����������
//		{
//		  LED1 = !LED1;//LED��״̬ȡ��
//		}
//	}
//	EXTI_ClearITPendingBit(EXTI_Line9);//����ж�״̬
//}

