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


double winwidth, winheight;   	// ���ڳߴ�
int start_choice=0; 			//��ʼʱҳ��ѡ�� 
int mode_choice=0;				//˼ά��ͼģ��ѡ�� 
int OpenMemory=0;				//��¼��ҳ��Ľ���ҳ�� ���ĳɵڶ�����ĸ��Сд�ˣ�ȫ��Сд���ſ����壬����open_memory 
int show_enter_buttons=0;		//���ƿ�ʼ���ư�ť���л� 
double mouse_x=0,mouse_y=0;
int g_add_point = 0;
int f_add_point = 0;
extern Colors Text;

linkedlistADT g_box = NULL, g_tail = NULL;
linkedlistADT f_line = NULL, f_tail = NULL;
Point g_mousepoint;

// �û�����ʾ����
void DisplayClear(void); 
void display(void); 
void drawMenu(void);
void drawstartbuttons(void);
void drawenterbuttons();

// �û����ַ��¼���Ӧ����
void CharEventProcess(char ch)
{
	uiGetChar(ch); // GUI�ַ�����
	display(); //ˢ����ʾ
}

// �û��ļ����¼���Ӧ����
void KeyboardEventProcess(int key, int event)
{
	uiGetKeyboard(key,event); // GUI��ȡ����
	display(); // ˢ����ʾ
}

// �û�������¼���Ӧ����
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
				if(button==LEFT_BUTTON && f_add_point){      //��������Ҽ��������� 
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
	SetWindowTitle("˼ά��ͼ");
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
	// ����
	DisplayClear();
	drawMenu();
	
	switch(start_choice)//����switch�����ӷ���ѡ�������趨��Ҳ��� 
	{
		case 0://��ʼҳ��
		{
			drawstartbuttons();
			break;
		}
		case 1://ģ��ͼҳ�� 
		{
			displayCanvasRect();
			drawTemplateButtons();
			break;
		}
		case 2://�հ׻�ͼҳ�� 
		{	
			displayCanvasRect();
			drawBlankDrawingButtons();
			displaycolorchoice();
			break;
		}
		case 3://�ļ���ҳ��
		{
			drawFileOpenButtons();
			break;
		}
		case 4://�ļ�����ҳ��
		{
			drawFileSaveButtons();
			break;
		}
	}
}

//���˵� 
void drawMenu()
{ 
	static char * menuListFile[] = {"�ļ�",  
		"��  | Ctrl-O", // ��ݼ��������[Ctrl-X]��ʽ�������ַ����Ľ�β
		"����  | Ctrl-S",
		"����  | Ctrl-E"};
	static char * menuListHelp[] = {"����",
		"ʹ��˵��  | Ctrl-M"};
	static char * selectedLabel = NULL;

	double fH = GetFontHeight();
	double x = 0; //fH/8;
	double y = winheight;
	double h = fH*1.5; // �ؼ��߶�
	double w = TextStringWidth(menuListHelp[0])*2; // �ؼ����
	double wlist = TextStringWidth(menuListHelp[1])*1.2;
	double xindent = winheight/20; // ����
	int    selection;
	
	//���˵��߿� 
	drawMenuBar(0,y-h,winwidth,h);
	
	// File �˵�
	selection = menuList(GenUIID(0), x, y-h, w, wlist, h, menuListFile, sizeof(menuListFile)/sizeof(menuListFile[0]));

	//����Ҳһ����switch
	switch(selection)
	{
		case 1://�����򿪽���
		{
			OpenMemory=start_choice;
			start_choice=3;
			break;
		}
		case 2://�����������
		{
			mode_choice=start_choice;
			if (mode_choice==1 || mode_choice==2) 	
			{
				OpenMemory=start_choice;
				start_choice=4;
			}
			else
			{
				drawBox(0, 0, winwidth, h, 1, "���ڻ��ƽ��������棡",'L' , "White");
			}
			
			break;
		}
		case 3://������أ���ͬ���������� �����ǲ��������������أ�����ĳɵڶ��η����򷵻���ҳ 
		{
			switch(start_choice) 
			{
				case 4:
				case 3://�ڴ򿪡����������ص��ϼ� 
				{
					start_choice=OpenMemory;
					OpenMemory=0; 
					break;
				}
				default://���������棬�򷵻ؿ�ʼ���� 
				{
					start_choice=0,show_enter_buttons=0;
					break;
				}
			}
			break;
		}
	 }
	// Help �˵�
	selection = menuList(GenUIID(0),x+w,y-h, w, wlist, h, menuListHelp,sizeof(menuListHelp)/sizeof(menuListHelp[0]));
	if(selection==1) 
	{
		switch(start_choice)//��ͬ����ʹ��˵��Ҳ��ͬ 
		{
			case 0:	{
				system("start ��ʼ�˵�ʹ��˵��.txt");//
				break;
			}
			case 1:{
				system("start ģ����ƽ���˵��.txt");
				break;
			}
			case 2:{
				system("start ���ɻ��ƽ���˵��.txt");
				break;
			}
			case 3:{
				system("start �򿪽���˵��.txt");
				break;
			}
			case 4:{
				system("start �������˵��.txt");
				break;
			}
			default:break; 
		}

	}

}

//����ť 
void drawstartbuttons()
{
	double fH = GetFontHeight();
	double h = fH*2; 
	double x = winwidth/3;  
	double y = winheight/2+h; 
	double w = TextStringWidth("��ʼ����")*3;
	
	//�л� ��ʼ���� ��ť��������ͼ��ť 
	if(!show_enter_buttons)
	if (button(GenUIID(0), x, y, w, h, "��ʼ����"))
	{
		show_enter_buttons=!show_enter_buttons; 
	} 
	
	//�˳� 
	if (button(GenUIID(0), x, y-3*h, w, h, "�˳�")) exit(-1);
	if(show_enter_buttons) drawenterbuttons();
}
void drawenterbuttons()
{
	double fH = GetFontHeight();
	double h = fH*2; 
	double x = winwidth/3;  
	double y = winheight/2+h; 
	double w = TextStringWidth("��ʼ����")*3;
	
	if (button(GenUIID(0), x, y+h, w, h, "����ģ��")) start_choice=1; 
	if (button(GenUIID(0), x, y-h, w, h, "�հ׻�ͼ")) start_choice=2;
}

