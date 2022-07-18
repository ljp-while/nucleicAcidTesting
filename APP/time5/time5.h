#ifndef  __TIME5_H
#define  __TIME5_H


#include "main.h"

#define TIME_VALUE_MAX  1*60*15  //1*60*15√Î

typedef struct{
	uint16_t timeCount;
	uint16_t succeedFlag;
}time5Struct;


void time5_enble(void);
void time5_disable(void);
void time5_init(uint16_t per,uint16_t psc);

#endif /*__TIME5_H*/


