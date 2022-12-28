#include <reg51.h>

//void Timer_Tran(unsigned int time_us,unsigned char *p_high,unsigned char *p_low) //����ʱ������ʱ����us��ʽ���룬����ָ��ָ���high low��Ϊ��ֱ�����ö�ʱ����ֵ
//{
//	unsigned int time = 65535-time_us+1;
//	unsigned char tmp;
//	tmp = (unsigned char)(time & 0x00FF);
//	*p_low = tmp;
//	tmp = (unsigned char)(time >> 8);
//	*p_high = tmp;
//}

void Timer0_Init()		//��ʼ����ʱ��0
{
	TMOD &= 0xF0; // ���ֶ�ʱ��1�����ǰ������ն�ʱ��0������
	TMOD |= 0x01; // ���ö�ʱ��ģʽΪxxxx0001������ʱ��0��ģʽ1
	TH0 = 0x73;		// ��ʱ����λ����	
	TL0 = 0x60;		// ��ʱ����λ����
	TR0 = 1;		
	ET0 = 1;
	EA = 1;
}

void Timer0_Stop()
{
	TR0 = 0;
	ET0 = 0;
	EA = 0;
}

//void Timer1_Init(unsigned char *p_high,unsigned char *p_low)		//��ʼ����ʱ��1
//{
//	TMOD &= 0x0F; // ���ֶ�ʱ��0�����ǰ������ն�ʱ��1������
//	TMOD |= 0x10; // ���ö�ʱ��ģʽΪ0001xxxx������ʱ��1��ģʽ1
//	TH1 = *p_high;		// ��ʱ����λ����	
//	TL1 = *p_low;		// ��ʱ����λ����
//	TR1 = 1;		
//	ET1 = 1;
//	EA = 1;
//}