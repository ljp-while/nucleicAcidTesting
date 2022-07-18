#include "time4.h"
#include "powerKey.h"

void time4_init(uint16_t per,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//ʹ��TIM4ʱ��
	TIM_TimeBaseInitStructure.TIM_Period=per;   //�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //������ʱ���ж�
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	TIM_Cmd(TIM4,DISABLE); //Ĭ�Ϲرն�ʱ��	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//��ʱ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	
}


/*******************************************************************************
* �� �� ��         : TIM4_IRQHandler
* ��������		   : TIM4�жϺ���
* ��    ��         : ��
* ��    ��         : ��
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
				printf("�����Լ�ģʽ\r\n");
				memset(&powerKeyMes, 0, sizeof(powerKeyMes));
				TIM_Cmd(TIM4,DISABLE); //�رն�ʱ��4
			}
			else if((powerKeyMes.lowLevelTime > LOW_LEVEL_TIMES_MAX) && (powerKeyMes.risingCount == 0))
			{
				GPIO_ResetBits(POWER_CONTROL_PIN_PORT,POWER_CONTROL_PIN);
				printf("�ػ�����\r\n");
				memset(&powerKeyMes, 0, sizeof(powerKeyMes));
				TIM_Cmd(TIM4,DISABLE); //�رն�ʱ��4
			}	
			else if(powerKeyMes.fallingCount == KEY_PRESS_COUNT_ONE)
			{
				printf("�Զ����ģʽ\r\n");
				time5_enble();
				memset(&powerKeyMes, 0, sizeof(powerKeyMes));
				TIM_Cmd(TIM4,DISABLE); //�رն�ʱ��4				
			}
			else
			{
				printf("pass\r\n");
				memset(&powerKeyMes, 0, sizeof(powerKeyMes));
				TIM_Cmd(TIM4,DISABLE); //�رն�ʱ��4				
			}
			powerKeyMes.fristDown = 1;
		}
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	
}


