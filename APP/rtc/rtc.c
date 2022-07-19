#include "rtc.h"


/*******************************************************************************
* �� �� ��         : RTC_Set_Time
* ��������		   : RTCʱ������
* ��    ��         : hour,min,sec:Сʱ,����,����
					 ampm:RTC_H12_AM/RTC_H12_PM
* ��    ��         : SUCEE(1),�ɹ�
					 ERROR(0),�����ʼ��ģʽʧ��
*******************************************************************************/ 
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	
	RTC_TimeTypeInitStructure.RTC_Hours=hour;
	RTC_TimeTypeInitStructure.RTC_Minutes=min;
	RTC_TimeTypeInitStructure.RTC_Seconds=sec;
	RTC_TimeTypeInitStructure.RTC_H12=ampm;
	
	return RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);
	
}

/*******************************************************************************
* �� �� ��         : RTC_Set_Date
* ��������		   : RTC��������
* ��    ��         : year,month,date:��(0~99),��(1~12),��(0~31)
					 week:����(1~7)
* ��    ��         : SUCEE(1),�ɹ�
					 ERROR(0),�����ʼ��ģʽʧ��
*******************************************************************************/ 
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
	
	RTC_DateTypeDef RTC_DateTypeInitStructure;
	RTC_DateTypeInitStructure.RTC_Date=date;
	RTC_DateTypeInitStructure.RTC_Month=month;
	RTC_DateTypeInitStructure.RTC_WeekDay=week;
	RTC_DateTypeInitStructure.RTC_Year=year;
	return RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);
}

/*******************************************************************************
* �� �� ��         : RTC_Config
* ��������		   : RTC��ʼ��
* ��    ��         : ��
* ��    ��         : 0,��ʼ���ɹ�
        			 1,LSE����ʧ��
*******************************************************************************/ 
uint8_t rtc_init(void)
{
	u16 i=0X1FFF;
	RTC_InitTypeDef RTC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//��RTC�󱸼Ĵ�����Դʱ��
	PWR_BackupAccessCmd(ENABLE);//�򿪺󱸼Ĵ�������
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x5050) //�ж��Ƿ��һ�γ�ʼ��RTC
	{
		RCC_LSEConfig(RCC_LSE_ON);//�����ⲿ32.768K RTCʱ��
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==0) //�ȴ�LSE����
		{
			i++;
			delay_ms(10);
		}
		if(i==0)
		{
			return 1;  //LSE����ʧ��
		}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); //ѡ��LSE��ΪRTCʱ��
		RCC_RTCCLKCmd(ENABLE);//ʹ��RTCʱ��
		
		RTC_InitStructure.RTC_AsynchPrediv=0x7f;
		RTC_InitStructure.RTC_HourFormat=RTC_HourFormat_24;
		RTC_InitStructure.RTC_SynchPrediv=0xff;
		RTC_Init(&RTC_InitStructure);
		RTC_Set_Time(16,30,50,RTC_H12_AM);
		RTC_Set_Date(20,4,33,3);
		RTC_WriteBackupRegister(RTC_BKP_DR0,0X5050); //RTCд�Ĵ���
	}
//		RTC_Set_Time(14,57,10,RTC_H12_AM);
//		RTC_Set_Date(22,6,28,2);
	return 0;
}

/*******************************************************************************
* �� �� ��         : RTC_Set_AlarmA
* ��������		   : ��������ʱ��(����������,24Сʱ��)
* ��    ��         : week:���ڼ�(1~7) 
					 hour,min,sec:Сʱ,����,����
* ��    ��         : ��
*******************************************************************************/
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	RTC_AlarmTypeDef RTC_AlarmTypeInitStructure;
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);//�ر�����A 
	
	RTC_TimeTypeInitStructure.RTC_Hours=hour;//Сʱ
	RTC_TimeTypeInitStructure.RTC_Minutes=min;//����
	RTC_TimeTypeInitStructure.RTC_Seconds=sec;//��
	RTC_TimeTypeInitStructure.RTC_H12=RTC_H12_AM;
  
	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDay=week;//����
	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_WeekDay;//��������
	RTC_AlarmTypeInitStructure.RTC_AlarmMask=RTC_AlarmMask_None;//��ȷƥ�����ڣ�ʱ����
	RTC_AlarmTypeInitStructure.RTC_AlarmTime=RTC_TimeTypeInitStructure;
	RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmTypeInitStructure);
 
	
	RTC_ClearITPendingBit(RTC_IT_ALRA);//���RTC����A�ı�־
	EXTI_ClearITPendingBit(EXTI_Line17);//���LINE17�ϵ��жϱ�־λ 
	
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);//��������A�ж�
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);//��������A 
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;//LINE17
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE17
    EXTI_Init(&EXTI_InitStructure);//����

	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);//����
}

/*******************************************************************************
* �� �� ��         : RTC_Set_WakeUp
* ��������		   : �����Ի��Ѷ�ʱ������  
* ��    ��         : wksel��
					 #define RTC_WakeUpClock_RTCCLK_Div16        ((uint32_t)0x00000000)
					 #define RTC_WakeUpClock_RTCCLK_Div8         ((uint32_t)0x00000001)
					 #define RTC_WakeUpClock_RTCCLK_Div4         ((uint32_t)0x00000002)
					 #define RTC_WakeUpClock_RTCCLK_Div2         ((uint32_t)0x00000003)
					 #define RTC_WakeUpClock_CK_SPRE_16bits      ((uint32_t)0x00000004)
					 #define RTC_WakeUpClock_CK_SPRE_17bits      ((uint32_t)0x00000006)
					 cnt:�Զ���װ��ֵ������0,�����ж�
* ��    ��         : ��
*******************************************************************************/
void RTC_Set_WakeUp(u32 wksel,u16 cnt)
{ 
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RTC_WakeUpCmd(DISABLE);//�ر�WAKE UP
	
	RTC_WakeUpClockConfig(wksel);//����ʱ��ѡ��
	
	RTC_SetWakeUpCounter(cnt);//����WAKE UP�Զ���װ�ؼĴ���
	
	
	RTC_ClearITPendingBit(RTC_IT_WUT); //���RTC WAKE UP�ı�־
	EXTI_ClearITPendingBit(EXTI_Line22);//���LINE22�ϵ��жϱ�־λ 
	 
	RTC_ITConfig(RTC_IT_WUT,ENABLE);//����WAKE UP ��ʱ���ж�
	RTC_WakeUpCmd( ENABLE);//����WAKE UP ��ʱ����
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;//LINE22
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE22
	EXTI_Init(&EXTI_InitStructure);//����
 
 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
}


uint8_t buf[40];
RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;


void display_time_date(void)
{
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	sprintf((char *)buf,"Time: %.2d:%.2d:%.2d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);
	_DEBUG_PRINT_("%s\r\n",buf);
//	LCD_ShowString(0,420,320,16,16,buf);  	  
	RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);
	sprintf((char *)buf,"Date: 20%d-%.2d-%.2d  Day %d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date,RTC_DateStruct.RTC_WeekDay);
//	LCD_ShowString(0,450,320,16,16,buf); 
}

