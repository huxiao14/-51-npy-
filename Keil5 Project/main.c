#include <reg51.h>
#include <oled.h>
#include <Timer.h>
#include <UI.h>
#include <EEPROM.h>

extern unsigned char code GM[],HX[],ALS[],MK[],BH[],JZW[],SRV[];
unsigned char i,tmp,k=0;
unsigned int count=0,ten_min=0,hour=0;
unsigned char score[7] = {0};

void PlayTime_Stop();
void PlayTime_Init();
void Score_plus(unsigned char *p);

void main()
{
	for (i=0;i<=6;i++){
		tmp = IAP_ReadByte(0x2800+i);
		if (tmp!=0xFF){
			score[i] = tmp;
		}
	}
	OLED_Init();
	OLED_Clear();
	for (i=0;i<=6;i++){
		if (score[i]!=0)k=1;
		if(k)OLED_ShowChar(85+i*6,0,score[i]+'0',1);
	}
//	OLED_DrawBMP(0,0,64,8,GM);
	UI_Start();
	for(;;){
		if (P2==0xFD){  //right to PlayTime p2 = 11111101
			PlayTime_Init();
		}
		
		
	}
}

void Timer0_PlayTime_Plus() interrupt 1 using 1 //定时器0负责计算练琴时间
{
	TH0 = 0x73;
	TL0 = 0x60;
	count++;
	if (count >= 100){ //中断次数判断，36ms*10000为360s，即0.1h
		ten_min++;
		count = 0;
	}
}

void PlayTime_Init() //开始计时并显示
{
	Timer0_Init(); //初始化
	for (i=0;i<4;i++){
		if (i<=1){
			OLED_ShowCHinese(32+(i<<4),2,13+i); //<<4 = *16
		}
		else{
			OLED_ShowCHinese(32+(i<<4),2,i-2);
		}
	}
	OLED_ShowChar(96,2,':',16);
	OLED_ShowChar(56,4,'.',16);
	OLED_ShowChar(72,4,'h',16);
	for(;;){
		if(ten_min==10){
			hour++;
			if (hour>=6)break;
			ten_min = 0;
		}
		OLED_ShowNum(48,4,hour,1,16);
		OLED_ShowNum(63,4,ten_min,1,16);
		if (P2 == 0xFE){ //mid stop PlayTime p2 = 11111110
			OLED_Clear();
			break;
		}
		//OLED_ShowNum(1,0,IAP_ReadByte(0x2000),1,16);
	}
	PlayTime_Stop();
}

void PlayTime_Stop() //停止并计算得分
{
	unsigned char Stop_Op=0;
	unsigned int delta_score = 0;
	Timer0_Stop();
	if (hour==0){
		if (ten_min<=2){Stop_Op = 0;}
		else{
			Stop_Op = 1;
			delta_score = ten_min*100;
		}
	}
	if (hour>=1&&hour<3){
		Stop_Op = 2;
		delta_score = 1000+(hour-1)*2000+ten_min*200;
	}
	if (hour>=3&&hour<6){
		Stop_Op = 3;
		delta_score = 5000+(hour-3)*3000+ten_min*300;
	}
	if (hour>=6){
		Stop_Op = 4;
		delta_score = 3000;
	}
	ten_min = 0;
	hour = 0;
	score[4] += ((delta_score/100)%10);
	score[3] += (delta_score/1000);
	Score_plus(score);
	IAP_EraseSector(0x2800);
	for(i=0;i<=6;i++){
		IAP_ProgramByte(0x2800+i,score[i]);
	}
	UI_Stop(Stop_Op,delta_score);
}

void Score_plus(unsigned char *p)
{
	unsigned char Score_i;
	for (Score_i=6;Score_i>=1;Score_i--){
		if (p[Score_i]>=10){
			p[Score_i] %= 10;
			p[Score_i-1]++;
		}
	}
}