#include <reg51.h>
#include <oled.h>
//#include <bmp.h>
#include <EEPROM.h>

unsigned char UI_i;
extern unsigned char code GM[],HX[],ALS[],MK[],BH[],JZW[],SRV[];

unsigned char Numlen(unsigned int num)
{
	unsigned char len = 1;
	while(num/=10)++len;
	return len;
}

void UI_Start() //欢迎界面
{
	for(UI_i=0;UI_i<=4;UI_i++){
		OLED_ShowCHinese(24+(UI_i<<4),2,UI_i);
		OLED_ShowCHinese(16+(UI_i<<4),4,UI_i+5); //欢迎语
	}
	OLED_ShowCHinese(96,4,10);
	for (;;){
		if (P2 != 0xFF){   //判断任意键是否有被按下 p2 != 11111111
			OLED_Clear();
			break;
		}
		OLED_ShowCHinese(56,6,11); //箭头闪烁
		Delay_50ms(10);
		if (P2 != 0xFF){
			OLED_Clear();
			break;
		}
		OLED_ShowCHinese(56,6,12);
		Delay_50ms(10);
	}
}

void UI_Stop(unsigned char op,unsigned int delta_score)  //停止练琴后显示的
{
	OLED_Clear();
	OLED_ShowCHinese(16,3,13);
	OLED_ShowCHinese(32,3,21);
	OLED_ShowCHinese(48,3,22);
	OLED_ShowCHinese(64,3,19);
	OLED_ShowCHinese(80,3,23);
	OLED_ShowCHinese(96,3,24);
	OLED_ShowChar(112,3,':',16);
	OLED_ShowNum(48,5,delta_score,Numlen(delta_score),16);
	switch (op){
		case 0:
			for (UI_i=0;UI_i<=4;UI_i++){
				OLED_ShowCHinese(24+(UI_i<<4),1,15+UI_i);
			}
			break;
		case 1:
			OLED_ShowCHinese(32,1,25);
			OLED_ShowCHinese(48,1,0);
			OLED_ShowCHinese(64,1,26);
			OLED_ShowCHinese(80,1,27);
			OLED_ShowChar(96,1,'~',16);
			break;
		case 2:
			OLED_ShowCHinese(48,1,28);
			OLED_ShowCHinese(64,1,29);
			OLED_ShowChar(80,1,'!',16);
			break;
		case 3:
			OLED_ShowCHinese(40,1,30);
			OLED_ShowCHinese(56,1,31);
			OLED_ShowCHinese(72,1,19);
			OLED_ShowChar(88,1,'~',16);
			break;
		case 4:
			OLED_ShowCHinese(40,1,32);
			OLED_ShowCHinese(56,1,33);
			OLED_ShowCHinese(72,1,19);
			OLED_ShowChar(88,1,'(',16);
			break;
	}
}

//void UI_Shop()
//{

//}