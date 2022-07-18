#ifndef  __TIME4_H
#define  __TIME4_H


#include "main.h"


#define KEY_PRESS_COUNT_ONE   1
#define KEY_PRESS_COUNT_TWO   2
#define LOW_LEVEL_TIMES_MAX   20  //2秒
#define PROCESS_TIMES_MAX     22  //2.2秒处理一次

void time4_init(uint16_t per,uint16_t psc);

#endif /*__TIME4_H*/


