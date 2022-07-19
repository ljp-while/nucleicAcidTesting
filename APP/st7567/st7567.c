#include "st7567.h"

/*
ascll取模
阴码  列行式  逆向
*/

uint8_t str3[6]={0x7f,0x40,0x40,0x40,0x40,0x00};//disp"L"
uint8_t positive[] = {0x00,0x00,0x00,0x00,0xE0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x0F,0x01,0x01,0x01};  /* + */
uint8_t negative[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00};  /* - */


uint8_t logo[][16] = {
	{0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00},  /* S */
	{0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00},  /* Y */
	{0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00},  /* N */
	{0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00},  /* T */
	{0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20},  /* H */
	{0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00},  /* G */
	{0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00},  /* E */
	{0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00},  /* N */
	{0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00},  /* E */
};

uint8_t invalid[][16] = {
	{0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},/* i */
	{0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20},/* n */
	{0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x03,0x0C,0x30,0x0C,0x03,0x00,0x00},/* v */
	{0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x19,0x24,0x24,0x12,0x3F,0x20,0x00},/* a */
	{0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},/* l */
	{0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},/* i */
	{0x00,0x00,0x80,0x80,0x80,0x90,0xF0,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20},/* d */

};

uint8_t error[][16] = {
	{0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x24,0x24,0x24,0x24,0x17,0x00},/* e */
	{0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00},/* r */
	{0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00},/* r */
	{0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00},/* o */
	{0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00},/* r */
};                                                                           
/****************************************************/
void delay(unsigned int delay) //延迟程序
{
	unsigned int i, j;

	for(i=0; i<delay; i++)
	for(j=0; j<100; j++);
}


void lcd_init(void)
{
	lcd_gpio_init();
	st7567_init();
//	dispstr(0);
	disp_clear();
	lcd_display_logo();
//	lcd_display_16x16_char(4, 60, 'e');
}

void lcd_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量
	RCC_AHB1PeriphClockCmd(LCD_RES_RCC|LCD_CS_RCC|LCD_A0_RCC,ENABLE); //使能端口时钟
	RCC_AHB1PeriphClockCmd(LCD_SCLK_RCC|LCD_SDA_RCC, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //输出模式
	GPIO_InitStructure.GPIO_Pin=LCD_RES_PIN;//管脚设置
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;//速度为100M
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//上拉
	GPIO_Init(LCD_RES_PIN_PORT,&GPIO_InitStructure); //初始化结构体

	GPIO_InitStructure.GPIO_Pin=LCD_CS_PIN;//管脚设置
	GPIO_Init(LCD_CS_PIN_PORT,&GPIO_InitStructure); //初始化结构体

	GPIO_InitStructure.GPIO_Pin=LCD_A0_PIN;//管脚设置
	GPIO_Init(LCD_A0_PIN_PORT,&GPIO_InitStructure); //初始化结构体

	GPIO_InitStructure.GPIO_Pin=LCD_SDA_PIN;//管脚设置
	GPIO_Init(LCD_SDA_PIN_PORT,&GPIO_InitStructure); //初始化结构体

	GPIO_InitStructure.GPIO_Pin=LCD_SCLK_PIN;//管脚设置
	GPIO_Init(LCD_SCLK_PIN_PORT,&GPIO_InitStructure); //初始化结构体	
}

void st7567_init(void)
{
	LCD_RES_PIN_HIGH;
	delay(30);
	LCD_RES_PIN_LOW;
	delay(30);
	LCD_RES_PIN_HIGH;
	delay(100);
	writecommand(0xE2);
	delay(10);
	writecommand(0xA2);//0xa2设置偏压比为1/9
	writecommand(0xA0);//0xA0设置列地址从00H开始
	writecommand(0xC8);//0xc8设置com扫描方向，从COM(n-1)到 COM0
	writecommand(0x23);//vop粗调
	writecommand(0x81);//vop双指令
	writecommand(0x32);//vop微调
	writecommand(0x2F);//0x2f电源状态、输出缓冲开、内部电压调整开，电压调节开关
	writecommand(0xB0);//0xb0设置页列地址
	writecommand(0xAF);//0xaf设置显示LCD开关
	writecommand(0xA6);	//0xa6设置正常显开关
}


/************************************************************************************/
void writecommand(uint8_t cmd) //写指令
{
	int i;
	LCD_CS_PIN_LOW;
	LCD_A0_PIN_LOW;
	for(i=0;i<8;i++)
	{
		if(cmd&0x80)
		LCD_SDA_PIN_HIGH;
		else
		LCD_SDA_PIN_LOW;
		LCD_SCLK_PIN_LOW;
		delay_us(5);
		LCD_SCLK_PIN_HIGH;
		delay_us(5);
		cmd=cmd<<1;
	}
	LCD_SCLK_PIN_LOW;
	LCD_CS_PIN_HIGH;

}
/****************************************************/
void writedata(uint8_t dat) //写数据
{
	int i;
	LCD_CS_PIN_LOW;
	LCD_A0_PIN_HIGH;
	for(i=0;i<8;i++)
	{
		if(dat&0x80)
		LCD_SDA_PIN_HIGH;
		else
		LCD_SDA_PIN_LOW;
		LCD_SCLK_PIN_LOW;
		delay_us(5);
		LCD_SCLK_PIN_HIGH;
		delay_us(5);
		dat=dat<<1;
	}
	LCD_SCLK_PIN_LOW;
	LCD_CS_PIN_HIGH;
}
/****************************************************/
void writedata1(uint8_t dat) //写数据  LCD_CS_PIN_HIGH
{
	int i;
	LCD_CS_PIN_HIGH;
	LCD_A0_PIN_HIGH;
	for(i=0;i<8;i++)
	{
		if(dat&0x80)
		LCD_SDA_PIN_HIGH;
		else
		LCD_SDA_PIN_LOW;
		LCD_SCLK_PIN_LOW;
		delay_us(5);
		LCD_SCLK_PIN_HIGH;
		delay_us(5);
		dat=dat<<1;
	}
	LCD_SCLK_PIN_LOW;
	LCD_CS_PIN_HIGH;
}

/****************************************************/
void disp_clear(void)   //清屏
{
	uint8_t	i,j;
	for(i=0;i<9;i++)
	{
		writecommand(0x00);
		writecommand(0xb0+i);
		writecommand(0x00);
		writecommand(0x10);
		for(j=0;j<132;j++)
		writedata(0x00);
	}
}
/****************************************************/
void disp_full(void)    //全显
{
	uint8_t	i,j;
	for(i=0;i<9;i++)
	{
		writecommand(0x00);
		writecommand(0xb0+i);
		writecommand(0x00);
		writecommand(0x10);
		for(j=0;j<132;j++)
		writedata(0xFF);
	}
}
 /****************************************************/
void disp_col(void)    //横条
{
	uint8_t	i,j;
	for(i=0;i<9;i++)
	{
		writecommand(0x00);
		writecommand(0xb0+i);
		writecommand(0x00);
		writecommand(0x10);
		for(j=0;j<132;j++)
		writedata(0xAA);
	}
}
 /****************************************************/
void disp_row(void)     //竖条
{
	uint8_t	i,j;
	for (i=0;i<9;i++)
	{
		writecommand(0xb0+i);
		writecommand(0x00);
		writecommand(0x10);
		for(j=0;j<128;j++)
		{
			writedata(0xFF);
			writedata(0x00);
		}
	}
}


void lcd_display_16x16_char(uint8_t Row, uint8_t Col, uint8_t data)
{
	uint8_t num = 0;
	uint8_t value = 0;
	uint8_t count = 0;
	uint8_t i = 0;
	uint8_t line = 25;
	uint8_t *p = NULL;
	for(value = Row; value < Row + 3; value++)
	{
		writecommand(0x00);
		writecommand(0xb0+value);
		writecommand(0x00);
		writecommand(0x10);
		for(i=0;i<132;i++)
		writedata(0x00);		
	}

	
	if(data == '+')
	{
		p = positive;
	}
	else if(data == '-')
	{
		p = negative;
	}
	else if(data == 'i')
	{
		for(num = 0; num < 7; num++)
		{
			count = 0;
			for(value = 0; value < 2; value++)
			{
				writecommand(0xb0 + Row + value);
				writecommand(0x10 | (line >> 4));
				writecommand(0x00 | (line & 0x0f));
				for(i = 0; i <8; i ++ )
				{
					writedata(invalid[num][count++]);
				}			
			}
			line += 8;
		}		
		return;
	}
	else if(data == 'e')
	{
		for(num = 0; num < 5; num++)
		{
			count = 0;
			for(value = 0; value < 2; value++)
			{
				writecommand(0xb0 + Row + value);
				writecommand(0x10 | (line >> 4));
				writecommand(0x00 | (line & 0x0f));
				for(i = 0; i <8; i ++ )
				{
					writedata(error[num][count++]);
				}			
			}
			line += 8;
		}		
		return;
	}
	for(value = 0; value < 2; value++)
	{
		writecommand(0xb0 + Row + value);
		writecommand(0x10 | (Col >> 4));
		writecommand(0x00 | (Col & 0x0f));
		for(i = 0; i <8; i ++ )
		{
			writedata(p[count++]);
			delay_ms(1);
		}			
	}



}
 /****************************************************/
void dispstr(uint8_t d)//disp"0",disp"L"
{
	uint8_t i,k;
	for(i=0;i<9;i++)
	{
		writecommand(0xb0+i);
		writecommand(0x00);
		writecommand(0x10);
//		for(j=0;j<22;j++)
		if(d==0)
		{
		//	for(k=0;k<8;k++)
		//	writedata(str2[k]);
		}
		else
		{
			for(k=0;k<6;k++)
			writedata(str3[k]);			
		}

	}
}
 /****************************************************/
void dispgraphiCS(void)   //最大框
{
	uint16_t i;
	for(i=0;i<9;i++)
	{
		writecommand(0xb0+i);
		writecommand(0x10);
		writecommand(0x00);
//		for(j=0;j<128;j++)
//		writedata(graph[i*128+j]);
	}
}
/****************************************************/
void dispblxstp(void)    //不良显示图片
{
	uint16_t i;
	for(i=0;i<9;i++)
	{
		writecommand(0xb0+i);
		writecommand(0x10);
		writecommand(0x00);
//		for(j=0;j<128;j++)
//		writedata1(blxstp[i*128+j]);
	}
}

void lcd_display_logo(void)
{
	uint8_t num = 0;
	uint8_t value = 0;
	uint8_t count = 0;
	uint8_t line = 25;
	uint8_t i = 0;
	for(num = 0; num < 9; num++)
	{
		count = 0;
		for(value = 0; value < 2; value++)
		{
			writecommand(0xb0+value);
			writecommand(0x10 | (line >> 4));
			writecommand(0x00 | (line & 0x0f));
			for(i = 0; i <8; i ++ )
			{
				writedata(logo[num][count++]);
				delay_ms(1);
			}			
		}
		line += 8;
	}	
}

