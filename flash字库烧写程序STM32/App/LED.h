#ifndef LED
#define LED

#define LED_Port_RCC RCC_APB2Periph_GPIOG //LEDʱ��
#define LED_PIN GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15 //LED����
#define LED_PORT GPIOG //LED�˿�

#define LED1 PGout(13)  //�̵�
#define LED2 PGout(14) //�Ƶ�
#define LED3 PGout(15) //���
void Led_Init(void);//LED��ʼ��

#endif


