#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

/* �����豸��*/
//#define  sFLASH_ID              0xEF3015   //W25X16
//#define  sFLASH_ID              0xEF4015	 //W25Q16
#define  sFLASH_ID              0XEF4017    //W25Q64
//#define  sFLASH_ID              0XEF4018   //W25Q128

/* W25Q64  64���飬ÿ����16��������ÿ������16ҳ  */
#define SPI_FLASH_PageSize              256      //  ÿҳ���ֽ���
#define SPI_FLASH_PerWritePageSize      256 // flashÿһҳ�Ĵ�С

/*�����-��ͷ*******************************/
#define W25X_WriteEnable		      0x06   // дʹ��
#define W25X_WriteDisable		      0x04  // д����
#define W25X_ReadStatusReg		    0x05  // ��״̬�Ĵ���1
#define W25X_WriteStatusReg		    0x01  // ��״̬�Ĵ���2
#define W25X_ReadData			        0x03  // ������
#define W25X_FastReadData		      0x0B  // ���ٶ�����
#define W25X_FastReadDual		      0x3B  // 
#define W25X_PageProgram		      0x02  // ҳ���
#define W25X_BlockErase			      0xD8  // ����� (64K)
#define W25X_SectorErase		      0x20  // ��������(4K)
#define W25X_ChipErase			      0xC7 // оƬ����
#define W25X_PowerDown			      0xB9  // оƬ����
#define W25X_ReleasePowerDown	    0xAB  // �ͷŵ���/����ID
#define W25X_DeviceID			        0xAB  // �ͷŵ���/����ID
#define W25X_ManufactDeviceID   	0x90  // ����/����ID
#define W25X_JedecDeviceID		    0x9F  // JEDEC ID

/* WIP(busy)��־��FLASH�ڲ�����д�� */
#define WIP_Flag                  0x01
#define Dummy_Byte                0xFF

/* ���Ŷ��� */
/*SPI�ӿڶ���-��ͷ****************************/
#define      FLASH_SPIx                        SPI1
#define      FLASH_SPI_APBxClock_FUN          RCC_APB2PeriphClockCmd
#define      FLASH_SPI_CLK                     RCC_APB2Periph_SPI1

//CS(NSS)���� Ƭѡѡ��ͨGPIO����
#define      FLASH_SPI_CS_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      FLASH_SPI_CS_CLK                  RCC_APB2Periph_GPIOA    
#define      FLASH_SPI_CS_PORT                 GPIOA
#define      FLASH_SPI_CS_PIN                  GPIO_Pin_4

//SCK����
#define      FLASH_SPI_SCK_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      FLASH_SPI_SCK_CLK                 RCC_APB2Periph_GPIOA   
#define      FLASH_SPI_SCK_PORT                GPIOA   
#define      FLASH_SPI_SCK_PIN                 GPIO_Pin_5
//MISO����
#define      FLASH_SPI_MISO_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      FLASH_SPI_MISO_CLK                RCC_APB2Periph_GPIOA    
#define      FLASH_SPI_MISO_PORT               GPIOA 
#define      FLASH_SPI_MISO_PIN                GPIO_Pin_6
//MOSI����
#define      FLASH_SPI_MOSI_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      FLASH_SPI_MOSI_CLK                RCC_APB2Periph_GPIOA    
#define      FLASH_SPI_MOSI_PORT               GPIOA 
#define      FLASH_SPI_MOSI_PIN                GPIO_Pin_7

#define  		SPI_FLASH_CS_LOW()     	GPIO_ResetBits( FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN )
#define  		SPI_FLASH_CS_HIGH()    	GPIO_SetBits( FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN )

/*�ȴ���ʱʱ��*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))  // SPI�������ݵȴ�ʱ��

/*flash ������ӡ����Ϣ  ���ô���1 */
#define FLASH_DEBUG_ON         1

#define FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

void SPI_FLASH_Init(void); // SPI��ʼ��
u32 SPI_FLASH_ReadDeviceID(void);	// ��ȡ�豸ID 						  
u32 SPI_FLASH_ReadID(void);  // ��ȡ����ID										  
void SPI_FLASH_Format(void);		 // ��ʽ��flash
void SPI_FLASH_SectorErase(u32 SectorAddr);	 // ����һ������	
void SPI_FLASH_BlockErase(u32 SectorAddr); // ����һ����										  
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);	 // д������	
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead); // ��������	
char Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2);				
u8 SPI_FLASH_SendByte(u8 byte); // дһ���ֽ�����		
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);		
void SPI_FLASH_ByteWrite(u8 pBuffer, u32 WriteAddr);										  
#endif