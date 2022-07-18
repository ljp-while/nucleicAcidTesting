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
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����	
	POWER_KEY_PERIPH_CLOCK_CMD(POWER_KEY_PERIPH_GPIOX,ENABLE); //ʹ�ܶ˿�ʱ��	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN; //����ģʽ
	GPIO_InitStructure.GPIO_Pin=POWER_KEY_PIN;//�ܽ�����
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//����
	GPIO_Init(POWER_KEY_PORT,&GPIO_InitStructure); //��ʼ���ṹ��	
	
	
	POWER_CONTROL_PIN_PERIPH_CLOCK_CMD(POWER_CONTROL_PIN_PERIPH_GPIOX,ENABLE); //ʹ�ܶ˿�ʱ��	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //����ģʽ
	GPIO_InitStructure.GPIO_Pin=POWER_CONTROL_PIN;//�ܽ�����
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//����
	GPIO_Init(POWER_CONTROL_PIN_PORT,&GPIO_InitStructure); //��ʼ���ṹ��
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
	//EXTI10 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//EXTI10�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���	
}

/*******************************************************************************
* �� �� ��         : EXTI10_IRQHandler
* ��������		   : �ⲿ�ж�10����
* ��    ��         : ��
* ��    ��         : ��
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
			if(get_power_key_state()) //������
			{
				powerKeyMes.risingCount++;
			}
			else  //�½���
			{
				powerKeyMes.fallingCount++;
			}
			if(powerKeyMes.firstPress == 0)
			{
				powerKeyMes.firstPress = 1;
				TIM_Cmd(TIM4,ENABLE); //������ʱ��4
			}			
		}

		
	}
	EXTI_ClearITPendingBit(EXTI_Line10);
}


