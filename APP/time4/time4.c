#include "time4.h"
#include "powerKey.h"

void time4_init(uint16_t per,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//使能TIM4时钟
	TIM_TimeBaseInitStructure.TIM_Period=per;   //自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //开启定时器中断
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	TIM_Cmd(TIM4,DISABLE); //默认关闭定时器	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//定时器中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	
	
}


/*******************************************************************************
* 函 数 名         : TIM4_IRQHandler
* 函数功能		   : TIM4中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update))
	{
		powerKeyMes.timeCount++;
		powerKeyMes.currentState = get_power_key_state();
		if(powerKeyMes.currentState == 0)
		{
			powerKeyMes.lowLevelTime++;		
		}
		if(powerKeyMes.timeCount > PROCESS_TIMES_MAX)
		{
			printf("powerKeyMes.risingCount = %d  powerKeyMes.fallingCount = %d\r\n",powerKeyMes.risingCount,powerKeyMes.fallingCount);
			if(powerKeyMes.fallingCount == KEY_PRESS_COUNT_TWO)
			{
				printf("连续自检模式\r\n");
				memset(&powerKeyMes, 0, sizeof(powerKeyMes));
				TIM_Cmd(TIM4,DISABLE); //关闭定时器4
			}
			else if((powerKeyMes.lowLevelTime > LOW_LEVEL_TIMES_MAX) && (powerKeyMes.risingCount == 0))
			{
				GPIO_ResetBits(POWER_CONTROL_PIN_PORT,POWER_CONTROL_PIN);
				printf("关机处理\r\n");
				memset(&powerKeyMes, 0, sizeof(powerKeyMes));
				TIM_Cmd(TIM4,DISABLE); //关闭定时器4
			}	
			else if(powerKeyMes.fallingCount == KEY_PRESS_COUNT_ONE)
			{
				printf("自动检测模式\r\n");
				time5_enble();
				memset(&powerKeyMes, 0, sizeof(powerKeyMes));
				TIM_Cmd(TIM4,DISABLE); //关闭定时器4				
			}
			else
			{
				printf("pass\r\n");
				memset(&powerKeyMes, 0, sizeof(powerKeyMes));
				TIM_Cmd(TIM4,DISABLE); //关闭定时器4				
			}
			powerKeyMes.fristDown = 1;
		}
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	
}


