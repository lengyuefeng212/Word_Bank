#ifndef __KEY_H
#define __KEY_H	

#define KEY1_IN_STA  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) //PB4����KEY1,��ȡ��ƽ״̬,�����ж�KEY1�Ƿ���
#define KEY2_IN_STA  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) //PB3����KEY2,��ȡ��ƽ״̬,�����ж�KEY2�Ƿ���

void KEY_Init(void);	   //������ʼ��
uchar KEY_Scan(void);       //����ɨ�躯��	
void KEY_Exti_Init(void);  //�����ⲿ�жϳ�ʼ������

#endif
