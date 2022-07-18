#include "time5.h"


time5Struct time5Info;


void time5_init(uint16_t per,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);//ʹ��TIM4ʱ��
	TIM_TimeBaseInitStructure.TIM_Period=per;   //�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //������ʱ���ж�
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;//��ʱ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	TIM_Cmd(TIM5,DISABLE); //Ĭ�Ϲرն�ʱ��	
	memset(&time5Info, 0, sizeof(time5Struct));
}

void time5_enble(void)
{
	TIM_Cmd(TIM5,ENABLE); //������ʱ��	
}

void time5_disable(void)
{
	TIM_Cmd(TIM5,DISABLE); //�رն�ʱ��	
}


/*******************************************************************************
* �� �� ��         : TIM5_IRQHandler
* ��������		   : TIM5�жϺ���
* ��    ��         : ��
* ��    ��         : ��
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


