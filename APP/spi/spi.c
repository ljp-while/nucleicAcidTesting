#include "spi.h"


uint32_t spiTxBuff[SPI_TRANSFER_LENGTH] = {0};
uint32_t spiRxBuff[SPI_TRANSFER_LENGTH] = {0};

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
void SPI1_Init(void)
{
	spi1_gpio_init();
	spi1_mode_init();
	spi1_dma_init();
}

//SPI1�ٶ����ú���
//SPI�ٶ�=fAPB2/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2ʱ��һ��Ϊ84Mhz��
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	SPI1->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI1
} 

//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
{		 			 
 
	while ((SPI1->SR & SPI_I2S_FLAG_TXE) == RESET);//�ȴ���������  
	SPI1->DR = TxData;
	while ((SPI1->SR & SPI_I2S_FLAG_RXNE) == RESET); //�ȴ�������һ��byte  
	return SPI1->DR;
 		    
}


void spi1_gpio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5����Ϊ SPI1
	
	//GPIOFB3,4,5��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//PB3~5���ù������	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��	
}

void spi1_mode_init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//ʹ��SPI1ʱ��
	//����ֻ���SPI�ڳ�ʼ��
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//��λSPI1
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//ֹͣ��λSPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ  SPI_CPOL_High
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����  SPI_CPHA_2Edge
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
	SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_RXNE|SPI_I2S_IT_TXE);
//	SPI1_ReadWriteByte(0xff);//��������		
}

void spi1_dma_init(void)
{
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
	spi1_dma_tx_config();
	spi1_dma_rx_config();
//	SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);	//ʹ��DMA Txͨ��
//	SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,ENABLE);	//ʹ��DMA Rxͨ��
}

void spi1_dma_transfer(uint8_t *rxBuff, uint8_t *txBuff, uint16_t length)
{
	/* �ر�DMAͨ�� */
	DMA_Cmd(DMA2_Stream2, DISABLE);
	DMA_Cmd(DMA2_Stream3, DISABLE);
	
	/* ���ô����ֽ��� */
	DMA_SetCurrDataCounter(DMA2_Stream2, (uint16_t)length);
	DMA_SetCurrDataCounter(DMA2_Stream3, (uint16_t)length);
	
	/* ���ͽ������ݴ洢��ַ���� */
	DMA2_Stream2->CR |= (0 << 10);
	DMA2_Stream3->CR |= (1 << 10);
	
	/* ���ý��պͷ��͵��ڴ��ַ */
	DMA2_Stream2->M0AR = (uint32_t)rxBuff;
	DMA2_Stream3->M0AR = (uint32_t)txBuff;

//	/* ��ȡһ��DR��ʹ����� */
//	SPI_PORT->DR;
//	
//	/* �ȴ�������Ϊ�� */
//	while(SPI_I2S_GetFlagStatus(SPI_PORT, SPI_I2S_FLAG_TXE) == RESET);
	
	/* ��DMAͨ�� */
	DMA_Cmd(DMA2_Stream2, ENABLE);
	DMA_Cmd(DMA2_Stream3, ENABLE);
	
	/* ������� */
	while( DMA_GetFlagStatus(DMA2_Stream3, DMA_FLAG_TCIF3) == RESET);
	while( DMA_GetFlagStatus(DMA2_Stream2, DMA_FLAG_TCIF2) == RESET);
	
	/* �ر�DMAͨ�� */
	DMA_Cmd(DMA2_Stream2, DISABLE);
	DMA_Cmd(DMA2_Stream3, DISABLE);	
	
	/* ���DMA������ɱ�־ */
	DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
	DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3);		
}

static void spi1_dma_tx_config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	/* ��DMA2ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
	/* DMA TX Config */
	DMA_InitStructure.DMA_Channel = DMA_Channel_3;                          //ѡ��DMAͨ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = SPI1_DR_ADDR;                //�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&spiTxBuff;        //�洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                 //�ڴ浽�ⲿ�豸
	DMA_InitStructure.DMA_BufferSize = 32;                                  //�������ݳ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //�����ַ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //�ڴ��ַ����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //�ڴ�����Ϊ8λ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //��������Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           //����ȡѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;                   //���ȼ�Ϊ�е�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  //����ȡFIFOģʽ
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           //����ȡFIFOģʽ���ò����޹ؽ�Ҫ
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;             //���δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;     //���δ���
	DMA_Init(DMA2_Stream3, &DMA_InitStructure);
	
}

static void spi1_dma_rx_config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	/* ��DMA2ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
//	DMA_DeInit(DMA1_Channel2);	//DMA1 ͨ��2 SPI1_RX 
	/* DMA RX config */
	DMA_InitStructure.DMA_Channel = DMA_Channel_3;                          //ѡ��DMAͨ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = SPI1_DR_ADDR;                //�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&spiRxBuff;        //�洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                 //�ڴ浽�ⲿ�豸
	DMA_InitStructure.DMA_BufferSize = 32;                                  //�������ݳ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //�����ַ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //�ڴ��ַ����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //�ڴ�����Ϊ8λ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //��������Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           //����ȡѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;                   //���ȼ�Ϊ�е�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  //����ȡFIFOģʽ
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           //����ȡFIFOģʽ���ò����޹ؽ�Ҫ
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;             //���δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;     //���δ���
	DMA_Init(DMA2_Stream2, &DMA_InitStructure);
	
}
