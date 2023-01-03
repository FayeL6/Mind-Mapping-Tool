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
#include "blankdrawing.h" 
#include "color.h"

extern double winheight,winwidth;
extern int start_choice,OpenMemory,mode_choice;
extern int main_topic,sub_topic;
extern char contents[100][100];
extern int isLogicGraphic,isStructGraphic,isFishBone;
extern Colors Text;
extern char lineColor[100];

FILE *fp;
char filename_save[100]; 
char filename_open[100];
char results[1000];
int istxt=0,isbin=0;
int istxt2=0,isbin2=0;
int newload=0;

void drawFileOpenButtons()//����һ�°�ť�Ĵ��롢���� 
{
	double fH = GetFontHeight();
	double x = winwidth/2;
	double y = 4*winheight/5;
	double w = TextStringWidth("���ļ�");
	double h = fH*2; 
	int a;
	
	a=GetPointSize();

	//���ı��ļ���ť 
	x=winwidth/12,y=winwidth*3/8,w=w*1.8,h=fH*3; //���ó�ʼ��������֮��İ�ť����ֻ��Ҫx,y,w,h ����һ��д�Ϳ��� 
	SetPenColor("Black"),drawLabel(x, y+h*2, "�̶��浵��"),SetPenColor("Blue"); 
	
	int i=0;
	char file_name[15];
	char file_button_name[15];
	
	for(i=0;i<3;i++)
	{
		sprintf(file_name,"�ı��ļ�%d.txt",i+1);
		sprintf(file_button_name,"�ı��ļ�%d",i+1);
		if (button(GenUIID(i),x,y,w,h, file_button_name)) 
		{
			fp=fopen(file_name,"r");
			fileopenchoice(fp);
		} 
		y-=2*h;
	}
	
	//���������ļ���ť 
	x+=w*1.3,y=winwidth*3/8; 
	for(i=0;i<3;i++)
	{
		sprintf(file_name,"�������ļ�%d.bin",i+1);
		sprintf(file_button_name,"�������ļ�%d",i+1);
		if (button(GenUIID(i),x,y,w,h, file_button_name)) 
		{
			fp=fopen(file_name,"rb");
			Bfileopenchoice(fp);
		} 
		y-=2*h; 
	}
	
	//��Label�ؼ� 
	x=winwidth/2,y=winwidth*3/8;
	SetPenColor("Black");
	drawLabel(x, y+2*h, "���Զ����ĵ���");
	drawLabel(x, y+h, "���ļ�����");
	drawLabel(x, y-h, "�ļ����ͣ�");
	SetPenColor("Blue"); 
	
	//��ѡ���ı��������ư�ť
	x+=w,y-=fH;//��Label���ı����������Ҫ����
	textbox(GenUIID(0),x,y+h,2*w,h, filename_open, 100);
	
	if(istxt2==1 )setButtonColors("Red","Red","Red","Red", 0);//�������ɹ����ɫ 
		
	if (button(GenUIID(0),x,y-h,w,h, istxt2?"���ı��ļ�":"�ı��ļ�"))
	{
		istxt2=!istxt2;isbin2=0;
	    	
	} 
	setButtonColors("Blue","Blue","Red","Red", 0);
	
	if(isbin2==1 )setButtonColors("Red","Red","Red","Red", 0);//�������ɹ����ɫ 
	if (button(GenUIID(0),x+w,y-h,w,h,isbin2?"�̶������ļ�":"�������ļ�"))
	{
		istxt2=0;isbin2=!isbin2;
	} 
	setButtonColors("Blue","Blue","Red","Red", 0);	
	
	//���򿪺ͷ��ذ�ť
	if (button(GenUIID(0),x,y-h*3,w*2,h,"���ļ�"))
	{
		if (istxt2==1)
		{
			fp=fopen(filename_open,"r");
			fileopenchoice(fp);
		}
		if (isbin2==1)
		{
			fp=fopen(filename_open,"rb");
			Bfileopenchoice(fp);
		}
	}
	
	SetPenColor("Black"); 
	SetPointSize(24);
	w=TextStringWidth("���ļ�"),x=winwidth/2-w/2;
	drawLabel(x,winheight*7/8, "���ļ�");//���ָ�����ֱ���ò�����ʾ 
	SetPointSize(a);
	SetPenColor("Blue"); 
	if (button(GenUIID(0),x,winheight/12,w,h,"����")) 
	{
		start_choice=OpenMemory;
	}
}

void drawFileSaveButtons()
{
	double fH = GetFontHeight();
	double x = winwidth/2;
	double y = 4*winheight/5;
	double w = TextStringWidth("�����ļ�");
	double h = fH*2; 
	int a;
	
	a=GetPointSize();
	
	//���ı��ļ���ť 
	x=winwidth/12,y=winwidth*3/8,w=w*1.8,h=fH*3; //���ó�ʼ��������֮��İ�ť����ֻ��Ҫx,y,w,h ����һ��д�Ϳ��� 
	SetPenColor("Black"),drawLabel(x, y+h*2, "�̶��浵��"),SetPenColor("Blue"); 
	
	int i=0;
	char file_name[15];
	char file_button_name[15];
	
	for(i=0;i<3;i++)
	{
		sprintf(file_name,"�ı��ļ�%d.txt",i+1);
		sprintf(file_button_name,"�ı��ļ�%d",i+1);
		if (button(GenUIID(i),x,y,w,h,file_button_name)) 
		{
			fp=fopen(file_name,"w");
			if (mode_choice==1) filesavetemplate(fp);
			if (mode_choice==2) filesavepicture(fp);
		}
		y-=2*h; 
	}
	
	//���������ļ���ť 
	x+=w*1.3,y=winwidth*3/8; 
	for(i=0;i<3;i++)
	{
		sprintf(file_name,"�������ļ�%d.bin",i+1);
		sprintf(file_button_name,"�������ļ�%d",i+1);
		if (button(GenUIID(i),x,y,w,h,file_button_name)) 
		{
			fp=fopen(file_name,"wb");
			if (mode_choice==1) filesavetemplate(fp);
			if (mode_choice==2) Bfilesavepicture(fp);
		}
		y-=2*h; 
	}
	//��Label�ؼ� 
	x=winwidth/2,y=winwidth*3/8;
	SetPenColor("Black");
	drawLabel(x, y+2*h, "�����Զ����ĵ���");
	drawLabel(x, y+h, "�����ļ�����");
	drawLabel(x, y-h, "�ļ����ͣ�");
	SetPenColor("Blue"); 
	
	//��ѡ���ı��������ư�ť
	x+=w,y-=fH;//��Label���ı����������Ҫ����
	
	if(textbox(GenUIID(0),x,y+h,2*w,h, filename_save, 100))
	{
		sprintf(results,"����������ļ���Ϊ��%s(ע���ı��ļ���׺��Ϊ.txt,�������ļ�Ϊ.bin)", filename_save);
	}
	drawBox(0, 0, winwidth, h/1.5, 0, results,'L' , "Blue");
	
	//�������ɹ����ɫ
	if(istxt==1 )setButtonColors("Red","Red","Red","Red", 0); 
	if (button(GenUIID(0),x,y-h,w,h, istxt?"���ı��ļ�":"�ı��ļ�"))
	{
		istxt=!istxt;isbin=0;
	    	
	} 
	setButtonColors("Blue","Blue","Red","Red", 0);
	
	//�������ɹ����ɫ
	if(isbin==1 )setButtonColors("Red","Red","Red","Red", 0); 
	if (button(GenUIID(0),x+w,y-h,w,h,isbin?"�̶������ļ�":"�������ļ�"))
	{
		istxt=0;isbin=!isbin;
	} 
	setButtonColors("Blue","Blue","Red","Red", 0);	
	
	//������ͷ��ذ�ť
	if (button(GenUIID(0),x,y-h*3,w*2,h,"�����ļ�"))
	{
		if (istxt==1)
		{
			fp=fopen(filename_save,"w");
			if (mode_choice==1) filesavetemplate(fp);
			if (mode_choice==2) filesavepicture(fp);
		}
		if (isbin==1)
		{
			fp=fopen(filename_save,"wb");
			if (mode_choice==1) filesavetemplate(fp);
			if (mode_choice==2) Bfilesavepicture(fp);
		}
	}
	
	SetPenColor("Black"); 
	SetPointSize(24);
	w=TextStringWidth("�����ļ�"),x=winwidth/2-w/2;
	drawLabel(x,winheight*7/8, "�����ļ�");//���ָ�����ֱ���ò�����ʾ 
	SetPointSize(a);
	SetPenColor("Blue"); 
	if (button(GenUIID(0),x,winheight/12,w,h,"����"))
	{
		start_choice=OpenMemory;
	}
}
