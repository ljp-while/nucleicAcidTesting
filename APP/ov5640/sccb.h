#ifndef __SCCB_H
#define __SCCB_H
#include "system.h"



////IO��������
//#define SCCB_SDA_IN()  {GPIOD->MODER&=~(3<<(7*2));GPIOD->MODER|=0<<7*2;}	//PD7 ����
//#define SCCB_SDA_OUT() {GPIOD->MODER&=~(3<<(7*2));GPIOD->MODER|=1<<7*2;} 	//PD7 ���


////IO��������	 
//#define SCCB_SCL    		PDout(6)	 	//SCL
//#define SCCB_SDA    		PDout(7) 		//SDA	 

//#define SCCB_READ_SDA    	PDin(7)  		//����SDA    


//IO��������
#define SCCB_SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9 ����
#define SCCB_SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} 	//PB9 ���


//IO��������	 
#define SCCB_SCL    		PBout(8)	 	//SCL
#define SCCB_SDA    		PBout(9) 		//SDA	 

#define SCCB_READ_SDA    	PBin(9)  		//����SDA    
#define SCCB_ID   			0X60  			//OV2640��ID

///////////////////////////////////////////
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void); 
#endif
