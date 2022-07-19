#include "main.h"

uint32_t jpeg_buf[SINGLE_TRANSFER_LENGTH];	//JPEG���ݻ���buf
uint32_t memory0Addr[SINGLE_TRANSFER_LENGTH] = { 0 };
uint32_t memory1Addr[SINGLE_TRANSFER_LENGTH] = { 0 };
volatile u32 image_data_len=0; 			//buf�е�JPEG��Ч���ݳ��� 
uint8_t rgbDataFlag = 0;
uint8_t manualFlag = 0;


//���ɼ���һ֡���ݺ�,���ô˺���
void frame_process_function(void)
{
	uint32_t *p;
	image_data_len=SINGLE_TRANSFER_LENGTH-DMA_GetCurrDataCounter(DMA2_Stream1);//�õ��˴����ݴ���ĳ���
	DCMI_Stop();	
	DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1);
	if(DMA_GetCurrentMemoryTarget(DMA2_Stream1) == DMA_Memory_0)
	{
		p = memory0Addr;
	}
	else
	{
		p = memory1Addr;
	}
	
	w25q128_write_no_check(dcmiDataCount*4096, (uint8_t*)p, image_data_len*4);
	_DEBUG_PRINT_("dcmiDataCount = %d\r\n",dcmiDataCount);
	rgbDataFlag = 1;
		
} 

void nucleic_acid_testing(void)
{ 

	//�Զ��Խ���ʼ��
	OV5640_RGB565_Mode();	//RGB565ģʽ 
//	OV5640_Light_Mode(0);	//�Զ�ģʽ
//	OV5640_Color_Saturation(3);//ɫ�ʱ��Ͷ�0
//	OV5640_Brightness(4);	//����0
//	OV5640_Contrast(3);		//�Աȶ�0
//	OV5640_Sharpness(33);	//�Զ����
//	OV5640_Focus_Init();
//	OV5640_Focus_Constant();//���������Խ�
	My_DCMI_Init();			//DCMI����
	DCMI_DMA_Init((u32)&memory0Addr,(u32)&memory1Addr,SINGLE_TRANSFER_LENGTH,DMA_MemoryDataSize_Word,DMA_MemoryInc_Enable);//DCMI DMA����   
 	OV5640_OutSize_Set(4,0,IMAGE_WIDTH,IMAGE_HIGHT); 
	_DEBUG_PRINT_("OV5640 OK\r\n"); 
	while(1)
	{ 
		if((currentMode.manualMode == 1) || (currentMode.autoMode == 1))    //���ջ�ȡͼ������
		{
			_DEBUG_PRINT_("manualMode = %d autoMode = %d\r\n",currentMode.manualMode,currentMode.autoMode);
			w25q128_erase_sector(0,80);   //����flashоƬ
			DCMI_Start();	//���¿�ʼ����
			if(currentMode.manualMode == 1)
			{
				manualFlag = 1;
			}
			else
			{
				manualFlag = 0;
			}
			currentMode.manualMode = 0;
			currentMode.autoMode = 0;
			
		}
		if(uartRxData == 0x55)  //ͨ�����ڰ����ݷ���ȥ  ����û��LCD��Ļʱ����
		{
			uint16_t line = 0;
			uint16_t count = 0;
			delay_ms(10);
			uint8_t *p = NULL;
			for(line = 0;line < IMAGE_WIDTH; line++)
			{
				EN25QXX_Read((uint8_t*)jpeg_buf,OVERTURN_IMAGE_ADDRESS+line*IMAGE_HIGHT*2,IMAGE_HIGHT*2);  //��ȡһ��
				p = (uint8_t*)jpeg_buf;
				for(count = 0; count < IMAGE_HIGHT * 2; count++)
				{
					USART_SendData(USART1,p[count]);
					while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
				}
			}
//			for(line = 0;line < IMAGE_HIGHT; line++)
//			{
//				EN25QXX_Read((uint8_t*)jpeg_buf,line*IMAGE_WIDTH*2,IMAGE_WIDTH*2);  //��ȡһ��
//				p = (uint8_t*)jpeg_buf;
//				for(count = 0; count < IMAGE_WIDTH * 2; count++)
//				{
//					USART_SendData(USART1,p[count]);
//					while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
//				}
//			}
			uartRxData = 0xff;
		}
		if(rgbDataFlag == 1)  //���ݴ������
		{
			image_data_analysis(IMAGE_HIGHT, IMAGE_WIDTH, X_START, Y_START, X_WIDTH, Y_HIGHT, (uint8_t*)jpeg_buf);
			rgbDataFlag = 0;
			dcmiDataCount = 0;
			if(manualFlag == 1)   //�ֶ�ģʽ ��Ҫһֱ���
			{
				currentMode.manualMode = 1;
			}
		}
//		display_time_date();	
	}       
} 

int main(void)
{ 
	SysTick_Init(168);  //��ʼ����ʱ����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	USART1_Init(921600);		//��ʼ�����ڲ�����Ϊ921600
	led_init();
	power_key_init();
//	time3_init(60000-1,84-1);//60ms�ж�һ��
	time4_init(1000-1,8400-1); //100ms�ж�һ��
	time5_init(10000-1,8400-1); //1s�ж�һ��
//	rtc_init();				//rtc��ʼ��
	lcd_init();            //lcd��ʼ��
	w25q128_init();       //flashоƬ��ʼ��  
	while(ov5640_init())//��ʼ�����
	{
		_DEBUG_PRINT_("OV5640 ERROR\r\n");
		delay_ms(1000);
	}
	nucleic_acid_testing();   
}
