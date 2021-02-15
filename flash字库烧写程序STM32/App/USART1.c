#include "All.h"

#ifdef  USART1_RX_ENABLE                   //如果使能接收功能
char Usart1_RxCompleted = 0;            //定义一个变量 0：表示接收未完成 1：表示接收完成 
unsigned int Usart1_RxCounter = 0;      //定义一个变量，记录串口1总共接收了多少字节的数据
char Usart1_RxBuff[USART1_RXBUFF_SIZE]; //定义一个数组，用于保存串口1接收到的数据   	
#endif

/********************************************
@功能:串口1初始化
@说明:
@参数: bps 波特率
@返回值: 无
*********************************************/
void USART1_Init(ulong bps)
{  	 	
  GPIO_InitTypeDef GPIO_InitStructure;     //定义一个设置GPIO功能的变量
	USART_InitTypeDef USART_InitStructure;   //定义一个设置串口功能的变量
#ifdef USART1_RX_ENABLE                         //如果使能接收功能
	NVIC_InitTypeDef NVIC_InitStructure;     //如果使能接收功能，定义一个设置中断的变量
#endif

#ifdef USART1_RX_ENABLE                                 //如果使能接收功能
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3
#endif	
      
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);  //使能串口1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //使能GPIOA时钟
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;              //准备设置PA9
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO速率50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	       //复用推挽输出，用于串口1的发送
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //设置PA9
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;             //准备设置PA10 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入，用于串口1的接收
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //设置PA10
	
	USART_InitStructure.USART_BaudRate = bps;                                    //波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8个数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
#ifdef USART1_RX_ENABLE               												   //如果使能接收模式
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //收发模式
#else                                                                              //如果不使能接收模式
	USART_InitStructure.USART_Mode = USART_Mode_Tx ;	                           //只发模式
#endif        
    USART_Init(USART1, &USART_InitStructure);                                      //设置串口1	

#ifdef USART1_RX_ENABLE  	         					        //如果使能接收模式
	USART_ClearFlag(USART1, USART_FLAG_RXNE);	            //清除接收标志位
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);          //开启接收中断
	// 要分开定义 不能用或一个函数定义
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);          //开启接收中断
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;       //设置串口1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//中断通道使能
	NVIC_Init(&NVIC_InitStructure);	                        //设置串口1中断
#endif  

	USART_Cmd(USART1, ENABLE);                              //使能串口1
}

/********************************************
@功能:串口1发送数据
@说明:
@参数: 格式化输出字符串和参数
@返回值: 无
*********************************************/
__align(8) char Usart1_TxBuff[USART1_TXBUFF_SIZE];  

void u1_printf(char* fmt,...) 
{  
	uint i,length;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf(Usart1_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)Usart1_TxBuff);//计算发送的数组中的数据的长度
	while((USART1->SR&0X40)==0); //判断串口1的状态 。可以发送数据
	for(i = 0;i < length;i ++)
	{			
		USART1->DR = Usart1_TxBuff[i]; //赋值
		while((USART1->SR&0X40)==0);	//等待发送完成
	}	
}

/********************************************
@功能:串口1接收中断
@说明: 
@参数: 无
@返回值: 无
*********************************************/
void USART1_IRQHandler(void)   
{          
  
	uint8_t res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){  //如果USART_IT_RXNE标志置位，表示有数据到了，进入if分支
		/* 把对应的位读出来自动清除中断 */
		//if(USART1->DR){                                     //非零值才保存到缓冲区	
			Usart1_RxBuff[Usart1_RxCounter ++] = USART1->DR;     //保存到缓冲区	
	   // }
	}
	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) // 接收到一帧数据
	{
		/* 清除寄存器的方法 */
		USART1->SR; // 先读SR
        USART1->DR; // 再读DR
		Usart1_RxCompleted = 1;  // 一帧数据接收完成
	}
}  

/* 格式化方式发送数据 */
int fputc(int ch,FILE *p)
{
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);//等待发送完成
  return ch;
}



