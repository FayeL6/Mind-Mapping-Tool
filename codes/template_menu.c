#include <windows.h>
#include <winuser.h>
#include <stdio.h>
#include <stdlib.h>
#include "conio.h"
#include "simpio.h"
#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"
#include "linkedlist.h"
#include <math.h>
#include "filehandling.h"

extern start_choice,OpenMemory,mode_choice;
extern double  winwidth, winheight;
int isLogicGraphic=0;
int isStructGraphic=0;
int isFishBone=0;
int main_topic=3;
int sub_topic=6;
char contents[100][100]={"中心观点","主观点1","主观点2","主观点3","主观点4",
                         "子观点1","子观点2","子观点3","子观点4","子观点5","子观点6","子观点7","子观点8"};
char contents_2[100][100]={"中心观点","主观点1","主观点2","主观点3","主观点4",
                         "子观点1","子观点2","子观点3","子观点4","子观点5","子观点6","子观点7","子观点8"};
char mainTopicText[10]="3";
char subTopicText[10]="6";
int text_number=0;
char mainOutPut[100]="";
char subOutPut[100]=""; 
int fail_to_draw=0;
extern int newload;
double Fixx; 

//清空屏幕文本 
void ClearTemplate()
{
	text_number=0;
	int i;
	for(i=0;i<100;i++)
	{
		strcpy(contents[i],"");
	}
	
}

//还原开始
void Back_to_Start()
{
	isLogicGraphic=0;
	isStructGraphic=0;
	isFishBone=0;
	main_topic=3;
	sub_topic=6;
	
	int i;
	for(i=0;i<13;i++)
	{
		strcpy(contents[i],contents_2[i]);
	}
	strcpy(mainTopicText,"3");
	strcpy(subTopicText,"6");
	text_number=0;
} 

void drawTemplateButtons()
{
	double fH = GetFontHeight();
	double h = fH*2; 
	double x = winwidth/12;  
	double y = winheight/12; //建议用winheight作参数 
	double w = TextStringWidth("逻辑图")*2;
	
	SetPenColor("Black"); 
	drawLabel(x, y+h*1.5, "模板选择：");
	
	if(newload==1){
		sprintf(subTopicText,"%d",sub_topic);
	    sprintf(mainTopicText,"%d",main_topic);
	    newload=0;
	}
		
	
	SetPenColor("Blue"); 
	if (button(GenUIID(0), x, y, w, h, "逻辑图")){
		isLogicGraphic=!isLogicGraphic;isStructGraphic=0;isFishBone=0;
	}
	if (button(GenUIID(0), x+1.2*w, y, w, h, "结构图")){
		isLogicGraphic=0;isStructGraphic=!isStructGraphic;isFishBone=0;
	}
	if (button(GenUIID(0), x+2.4*w, y, w, h, "鱼骨图")){
		isLogicGraphic=0;isStructGraphic=0;isFishBone=!isFishBone;
	}
	
	//增加了两个按钮 
	y-=h*1.2;
	if (button(GenUIID(0), x, y, w*1.65, h, "清空文本")){ 
		ClearTemplate(); 
	}
	if (button(GenUIID(0), x+w*1.75, y, w*1.65, h, "回到初始")){ 
		Back_to_Start(); 
	}
	
	x+=3.5*w,y+=h*1.2;
	SetPenColor("Black"); 
	drawLabel(x, y+h*1.5, "内容选择：");
	drawLabel(x, y+h*0.5, "主观点数：");
	drawLabel(x, y-h*0.5, "子观点数：");
	
	if(textbox(GenUIID(0), x+TextStringWidth("内容选择："), 0.7, w/2, 0.8*h, mainTopicText, sizeof( mainTopicText))) 
	{
		main_topic=mainTopicText[0]-'0';
		sprintf(mainOutPut,"请输入2-4内的整数"); 
    }
    
    drawLabel(x+TextStringWidth("内容选择：")+w*2/3, 0.8, mainOutPut);	
    if(textbox(GenUIID(0), x+TextStringWidth("内容选择："), 0.3, w/2, 0.8*h, subTopicText, sizeof( subTopicText))) 
	{
		sub_topic=subTopicText[0]-'0';	
		sprintf(subOutPut,"请输入0-%d内的整数",2*main_topic);
	}
	drawLabel(x+TextStringWidth("内容选择：")+w*2/3, 0.4, subOutPut);
	

	x=winwidth*0.85;
	//补充了打开和保存跳转按钮
	if (button(GenUIID(0), x, y+h, w, h, "打开"))
	{
		start_choice=3;
		OpenMemory= 1;
	} 
	if (button(GenUIID(0), x, y-h*0.2, w, h, "保存"))
	{
		mode_choice=1; 
		start_choice=4;
		OpenMemory = 1;
	} 
	if (button(GenUIID(1),x,y-h*1.4,w,h,"返回")) start_choice=0;
    SetPenColor("White");
    MovePen(0,0);
    DrawLine(w,h);
    SetPenColor("Blue");

	if (isLogicGraphic==1)	
	{
		drawlogicgraphic();
		drawlogicgraphicText();
	}
	
	if (isStructGraphic==1) 
	{
		drawstructgraphic();
		drawstructgraphicText();
	}
	
	if (isFishBone==1)
	{
		drawfishbone();
		drawfishboneText();
	}
	
	if(main_topic<2 || main_topic>4 || sub_topic<0 || sub_topic>2*main_topic)
	{
		fail_to_draw=1;
	}
	else fail_to_draw=0;
	if(fail_to_draw==1)
	{
		drawBox(0, 0, winwidth, h/1.5, 1, "无法绘制!",'L' , "White");
	}
}

