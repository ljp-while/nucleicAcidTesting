#ifndef _flash_H
#define _flash_H

#include "main.h"

#define W25Q128_SECTION_LENGTH 4096
#define OVERTURN_IMAGE_ADDRESS 40*W25Q128_SECTION_LENGTH
//EN25X系列/Q系列芯片列表	   
//EN25Q80  ID  0XEF13
//EN25Q16  ID  0XEF14
//EN25Q32  ID  0XEF15
//EN25Q64  ID  0XEF16	
//EN25Q128 ID  0XEF17	
#define EN25Q80 	0XEF13 	
#define EN25Q16 	0XEF14
#define EN25Q32 	0XEF15
//#define EN25Q64 	0XEF16
#define EN25Q128	0XEF17
//#define EN25Q128	0X4000
//#define EN25Q64 	0XC816
//#define EN25Q64 	0X1C16		//GD25QXX
//#define EN25Q64 	0X2016		//XM25QHXX
#define EN25Q64 	0XC216		//MXIC		
//#define EN25Q128	0XC817

extern u16 EN25QXX_TYPE;					//定义EN25QXX芯片型号		   

#define	EN25QXX_CS 		PAout(1)  		//EN25QXX的片选信号

#define W25Q128_CS_PORT GPIOA
#define W25Q128_CS_PIN  GPIO_Pin_1

#define W25Q128_CS_LOW  W25Q128_CS_PORT->BSRRH = W25Q128_CS_PIN
#define W25Q128_CS_HIGH W25Q128_CS_PORT->BSRRL = W25Q128_CS_PIN


//#define	EN25QXX_CS 		PBout(14)  		//EN25QXX的片选信号

//#define W25Q128_CS_PORT GPIOB
//#define W25Q128_CS_PIN  GPIO_Pin_14

//#define W25Q128_CS_LOW  W25Q128_CS_PORT->BSRRH = W25Q128_CS_PIN
//#define W25Q128_CS_HIGH W25Q128_CS_PORT->BSRRL = W25Q128_CS_PIN

//指令表
#define EN25X_WriteEnable		0x06 
#define EN25X_WriteDisable		0x04 
#define EN25X_ReadStatusReg		0x05 
#define EN25X_WriteStatusReg	0x01 
#define EN25X_ReadData			0x03 
#define EN25X_FastReadData		0x0B 
#define EN25X_FastReadDual		0x3B 
#define EN25X_PageProgram		0x02 
#define EN25X_BlockErase		0xD8 
#define EN25X_SectorErase		0x20 
#define EN25X_ChipErase			0xC7 
#define EN25X_PowerDown			0xB9 
#define EN25X_ReleasePowerDown	0xAB 
#define EN25X_DeviceID			0xAB 
#define EN25X_ManufactDeviceID	0x90 
#define EN25X_JedecDeviceID		0x9F 

void w25q128_init(void);
u16  EN25QXX_ReadID(void);  	    		//读取FLASH ID
u8	 EN25QXX_ReadSR(void);        		//读取状态寄存器 
void EN25QXX_Write_SR(u8 sr);  			//写状态寄存器
void EN25QXX_Write_Enable(void);  		//写使能 
void EN25QXX_Write_Disable(void);		//写保护
void EN25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void EN25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //读取flash
void EN25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//写入flash
void EN25QXX_Erase_Chip(void);    	  	//整片擦除
void EN25QXX_Erase_Sector(u32 Dst_Addr);	//扇区擦除
void EN25QXX_Wait_Busy(void);           	//等待空闲
void EN25QXX_PowerDown(void);        	//进入掉电模式
void EN25QXX_WAKEUP(void);				//唤醒


void w25q128_write_buff(uint32_t WriteAddr, uint8_t *pBuffer, uint16_t NumByteToWrite);
void w25q128_erase_sector(uint32_t addr, uint16_t val);
static void w25q128_wait_idie(void);
static void w25q128_write_page(uint32_t addr, uint8_t* buff, uint16_t length);
void w25q128_write_no_check(uint32_t WriteAddr, uint8_t* pBuffer, uint32_t NumByteToWrite);


#endif
