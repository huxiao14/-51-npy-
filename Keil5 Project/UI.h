#include <reg51.h>
#include <oled.h>
#ifndef __UI_H
#define __UI_H

void UI_Start();
void UI_Stop(unsigned char op,unsigned int delta_score);
unsigned char Numlen(unsigned int num);
void UI_Dog();
void UI_Continue();
void Score_show(unsigned char *p);

#endif