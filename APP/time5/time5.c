#include "time5.h"


time5Struct time5Info;


void time5_init(uint16_t per,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);//使能TIM4时钟
	TIM_TimeBaseInitStructure.TIM_Period=per;   //自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //开启定时器中断
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;//定时器中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	
	TIM_Cmd(TIM5,DISABLE); //默认关闭定时器	
	memset(&time5Info, 0, sizeof(time5Struct));
}

void time5_enble(void)
{
	TIM_Cmd(TIM5,ENABLE); //开启定时器	
}

void time5_disable(void)
{
	TIM_Cmd(TIM5,DISABLE); //关闭定时器	
}


/*******************************************************************************
* 函 数 名         : TIM5_IRQHandler
* 函数功能		   : TIM5中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update))
	{
		time5Info.timeCount++;
		if(time5Info.timeCount == TIME_VALUE_MAX)
		{
			time5Info.timeCount = 0;
			time5Info.succeedFlag = 1;
			time5_disable();
			printf("%d ok\r\n",TIME_VALUE_MAX);
		}
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);	
}


