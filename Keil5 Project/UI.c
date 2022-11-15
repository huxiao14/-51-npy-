#include <reg51.h>
#include <oled.h>

unsigned char UI_i;

void UI_Start() //欢迎界面
{
	for(UI_i=0;UI_i<=4;UI_i++){
		OLED_ShowCHinese(24+(UI_i<<4),1,UI_i);
		OLED_ShowCHinese(16+(UI_i<<4),3,UI_i+5);
	}
	OLED_ShowCHinese(96,3,10);
	for (;;){
		if (P2 != 0xFF){   //判断任意键是否有被按下
			OLED_Clear();
			break;
		}
		OLED_ShowCHinese(56,5,11);
		Delay_50ms(15);
		if (P2 != 0xFF){
			OLED_Clear();
			break;
		}
		OLED_ShowCHinese(56,5,12);
		Delay_50ms(15);
	}
}