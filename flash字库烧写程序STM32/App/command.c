#include "All.h"

u32 fontype = LISHU; ////��������2017-08-13
uint8_t flashbuff[132];  // ��flash�ж�ȡ�������� һ��������132�ֽ�

/* ��flash��ȡ��ָ�����ַ��ĵ�ַ
*Char_Rx_Buffer:  �������ݵ�����
Char_Rx_Length���������ݵĳ���
*/
void getAddr(uint8_t *Char_Rx_Buffer,uint16_t Char_Rx_Length)
{
	uint32_t offset;  // flash�ֿ��ַƫ����
    uint8_t  qh, wh;   // �����λ��
	uint16_t x,i;  // �����ַ��ĳ���
	/* ��ģ�ĳ��� �ֽ��� */
	uint8_t    Word_Touch_High, Word_Touch_Wide, Word_Touch_Size,  \
	               Word_Touch_Size_Half, Word_Touch_Wide_Half;
	
	//������ģָ���Ǻ��֣��������ģָ����ASCALL��
    Word_Touch_High = 33; // �����ĸ߹�����λ
    Word_Touch_Wide = 32; // ��ģ�Ŀ�����λ
	 Word_Touch_Wide_Half = Word_Touch_Wide / 2; //�������ģ��λ��// �����ģ�ĸ���һ����
	 /* ��ģ���ֽ��� */
    Word_Touch_Size = (Word_Touch_High * Word_Touch_Wide) / 8; //������ģ���ж����ֽ�
    Word_Touch_Size_Half = (Word_Touch_High * Word_Touch_Wide_Half) / 8; //�����ģ�ж����ֽ�

    for(x = 0; x < Char_Rx_Length;)           //�ܹ��ж��ٸ�����
    {
		//ASCII�����Ϊ��0111 1111 
        if(Char_Rx_Buffer[x] & 0x80) //�ж��Ƿ�Ϊ����
        {
			/*  ȡ����Ӧ�ĺ��ֵ��ֽ��� */
			/* ���� = bn1 - 160    (160 = ʮ������ 0xA0) 
			   λ�� = bn1 - 160
			   bn1,bn2 �ֱ��ʾ���ֻ�����ĵ�һ���ֽں͵ڶ����ֽ� */
			    qh    =    Char_Rx_Buffer[x]    -    0xA0;      //����2017-08-13���ֿ��а���ȫ���ַ�
               wh    =    Char_Rx_Buffer[x + 1]  -    0xA0; //λ��2017-08-13���ֿ��а���ȫ���ַ�
			/*        ƫ�Ƶ�ַ = (94*(����-1) + (λ�� -1)) * �ֽ���      */
			/*  fontype ��ÿ�������ŵ���ʼ��ַ  */
            offset  = (u32)(94 * (qh - 1) + (wh - 1)) * Word_Touch_Size + fontype; //////ͨ����λ����㣬���㺺�ִ���FLASH�е�λ��(��FLASH�еĴ洢����+ASCII)2017-08-13
            /* ��flash �ж�ȡ������ֵ��ֽ���  */
            SPI_FLASH_BufferRead(flashbuff, offset, Word_Touch_Size); //��ȡFLASH�е�һ�����ֵ���ģ
			 x = x + 2; //���ֱ���+2�����ں��ֵı��룺һ��������������,����ASCLL�ı��룺һ��ASCLL����һ���룩
			
						
			for(i = 0;i< Word_Touch_Size;i++)
			printf("%x ",flashbuff[i]);
			printf("\r\n%d\r\n\r\n\r\n",Word_Touch_Size);
			
		}else{   // ��ĸ 
		     qh  =  Char_Rx_Buffer[x]; //��ȡASCLL���еı���
			 //offset= (u32)((94*(0xF7-0xB0)+0xFE-0XA1+1)*Word_Touch_Size+(qh-0x20)*Word_Touch_Size_Half);//����ASCLL�����FLASH�е�λ��(��FLASH�еĴ洢����+ASCLL)
            offset = fontype + ASCII + (qh - 0x20) * Word_Touch_Size_Half; //ASCII�����ʼ��ַ����ʵ���ֿ�Ϊ׼��
            SPI_FLASH_BufferRead(flashbuff, offset, Word_Touch_Size_Half); //��ȡFLASH�е�һ��Ӣ���ַ�����ģ
            x = x + 1; //Ӣ�ı���+1�����ں��ֵı��룺һ��������������,����ASCLL�ı��룺һ��ASCLL����һ���룩
			
			for(i = 0;i< Word_Touch_Size_Half;i++)
			printf("%x ",flashbuff[i]);
			printf("\r\n%d\r\n\r\n\r\n",Word_Touch_Size_Half);
		}/* ��ĸ*/	 
	}/* �ַ��ĳ��� */
}



/*�����ݴ����ݻ������������洢�� 
	*dat : ����������
	����ֵ�� ���ؽ��յ����ݵĳ���
*/
uint16_t CopyData(uint8_t *dat) {          
	uint16_t len;     // �����м����    
	/* �жϽ��յ����������Ƿ��лس� */
//	if(strstr(Usart1_RxBuff,"\r\n")) 
//	{
//	  printf("�ҵ�\r\n");
//	}
	memcpy(dat,Usart1_RxBuff,Usart1_RxCounter); // �������յ�������
	len = Usart1_RxCounter;  // �洢���յ������ݵĳ���
	memset(Usart1_RxBuff,Usart1_RxCounter,0); // ��ս��ջ�����
	Usart1_RxCounter = 0;  // ����ȴ��´δ���
	
	return len;  // �������ݵĳ���	
}
	
/* �����յ�4���ֽڵ�����ת��Ϊһ��32λ�ĵ�ַ*/
uint32_t transform(uint8_t *data)
{
	uint32_t addr;  // ��ַ
  	//  ��ȡҪд������ݵĵ�ַ 
	//uint8_t len = strlen(data);
	//printf("aa%d ",len);
    // �ж����ݵĸ������������4���ֽڣ���ô����ǰ�油0
//	switch(len)
//	{
//		case 0:  // ���ù�
//			    break
//		case 1: 
//			break;
//		case 2:
//			break;
//		case 3:
//			break;
//		case 4: // ���ù�
//			break;
//	}
	//printf("yy��ַ,%x,%x,%x,%x\r\n",data[0],data[1],data[2],data[3]);
	addr=data[0];  
//	printf("%d\r\n",addr);
	addr=(addr<<8)|data[1];
//	printf("%d\r\n",addr);
	addr=(addr<<8)|data[2];
//	printf("%d\r\n",addr);
	addr=(addr<<8)|data[3];  // 
	//printf("%d\r\n",addr);
	return addr;
}

/*�����û����͵����ݣ�д��Ͷ����ĵ�ַ��
*p: Ҫ���͵���ʾ��Ϣ
*data: ���յ����� 
*/
uint16_t CMD_Request(uint8_t *p,uint8_t* data){
    uint16_t len = 0; // ���ݵĳ���
	
	printf("%s", p); // ��ӡ�ʵ�����
     /* �ȴ��������� */
	while(Usart1_RxCompleted == 0);  // �ȴ�һ֡���ݽ�����
	    Usart1_RxCompleted = 0;
	
	len = CopyData(data); // ��ȡ���յ�����
	//printf("��һ��%s\r\n",data);
	return len;  // ���س���
}

/* flash �����  ͨ���������flash�Ĳ��� */
void CMD_Flash(uint8_t *p) {
	uint8_t data[256]={0};           // ��ʱ�洢Ҫ������ַ�������
	uint32_t addr=0;                // flashҪ�����ĵ�ַ 
	uint32_t ID = 0;  // ����ID
	uint16_t len = 0;  // ���ݵĳ���
	uint16_t i = 0;
	if (*p == 'w') {                //�ж�����Ĳ����Ƿ�Ϊw д����
	    CMD_Request("������24λ��ַ:\r\n",data); // ��ȡ��ַ
		//����ַ���д���
		addr = transform(data);
		printf("������ĵ�ַ��0x%x\r\n",addr);
	    len = CMD_Request("������Ҫд�������:\r\n",data);
		// ���Ҫд�������
		printf("date::%s,len::%d\r\n",data,len);
		//д������
		SPI_FLASH_BufferWrite(data,addr,len);
		
	}else if (*p == 'r') {           //�ж�����Ĳ����Ƿ�Ϊr ������
        CMD_Request("������24λ��ַ:\r\n",data); // ��ȡ��ַ
		//����ַ���д���
		//printf("��ַԭʼ��%s\r\n",data);
		addr = transform(data);
		printf("������ĵ�ַ��0x%x\r\n",addr);
	    uint8_t len1 = CMD_Request("������Ҫ��ȡ�����ݵĳ���\r\n",data);
		//printf("%s,%d",data,len1);
		// ��ȡ��ȡ�����ݵĳ��� 
		len = 0;
		for(uint8_t i = 0; i < len1; i++)
		{
		   len = len * 10 + (data[i] - 48);  // ��������ַ�������ת��Ϊ����
			// printf("%d ",len);
		}
        printf("������Ķ����ĳ�����%d\r\n",len);
		//��������
		memset(data,sizeof(data),0);  // ��ջ�����
		SPI_FLASH_BufferRead(data,addr,len);  
		//��������
	    // 	printf("%s\r\n",data);  // �ַ���
		for(i = 0; i < len; i++)
		    printf("%x ",data[i]);  // ʮ������
	}else if (*p == 'i'){             //�ж�����Ĳ����Ƿ�Ϊi  ��ȡ����ID
	    ID = SPI_FLASH_ReadDeviceID();
		printf("flash��ID��%d\r\n", ID);
	}else if (*p == 'f'){             //�ж�����Ĳ����Ƿ�Ϊf  ��ʽ��flash
	    printf("��ȴ�-------\r\n");
		SPI_FLASH_Format();
		printf("��ʽ����ɣ�\r\n");
	}else if (*p == 's'){             //�ж�����Ĳ����Ƿ�Ϊs  ��������
	   CMD_Request("������24λ��ַ:\r\n",data); // ��ȡ��ַ
		//����ַ���д���
		addr = transform(data);
	   SPI_FLASH_SectorErase(addr);
	   printf("�Ѿ�������ַΪ%d������\r\n",addr);
	}else if (*p == 'b'){             //�ж�����Ĳ����Ƿ�Ϊb  �����
	   CMD_Request("������24λ��ַ:\r\n",data); // ��ȡ��ַ
		//����ַ���д���
		addr = transform(data);
	    SPI_FLASH_BlockErase(addr);
		printf("�Ѿ�������ַΪ%d�Ŀ�\r\n",addr);
	}else if (*p == 'h'){  // �����ĵ�
	   printf("		======================\r\n\
		flash -w    д����\r\n \
		flash -r     ������\r\n \
		flash -i     ������ID\r\n \
		flash -f     ��ʽ��\r\n \
		flash -s    ��������\r\n \
		flash -b    �����\r\n \
		======================\r\n" );
	}else {                            //û���ҵ�����
		printf("flash�����ʽ�����⣬ʾ��: flash -w 0x00 \r\n  ����: -w ��flashд����\r\n -r ��ȡflash������\r\n");
	}
}

/*ָ����� �����Ӧָ��Ķ���*/
void Command() {
	uchar i;
	uint16_t len; // �������ݵĳ���
	uint8_t cmd[256];   // �������ݵĻ�����
	
	/*------------�����б�------------*/
	uchar  cmd1[] = "LED1 on";
	uchar  cmd2[] = "LED1 off";
	uchar  cmd3[] = "LED2 on";
	uchar  cmd4[] = "LED2 off";
	uchar  cmd5[] = "return";
	uchar  cmd6[] = "flash -";
	uchar  cmd7[] = "printf ";    
	uchar  cmd8[] = "\r";     // �س��൱��������ִ���������
	uchar warn[] = "There is no corresponding command\r\n";  //û�ж�Ӧ���������
	uchar hint[] = " -->\r\n";                                //����ִ�гɹ���ʶ��
	
	
	/*------------����Ȼ���------------*/
	uchar  cmdLen[] = { sizeof(cmd1) - 1, sizeof(cmd2) - 1, sizeof(cmd3) - 1, sizeof(cmd4) - 1, sizeof(cmd5) - 1,
	sizeof(cmd6) - 1 ,sizeof(cmd7)-1,sizeof(cmd8) - 1};
	/*------------�����ַ����------------*/
	uchar  *cmdPtr[] = {cmd1,cmd2, cmd3, cmd4, cmd5, cmd6 ,cmd7,cmd8};

    len = CopyData(cmd);  // ����ʵ�ʽ��յ��ֽڳ���
	/*------------���������б�------------*/
	for (i = 0; i < sizeof(cmdLen); i++) {
		if (len >= cmdLen[i]) {      //���ȱȽϽ��յ�������֡�����Ƿ���������
			if (Buffercmp(cmd, cmdPtr[i] )== 0) {  // ����0 һ��
				break;
			}
		}
	}
	
	/*------------�����ӦҪִ�еĶ���-----------*/
	switch (i) {
	case 0:
		LED1 = 0;      //��LED1
			break;
	case 1:
		LED1 = 1;    //�ر�LLED1
		break;
	case 2:
		LED2 = 0;     //��LED2
		break;
	case 3:
		LED2 = 1;    //�ر�LED2
		break;
	case 4:
		cmd[len] = '\0';              //������֡������ӻ��з�
		//USART1_SendMulti(&cmd[cmdLen[4]+1],len-cmdLen[4]);  //�������������ַ���
		break;
	case 5:                         //flash����
		CMD_Flash(&cmd[cmdLen[5]]); //�������������ݽ���flash���������
		break;
	case 6:
		cmd[len] = '\0';              //������֡������ӽ�����
		// LCD_printf(&cmd[cmdLen[6]]);   //��LCD����ʾ�ո������ַ���
		break;
	case 7:  // �س�
        // ɶҲ��ִ��
		break;
	default:
		//printf("%s\r\n",warn);      //����û�ж�Ӧ���������
	     printf("%s,,,%d\r\n",cmd,len);  // ��ӡ���͵�����
	     printf("\r\n================\r\n");
	     getAddr(cmd,len);
		return;                       //û�ж�Ӧ�������ȷ��;����Ȼ������˺���
	}
	printf("%s\r\n",hint);    // ����ִ������ı�־
}

/*�����Ƿ���ָ���*/
void MoniorCM(void)
{
	static uint32_t writeaddr  = 0;
	if (Usart1_RxCompleted) {  // ���һ֡���ݽ������
		Usart1_RxCompleted = 0;
//	    SPI_FLASH_BufferWrite(Usart1_RxBuff,writeaddr,256);
//		printf("дһҳ���\r\n");
//		writeaddr += 256;
		Command();  // ִ������
	}
}
