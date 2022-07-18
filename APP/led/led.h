#ifndef _led_H
#define _led_H

#include "main.h"

/*  LEDʱ�Ӷ˿ڡ����Ŷ��� */
#define WHITE_LED_PORT 			GPIOC  
#define WHITE_LED_PIN 			GPIO_Pin_10
#define WHITE_LED_PORT_RCC		RCC_AHB1Periph_GPIOC

#define GREEN_LED_PORT 			GPIOE   
#define GREEN_LED_PIN 			GPIO_Pin_3
#define GREEN_LED_PORT_RCC		RCC_AHB1Periph_GPIOE


#define WHITE_LED PCout(10)  	
#define GREEN_LED PEout(3) 

void led_init(void);


#endif
