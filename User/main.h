#ifndef  __MAIN_H
#define  __MAIN_H


#include "system.h"
#include "SysTick.h"
#include "usart.h"
#include "led.h"  
#include "time.h" 
#include "ov5640.h" 
#include "dcmi.h" 
#include "imageProcess.h"
#include "flash.h"
#include "powerKey.h"
#include "time4.h"
#include "time5.h"
#include "rtc.h"
#include <string.h>
#include "st7567.h"
#include "spi.h"


#define SINGLE_TRANSFER_LENGTH   1024
extern uint8_t memory0Flag;
extern uint8_t memory1Flag;
extern uint32_t dcmiDataCount;


#define IMAGE_WIDTH  320
#define IMAGE_HIGHT  240


#define X_START   60  
#define Y_START   90 
#define X_WIDTH   100
#define Y_HIGHT   40

#define _DEBUG_ 1

#if _DEBUG_
#define _DEBUG_PRINT_(...) printf(__VA_ARGS__)
#else
#define _DEBUG_PRINT_(...)
#endif


#endif /*__MAIN_H*/

