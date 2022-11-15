#include <reg51.h>
#include <oled.h>
#include <Timer.h>
#include <UI.h>
#include <EEPROM.h>

unsigned char high,low,i;
//unsigned char *p_high=&high,*p_low=&low;
unsigned int count=0;
unsigned int ten_min=0,hour=0,len_min,len_hour;

void PlayTime_Init() //��ʼ��ʱ����ʾ
{
//	Timer_Tran(36000,p_high,p_low);  //36ms�Ķ�ʱ��
	Timer0_Init(); //��ʼ��
	for (i=0;i<4;i++){
		OLED_ShowCHinese(32+(i<<4),2,13+i); //<<4 = *16
	}
	OLED_ShowChar(96,2,':',16);
	for(;;){
		if(ten_min==10){
			hour++;
			ten_min = 0;
		}
		OLED_ShowNum(48,4,hour,1,16);
		OLED_ShowChar(56,4,'.',16);
		OLED_ShowNum(63,4,ten_min,1,16);
		OLED_ShowChar(72,4,'h',16);
		//OLED_ShowNum(1,0,IAP_ReadByte(0x2000),1,16);
	}
}

void main()
{
	OLED_Init();
	OLED_Clear();
	UI_Start();
	//IAP_ProgramByte(0x2000,5);
	for(;;){
		PlayTime_Init();
		
	}
}

void Timer0_PlayTime_Plus() interrupt 1 using 1 //��ʱ��0�����������ʱ��
{
	TH0 = 0x73;
	TL0 = 0x60;
	count++;
	if (count >= 10000){ //�жϴ����жϣ�36ms*10000Ϊ360s����0.1h
		ten_min++;
		count = 0;
	}
}