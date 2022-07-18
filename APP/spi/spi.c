#include "spi.h"


uint32_t spiTxBuff[SPI_TRANSFER_LENGTH] = {0};
uint32_t spiRxBuff[SPI_TRANSFER_LENGTH] = {0};

//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI1的初始化
void SPI1_Init(void)
{
	spi1_gpio_init();
	spi1_mode_init();
	spi1_dma_init();
}

//SPI1速度设置函数
//SPI速度=fAPB2/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2时钟一般为84Mhz：
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	SPI1->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); //使能SPI1
} 

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{		 			 
 
	while ((SPI1->SR & SPI_I2S_FLAG_TXE) == RESET);//等待发送区空  
	SPI1->DR = TxData;
	while ((SPI1->SR & SPI_I2S_FLAG_RXNE) == RESET); //等待接收完一个byte  
	return SPI1->DR;
 		    
}


void spi1_gpio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5复用为 SPI1
	
	//GPIOFB3,4,5初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//PB3~5复用功能输出	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化	
}

void spi1_mode_init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//使能SPI1时钟
	//这里只针对SPI口初始化
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//复位SPI1
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//停止复位SPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平  SPI_CPOL_High
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样  SPI_CPHA_2Edge
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_RXNE|SPI_I2S_IT_TXE);
//	SPI1_ReadWriteByte(0xff);//启动传输		
}

void spi1_dma_init(void)
{
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
	spi1_dma_tx_config();
	spi1_dma_rx_config();
//	SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);	//使能DMA Tx通道
//	SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,ENABLE);	//使能DMA Rx通道
}

void spi1_dma_transfer(uint8_t *rxBuff, uint8_t *txBuff, uint16_t length)
{
	/* 关闭DMA通道 */
	DMA_Cmd(DMA2_Stream2, DISABLE);
	DMA_Cmd(DMA2_Stream3, DISABLE);
	
	/* 设置传输字节数 */
	DMA_SetCurrDataCounter(DMA2_Stream2, (uint16_t)length);
	DMA_SetCurrDataCounter(DMA2_Stream3, (uint16_t)length);
	
	/* 发送接收数据存储地址自增 */
	DMA2_Stream2->CR |= (0 << 10);
	DMA2_Stream3->CR |= (1 << 10);
	
	/* 设置接收和发送的内存地址 */
	DMA2_Stream2->M0AR = (uint32_t)rxBuff;
	DMA2_Stream3->M0AR = (uint32_t)txBuff;

//	/* 读取一次DR，使其清空 */
//	SPI_PORT->DR;
//	
//	/* 等待发送区为空 */
//	while(SPI_I2S_GetFlagStatus(SPI_PORT, SPI_I2S_FLAG_TXE) == RESET);
	
	/* 打开DMA通道 */
	DMA_Cmd(DMA2_Stream2, ENABLE);
	DMA_Cmd(DMA2_Stream3, ENABLE);
	
	/* 传输完成 */
	while( DMA_GetFlagStatus(DMA2_Stream3, DMA_FLAG_TCIF3) == RESET);
	while( DMA_GetFlagStatus(DMA2_Stream2, DMA_FLAG_TCIF2) == RESET);
	
	/* 关闭DMA通道 */
	DMA_Cmd(DMA2_Stream2, DISABLE);
	DMA_Cmd(DMA2_Stream3, DISABLE);	
	
	/* 清除DMA传输完成标志 */
	DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
	DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3);		
}

static void spi1_dma_tx_config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	/* 打开DMA2时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
	/* DMA TX Config */
	DMA_InitStructure.DMA_Channel = DMA_Channel_3;                          //选择DMA通道
	DMA_InitStructure.DMA_PeripheralBaseAddr = SPI1_DR_ADDR;                //外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&spiTxBuff;        //存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                 //内存到外部设备
	DMA_InitStructure.DMA_BufferSize = 32;                                  //传输数据长度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //外设地址不自增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //内存地址自增
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //内存数据为8位
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           //不采取循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;                   //优先级为中等
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  //不采取FIFO模式
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           //不采取FIFO模式，该参数无关紧要
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;             //单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;     //单次传输
	DMA_Init(DMA2_Stream3, &DMA_InitStructure);
	
}

static void spi1_dma_rx_config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	/* 打开DMA2时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
//	DMA_DeInit(DMA1_Channel2);	//DMA1 通道2 SPI1_RX 
	/* DMA RX config */
	DMA_InitStructure.DMA_Channel = DMA_Channel_3;                          //选择DMA通道
	DMA_InitStructure.DMA_PeripheralBaseAddr = SPI1_DR_ADDR;                //外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&spiRxBuff;        //存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                 //内存到外部设备
	DMA_InitStructure.DMA_BufferSize = 32;                                  //传输数据长度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //外设地址不自增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //内存地址自增
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //内存数据为8位
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           //不采取循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;                   //优先级为中等
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  //不采取FIFO模式
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           //不采取FIFO模式，该参数无关紧要
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;             //单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;     //单次传输
	DMA_Init(DMA2_Stream2, &DMA_InitStructure);
	
}
