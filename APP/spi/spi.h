#ifndef _spi_H
#define _spi_H

#include "system.h"


#define SPI_PORT             SPI1
#define SPI_TRANSFER_LENGTH  4
#define SPI1_DR_ADDR    (uint32_t)0x4001300C


extern uint32_t spiTxBuff[SPI_TRANSFER_LENGTH];
extern uint32_t spiRxBuff[SPI_TRANSFER_LENGTH];

void SPI1_Init(void);			 //初始化SPI1口
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI1速度   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI1总线读写一个字节

void spi1_gpio_init(void);
void spi1_mode_init(void);
void spi1_dma_init(void);
static void spi1_dma_tx_config(void);
static void spi1_dma_rx_config(void);
void spi1_dma_transfer(uint8_t *rxBuff, uint8_t *txBuff, uint16_t length);
#endif
