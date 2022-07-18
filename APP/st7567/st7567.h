#ifndef  __ST7567_H
#define  __ST7567_H

#include"main.h"

#define LCD_RES_RCC      RCC_AHB1Periph_GPIOD
#define LCD_RES_PIN      GPIO_Pin_1
#define LCD_RES_PIN_PORT GPIOD

#define LCD_CS_RCC       RCC_AHB1Periph_GPIOD
#define LCD_CS_PIN       GPIO_Pin_2
#define LCD_CS_PIN_PORT  GPIOD

#define LCD_A0_RCC       RCC_AHB1Periph_GPIOD
#define LCD_A0_PIN       GPIO_Pin_0
#define LCD_A0_PIN_PORT  GPIOD


#define LCD_SDA_RCC       RCC_AHB1Periph_GPIOB
#define LCD_SDA_PIN       GPIO_Pin_11
#define LCD_SDA_PIN_PORT  GPIOB    

#define LCD_SCLK_RCC      RCC_AHB1Periph_GPIOB
#define LCD_SCLK_PIN      GPIO_Pin_10
#define LCD_SCLK_PIN_PORT GPIOB    



#define LCD_SDA_PIN_HIGH     GPIO_SetBits(LCD_SDA_PIN_PORT,LCD_SDA_PIN)
#define LCD_SDA_PIN_LOW      GPIO_ResetBits(LCD_SDA_PIN_PORT,LCD_SDA_PIN)

#define LCD_SCLK_PIN_HIGH    GPIO_SetBits(LCD_SCLK_PIN_PORT,LCD_SCLK_PIN)
#define LCD_SCLK_PIN_LOW     GPIO_ResetBits(LCD_SCLK_PIN_PORT,LCD_SCLK_PIN)


#define LCD_RES_PIN_HIGH     GPIO_SetBits(LCD_RES_PIN_PORT,LCD_RES_PIN)
#define LCD_RES_PIN_LOW      GPIO_ResetBits(LCD_RES_PIN_PORT,LCD_RES_PIN)

#define LCD_CS_PIN_HIGH      GPIO_SetBits(LCD_CS_PIN_PORT,LCD_CS_PIN)
#define LCD_CS_PIN_LOW       GPIO_ResetBits(LCD_CS_PIN_PORT,LCD_CS_PIN)

#define LCD_A0_PIN_HIGH      GPIO_SetBits(LCD_A0_PIN_PORT,LCD_A0_PIN)
#define LCD_A0_PIN_LOW       GPIO_ResetBits(LCD_A0_PIN_PORT,LCD_A0_PIN)

void lcd_display_logo(void);
void lcd_init(void);
void lcd_gpio_init(void);
void st7567_init(void);
void writecommand(uint8_t cmd);	// 写命令
void writedata(uint8_t dataa);//写数据
void writedata1(uint8_t dataa);//写数据
void disp_clear(void);
void disp_full(void);
void disp_col(void);
void disp_row(void);
void dispgraphiCS(void);//dispgraphiCS
void dispblxstp(void);//dispgraphiCS
void dispstr(uint8_t	d);//显示字符，0是第一行，1是第二行
void lcd_display_16x16_char(uint8_t x, uint8_t y, uint8_t data);

#endif /*__ST7567_H*/

