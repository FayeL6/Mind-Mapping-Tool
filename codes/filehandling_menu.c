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

void drawFileOpenButtons()//简化了一下按钮的代码、、、 
{
	double fH = GetFontHeight();
	double x = winwidth/2;
	double y = 4*winheight/5;
	double w = TextStringWidth("打开文件");
	double h = fH*2; 
	int a;
	
	a=GetPointSize();

	//画文本文件按钮 
	x=winwidth/12,y=winwidth*3/8,w=w*1.8,h=fH*3; //设置初始参数，让之后的按钮变量只需要x,y,w,h 连着一行写就可以 
	SetPenColor("Black"),drawLabel(x, y+h*2, "固定存档："),SetPenColor("Blue"); 
	
	int i=0;
	char file_name[15];
	char file_button_name[15];
	
	for(i=0;i<3;i++)
	{
		sprintf(file_name,"文本文件%d.txt",i+1);
		sprintf(file_button_name,"文本文件%d",i+1);
		if (button(GenUIID(i),x,y,w,h, file_button_name)) 
		{
			fp=fopen(file_name,"r");
			fileopenchoice(fp);
		} 
		y-=2*h;
	}
	
	//画二进制文件按钮 
	x+=w*1.3,y=winwidth*3/8; 
	for(i=0;i<3;i++)
	{
		sprintf(file_name,"二进制文件%d.bin",i+1);
		sprintf(file_button_name,"二进制文件%d",i+1);
		if (button(GenUIID(i),x,y,w,h, file_button_name)) 
		{
			fp=fopen(file_name,"rb");
			Bfileopenchoice(fp);
		} 
		y-=2*h; 
	}
	
	//画Label控件 
	x=winwidth/2,y=winwidth*3/8;
	SetPenColor("Black");
	drawLabel(x, y+2*h, "打开自定义文档：");
	drawLabel(x, y+h, "打开文件名：");
	drawLabel(x, y-h, "文件类型：");
	SetPenColor("Blue"); 
	
	//画选择文本、二进制按钮
	x+=w,y-=fH;//让Label和文本框对齐所以要下移
	textbox(GenUIID(0),x,y+h,2*w,h, filename_open, 100);
	
	if(istxt2==1 )setButtonColors("Red","Red","Red","Red", 0);//如果点击成功则变色 
		
	if (button(GenUIID(0),x,y-h,w,h, istxt2?"√文本文件":"文本文件"))
	{
		istxt2=!istxt2;isbin2=0;
	    	
	} 
	setButtonColors("Blue","Blue","Red","Red", 0);
	
	if(isbin2==1 )setButtonColors("Red","Red","Red","Red", 0);//如果点击成功则变色 
	if (button(GenUIID(0),x+w,y-h,w,h,isbin2?"√二进制文件":"二进制文件"))
	{
		istxt2=0;isbin2=!isbin2;
	} 
	setButtonColors("Blue","Blue","Red","Red", 0);	
	
	//画打开和返回按钮
	if (button(GenUIID(0),x,y-h*3,w*2,h,"打开文件"))
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
	w=TextStringWidth("打开文件"),x=winwidth/2-w/2;
	drawLabel(x,winheight*7/8, "打开文件");//这种个例就直接用参数表示 
	SetPointSize(a);
	SetPenColor("Blue"); 
	if (button(GenUIID(0),x,winheight/12,w,h,"返回")) 
	{
		start_choice=OpenMemory;
	}
}

void drawFileSaveButtons()
{
	double fH = GetFontHeight();
	double x = winwidth/2;
	double y = 4*winheight/5;
	double w = TextStringWidth("保存文件");
	double h = fH*2; 
	int a;
	
	a=GetPointSize();
	
	//画文本文件按钮 
	x=winwidth/12,y=winwidth*3/8,w=w*1.8,h=fH*3; //设置初始参数，让之后的按钮变量只需要x,y,w,h 连着一行写就可以 
	SetPenColor("Black"),drawLabel(x, y+h*2, "固定存档："),SetPenColor("Blue"); 
	
	int i=0;
	char file_name[15];
	char file_button_name[15];
	
	for(i=0;i<3;i++)
	{
		sprintf(file_name,"文本文件%d.txt",i+1);
		sprintf(file_button_name,"文本文件%d",i+1);
		if (button(GenUIID(i),x,y,w,h,file_button_name)) 
		{
			fp=fopen(file_name,"w");
			if (mode_choice==1) filesavetemplate(fp);
			if (mode_choice==2) filesavepicture(fp);
		}
		y-=2*h; 
	}
	
	//画二进制文件按钮 
	x+=w*1.3,y=winwidth*3/8; 
	for(i=0;i<3;i++)
	{
		sprintf(file_name,"二进制文件%d.bin",i+1);
		sprintf(file_button_name,"二进制文件%d",i+1);
		if (button(GenUIID(i),x,y,w,h,file_button_name)) 
		{
			fp=fopen(file_name,"wb");
			if (mode_choice==1) filesavetemplate(fp);
			if (mode_choice==2) Bfilesavepicture(fp);
		}
		y-=2*h; 
	}
	//画Label控件 
	x=winwidth/2,y=winwidth*3/8;
	SetPenColor("Black");
	drawLabel(x, y+2*h, "保存自定义文档：");
	drawLabel(x, y+h, "保存文件名：");
	drawLabel(x, y-h, "文件类型：");
	SetPenColor("Blue"); 
	
	//画选择文本、二进制按钮
	x+=w,y-=fH;//让Label和文本框对齐所以要下移
	
	if(textbox(GenUIID(0),x,y+h,2*w,h, filename_save, 100))
	{
		sprintf(results,"即将保存的文件名为：%s(注意文本文件后缀名为.txt,二进制文件为.bin)", filename_save);
	}
	drawBox(0, 0, winwidth, h/1.5, 0, results,'L' , "Blue");
	
	//如果点击成功则变色
	if(istxt==1 )setButtonColors("Red","Red","Red","Red", 0); 
	if (button(GenUIID(0),x,y-h,w,h, istxt?"√文本文件":"文本文件"))
	{
		istxt=!istxt;isbin=0;
	    	
	} 
	setButtonColors("Blue","Blue","Red","Red", 0);
	
	//如果点击成功则变色
	if(isbin==1 )setButtonColors("Red","Red","Red","Red", 0); 
	if (button(GenUIID(0),x+w,y-h,w,h,isbin?"√二进制文件":"二进制文件"))
	{
		istxt=0;isbin=!isbin;
	} 
	setButtonColors("Blue","Blue","Red","Red", 0);	
	
	//画保存和返回按钮
	if (button(GenUIID(0),x,y-h*3,w*2,h,"保存文件"))
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
	w=TextStringWidth("保存文件"),x=winwidth/2-w/2;
	drawLabel(x,winheight*7/8, "保存文件");//这种个例就直接用参数表示 
	SetPointSize(a);
	SetPenColor("Blue"); 
	if (button(GenUIID(0),x,winheight/12,w,h,"返回"))
	{
		start_choice=OpenMemory;
	}
}
