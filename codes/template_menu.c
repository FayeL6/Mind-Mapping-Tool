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
char contents[100][100]={"���Ĺ۵�","���۵�1","���۵�2","���۵�3","���۵�4",
                         "�ӹ۵�1","�ӹ۵�2","�ӹ۵�3","�ӹ۵�4","�ӹ۵�5","�ӹ۵�6","�ӹ۵�7","�ӹ۵�8"};
char contents_2[100][100]={"���Ĺ۵�","���۵�1","���۵�2","���۵�3","���۵�4",
                         "�ӹ۵�1","�ӹ۵�2","�ӹ۵�3","�ӹ۵�4","�ӹ۵�5","�ӹ۵�6","�ӹ۵�7","�ӹ۵�8"};
char mainTopicText[10]="3";
char subTopicText[10]="6";
int text_number=0;
char mainOutPut[100]="";
char subOutPut[100]=""; 
int fail_to_draw=0;
extern int newload;
double Fixx; 

//�����Ļ�ı� 
void ClearTemplate()
{
	text_number=0;
	int i;
	for(i=0;i<100;i++)
	{
		strcpy(contents[i],"");
	}
	
}

//��ԭ��ʼ
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
	double y = winheight/12; //������winheight������ 
	double w = TextStringWidth("�߼�ͼ")*2;
	
	SetPenColor("Black"); 
	drawLabel(x, y+h*1.5, "ģ��ѡ��");
	
	if(newload==1){
		sprintf(subTopicText,"%d",sub_topic);
	    sprintf(mainTopicText,"%d",main_topic);
	    newload=0;
	}
		
	
	SetPenColor("Blue"); 
	if (button(GenUIID(0), x, y, w, h, "�߼�ͼ")){
		isLogicGraphic=!isLogicGraphic;isStructGraphic=0;isFishBone=0;
	}
	if (button(GenUIID(0), x+1.2*w, y, w, h, "�ṹͼ")){
		isLogicGraphic=0;isStructGraphic=!isStructGraphic;isFishBone=0;
	}
	if (button(GenUIID(0), x+2.4*w, y, w, h, "���ͼ")){
		isLogicGraphic=0;isStructGraphic=0;isFishBone=!isFishBone;
	}
	
	//������������ť 
	y-=h*1.2;
	if (button(GenUIID(0), x, y, w*1.65, h, "����ı�")){ 
		ClearTemplate(); 
	}
	if (button(GenUIID(0), x+w*1.75, y, w*1.65, h, "�ص���ʼ")){ 
		Back_to_Start(); 
	}
	
	x+=3.5*w,y+=h*1.2;
	SetPenColor("Black"); 
	drawLabel(x, y+h*1.5, "����ѡ��");
	drawLabel(x, y+h*0.5, "���۵�����");
	drawLabel(x, y-h*0.5, "�ӹ۵�����");
	
	if(textbox(GenUIID(0), x+TextStringWidth("����ѡ��"), 0.7, w/2, 0.8*h, mainTopicText, sizeof( mainTopicText))) 
	{
		main_topic=mainTopicText[0]-'0';
		sprintf(mainOutPut,"������2-4�ڵ�����"); 
    }
    
    drawLabel(x+TextStringWidth("����ѡ��")+w*2/3, 0.8, mainOutPut);	
    if(textbox(GenUIID(0), x+TextStringWidth("����ѡ��"), 0.3, w/2, 0.8*h, subTopicText, sizeof( subTopicText))) 
	{
		sub_topic=subTopicText[0]-'0';	
		sprintf(subOutPut,"������0-%d�ڵ�����",2*main_topic);
	}
	drawLabel(x+TextStringWidth("����ѡ��")+w*2/3, 0.4, subOutPut);
	

	x=winwidth*0.85;
	//�����˴򿪺ͱ�����ת��ť
	if (button(GenUIID(0), x, y+h, w, h, "��"))
	{
		start_choice=3;
		OpenMemory= 1;
	} 
	if (button(GenUIID(0), x, y-h*0.2, w, h, "����"))
	{
		mode_choice=1; 
		start_choice=4;
		OpenMemory = 1;
	} 
	if (button(GenUIID(1),x,y-h*1.4,w,h,"����")) start_choice=0;
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
		drawBox(0, 0, winwidth, h/1.5, 1, "�޷�����!",'L' , "White");
	}
}

