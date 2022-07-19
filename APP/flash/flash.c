#include "flash.h"



u16 EN25QXX_TYPE=EN25Q128;	//Ĭ����EN25Q128


//4KbytesΪһ��Sector
//16������Ϊ1��Block
//EN25Q128
//����Ϊ16M�ֽ�,����256��Block,4096��Sector 
													 
//��ʼ��SPI FLASH��IO��
void w25q128_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��	

	GPIO_InitStructure.GPIO_Pin = W25Q128_CS_PIN;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(W25Q128_CS_PORT, &GPIO_InitStructure);//��ʼ��
	W25Q128_CS_HIGH;			//SPI FLASH��ѡ��
	SPI1_Init();		   			//��ʼ��SPI
}

void w25q128_write_buff(uint32_t WriteAddr, uint8_t *pBuffer, uint16_t NumByteToWrite)
{


	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
  
 
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	

		if(i<secremain)//��Ҫ����
		{
			EN25QXX_Erase_Sector(secpos);//�����������

			w25q128_write_no_check(secpos*4096, pBuffer,4096);//д�Ѿ������˵�,ֱ��д������ʣ������. 				    

		}else w25q128_write_no_check(WriteAddr, pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	}
}
void w25q128_write_no_check(uint32_t WriteAddr, uint8_t* pBuffer, uint32_t NumByteToWrite)
{
	uint16_t pageremain = NumByteToWrite % 256;	  
	uint16_t pageCount = NumByteToWrite / 256;
	uint16_t count = 0;
	for(count = 0; count < pageCount; count++)
	{
		w25q128_write_page(WriteAddr,pBuffer,256);
		WriteAddr += 256;
		pBuffer += 256;
	}
	if(pageremain != 0)
	{
		w25q128_write_page(WriteAddr,pBuffer,pageremain);
	}
	
}

static void w25q128_write_page(uint32_t addr, uint8_t* buff, uint16_t length)
{
	;
	uint16_t count = 0;
	
	if(length == 0 )
	{
		return;
	}
	W25Q128_CS_LOW;                            //ʹ������ 
    SPI1_ReadWriteByte(EN25X_WriteEnable);      //����дʹ�� 	
	W25Q128_CS_HIGH; 
	w25q128_wait_idie();
	W25Q128_CS_LOW;                            //ʹ������
    SPI1_ReadWriteByte(EN25X_PageProgram);      //����дҳ����   
    SPI1_ReadWriteByte((uint8_t)((addr)>>16)); //����24bit��ַ    
    SPI1_ReadWriteByte((uint8_t)((addr)>>8));   
    SPI1_ReadWriteByte((uint8_t)addr);
	for(count = 0; count < length; count++)
	{
		 SPI1_ReadWriteByte(buff[count]);
	}
//	spi1_dma_transfer((uint8_t*)spiRxBuff, buff, length);
	W25Q128_CS_HIGH;  
	w25q128_wait_idie();
}

void w25q128_erase_sector(uint32_t addr, uint16_t val)
{
	uint16_t sectorCount = 0;
	                            //ʹ������ 
	for(sectorCount = 0; sectorCount < val; sectorCount++)
	{
		W25Q128_CS_LOW;
		SPI1_ReadWriteByte(EN25X_WriteEnable);      //����дʹ��
		W25Q128_CS_HIGH;
		w25q128_wait_idie(); 
		W25Q128_CS_LOW;
		SPI1_ReadWriteByte(EN25X_SectorErase);      //������������ָ�� 
		SPI1_ReadWriteByte((u8)((addr)>>16));  //����24bit��ַ    
		SPI1_ReadWriteByte((u8)((addr)>>8));   
		SPI1_ReadWriteByte((u8)addr);  
		W25Q128_CS_HIGH;
		w25q128_wait_idie(); 
		addr += W25Q128_SECTION_LENGTH;
	}

}

static void w25q128_wait_idie(void)
{
	uint8_t state = 0xff;
	W25Q128_CS_LOW;
	do
	{
		SPI1_ReadWriteByte(EN25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
		state=SPI1_ReadWriteByte(0xff);  
	}
	while((state&0x01)==0x01);  //
	W25Q128_CS_HIGH; 
	
	
}


//��ȡEN25QXX��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
u8 EN25QXX_ReadSR(void)   
{  
	u8 byte=0;   
	W25Q128_CS_LOW;                            //ʹ������   
	SPI1_ReadWriteByte(EN25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI1_ReadWriteByte(0Xff);             //��ȡһ���ֽ�  
	W25Q128_CS_HIGH;                            //ȡ��Ƭѡ     
	return byte;   
} 

//дEN25QXX״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void EN25QXX_Write_SR(u8 sr)   
{   
	W25Q128_CS_LOW;                            //ʹ������   
	SPI1_ReadWriteByte(EN25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������    
	SPI1_ReadWriteByte(sr);               //д��һ���ֽ�  
	W25Q128_CS_HIGH;                            //ȡ��Ƭѡ     	      
}  

//EN25QXXдʹ��	
//��WEL��λ   
void EN25QXX_Write_Enable(void)   
{
	W25Q128_CS_LOW;                            //ʹ������   
    SPI1_ReadWriteByte(EN25X_WriteEnable);      //����дʹ��  
	W25Q128_CS_HIGH;                            //ȡ��Ƭѡ     	      
} 

//EN25QXXд��ֹ	
//��WEL����  
void EN25QXX_Write_Disable(void)   
{  
	W25Q128_CS_LOW;                            //ʹ������   
    SPI1_ReadWriteByte(EN25X_WriteDisable);     //����д��ָֹ��    
	W25Q128_CS_HIGH;                            //ȡ��Ƭѡ     	      
} 

//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪEN25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪEN25Q16    
//0XEF15,��ʾоƬ�ͺ�ΪEN25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪEN25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪEN25Q128 	  
u16 EN25QXX_ReadID(void)
{
	uint16_t temp = 0;
	W25Q128_CS_LOW;				    
//	spi1_dma_transfer(rxBuff, idBuff, cnt);
	SPI1_ReadWriteByte(0x90);//���Ͷ�ȡID����	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 			   
	temp|=SPI1_ReadWriteByte(0xFF)<<8;  
	temp|=SPI1_ReadWriteByte(0xFF);	 
	W25Q128_CS_HIGH;		
	return temp;
}  

//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void EN25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;   										    
	W25Q128_CS_LOW;                            //ʹ������   
    SPI1_ReadWriteByte(EN25X_ReadData);         //���Ͷ�ȡ����   
    SPI1_ReadWriteByte((u8)((ReadAddr)>>16));  //����24bit��ַ    
    SPI1_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPI1_ReadWriteByte((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI1_ReadWriteByte(0XFF);   //ѭ������  
    }
	W25Q128_CS_HIGH;  		
	w25q128_wait_idie(); 	
}  

//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void EN25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
    EN25QXX_Write_Enable();                  //SET WEL 
	W25Q128_CS_LOW;                            //ʹ������   
    SPI1_ReadWriteByte(EN25X_PageProgram);      //����дҳ����   
    SPI1_ReadWriteByte((u8)((WriteAddr)>>16)); //����24bit��ַ    
    SPI1_ReadWriteByte((u8)((WriteAddr)>>8));   
    SPI1_ReadWriteByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPI1_ReadWriteByte(pBuffer[i]);//ѭ��д��  
	W25Q128_CS_HIGH;                            //ȡ��Ƭѡ 
	EN25QXX_Wait_Busy();					   //�ȴ�д�����
}

//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void EN25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		EN25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	}	    
} 

//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���65535)   
u8 EN25QXX_BUFFER[4096];		 
void EN25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * EN25QXX_BUF;	  
   	EN25QXX_BUF=EN25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		EN25QXX_Read(EN25QXX_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(EN25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			EN25QXX_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				EN25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			EN25QXX_Write_NoCheck(EN25QXX_BUF,secpos*4096,4096);//д����������  

		}else EN25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	}	 
}

//��������оƬ		  
//�ȴ�ʱ�䳬��...
void EN25QXX_Erase_Chip(void)   
{                                   
    EN25QXX_Write_Enable();                  //SET WEL 
    EN25QXX_Wait_Busy();   
  	W25Q128_CS_LOW;                            //ʹ������   
    SPI1_ReadWriteByte(EN25X_ChipErase);        //����Ƭ��������  
	W25Q128_CS_HIGH;                            //ȡ��Ƭѡ     	      
	EN25QXX_Wait_Busy();   				   //�ȴ�оƬ��������
}   

//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ��ɽ��������ʱ��:150ms
void EN25QXX_Erase_Sector(u32 Dst_Addr)   
{  
	//����falsh�������,������   
 	_DEBUG_PRINT_("fe:%x\r\n",Dst_Addr);	  
 	Dst_Addr*=4096;
    EN25QXX_Write_Enable();                  //SET WEL 	 
    EN25QXX_Wait_Busy();   
  	W25Q128_CS_LOW;                            //ʹ������   
    SPI1_ReadWriteByte(EN25X_SectorErase);      //������������ָ�� 
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>16));  //����24bit��ַ    
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPI1_ReadWriteByte((u8)Dst_Addr);  
	W25Q128_CS_HIGH;                            //ȡ��Ƭѡ     	      
    EN25QXX_Wait_Busy();   				   //�ȴ��������
}

//�ȴ�����
void EN25QXX_Wait_Busy(void)   
{   
	while((EN25QXX_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}

//�������ģʽ
void EN25QXX_PowerDown(void)   
{ 
  	W25Q128_CS_LOW;                            //ʹ������   
    SPI1_ReadWriteByte(EN25X_PowerDown);        //���͵�������  
	W25Q128_CS_HIGH;                            //ȡ��Ƭѡ     	      
    delay_us(3);                               //�ȴ�TPD  
}   

//����
void EN25QXX_WAKEUP(void)   
{  
  	W25Q128_CS_LOW;                            //ʹ������   
    SPI1_ReadWriteByte(EN25X_ReleasePowerDown);   //  send EN25X_PowerDown command 0xAB    
	W25Q128_CS_HIGH;                            //ȡ��Ƭѡ     	      
    delay_us(3);                               //�ȴ�TRES1
}   



