#include <stm32f10x.h> //32头文件
#include <stdio.h> 
#include <string.h>
#include "stdarg.h"		//包含需要的头文件 

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;


#include "USART1.h" //串口1
//#include "USART2.h" //串口1
#include "LED.h" //LED灯
#include "System.h" //位带
#include "SysTick.h" //延时函数
#include "Key.h" //按键
#include "command.h" 
#include "bsp_spi_flash.h"






