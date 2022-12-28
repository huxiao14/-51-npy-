#include <reg51.h>
#ifndef __TIMER_H
#define __TIMER_H

//void Timer_Tran(unsigned int time_us,unsigned char *p_high,unsigned char *p_low);
void Timer0_Init();
//void Timer1_Init(unsigned char *p_high,unsigned char *p_low);
void Timer0_Stop();

#endif