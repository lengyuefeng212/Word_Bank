#include "All.h"

u32 fontype = LISHU; ////字体类型2017-08-13
uint8_t flashbuff[132];  // 从flash中读取的数组存放 一个汉字是132字节

/* 从flash中取出指定的字符的地址
*Char_Rx_Buffer:  接收数据的数据
Char_Rx_Length：接收数据的长度
*/
void getAddr(uint8_t *Char_Rx_Buffer,uint16_t Char_Rx_Length)
{
	uint32_t offset;  // flash字库地址偏移量
    uint8_t  qh, wh;   // 区码和位码
	uint16_t x,i;  // 接收字符的长度
	/* 字模的长宽 字节数 */
	uint8_t    Word_Touch_High, Word_Touch_Wide, Word_Touch_Size,  \
	               Word_Touch_Size_Half, Word_Touch_Wide_Half;
	
	//单个字模指的是汉字，半个单字模指的是ASCALL码
    Word_Touch_High = 33; // 字摸的高共多少位
    Word_Touch_Wide = 32; // 字模的宽共多少位
	 Word_Touch_Wide_Half = Word_Touch_Wide / 2; //半个单字模的位宽// 半个字模的高是一样的
	 /* 字模的字节数 */
    Word_Touch_Size = (Word_Touch_High * Word_Touch_Wide) / 8; //单个字模共有多少字节
    Word_Touch_Size_Half = (Word_Touch_High * Word_Touch_Wide_Half) / 8; //半个字模有多少字节

    for(x = 0; x < Char_Rx_Length;)           //总共有多少个数据
    {
		//ASCII码最高为：0111 1111 
        if(Char_Rx_Buffer[x] & 0x80) //判断是否为汉字
        {
			/*  取出对应的汉字的字节码 */
			/* 区码 = bn1 - 160    (160 = 十六进制 0xA0) 
			   位码 = bn1 - 160
			   bn1,bn2 分别表示汉字机内码的第一个字节和第二个字节 */
			    qh    =    Char_Rx_Buffer[x]    -    0xA0;      //区码2017-08-13汉字库中包含全角字符
               wh    =    Char_Rx_Buffer[x + 1]  -    0xA0; //位码2017-08-13汉字库中包含全角字符
			/*        偏移地址 = (94*(区码-1) + (位码 -1)) * 字节数      */
			/*  fontype 是每个字体存放的起始地址  */
            offset  = (u32)(94 * (qh - 1) + (wh - 1)) * Word_Touch_Size + fontype; //////通过区位码计算，计算汉字存在FLASH中的位置(在FLASH中的存储汉字+ASCII)2017-08-13
            /* 从flash 中读取这个汉字的字节码  */
            SPI_FLASH_BufferRead(flashbuff, offset, Word_Touch_Size); //读取FLASH中的一个汉字的字模
			 x = x + 2; //汉字编码+2（对于汉字的编码：一个汉字用两个码,对于ASCLL的编码：一个ASCLL码用一个码）
			
						
			for(i = 0;i< Word_Touch_Size;i++)
			printf("%x ",flashbuff[i]);
			printf("\r\n%d\r\n\r\n\r\n",Word_Touch_Size);
			
		}else{   // 字母 
		     qh  =  Char_Rx_Buffer[x]; //获取ASCLL码中的编码
			 //offset= (u32)((94*(0xF7-0xB0)+0xFE-0XA1+1)*Word_Touch_Size+(qh-0x20)*Word_Touch_Size_Half);//计算ASCLL码存在FLASH中的位置(在FLASH中的存储汉字+ASCLL)
            offset = fontype + ASCII + (qh - 0x20) * Word_Touch_Size_Half; //ASCII码的起始地址，以实际字库为准。
            SPI_FLASH_BufferRead(flashbuff, offset, Word_Touch_Size_Half); //读取FLASH中的一个英文字符的字模
            x = x + 1; //英文编码+1（对于汉字的编码：一个汉字用两个码,对于ASCLL的编码：一个ASCLL码用一个码）
			
			for(i = 0;i< Word_Touch_Size_Half;i++)
			printf("%x ",flashbuff[i]);
			printf("\r\n%d\r\n\r\n\r\n",Word_Touch_Size_Half);
		}/* 字母*/	 
	}/* 字符的长度 */
}



/*把数据从数据缓冲区拷贝到存储区 
	*dat : 拷贝的数据
	返回值： 返回接收的数据的长度
*/
uint16_t CopyData(uint8_t *dat) {          
	uint16_t len;     // 长度中间变量    
	/* 判断接收到的数据中是否有回车 */
//	if(strstr(Usart1_RxBuff,"\r\n")) 
//	{
//	  printf("找到\r\n");
//	}
	memcpy(dat,Usart1_RxBuff,Usart1_RxCounter); // 拷贝接收到的数据
	len = Usart1_RxCounter;  // 存储接收到的数据的长度
	memset(Usart1_RxBuff,Usart1_RxCounter,0); // 清空接收缓冲区
	Usart1_RxCounter = 0;  // 清零等待下次传输
	
	return len;  // 返回数据的长度	
}
	
/* 将接收的4个字节的数据转化为一个32位的地址*/
uint32_t transform(uint8_t *data)
{
	uint32_t addr;  // 地址
  	//  获取要写入的数据的地址 
	//uint8_t len = strlen(data);
	//printf("aa%d ",len);
    // 判断数据的个数，如果不够4个字节，那么就在前面补0
//	switch(len)
//	{
//		case 0:  // 不用管
//			    break
//		case 1: 
//			break;
//		case 2:
//			break;
//		case 3:
//			break;
//		case 4: // 不用管
//			break;
//	}
	//printf("yy地址,%x,%x,%x,%x\r\n",data[0],data[1],data[2],data[3]);
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

/*接收用户发送的数据（写入和读出的地址）
*p: 要发送的提示信息
*data: 接收的数据 
*/
uint16_t CMD_Request(uint8_t *p,uint8_t* data){
    uint16_t len = 0; // 数据的长度
	
	printf("%s", p); // 打印问的问题
     /* 等待接收数据 */
	while(Usart1_RxCompleted == 0);  // 等待一帧数据接收完
	    Usart1_RxCompleted = 0;
	
	len = CopyData(data); // 获取接收的数据
	//printf("第一次%s\r\n",data);
	return len;  // 返回长度
}

/* flash 命令函数  通过命令控制flash的操作 */
void CMD_Flash(uint8_t *p) {
	uint8_t data[256]={0};           // 临时存储要处理的字符串数据
	uint32_t addr=0;                // flash要操作的地址 
	uint32_t ID = 0;  // 器件ID
	uint16_t len = 0;  // 数据的长度
	uint16_t i = 0;
	if (*p == 'w') {                //判断命令的参数是否为w 写数据
	    CMD_Request("请输入24位地址:\r\n",data); // 获取地址
		//将地址进行处理
		addr = transform(data);
		printf("你输入的地址是0x%x\r\n",addr);
	    len = CMD_Request("请输入要写入的数据:\r\n",data);
		// 输出要写入的数据
		printf("date::%s,len::%d\r\n",data,len);
		//写入数据
		SPI_FLASH_BufferWrite(data,addr,len);
		
	}else if (*p == 'r') {           //判断命令的参数是否为r 读数据
        CMD_Request("请输入24位地址:\r\n",data); // 获取地址
		//将地址进行处理
		//printf("地址原始：%s\r\n",data);
		addr = transform(data);
		printf("你输入的地址是0x%x\r\n",addr);
	    uint8_t len1 = CMD_Request("请输入要读取的数据的长度\r\n",data);
		//printf("%s,%d",data,len1);
		// 获取读取的数据的长度 
		len = 0;
		for(uint8_t i = 0; i < len1; i++)
		{
		   len = len * 10 + (data[i] - 48);  // 将输入的字符串数组转化为整形
			// printf("%d ",len);
		}
        printf("你输入的读出的长度是%d\r\n",len);
		//读出数据
		memset(data,sizeof(data),0);  // 清空缓冲区
		SPI_FLASH_BufferRead(data,addr,len);  
		//发送数据
	    // 	printf("%s\r\n",data);  // 字符串
		for(i = 0; i < len; i++)
		    printf("%x ",data[i]);  // 十六进制
	}else if (*p == 'i'){             //判断命令的参数是否为i  读取器件ID
	    ID = SPI_FLASH_ReadDeviceID();
		printf("flash的ID是%d\r\n", ID);
	}else if (*p == 'f'){             //判断命令的参数是否为f  格式化flash
	    printf("请等待-------\r\n");
		SPI_FLASH_Format();
		printf("格式化完成！\r\n");
	}else if (*p == 's'){             //判断命令的参数是否为s  扇区擦除
	   CMD_Request("请输入24位地址:\r\n",data); // 获取地址
		//将地址进行处理
		addr = transform(data);
	   SPI_FLASH_SectorErase(addr);
	   printf("已经擦除地址为%d的扇区\r\n",addr);
	}else if (*p == 'b'){             //判断命令的参数是否为b  块擦除
	   CMD_Request("请输入24位地址:\r\n",data); // 获取地址
		//将地址进行处理
		addr = transform(data);
	    SPI_FLASH_BlockErase(addr);
		printf("已经擦除地址为%d的块\r\n",addr);
	}else if (*p == 'h'){  // 帮助文档
	   printf("		======================\r\n\
		flash -w    写数据\r\n \
		flash -r     读数据\r\n \
		flash -i     读器件ID\r\n \
		flash -f     格式化\r\n \
		flash -s    扇区擦除\r\n \
		flash -b    块擦除\r\n \
		======================\r\n" );
	}else {                            //没有找到参数
		printf("flash命令格式有问题，示范: flash -w 0x00 \r\n  参数: -w 向flash写数据\r\n -r 读取flash的数据\r\n");
	}
}

/*指令处理函数 处理对应指令的动作*/
void Command() {
	uchar i;
	uint16_t len; // 接收数据的长度
	uint8_t cmd[256];   // 接收数据的缓冲区
	
	/*------------命令列表------------*/
	uchar  cmd1[] = "LED1 on";
	uchar  cmd2[] = "LED1 off";
	uchar  cmd3[] = "LED2 on";
	uchar  cmd4[] = "LED2 off";
	uchar  cmd5[] = "return";
	uchar  cmd6[] = "flash -";
	uchar  cmd7[] = "printf ";    
	uchar  cmd8[] = "\r";     // 回车相当于是重新执行命令操作
	uchar warn[] = "There is no corresponding command\r\n";  //没有对应的命令警告语
	uchar hint[] = " -->\r\n";                                //命令执行成功标识符
	
	
	/*------------命令长度汇总------------*/
	uchar  cmdLen[] = { sizeof(cmd1) - 1, sizeof(cmd2) - 1, sizeof(cmd3) - 1, sizeof(cmd4) - 1, sizeof(cmd5) - 1,
	sizeof(cmd6) - 1 ,sizeof(cmd7)-1,sizeof(cmd8) - 1};
	/*------------命令地址汇总------------*/
	uchar  *cmdPtr[] = {cmd1,cmd2, cmd3, cmd4, cmd5, cmd6 ,cmd7,cmd8};

    len = CopyData(cmd);  // 返回实际接收的字节长度
	/*------------遍历命令列表------------*/
	for (i = 0; i < sizeof(cmdLen); i++) {
		if (len >= cmdLen[i]) {      //首先比较接收到的数据帧长度是否大于命令长度
			if (Buffercmp(cmd, cmdPtr[i] )== 0) {  // 返回0 一样
				break;
			}
		}
	}
	
	/*------------命令对应要执行的动作-----------*/
	switch (i) {
	case 0:
		LED1 = 0;      //打开LED1
			break;
	case 1:
		LED1 = 1;    //关闭LLED1
		break;
	case 2:
		LED2 = 0;     //打开LED2
		break;
	case 3:
		LED2 = 1;    //关闭LED2
		break;
	case 4:
		cmd[len] = '\0';              //在数据帧后面添加换行符
		//USART1_SendMulti(&cmd[cmdLen[4]+1],len-cmdLen[4]);  //返回命令后面的字符串
		break;
	case 5:                         //flash操作
		CMD_Flash(&cmd[cmdLen[5]]); //把命令后面的数据交给flash命令函数处理
		break;
	case 6:
		cmd[len] = '\0';              //在数据帧后面添加结束符
		// LCD_printf(&cmd[cmdLen[6]]);   //在LCD上显示空格后面的字符串
		break;
	case 7:  // 回车
        // 啥也不执行
		break;
	default:
		//printf("%s\r\n",warn);      //发送没有对应的命令警告语
	     printf("%s,,,%d\r\n",cmd,len);  // 打印发送的命令
	     printf("\r\n================\r\n");
	     getAddr(cmd,len);
		return;                       //没有对应的命令先发送警告语，然后结束此函数
	}
	printf("%s\r\n",hint);    // 发送执行命令的标志
}

/*监视是否有指令到来*/
void MoniorCM(void)
{
	static uint32_t writeaddr  = 0;
	if (Usart1_RxCompleted) {  // 如果一帧数据接收完成
		Usart1_RxCompleted = 0;
//	    SPI_FLASH_BufferWrite(Usart1_RxBuff,writeaddr,256);
//		printf("写一页完成\r\n");
//		writeaddr += 256;
		Command();  // 执行命令
	}
}
