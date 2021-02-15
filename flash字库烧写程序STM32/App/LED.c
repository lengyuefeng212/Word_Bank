#include "All.h"

void Led_Init()
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LED_Port_RCC,ENABLE);//系统时钟使能
	
	GPIO_InitStructure.GPIO_Pin = LED_PIN; //定义管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//管脚输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//管脚的速度
	GPIO_Init(LED_PORT,&GPIO_InitStructure); //初始化端口
	
	GPIO_SetBits(LED_PORT,LED_PIN);//B11管脚置0 
}
