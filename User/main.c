#include "main.h"

#define IMAGE_WIDTH  320
#define IMAGE_HIGHT  240


#define X_START   60  
#define Y_START   90 
#define X_WIDTH   100
#define Y_HIGHT   40


//#define X_START   60  
//#define Y_START   95 
//#define X_WIDTH   100
//#define Y_HIGHT   40
#define SINGLE_TRANSFER_LENGTH   1024
uint32_t jpeg_buf[SINGLE_TRANSFER_LENGTH];	//JPEG数据缓存buf
uint32_t memory0Addr[SINGLE_TRANSFER_LENGTH] = { 0 };
uint32_t memory1Addr[SINGLE_TRANSFER_LENGTH] = { 0 };
volatile u32 jpeg_data_len=0; 			//buf中的JPEG有效数据长度 
uint8_t rgbDataFlag = 0;
extern uint8_t memory0Flag;
extern uint8_t memory1Flag;
extern uint32_t dcmiDataCount;

//当采集完一帧数据后,调用此函数
void frame_process_function(void)
{
	uint32_t *p;
	jpeg_data_len=SINGLE_TRANSFER_LENGTH-DMA_GetCurrDataCounter(DMA2_Stream1);//得到此次数据传输的长度
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
	
	w25q128_write_no_check(dcmiDataCount*4096, (uint8_t*)p, jpeg_data_len*4);
	printf("dcmiDataCount = %d\r\n",dcmiDataCount);
	rgbDataFlag = 1;
		
} 

void nucleic_acid_testing(void)
{ 
	//自动对焦初始化
	OV5640_RGB565_Mode();	//RGB565模式 
//	OV5640_Light_Mode(0);	//自动模式
//	OV5640_Color_Saturation(3);//色彩饱和度0
//	OV5640_Brightness(4);	//亮度0
//	OV5640_Contrast(3);		//对比度0
//	OV5640_Sharpness(33);	//自动锐度
//	OV5640_Focus_Init();
//	OV5640_Focus_Constant();//启动持续对焦
	My_DCMI_Init();			//DCMI配置
	DCMI_DMA_Init((u32)&memory0Addr,(u32)&memory1Addr,SINGLE_TRANSFER_LENGTH,DMA_MemoryDataSize_Word,DMA_MemoryInc_Enable);//DCMI DMA配置   
 	OV5640_OutSize_Set(4,0,IMAGE_WIDTH,IMAGE_HIGHT); 
	printf("OV5640 OK\r\n"); 
	while(1)
	{ 
		if(uartRxData == 0x54)    //再次分析
		{
			w25q128_erase_sector(0,80);
			DCMI_Start();	//重新开始传输
			uartRxData = 0xff;
		}
		if(uartRxData == 0x55)  //通过串口把数据发出去  方便没有LCD屏幕时调试
		{
			uint16_t line = 0;
			uint16_t count = 0;
			delay_ms(10);
			uint8_t *p = NULL;
			for(line = 0;line < IMAGE_WIDTH; line++)
			{
				EN25QXX_Read((uint8_t*)jpeg_buf,OVERTURN_IMAGE_ADDRESS+line*IMAGE_HIGHT*2,IMAGE_HIGHT*2);  //读取一行
				p = (uint8_t*)jpeg_buf;
				for(count = 0; count < IMAGE_HIGHT * 2; count++)
				{
					USART_SendData(USART1,p[count]);
					while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
				}
			}
//			for(line = 0;line < IMAGE_HIGHT; line++)
//			{
//				EN25QXX_Read((uint8_t*)jpeg_buf,line*IMAGE_WIDTH*2,IMAGE_WIDTH*2);  //读取一行
//				p = (uint8_t*)jpeg_buf;
//				for(count = 0; count < IMAGE_WIDTH * 2; count++)
//				{
//					USART_SendData(USART1,p[count]);
//					while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
//				}
//			}
			uartRxData = 0xff;
		}
		if(rgbDataFlag == 1)  //数据传输完毕
		{
			uint8_t state = 0;
			uint8_t cCharState = 0;
			uint8_t tCharState = 0;
			image_rollovers(IMAGE_WIDTH, IMAGE_HIGHT, (uint8_t*)jpeg_buf);  //向左翻转图像
			rgb565_to_gray(IMAGE_HIGHT, IMAGE_WIDTH, (uint8_t*)jpeg_buf);  //rgb转灰度
			histogram_distribution(IMAGE_HIGHT, IMAGE_WIDTH, X_START, Y_START, X_WIDTH, Y_HIGHT, (uint8_t*)jpeg_buf);  //寻找二值化阈值
			gray_to_bin(IMAGE_HIGHT, IMAGE_WIDTH, X_START, Y_START, X_WIDTH, Y_HIGHT, (uint8_t*)jpeg_buf);   //C T二值化
			state = char_location(IMAGE_HIGHT, IMAGE_WIDTH, X_START, Y_START, X_WIDTH, Y_HIGHT, (uint8_t*)jpeg_buf);  //定位C T位置
			if(state == 1)
			{
				cCharState = c_gray_value_statistics(IMAGE_HIGHT, IMAGE_WIDTH, X_START, Y_START, X_WIDTH, Y_HIGHT, (uint8_t*)jpeg_buf);  //C字符  灰度值分析
				tCharState = t_gray_value_statistics(IMAGE_HIGHT, IMAGE_WIDTH, X_START, Y_START, X_WIDTH, Y_HIGHT, (uint8_t*)jpeg_buf);  //T字符  灰度值分析
//				if(cCharState == 0)
//				{
//					lcd_display_16x16_char(4, 60, 'e');
//					printf("无效\r\n");
//				}
//				else if(cCharState == 1 && tCharState == 0)
//				{
//					lcd_display_16x16_char(4, 60, '-');
//					printf("阴性\r\n");
//				}
//				else if(cCharState == 1 && tCharState == 1)
//				{
//					lcd_display_16x16_char(4, 60, '+');
//					printf("阳性\r\n");
//				}
			}
			else
			{
				printf("error\r\n");
			}
			
			rgbDataFlag = 0;
			dcmiDataCount = 0;
		}
//		display_time_date();	
	}       
} 

int main(void)
{ 
	SysTick_Init(168);  //初始化延时函数
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	USART1_Init(921600);		//初始化串口波特率为921600
	led_init();
	power_key_init();
//	time3_init(60000-1,84-1);//60ms中断一次
	time4_init(1000-1,8400-1); //100ms中断一次
//	while(1);
//	time5_init(10000-1,8400-1); //1s中断一次
	
//	rtc_init();
	lcd_init();            //lcd初始化
	w25q128_init();       //flash芯片初始化
	w25q128_erase_sector(0,80);  
 
	while(ov5640_init())//初始化相机
	{
		printf("OV5640 ERROR\r\n");
		delay_ms(1000);
	}
	nucleic_acid_testing(); 
}
