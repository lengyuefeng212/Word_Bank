#include "All.h"

/********************************************
@功能:初始化按键端口(PB9)(PB8)
@说明:
@参数: 无
@返回值: 无
*********************************************/
void KEY_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;                       //定义一个设置IO的结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);    //使能PB端口时钟
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;                 //准备设置PB4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   		   //上拉输入，默认状态是高电平
	GPIO_Init(GPIOB, &GPIO_InitStructure);            		   //设置PB4
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;                 //准备设置PB3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   		   //上拉输入，默认状态是高电平
	GPIO_Init(GPIOB, &GPIO_InitStructure);            		   //设置PB3
} 
/********************************************
@功能:按键扫描
@说明:
@参数: 无
@返回值: 无
*********************************************/
uchar KEY_Scan(void)
{	 	  
	if(KEY1_IN_STA==0){                  //判断控制KEY1的IO输入电平，是否是低电平，如果是进入if	
		delay_ms(5);                     //延时5ms，消除抖动的误判
		if(KEY1_IN_STA==0){              //消除抖动的影响后，再次判断控制KEY1的IO输入电平，是否是低电平，如果是进入if，说明按键按下了	
			while(1){                    //等待按键抬起
				if(KEY1_IN_STA==1){      //判断控制KEY1的IO输入电平，是否是高电平，如果是进入if	
					delay_ms(5);         //延时5ms，消除抖动的误判
					if(KEY1_IN_STA==1){  //消除抖动的影响后，再次判断控制KEY1的IO输入电平，是否是高电平，如果是进入if，说明按键抬起了
						return 1;        //返回1，表示KEY1按下了
					}
				}
			}
		}
	}
	//else 
//	if(KEY2_IN_STA==0){            //判断控制KEY2的IO输入电平，是否是低电平，如果是进入if	
//		delay_ms(5);                     //延时5ms，消除抖动的误判
//		if(KEY2_IN_STA==0){              //消除抖动的影响后，再次判断控制KEY2的IO输入电平，是否是低电平，如果是进入if，说明按键按下了	
//			while(1){                    //等待按键抬起
//				if(KEY2_IN_STA==1){      //判断控制KEY2的IO输入电平，是否是高电平，如果是进入if
//					delay_ms(5);         //延时5ms，消除抖动的误判
//					if(KEY2_IN_STA==1){  //消除抖动的影响后，再次判断控制KEY2的IO输入电平，是否是高电平，如果是进入if，说明按键抬起了
//						return 2;        //返回2，表示KEY2按下了
//					}
//				}
//			}
//		}
	//}	
	return 0;                            //如果没有按键按下，返回0
}
///********************************************
//@功能:按键外部中断
//@说明:
//@参数: 无
//@返回值: 无
//*********************************************/  
//void KEY_Exti_Init(void)
//{
//	 NVIC_InitTypeDef   NVIC_InitStructure; //定义一个设置IO的结构体
//	 EXTI_InitTypeDef   EXTI_InitStructure; //定义一个设置中断的结构体 
//   //时钟使能
//  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	          //使能GPIO复用功能时钟
//  	//GPIO中断映射
//	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9);    //配置KEY1-PB9中断线
//  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8);    //配置KEY2-PB8中断线
//    //中断初始化配置 (中断线 中断方式)
//  	EXTI_InitStructure.EXTI_Line = EXTI_Line9|EXTI_Line8;	         //准备设置KEY2-PA1外部9.8中断线
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //中断方式
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //下降沿触发中断
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //使能
//  	EXTI_Init(&EXTI_InitStructure);	 	                          //设置外部中断1线
//    //中断优先级  配置
//	  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			  //准备设置外部中断9
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //抢占优先级3 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		      //子优先级0
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			      //使能外部中断0通道
//  	NVIC_Init(&NVIC_InitStructure);                               //设置外部中断0
//                     
//}
///********************************************
//@功能:中断服务函数
//@说明:
//@参数: 无
//@返回值: 无
//*********************************************/  
//void EXTI9_5_IRQHandler(void)
//{
//  if(EXTI_GetITStatus(EXTI_Line9) == SET) //获取中断9的状态 确定是进入中断
//	{
//	   delay_ms(10);//延迟，消抖
//		if(KEY1_IN_STA == 0)//确定按键按下
//		{
//		  LED1 = !LED1;//LED的状态取反
//		}
//	}
//	EXTI_ClearITPendingBit(EXTI_Line9);//清除中断状态
//}

