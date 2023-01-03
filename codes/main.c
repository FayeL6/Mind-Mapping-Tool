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

#include "CanvasRect.h"
#include "template.h"
#include "blankdrawing.h"
#include "filehandling.h"
#include "color.h"


double winwidth, winheight;   	// 窗口尺寸
int start_choice=0; 			//开始时页面选择 
int mode_choice=0;				//思维导图模块选择 
int OpenMemory=0;				//记录打开页面的进入页面 ，改成第二个字母大小写了，全部小写连着看不清，或者open_memory 
int show_enter_buttons=0;		//控制开始绘制按钮的切换 
double mouse_x=0,mouse_y=0;
int g_add_point = 0;
int f_add_point = 0;
extern Colors Text;

linkedlistADT g_box = NULL, g_tail = NULL;
linkedlistADT f_line = NULL, f_tail = NULL;
Point g_mousepoint;

// 用户的显示函数
void DisplayClear(void); 
void display(void); 
void drawMenu(void);
void drawstartbuttons(void);
void drawenterbuttons();

// 用户的字符事件响应函数
void CharEventProcess(char ch)
{
	uiGetChar(ch); // GUI字符输入
	display(); //刷新显示
}

// 用户的键盘事件响应函数
void KeyboardEventProcess(int key, int event)
{
	uiGetKeyboard(key,event); // GUI获取键盘
	display(); // 刷新显示
}

// 用户的鼠标事件响应函数
void myMouseEvent( int x, int y, int button, int event)
{
	mouse_x = ScaleXInches(x);
	mouse_y = ScaleYInches(y);
	uiGetMouse(x, y, button, event);
	
	if( InCanvasRect(mouse_x, mouse_y) )
	{
		g_mousepoint.x = mouse_x;
		g_mousepoint.y = mouse_y;
		switch(event)
		{
			case BUTTON_UP:
				if(button==LEFT_BUTTON && g_add_point){
					AddboxPoint(g_mousepoint.x, g_mousepoint.y, Text);
				}
				if(button==LEFT_BUTTON && f_add_point){      //区分左键右键加入链表 
					AddlinePoint(g_mousepoint.x, g_mousepoint.y, 1);
				}
				if(button==RIGHT_BUTTON && f_add_point){
					AddlinePoint(g_mousepoint.x, g_mousepoint.y, 0);
				}
				break;
			case MOUSEMOVE:
				break;
		}
	}
	display();
}

void Main()
{
	SetWindowTitle("思维导图");
	InitGraphics();
	SetWindowSize(15, 10);
	
	registerMouseEvent(myMouseEvent);
	registerKeyboardEvent(KeyboardEventProcess);
	registerCharEvent(CharEventProcess);
	
	winwidth = GetWindowWidth();
    winheight = GetWindowHeight();
	
	g_box = NewLinkedList();
	f_line = NewLinkedList();
	
	//InitConsole();
}

void display()
{
	// 清屏
	DisplayClear();
	drawMenu();
	
	switch(start_choice)//换成switch，更加符合选择界面的设定，也简洁 
	{
		case 0://开始页面
		{
			drawstartbuttons();
			break;
		}
		case 1://模板图页面 
		{
			displayCanvasRect();
			drawTemplateButtons();
			break;
		}
		case 2://空白绘图页面 
		{	
			displayCanvasRect();
			drawBlankDrawingButtons();
			displaycolorchoice();
			break;
		}
		case 3://文件打开页面
		{
			drawFileOpenButtons();
			break;
		}
		case 4://文件保存页面
		{
			drawFileSaveButtons();
			break;
		}
	}
}

//画菜单 
void drawMenu()
{ 
	static char * menuListFile[] = {"文件",  
		"打开  | Ctrl-O", // 快捷键必须采用[Ctrl-X]格式，放在字符串的结尾
		"保存  | Ctrl-S",
		"返回  | Ctrl-E"};
	static char * menuListHelp[] = {"帮助",
		"使用说明  | Ctrl-M"};
	static char * selectedLabel = NULL;

	double fH = GetFontHeight();
	double x = 0; //fH/8;
	double y = winheight;
	double h = fH*1.5; // 控件高度
	double w = TextStringWidth(menuListHelp[0])*2; // 控件宽度
	double wlist = TextStringWidth(menuListHelp[1])*1.2;
	double xindent = winheight/20; // 缩进
	int    selection;
	
	//画菜单边框 
	drawMenuBar(0,y-h,winwidth,h);
	
	// File 菜单
	selection = menuList(GenUIID(0), x, y-h, w, wlist, h, menuListFile, sizeof(menuListFile)/sizeof(menuListFile[0]));

	//这里也一样用switch
	switch(selection)
	{
		case 1://跳到打开界面
		{
			OpenMemory=start_choice;
			start_choice=3;
			break;
		}
		case 2://跳到保存界面
		{
			mode_choice=start_choice;
			if (mode_choice==1 || mode_choice==2) 	
			{
				OpenMemory=start_choice;
				start_choice=4;
			}
			else
			{
				drawBox(0, 0, winwidth, h, 1, "请在绘制界面点击保存！",'L' , "White");
			}
			
			break;
		}
		case 3://点击返回，不同情况具体分析 ，但是不能做到连续返回，不如改成第二次返回则返回首页 
		{
			switch(start_choice) 
			{
				case 4:
				case 3://在打开、保存界面则回到上级 
				{
					start_choice=OpenMemory;
					OpenMemory=0; 
					break;
				}
				default://在其他界面，则返回开始界面 
				{
					start_choice=0,show_enter_buttons=0;
					break;
				}
			}
			break;
		}
	 }
	// Help 菜单
	selection = menuList(GenUIID(0),x+w,y-h, w, wlist, h, menuListHelp,sizeof(menuListHelp)/sizeof(menuListHelp[0]));
	if(selection==1) 
	{
		switch(start_choice)//不同界面使用说明也不同 
		{
			case 0:	{
				system("start 开始菜单使用说明.txt");//
				break;
			}
			case 1:{
				system("start 模板绘制界面说明.txt");
				break;
			}
			case 2:{
				system("start 自由绘制界面说明.txt");
				break;
			}
			case 3:{
				system("start 打开界面说明.txt");
				break;
			}
			case 4:{
				system("start 保存界面说明.txt");
				break;
			}
			default:break; 
		}

	}

}

//画按钮 
void drawstartbuttons()
{
	double fH = GetFontHeight();
	double h = fH*2; 
	double x = winwidth/3;  
	double y = winheight/2+h; 
	double w = TextStringWidth("开始绘制")*3;
	
	//切换 开始绘制 按钮与两个绘图按钮 
	if(!show_enter_buttons)
	if (button(GenUIID(0), x, y, w, h, "开始绘制"))
	{
		show_enter_buttons=!show_enter_buttons; 
	} 
	
	//退出 
	if (button(GenUIID(0), x, y-3*h, w, h, "退出")) exit(-1);
	if(show_enter_buttons) drawenterbuttons();
}
void drawenterbuttons()
{
	double fH = GetFontHeight();
	double h = fH*2; 
	double x = winwidth/3;  
	double y = winheight/2+h; 
	double w = TextStringWidth("开始绘制")*3;
	
	if (button(GenUIID(0), x, y+h, w, h, "套用模板")) start_choice=1; 
	if (button(GenUIID(0), x, y-h, w, h, "空白绘图")) start_choice=2;
}

