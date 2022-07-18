#include "powerKey.h"
#include <string.h>

powerKeyInfo powerKeyMes;


uint8_t get_power_key_state(void)
{
	return GPIO_ReadInputDataBit(POWER_KEY_PORT,POWER_KEY_PIN);
}

void power_key_init(void)
{
	power_key_gpio_config();
	power_key_exit_config();
	power_key_nvic_config();
	memset(&powerKeyMes, 0, sizeof(powerKeyInfo));
	
}


static void power_key_gpio_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量	
	POWER_KEY_PERIPH_CLOCK_CMD(POWER_KEY_PERIPH_GPIOX,ENABLE); //使能端口时钟	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN; //输入模式
	GPIO_InitStructure.GPIO_Pin=POWER_KEY_PIN;//管脚设置
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//浮空
	GPIO_Init(POWER_KEY_PORT,&GPIO_InitStructure); //初始化结构体	
	
	
	POWER_CONTROL_PIN_PERIPH_CLOCK_CMD(POWER_CONTROL_PIN_PERIPH_GPIOX,ENABLE); //使能端口时钟	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //输入模式
	GPIO_InitStructure.GPIO_Pin=POWER_CONTROL_PIN;//管脚设置
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//上拉
	GPIO_Init(POWER_CONTROL_PIN_PORT,&GPIO_InitStructure); //初始化结构体
 	GPIO_SetBits(POWER_CONTROL_PIN_PORT,POWER_CONTROL_PIN);
//	GPIO_ResetBits(POWER_CONTROL_PIN_PORT,POWER_CONTROL_PIN);
}

static void power_key_exit_config(void)
{
	EXTI_InitTypeDef  EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD,EXTI_PinSource10);
	EXTI_InitStructure.EXTI_Line=EXTI_Line10; 
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_ClearITPendingBit(EXTI_Line10);
}


static void power_key_nvic_config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//EXTI10 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//EXTI10中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器	
}

/*******************************************************************************
* 函 数 名         : EXTI10_IRQHandler
* 函数功能		   : 外部中断10函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line10)==1)
	{
		delay_ms(10);
		if(powerKeyMes.fristDown == 0)
		{
			powerKeyMes.fristDown = 1;
		}
		else
		{
			if(get_power_key_state()) //上升沿
			{
				powerKeyMes.risingCount++;
			}
			else  //下降沿
			{
				powerKeyMes.fallingCount++;
			}
			if(powerKeyMes.firstPress == 0)
			{
				powerKeyMes.firstPress = 1;
				TIM_Cmd(TIM4,ENABLE); //开启定时器4
			}			
		}

		
	}
	EXTI_ClearITPendingBit(EXTI_Line10);
}


