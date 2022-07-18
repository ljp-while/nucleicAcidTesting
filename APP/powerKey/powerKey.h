#ifndef  __POWERKEY_H
#define  __POWERKEY_H

#include "main.h"



#define POWER_KEY_PIN    		GPIO_Pin_10    	
#define POWER_KEY_PORT 			GPIOD 		//定义端口
#define POWER_KEY_PERIPH_CLOCK_CMD RCC_AHB1PeriphClockCmd
#define POWER_KEY_PERIPH_GPIOX RCC_AHB1Periph_GPIOD

#define POWER_CONTROL_PIN       GPIO_Pin_11
#define POWER_CONTROL_PIN_PORT  GPIOD 		//定义端口
#define POWER_CONTROL_PIN_PERIPH_CLOCK_CMD RCC_AHB1PeriphClockCmd
#define POWER_CONTROL_PIN_PERIPH_GPIOX RCC_AHB1Periph_GPIOD


typedef struct {
	uint8_t fristDown;
	uint8_t firstPress;
	uint8_t currentState;
	uint8_t risingCount;
	uint8_t fallingCount;
	uint16_t timeCount;
	uint8_t lowLevelTime;
}powerKeyInfo;

extern powerKeyInfo powerKeyMes;


uint8_t get_power_key_state(void);
void power_key_init(void);
static void power_key_gpio_config(void);
static void power_key_exit_config(void);
static void power_key_nvic_config(void);

#endif /*__POWERKEY_H*/

