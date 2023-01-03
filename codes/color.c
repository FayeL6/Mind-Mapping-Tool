#include <windows.h>
#include <stdlib.h>
#include <winuser.h>
#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"
#include "linkedlist.h"

//������ı�ȫ�ֱ���Text��lineColor��Ȼ����blankdrawing�ļӵ㺯������������ʱȫ�ֱ���������ݴ�Ž�ȥ
//Ȼ���ڻ���ͻ��ߵĺ���ǰ��һ��SetXXXColor������ʹ��ָ��p�����ɫ���� 

typedef struct{
	char textframe[100];	//�߿���ɫ 
	char textletter[100];	//������ɫ 
	int whetherfill;	//�Ƿ���� 
}Colors;	//�����½ṹ��������ֿ�ĸ�����ɫ���� 

Colors Text = {"Blue", "Blue", 0};	//���ֿ���ɫ��ȫ�ֱ��� 
char lineColor[100] = "Black";		//����ɫ��ȫ�ֱ��� 

int Framecolor = 0, Lettercolor = 0, iflineColor=0;	 //���ڿ���ɫѡ��ť�Ƿ񱻰��� 
extern double winwidth, winheight;


void displaycolorchoice()
{
	double fH = GetFontHeight();
	double h=fH*1.5; 
	double x = winwidth;
	double y = winheight;
	double w=1.5;
	SetPenColor("Blue");
	
	//��ʾ���� 
	y-=1.6*h;
	drawLabel(x-=1.5,y, "      ѡ��߿���ɫ");
	drawLabel(x-=1.5,y, "      ѡ��������ɫ");
	drawLabel(x-=1.5,y, "          �Ƿ����");
	drawLabel(x-=1.5,y, "      ѡ��������ɫ");
	
	//��ɫѡ��ť 
	x = winwidth, y = winheight-h;
	if (button(GenUIID(0), x-=1.5,y, w, h, Text.textframe)){
		Framecolor = !Framecolor; Lettercolor = 0; iflineColor = 0;
	}
	if (button(GenUIID(0), x-=1.5,y, w, h, Text.textletter)){
		Framecolor = 0; Lettercolor = !Lettercolor; iflineColor = 0;
	}
	if (button(GenUIID(0), x-=1.5,y, w, h, Text.whetherfill ? "��":"��")){
		Text.whetherfill = !Text.whetherfill; //�Ƿ������ 0 1 ��ʾ���� 
	}
	if (button(GenUIID(0), x-=1.5,y, w, h, lineColor)){
		Framecolor = 0; Lettercolor = 0;iflineColor = !iflineColor;
	}
	//�������ɫѡ��İ�ť�����£�ִ��������� �ı���ɫ 
	if( Framecolor || Lettercolor || iflineColor )//ʡ��==1
	 {
		changecolor();
	}
}

void changecolor()
{
	double x = winwidth, y = winheight;
	char to_color[100];  //����������ݴ�һ�������ѡ�����ɫ����������е�� �� n ����1 
	int i,n=0;
	//һ����ʾ��ɫ���ģ�һ����ʾ��ɫӢ�� 
	char color_ch[][5]={"��ɫ","��ɫ","��ɫ","��ɫ","��ɫ","��ɫ","��ɫ","��ɫ","��ɫ","��ɫ","��ɫ","Ʒ��","ǳ��","���"};
	char color_en[][12]={"Black","Blue","Red","Green","Gray","Orange","Yellow","Brown","White","Cyan","Violet","Magenta","Light Gray","Dark Gray"};
	
	for(i-0;i<sizeof(color_ch)/sizeof(color_ch[0]);i++)//��Ϊ���й��ɵ����Ըĳ�ѭ�� 
	if(button(GenUIID(i), x-0.5, y-1-i*0.25, 0.5, 0.25, color_ch[i]))
	{
		strcpy(to_color, color_en[i]);
		n=1;
	}
	//����е�� Ƕ��if�ж��ǵ�����һ����ɫѡ��ť 
	if( n==1 ){
		if( Framecolor == 1 )//�߿� 
		{
			strcpy(Text.textframe, to_color);		
		}
		else if( Lettercolor == 1 )//����
		{
			strcpy(Text.textletter, to_color);	
		} 
	    else //�� 
	    {
			strcpy(lineColor, to_color);
		} 
	} 
	
}
