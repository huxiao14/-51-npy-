#include <reg51.h>
#include <oled.h>

unsigned int count=0;

void Timer0_Init(void)		//��ʼ����ʱ��
{
	TMOD = 0x01; // ���ö�ʱ��ģʽΪ00010000������ʱ��0��ģʽ1
	TL0 = 0x66;		//??????
	TH0 = 0xFC;		//??????
	TR0 = 1;		//???1????
	ET0 = 1;
	EA = 1;
	//PT0 = 0;
}

void main()
{
	unsigned char k=0;
//	P3 &= 0xFE;
	OLED_Init();
	Timer0_Init();
	for(;;){
		OLED_ShowChar(0,k,'A',0);
		k++;
		k %= 7;
		//OLED_Clear();
	}
}

void Timer1_ShowChar() interrupt 1 using 1
{
	TL0 = 0x66;		//??????
	TH0 = 0xFC;
	count++;
	if (count >= 5000){
		P3 &= 0xFE;
		count = 0;
	}
}