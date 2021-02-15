#include "All.h"

__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
//__IO TestStatus TransferStatus1 = FAILED;

#define BufferSize  4096
/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[] = "感谢您选用野火stm32开发板\r\n";
uint8_t Rx_Buffer[BufferSize];

/********************************************
@功能:main函数
@说明:
@参数: 无
@返回值: 无
*********************************************/
int main()
{
	uchar flag = 0;
	uint i = 0;
	uint32_t writeaddr = 0;
	char status; 
	SysTick_Init(72);//初始化时钟72M
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3
	USART1_Init(115200); //初始化串口1
	Led_Init(); //LED初始化
	KEY_Init();//按键初始化
    
	/* 8M flash SPI 初始化 */
	SPI_FLASH_Init();
	/* 获取flash的 Device ID */
	DeviceID = SPI_FLASH_ReadDeviceID();
	delay_ms(200);
	/* 获取 SPI Flash ID */
	FlashID = SPI_FLASH_ReadID();	
	printf("\r\n FlashID is 0x%X,\
	Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	/* 检验 SPI Flash ID */
	if (FlashID == sFLASH_ID)
	{	
		printf("\r\n 检测到串行flash W25Q64 !\r\n");
		
		/* 擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除 */
		// 这里擦除4K，即一个扇区，擦除的最小单位是扇区
	  // SPI_FLASH_SectorErase(0x00000000);	 // 擦除第一个扇区 	 
		
		/* 将发送缓冲区的数据写到flash中 */
		// 这里写一页，一页的大小为256个字节
//	SPI_FLASH_BufferWrite(Tx_Buffer, 0x00000000, BufferSize);		
//	printf("\r\n 写入的数据为：%s \r\t", Tx_Buffer);
//		
//		/* 将刚刚写入的数据读出来放到接收缓冲区中 */
//		SPI_FLASH_BufferRead(Rx_Buffer, 0x000000, 1536);
//		for(i = 0;i<256;i++)
//		 printf("%x ",Rx_Buffer[i]);
		
		//printf("\r\n 读出的数据为：%s \r\n", Rx_Buffer);
//		
//		/* 检查写入的数据与读出的数据是否相等 */
//		status = Buffercmp(Tx_Buffer, Rx_Buffer);
//		
//		if(status == 0)
//		{ 
//			printf("\r\n 8M串行flash(W25Q64)测试成功!\n\r");
//		}
//		else
//		{        
//			printf("\r\n 8M串行flash(W25Q64)测试失败!\n\r");
//		}
	}
	else  // 器件号不匹配 要么没有这个设备，要么就是设备不匹配
	{ 
		printf("\r\n 获取不到 W25Q64 ID!\n\r");
	}
   // SPI_FLASH_Format();  // 格式化flash 
	while(1)
	 { 
		 // 判断按键按下
		 if(KEY_Scan() == 1)  // 第一个按键按下
		 {
		      flag = !flag; // 反转
		 }
//		 
		if(flag == 0)
		{
			LED1 = 0;
			LED2 = 1;
		 if(Usart1_RxCompleted)
		 {
			 Usart1_RxCompleted = 0; // 完后
		  	SPI_FLASH_BufferWrite(Usart1_RxBuff, writeaddr, Usart1_RxCounter);	
			 writeaddr += Usart1_RxCounter;  // 一次写入256个字节
          //	printf("\r\n 写入的数据为：%s \r\t", Usart1_RxBuff);
			 printf("写入成功\r\n");
			 
			/* 将刚刚写入的数据读出来放到接收缓冲区中 */
		    //SPI_FLASH_BufferRead(Rx_Buffer, 0x000000, Usart1_RxCounter);
	       // printf("\r\n 读出的数据为：%s \r\n", Rx_Buffer);

			
            memset(Usart1_RxBuff,Usart1_RxCounter,0);		
            Usart1_RxCounter = 0;			 
		 }
	  }
	else
	{
		LED2 = 0;
		LED1 = 1;
	        MoniorCM();  // 检测命令
	}
//	
		 if(i%20==0)
		 {
		   LED3 = !LED3;
		   //u1_printf("Hello,luatao!!!\r\n");
		 }
		 i++;
		// delay_ms(50);
	 } 
}
