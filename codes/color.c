#include <windows.h>
#include <stdlib.h>
#include <winuser.h>
#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"
#include "linkedlist.h"

//在这里改变全局变量Text和lineColor，然后在blankdrawing的加点函数那里把鼠标点击时全局变量里的数据存放进去
//然后在画框和画线的函数前加一个SetXXXColor函数，使用指针p里的颜色数据 

typedef struct{
	char textframe[100];	//边框颜色 
	char textletter[100];	//文字颜色 
	int whetherfill;	//是否填充 
}Colors;	//定义新结构，存放文字框的各种颜色数据 

Colors Text = {"Blue", "Blue", 0};	//文字框颜色的全局变量 
char lineColor[100] = "Black";		//线颜色的全局变量 

int Framecolor = 0, Lettercolor = 0, iflineColor=0;	 //用于看颜色选择按钮是否被按下 
extern double winwidth, winheight;


void displaycolorchoice()
{
	double fH = GetFontHeight();
	double h=fH*1.5; 
	double x = winwidth;
	double y = winheight;
	double w=1.5;
	SetPenColor("Blue");
	
	//显示文字 
	y-=1.6*h;
	drawLabel(x-=1.5,y, "      选择边框颜色");
	drawLabel(x-=1.5,y, "      选择文字颜色");
	drawLabel(x-=1.5,y, "          是否填充");
	drawLabel(x-=1.5,y, "      选择线条颜色");
	
	//颜色选择按钮 
	x = winwidth, y = winheight-h;
	if (button(GenUIID(0), x-=1.5,y, w, h, Text.textframe)){
		Framecolor = !Framecolor; Lettercolor = 0; iflineColor = 0;
	}
	if (button(GenUIID(0), x-=1.5,y, w, h, Text.textletter)){
		Framecolor = 0; Lettercolor = !Lettercolor; iflineColor = 0;
	}
	if (button(GenUIID(0), x-=1.5,y, w, h, Text.whetherfill ? "是":"否")){
		Text.whetherfill = !Text.whetherfill; //是否填充用 0 1 表示就行 
	}
	if (button(GenUIID(0), x-=1.5,y, w, h, lineColor)){
		Framecolor = 0; Lettercolor = 0;iflineColor = !iflineColor;
	}
	//如果有颜色选择的按钮被按下，执行这个函数 改变颜色 
	if( Framecolor || Lettercolor || iflineColor )//省略==1
	 {
		changecolor();
	}
}

void changecolor()
{
	double x = winwidth, y = winheight;
	char to_color[100];  //用这个变量暂存一下鼠标点击选择的颜色，并且如果有点击 让 n 等于1 
	int i,n=0;
	//一个表示颜色中文，一个表示颜色英文 
	char color_ch[][5]={"黑色","蓝色","红色","绿色","灰色","橘色","黄色","棕色","白色","青色","紫色","品红","浅灰","深灰"};
	char color_en[][12]={"Black","Blue","Red","Green","Gray","Orange","Yellow","Brown","White","Cyan","Violet","Magenta","Light Gray","Dark Gray"};
	
	for(i-0;i<sizeof(color_ch)/sizeof(color_ch[0]);i++)//因为是有规律的所以改成循环 
	if(button(GenUIID(i), x-0.5, y-1-i*0.25, 0.5, 0.25, color_ch[i]))
	{
		strcpy(to_color, color_en[i]);
		n=1;
	}
	//如果有点击 嵌套if判断是点了哪一个颜色选择按钮 
	if( n==1 ){
		if( Framecolor == 1 )//边框 
		{
			strcpy(Text.textframe, to_color);		
		}
		else if( Lettercolor == 1 )//文字
		{
			strcpy(Text.textletter, to_color);	
		} 
	    else //线 
	    {
			strcpy(lineColor, to_color);
		} 
	} 
	
}
