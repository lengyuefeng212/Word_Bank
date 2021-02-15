#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

/* 器件设备号*/
//#define  sFLASH_ID              0xEF3015   //W25X16
//#define  sFLASH_ID              0xEF4015	 //W25Q16
#define  sFLASH_ID              0XEF4017    //W25Q64
//#define  sFLASH_ID              0XEF4018   //W25Q128

/* W25Q64  64个块，每个块16个扇区，每个扇区16页  */
#define SPI_FLASH_PageSize              256      //  每页的字节数
#define SPI_FLASH_PerWritePageSize      256 // flash每一页的大小

/*命令定义-开头*******************************/
#define W25X_WriteEnable		      0x06   // 写使能
#define W25X_WriteDisable		      0x04  // 写禁能
#define W25X_ReadStatusReg		    0x05  // 读状态寄存器1
#define W25X_WriteStatusReg		    0x01  // 读状态寄存器2
#define W25X_ReadData			        0x03  // 读数据
#define W25X_FastReadData		      0x0B  // 快速读数据
#define W25X_FastReadDual		      0x3B  // 
#define W25X_PageProgram		      0x02  // 页编程
#define W25X_BlockErase			      0xD8  // 块擦除 (64K)
#define W25X_SectorErase		      0x20  // 扇区擦除(4K)
#define W25X_ChipErase			      0xC7 // 芯片擦除
#define W25X_PowerDown			      0xB9  // 芯片掉电
#define W25X_ReleasePowerDown	    0xAB  // 释放掉电/器件ID
#define W25X_DeviceID			        0xAB  // 释放掉电/器件ID
#define W25X_ManufactDeviceID   	0x90  // 制造/器件ID
#define W25X_JedecDeviceID		    0x9F  // JEDEC ID

/* WIP(busy)标志，FLASH内部正在写入 */
#define WIP_Flag                  0x01
#define Dummy_Byte                0xFF

/* 引脚定义 */
/*SPI接口定义-开头****************************/
#define      FLASH_SPIx                        SPI1
#define      FLASH_SPI_APBxClock_FUN          RCC_APB2PeriphClockCmd
#define      FLASH_SPI_CLK                     RCC_APB2Periph_SPI1

//CS(NSS)引脚 片选选普通GPIO即可
#define      FLASH_SPI_CS_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      FLASH_SPI_CS_CLK                  RCC_APB2Periph_GPIOA    
#define      FLASH_SPI_CS_PORT                 GPIOA
#define      FLASH_SPI_CS_PIN                  GPIO_Pin_4

//SCK引脚
#define      FLASH_SPI_SCK_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      FLASH_SPI_SCK_CLK                 RCC_APB2Periph_GPIOA   
#define      FLASH_SPI_SCK_PORT                GPIOA   
#define      FLASH_SPI_SCK_PIN                 GPIO_Pin_5
//MISO引脚
#define      FLASH_SPI_MISO_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      FLASH_SPI_MISO_CLK                RCC_APB2Periph_GPIOA    
#define      FLASH_SPI_MISO_PORT               GPIOA 
#define      FLASH_SPI_MISO_PIN                GPIO_Pin_6
//MOSI引脚
#define      FLASH_SPI_MOSI_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      FLASH_SPI_MOSI_CLK                RCC_APB2Periph_GPIOA    
#define      FLASH_SPI_MOSI_PORT               GPIOA 
#define      FLASH_SPI_MOSI_PIN                GPIO_Pin_7

#define  		SPI_FLASH_CS_LOW()     	GPIO_ResetBits( FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN )
#define  		SPI_FLASH_CS_HIGH()    	GPIO_SetBits( FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN )

/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))  // SPI发送数据等待时间

/*flash 工作打印的信息  调用串口1 */
#define FLASH_DEBUG_ON         1

#define FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

void SPI_FLASH_Init(void); // SPI初始化
u32 SPI_FLASH_ReadDeviceID(void);	// 获取设备ID 						  
u32 SPI_FLASH_ReadID(void);  // 获取器件ID										  
void SPI_FLASH_Format(void);		 // 格式化flash
void SPI_FLASH_SectorErase(u32 SectorAddr);	 // 擦除一个扇区	
void SPI_FLASH_BlockErase(u32 SectorAddr); // 擦除一个块										  
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);	 // 写入数据	
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead); // 读出数据	
char Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2);				
u8 SPI_FLASH_SendByte(u8 byte); // 写一个字节数据		
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);		
void SPI_FLASH_ByteWrite(u8 pBuffer, u32 WriteAddr);										  
#endif