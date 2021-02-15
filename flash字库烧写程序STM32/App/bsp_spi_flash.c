#include "All.h"

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    // 发送数据超时时间

/* falsh SPI初始化 */
void SPI_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 使能SPI时钟 */
	FLASH_SPI_APBxClock_FUN ( FLASH_SPI_CLK, ENABLE );
	
	/* 使能SPI引脚相关的时钟 */
 	FLASH_SPI_CS_APBxClock_FUN ( FLASH_SPI_CS_CLK|FLASH_SPI_SCK_CLK|
															FLASH_SPI_MISO_PIN|FLASH_SPI_MOSI_PIN, ENABLE );
	
  /* 配置SPI的 CS引脚，普通IO即可 */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(FLASH_SPI_CS_PORT, &GPIO_InitStructure);
	
  /* 配置SPI的 SCK引脚*/
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(FLASH_SPI_SCK_PORT, &GPIO_InitStructure);

  /* 配置SPI的 MISO引脚*/
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;
  GPIO_Init(FLASH_SPI_MISO_PORT, &GPIO_InitStructure);

  /* 配置SPI的 MOSI引脚*/
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
  GPIO_Init(FLASH_SPI_MOSI_PORT, &GPIO_InitStructure);

  /* 停止信号 FLASH: CS引脚高电平*/
  SPI_FLASH_CS_HIGH();

  /* SPI 模式配置 */
  // FLASH芯片 支持SPI模式0及模式3，据此设置CPOL CPHA
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  // 配置SPI的传输模式，配置为全双工模式
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; // 配置SPI的工作模式，配置为主机模式
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // 配置传输数据带宽，设置为8bit模式
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; // CPOL极性 配置SPI空闲时钟的电平，配置为高电平
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; // CPHA相位 配置SPI在时钟的第一个或第二个边缘采集数据，配置成第二个边缘
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; // SPI的片选管脚是硬件控制(SPI模块外部控制)还是软件控制，配置成软件
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;// 预设置SPI的时钟分频，配置成4分频，SPI外设最快为18MHZ
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; // 数据位的发送顺序，即先发送数据的高位(MSB)还是地位(LSB)，设置为高位
  SPI_InitStructure.SPI_CRCPolynomial = 7; // 指定用于CRC计算的多项式
  SPI_Init(FLASH_SPIx , &SPI_InitStructure); // 根据SPI配置参数初始化SPI外设寄存器

  /* 使能 SPI  */
  SPI_Cmd(FLASH_SPIx , ENABLE);  // 使能SPI外设
}

/* 发送数据等待超时回调函数  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
  FLASH_ERROR("SPI 等待超时!errorCode = %d",errorCode);
  return 0;
}
   
/* SPI发送一个字节的数据 */
u8 SPI_FLASH_SendByte(u8 byte)
{
	 SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 等待发送缓冲区为空，TXE事件 */
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_TXE) == RESET)
	{
      if((SPITimeout--) == 0)   // 超时等待时间结束
		  return SPI_TIMEOUT_UserCallback(0);  // 调用超时回调函数
   }

  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
  SPI_I2S_SendData(FLASH_SPIx , byte);

  SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 等待接收缓冲区非空，RXNE事件 */
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((SPITimeout--) == 0) 
		return SPI_TIMEOUT_UserCallback(1);
   }

  /* 读取数据寄存器，获取接收缓冲区数据 */
  return SPI_I2S_ReceiveData(FLASH_SPIx );
}

/* SPI发送两个字节的数据 */
u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
	  SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 等待发送缓冲区为空，TXE事件 */
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) 
		return SPI_TIMEOUT_UserCallback(2);
   }
	
  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
  SPI_I2S_SendData(FLASH_SPIx , HalfWord);

	 SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 等待接收缓冲区非空，RXNE事件 */
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_RXNE) == RESET)
	 {
    if((SPITimeout--) == 0) 
		return SPI_TIMEOUT_UserCallback(3);
   }
  /* 读取数据寄存器，获取接收缓冲区数据 */
  return SPI_I2S_ReceiveData(FLASH_SPIx );
}

/* SPI读取一个字节的数据 */
u8 SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}

/* 写使能命令  */
void SPI_FLASH_WriteEnable(void)
{
  /* 通讯开始：CS低 */
  SPI_FLASH_CS_LOW();

  /* 发送写使能命令*/
  SPI_FLASH_SendByte(W25X_WriteEnable);

  /*通讯结束：CS高 */
  SPI_FLASH_CS_HIGH();
}

/*  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕 */
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;

  /* 选择 FLASH: CS 低 */
  SPI_FLASH_CS_LOW();

  /* 发送 读状态寄存器 命令 */
  SPI_FLASH_SendByte(W25X_ReadStatusReg);

  /* 若FLASH忙碌，则等待 */
  do
  {
		/* 读取FLASH芯片的状态寄存器 */
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);	  // 要读必须先写 
  }
  while ((FLASH_Status & WIP_Flag) == SET);  /* 正在写入标志 最后一位*/

  /* 停止信号  FLASH: CS 高 */
  SPI_FLASH_CS_HIGH();
}
/* 擦除flash扇区 一个扇区4K 12位数据 */
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable();
  SPI_FLASH_WaitForWriteEnd();  // 等待写结束
  /* 擦除扇区 */
  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW();
  /* 发送扇区擦除指令*/
  SPI_FLASH_SendByte(W25X_SectorErase);
  /*发送擦除扇区地址的高位*/
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* 发送擦除扇区地址的中位 */
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* 发送擦除扇区地址的低位 */
  SPI_FLASH_SendByte(SectorAddr & 0xFF);
  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();
  /* 等待擦除完毕*/
  SPI_FLASH_WaitForWriteEnd();
}
/* 擦除flash块一个块64K */
void SPI_FLASH_BlockErase(u32 SectorAddr)
{
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable();
  SPI_FLASH_WaitForWriteEnd();  // 等待写结束
  /* 擦除扇区 */
  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW();
  /* 发送扇区擦除指令*/
  SPI_FLASH_SendByte(W25X_SectorErase);
  /*发送擦除扇区地址的高位*/
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* 发送擦除扇区地址的中位 */
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* 发送擦除扇区地址的低位 */
  SPI_FLASH_SendByte(SectorAddr & 0xFF);
  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();
  /* 等待擦除完毕*/
  SPI_FLASH_WaitForWriteEnd();
}
/* 格式化flash */
void SPI_FLASH_Format(void)
{
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable();

  /* 整块 Erase */
  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW();
  /* 发送整块擦除指令*/
  SPI_FLASH_SendByte(W25X_ChipErase);
  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();

  /* 等待擦除完毕*/
  SPI_FLASH_WaitForWriteEnd();
}

/* 写入一个字节 */
void SPI_FLASH_ByteWrite(u8 pBuffer, u32 WriteAddr)
{
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable();

  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW();
  /* 写页写指令*/
  SPI_FLASH_SendByte(W25X_PageProgram);  // 页编程
	
  /*发送写地址的高位*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*发送写地址的中位*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*发送写地址的低位*/
  SPI_FLASH_SendByte(WriteAddr & 0xFF);
	
    /* 发送当前要写入的字节数据 */
  SPI_FLASH_SendByte(pBuffer);
  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();
  /* 等待写入完毕*/
  SPI_FLASH_WaitForWriteEnd();
}

/** 对flash按页写入数据，写入数据之前需要先擦除扇区 
     pBuffer: 要写入数据的指针
     WriteAddr: 要写入flash的地址
     NumByteToWrite： 写入数据的长度，要小于每页的大小（256）
*/
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable();

  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW();
  /* 写页写指令*/
  SPI_FLASH_SendByte(W25X_PageProgram);  // 页编程
  /*发送写地址的高位*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*发送写地址的中位*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*发送写地址的低位*/
  SPI_FLASH_SendByte(WriteAddr & 0xFF);

	/* 写入数据超过最大写入长度  只写入最大数   多写会覆盖前面的 */
  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
     FLASH_ERROR("SPI_FLASH_PageWrite too large!"); 
  }

  /* 写入数据*/
  while (NumByteToWrite--)
  {
    /* 发送当前要写入的字节数据 */
    SPI_FLASH_SendByte(*pBuffer);
    /* 指向下一字节数据 */
    pBuffer++;
  }

  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();

  /* 等待写入完毕*/
  SPI_FLASH_WaitForWriteEnd();
}

/* flash写入数据  可以不按页写   调用本函数需要先擦除扇区
	pBuffer:要写入数据的指针
	WriteAddr: 写入的 地址
	NumByteToWrite:写入的数据的长度
*/
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
  Addr = WriteAddr % SPI_FLASH_PageSize;
	
	/*差count个数据值，刚好可以对齐到页地址*/
  count = SPI_FLASH_PageSize - Addr;
	/*计算出要写多少整数页*/
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/*mod运算求余，计算出剩余不满一页的字节数*/
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
	
	/* Addr=0,则WriteAddr 刚好按页对齐 aligned  */
  if (Addr == 0)
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    { 
			/*先把整数页都写了*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize); // 写入一页
        WriteAddr +=  SPI_FLASH_PageSize; // 下一页的地址
        pBuffer += SPI_FLASH_PageSize;  // 加的是地址 
      }
			/*若有多余的不满一页的数据，把它写完*/
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
	/* 若地址与 SPI_FLASH_PageSize 不对齐  */
  else 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0)
    {
			/*当前页剩余的count个位置比NumOfSingle小，一页写不完*/
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
				/*先写满当前页*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
				
        WriteAddr +=  count;
        pBuffer += count;
				/*再写剩余的数据*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*地址不对齐多出的count分开处理，不加入这个运算*/
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
			
			/* 先写完count个数据，为的是让下一次要写的地址对齐 */
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
			
			/* 接下来就重复地址对齐的情况 */
      WriteAddr +=  count;
      pBuffer += count;
			/*把整数页都写了*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*若有多余的不满一页的数据，把它写完*/
      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

/*  读取flash的数据 
	pBuffer: 存储读出来的数据的指针
	ReadAddr: 读取flash的地址
	NumByteToRead： 读取的数据的长度
*/
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW();

  /* 发送 读 指令 */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* 发送 读 地址高位 */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* 发送 读 地址中位 */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* 发送 读 地址低位 */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
	
	/* 读取数据 */
  while (NumByteToRead--) /* while there is data to be read */
  {
    /* 读取一个字节*/
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    /* 指向下一个字节缓冲区 */
    pBuffer++;
  }

  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();
}

/* 读取flash的器件ID */
u32 SPI_FLASH_ReadID(void)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* 开始通讯：CS低电平 */
  SPI_FLASH_CS_LOW();

  /* 发送JEDEC指令，读取ID */
  SPI_FLASH_SendByte(W25X_JedecDeviceID);

  /* 读取一个字节数据 */
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

  /* 读取一个字节数据 */
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

  /* 读取一个字节数据 */
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

 /* 停止通讯：CS高电平 */
  SPI_FLASH_CS_HIGH();

  /*把数据组合起来，作为函数的返回值*/
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}

/* 读取flash 的设备ID */
u32 SPI_FLASH_ReadDeviceID(void)
{
  u32 Temp = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "RDID " instruction */
  SPI_FLASH_SendByte(W25X_DeviceID);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  
  /* Read a byte from the FLASH */
  Temp = SPI_FLASH_SendByte(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  return Temp;
}

/* flash快速读顺序读 
   ReadAddr: 要读取的flash的地址
*/
void SPI_FLASH_StartReadSequence(u32 ReadAddr)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
}


/* 进入掉电模式 */
void SPI_Flash_PowerDown(void)   
{ 
  /* 通讯开始：CS低 */
  SPI_FLASH_CS_LOW();

  /* 发送 掉电 命令 */
  SPI_FLASH_SendByte(W25X_PowerDown);

  /*通讯结束：CS高 */
  SPI_FLASH_CS_HIGH();
}  

/*  唤醒 */
void SPI_Flash_WAKEUP(void)   
{
  /*选择 FLASH: CS 低 */
  SPI_FLASH_CS_LOW();

  /* 发送 上电 命令 */
  SPI_FLASH_SendByte(W25X_ReleasePowerDown);

   /* 停止信号 FLASH: CS 高 */
  SPI_FLASH_CS_HIGH();
}   

/* 比较两个数组的数据是否相同 
	返回0，相等
	返回-1，不相等
*/
char Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2)
{
	u32 len1 =0;
	len1 = strlen(pBuffer2);  // 获取固定命令的长度进行判断 ,这个数据小于等于接收到的数据的长度
	/* 如果短的长度的这些数据都相等，则相等*/  
	while(len1--)
	  {
		if(*pBuffer1 != *pBuffer2)
		{
		  return -1;  
		}
		pBuffer1++;
		pBuffer2++;
	  }
	  return 0;
}
