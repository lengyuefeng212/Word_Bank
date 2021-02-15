#include "All.h"

void Led_Init()
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LED_Port_RCC,ENABLE);//ϵͳʱ��ʹ��
	
	GPIO_InitStructure.GPIO_Pin = LED_PIN; //����ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�ܽ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ܽŵ��ٶ�
	GPIO_Init(LED_PORT,&GPIO_InitStructure); //��ʼ���˿�
	
	GPIO_SetBits(LED_PORT,LED_PIN);//B11�ܽ���0 
}
