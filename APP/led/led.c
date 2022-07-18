#include "led.h"

/*******************************************************************************
* �� �� ��         : LED_Init
* ��������		   : LED��ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����
	
	RCC_AHB1PeriphClockCmd(WHITE_LED_PORT_RCC|GREEN_LED_PORT_RCC,ENABLE); //ʹ�ܶ˿�ʱ��
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //���ģʽ
	GPIO_InitStructure.GPIO_Pin=WHITE_LED_PIN;//�ܽ�����
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;//�ٶ�Ϊ100M
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//����
	GPIO_Init(WHITE_LED_PORT,&GPIO_InitStructure); //��ʼ���ṹ��
	GPIO_SetBits(WHITE_LED_PORT,WHITE_LED_PIN);
	
	GPIO_InitStructure.GPIO_Pin=GREEN_LED_PIN;//�ܽ�����
	GPIO_Init(GREEN_LED_PORT,&GPIO_InitStructure); //��ʼ���ṹ��
	GPIO_SetBits(GREEN_LED_PORT,GREEN_LED_PIN);
	
	WHITE_LED = 1;
	GREEN_LED = 1;
}



