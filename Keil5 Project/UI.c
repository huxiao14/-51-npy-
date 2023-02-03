#include <reg51.h>
#include <oled.h>
#include <bmp.h>
#include <EEPROM.h>

void UI_Buy(unsigned char op);
void UI_Start();

unsigned char UI_score[7] = {0};
unsigned char UI_i,dh=0,dl=0;
bit op_h=1,op_f=0;
extern unsigned char code OD[];
extern unsigned char code CD[];
extern unsigned char code TD[];
extern unsigned char code Meat[];
extern unsigned char code Bone[];
extern unsigned char code Gift[];

//unsigned char Numlen(unsigned int num)
//{
//	unsigned char len = 1;
//	while(num/=10)++len;
//	return len;
//}

void Score_show(unsigned char *p)
{
	unsigned char Score_i,Score_k=0;
	for (Score_i=0;Score_i<=6;Score_i++){
		if (p[Score_i]!=0)Score_k=1;
		if(Score_k)OLED_ShowChar(85+Score_i*6,0,p[Score_i]+'0',1);
	}
}

void Score_minus(unsigned char *p)
{
	unsigned char Score_i;
	for (Score_i=6;Score_i>=1;Score_i--){
		if (p[Score_i]>10){
			p[Score_i-1]--;
			p[Score_i] += 10;
		}
	}
}



void UI_Continue()
{
	Delay_50ms(20);
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

void UI_Start() //欢迎界面
{
	for(UI_i=0;UI_i<=4;UI_i++){
		OLED_ShowCHinese(24+(UI_i<<4),2,UI_i);
		OLED_ShowCHinese(16+(UI_i<<4),4,UI_i+5); //欢迎语
	}
	OLED_ShowCHinese(96,4,10);
	UI_Continue();
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
//	OLED_ShowNum(48,5,delta_score,4,16);
	for (UI_i=0;UI_i<=4;UI_i++){
		OLED_ShowChar(72-(UI_i<<3),5,delta_score%10+'0',16);
		delta_score /= 10;
	}
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
			OLED_ShowChar(96,1,'~'-66,16);
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
			OLED_ShowChar(88,1,'~'-66,16);
			break;
		case 4:
			OLED_ShowCHinese(40,1,32);
			OLED_ShowCHinese(56,1,33);
			OLED_ShowCHinese(72,1,19);
			OLED_ShowChar(88,1,'(',16);
			break;
	}
	Delay_50ms(50);
	UI_Continue();
	UI_Start();
}

void UI_Dog()
{
	unsigned char op_o=1,tmp=0;      //op_h = 0 means dog is hungry, op_f = 1 means already bounght some food,所有函数内变量要在开头定义
	for (UI_i=0;UI_i<=6;UI_i++){
		tmp = IAP_ReadByte(0x2800+UI_i);
		if (tmp!=0xFF){
			UI_score[UI_i] = tmp;
		}
	}
	if (IAP_ReadByte(0x2C00)!=0xFF)dh = IAP_ReadByte(0x2C00);	//2C00 --> doghungry
	else{dh = 0;}
	if (IAP_ReadByte(0x2E00)!=0xFF){dl = IAP_ReadByte(0x2E00);} //2E00 --> doglove
	else{IAP_ProgramByte(0x2E00,20);dl = 20;}
	if (IAP_ReadByte(0x3000)==1){
		IAP_EraseSector(0x3000);
		OLED_ShowCHinese(32,2,13);
		OLED_ShowCHinese(48,2,42);
		OLED_ShowCHinese(64,2,43);
		OLED_ShowCHinese(80,2,19);
		OLED_ShowCHinese(24,4,25);
		for (UI_i=0;UI_i<=3;UI_i++){
			OLED_ShowCHinese(40+(UI_i<<4),4,44+UI_i);
		}
		Delay_50ms(20);
		UI_Continue();
	}
	if (dh>=5){
		op_h = 0;
		if (dh>=10){
			OLED_ShowCHinese(56,2,34);
			OLED_ShowCHinese(72,2,19);
			OLED_ShowCHinese(88,2,37);
			OLED_ShowCHinese(104,2,37);
			OLED_DrawBMP(0,0,50,5,CD);
		}
		else{
			OLED_ShowCHinese(56,2,34);
			OLED_ShowCHinese(72,2,34);
			OLED_ShowChar(88,2,'~'-66,16);
			OLED_DrawBMP(0,0,50,5,OD);
		}
	}
	else{
		if (dl>=5&&dl<=45){
			OLED_DrawBMP(0,0,50,5,OD);
			OLED_ShowCHinese(56,2,13);
			OLED_ShowCHinese(72,2,35);
			OLED_ShowCHinese(88,2,36);
			OLED_ShowChar(104,2,'~'-66,16);
		}
		else if (dl<5){
			OLED_ShowCHinese(56,2,37);
			OLED_ShowCHinese(72,2,37);
			OLED_DrawBMP(0,0,50,5,CD);
		}
		else if (dl>45){
			OLED_DrawBMP(0,0,50,5,TD);
			OLED_ShowCHinese(56,2,40);
			OLED_ShowCHinese(72,2,41);
			OLED_ShowCHinese(88,2,13);
			OLED_ShowCHinese(104,2,35);
		}
	}
	while (P2 == 0xFB);
	for (;;){
		
		OLED_DrawBMP(14,5,46,7,Bone);
		OLED_DrawBMP(60,5,82,7,Meat);
		OLED_DrawBMP(96,5,111,7,Gift);
		if (P2 == 0xFD&&op_o<2){op_o++;while(P2 == 0xFD);}
		else if (P2 == 0xFB&&op_o>0){op_o--;while(P2 == 0xFB);}
		else if (P2 == 0xF7){while (P2==0xF7);UI_Buy(op_o);}
		else if (P2 == 0xEF){while(P2 == 0xEF);OLED_Clear();UI_Start();break;}
		Delay_50ms(8);
		if (P2 == 0xFD&&op_o<2){op_o++;while(P2 == 0xFD);}
		else if (P2 == 0xFB&&op_o>0){op_o--;while(P2 == 0xFB);}
		else if (P2 == 0xF7){while (P2==0xF7);UI_Buy(op_o);}
		else if (P2 == 0xEF){while(P2 == 0xEF);OLED_Clear();UI_Start();break;}
		if (op_o==0){
			OLED_ShowCHinese(14,5,12);
			OLED_ShowCHinese(30,5,12);
		}
		else if (op_o==1){
			OLED_ShowCHinese(60,5,12);
			OLED_ShowCHinese(76,5,12);
		}
		else if (op_o==2){
			OLED_ShowCHinese(96,5,12);
			OLED_ShowCHinese(112,5,12);
		}
		Delay_50ms(8);
	}
	if ((op_h|op_f)==0){
		if (dl>=10){
			dl-=10;
			IAP_EraseSector(0x2E00);
			IAP_ProgramByte(0x2E00,dl);
		}
	}
}

void UI_Buy(unsigned char op)
{
	bit k = 0;
	for (UI_i=0;UI_i<3;UI_i++){
		if (UI_score[UI_i]!=0){
			k = 1;
			break;
		}
	}
	if (op==0){
		if (UI_score[3]>=2)k=1;
		if (k){
			op_f = 1;
			OLED_DrawBMP(0,0,50,5,TD);
			OLED_ShowCHinese(56,2,38);
			OLED_ShowCHinese(72,2,38);
			OLED_ShowCHinese(88,2,13);
			OLED_ShowCHinese(104,2,36);
			if (dl<240)dl += 10;
			else {IAP_ProgramByte(0x3000,1);dl=20;}
			IAP_EraseSector(0x2E00);
			IAP_ProgramByte(0x2E00,dl);
			if (dh>=5)dh -= 5;
			IAP_EraseSector(0x2C00);
			IAP_ProgramByte(0x2C00,dh);
			UI_score[3] -= 2;
		}
	}
	else if (op==1){
		if (UI_score[3]!=0||UI_score[4]>=5)k=1;
		if (k){
			op_f = 1;
			OLED_DrawBMP(0,0,50,5,TD);
			OLED_ShowCHinese(56,2,35);
			OLED_ShowCHinese(72,2,35);
			OLED_ShowCHinese(88,2,39);
			OLED_ShowCHinese(104,2,36);
			if (dl<245)dl += 5;
			else {IAP_ProgramByte(0x3000,1);dl=20;}
			IAP_EraseSector(0x2E00);
			IAP_ProgramByte(0x2E00,dl);
			if (dh>=5)dh -= 5;
			IAP_EraseSector(0x2C00);
			IAP_ProgramByte(0x2C00,dh);
			UI_score[4] -= 5;
		}
	}
	else if (op==2){
		if (UI_score[3]>=5)k=1;
		if(k){
			op_f = 1;
			OLED_DrawBMP(0,0,50,5,TD);
			OLED_ShowCHinese(56,2,38);
			OLED_ShowCHinese(72,2,38);
			OLED_ShowCHinese(88,2,40);
			OLED_ShowCHinese(104,2,41);
			if (dl<210)dl += 40;
			else {IAP_ProgramByte(0x3000,1);dl=20;}
			IAP_EraseSector(0x2E00);
			IAP_ProgramByte(0x2E00,dl);
			UI_score[3] -= 5;
		}
	}
	Score_minus(UI_score);
	for (UI_i=0;UI_i<=7;UI_i++)OLED_ShowChar(127-(UI_i<<2),0,' ',1);
	Score_show(UI_score);
	IAP_EraseSector(0x2800);
	for (UI_i=0;UI_i<=6;UI_i++){
		IAP_ProgramByte(0x2800+UI_i,UI_score[UI_i]);
	}
}