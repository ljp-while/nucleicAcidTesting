#include "led.h"

/*******************************************************************************
* 函 数 名         : LED_Init
* 函数功能		   : LED初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量
	
	RCC_AHB1PeriphClockCmd(WHITE_LED_PORT_RCC|GREEN_LED_PORT_RCC,ENABLE); //使能端口时钟
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //输出模式
	GPIO_InitStructure.GPIO_Pin=WHITE_LED_PIN;//管脚设置
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;//速度为100M
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//上拉
	GPIO_Init(WHITE_LED_PORT,&GPIO_InitStructure); //初始化结构体
	GPIO_SetBits(WHITE_LED_PORT,WHITE_LED_PIN);
	
	GPIO_InitStructure.GPIO_Pin=GREEN_LED_PIN;//管脚设置
	GPIO_Init(GREEN_LED_PORT,&GPIO_InitStructure); //初始化结构体
	GPIO_SetBits(GREEN_LED_PORT,GREEN_LED_PIN);
	
	WHITE_LED = 1;
	GREEN_LED = 1;
}



