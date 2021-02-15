#ifndef LED
#define LED

#define LED_Port_RCC RCC_APB2Periph_GPIOG //LEDÊ±ÖÓ
#define LED_PIN GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15 //LEDÒý½Å
#define LED_PORT GPIOG //LED¶Ë¿Ú

#define LED1 PGout(13)  //ÂÌµÆ
#define LED2 PGout(14) //»ÆµÆ
#define LED3 PGout(15) //ºìµÆ
void Led_Init(void);//LED³õÊ¼»¯

#endif


