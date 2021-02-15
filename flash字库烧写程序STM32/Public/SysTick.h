#ifndef _SysTick_H //SysTick 已经定义过
#define _SysTick_H



void SysTick_Init(uchar SYSCLK);//系统定时器初始化
void delay_us(ulong nus);//us级延时
void delay_ms(uint nms);//ms级延时 最大是1864 = 1.864s

#endif
