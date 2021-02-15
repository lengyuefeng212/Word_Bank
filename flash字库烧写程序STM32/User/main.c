#include "All.h"

__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
//__IO TestStatus TransferStatus1 = FAILED;

#define BufferSize  4096
/* ���ͻ�������ʼ�� */
uint8_t Tx_Buffer[] = "��л��ѡ��Ұ��stm32������\r\n";
uint8_t Rx_Buffer[BufferSize];

/********************************************
@����:main����
@˵��:
@����: ��
@����ֵ: ��
*********************************************/
int main()
{
	uchar flag = 0;
	uint i = 0;
	uint32_t writeaddr = 0;
	char status; 
	SysTick_Init(72);//��ʼ��ʱ��72M
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3
	USART1_Init(115200); //��ʼ������1
	Led_Init(); //LED��ʼ��
	KEY_Init();//������ʼ��
    
	/* 8M flash SPI ��ʼ�� */
	SPI_FLASH_Init();
	/* ��ȡflash�� Device ID */
	DeviceID = SPI_FLASH_ReadDeviceID();
	delay_ms(200);
	/* ��ȡ SPI Flash ID */
	FlashID = SPI_FLASH_ReadID();	
	printf("\r\n FlashID is 0x%X,\
	Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	/* ���� SPI Flash ID */
	if (FlashID == sFLASH_ID)
	{	
		printf("\r\n ��⵽����flash W25Q64 !\r\n");
		
		/* ������Ҫд��� SPI FLASH ������FLASHд��ǰҪ�Ȳ��� */
		// �������4K����һ����������������С��λ������
	  // SPI_FLASH_SectorErase(0x00000000);	 // ������һ������ 	 
		
		/* �����ͻ�����������д��flash�� */
		// ����дһҳ��һҳ�Ĵ�СΪ256���ֽ�
//	SPI_FLASH_BufferWrite(Tx_Buffer, 0x00000000, BufferSize);		
//	printf("\r\n д�������Ϊ��%s \r\t", Tx_Buffer);
//		
//		/* ���ո�д������ݶ������ŵ����ջ������� */
//		SPI_FLASH_BufferRead(Rx_Buffer, 0x000000, 1536);
//		for(i = 0;i<256;i++)
//		 printf("%x ",Rx_Buffer[i]);
		
		//printf("\r\n ����������Ϊ��%s \r\n", Rx_Buffer);
//		
//		/* ���д�������������������Ƿ���� */
//		status = Buffercmp(Tx_Buffer, Rx_Buffer);
//		
//		if(status == 0)
//		{ 
//			printf("\r\n 8M����flash(W25Q64)���Գɹ�!\n\r");
//		}
//		else
//		{        
//			printf("\r\n 8M����flash(W25Q64)����ʧ��!\n\r");
//		}
	}
	else  // �����Ų�ƥ�� Ҫôû������豸��Ҫô�����豸��ƥ��
	{ 
		printf("\r\n ��ȡ���� W25Q64 ID!\n\r");
	}
   // SPI_FLASH_Format();  // ��ʽ��flash 
	while(1)
	 { 
		 // �жϰ�������
		 if(KEY_Scan() == 1)  // ��һ����������
		 {
		      flag = !flag; // ��ת
		 }
//		 
		if(flag == 0)
		{
			LED1 = 0;
			LED2 = 1;
		 if(Usart1_RxCompleted)
		 {
			 Usart1_RxCompleted = 0; // ���
		  	SPI_FLASH_BufferWrite(Usart1_RxBuff, writeaddr, Usart1_RxCounter);	
			 writeaddr += Usart1_RxCounter;  // һ��д��256���ֽ�
          //	printf("\r\n д�������Ϊ��%s \r\t", Usart1_RxBuff);
			 printf("д��ɹ�\r\n");
			 
			/* ���ո�д������ݶ������ŵ����ջ������� */
		    //SPI_FLASH_BufferRead(Rx_Buffer, 0x000000, Usart1_RxCounter);
	       // printf("\r\n ����������Ϊ��%s \r\n", Rx_Buffer);

			
            memset(Usart1_RxBuff,Usart1_RxCounter,0);		
            Usart1_RxCounter = 0;			 
		 }
	  }
	else
	{
		LED2 = 0;
		LED1 = 1;
	        MoniorCM();  // �������
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
