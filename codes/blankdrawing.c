#include <windows.h>
#include <winuser.h>
#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"
#include "linkedlist.h"
#include "color.h"

extern start_choice,OpenMemory,mode_choice;
extern double mouse_x, mouse_y;
extern double winwidth,winheight;

extern int g_add_point;
extern int f_add_point;
int g_delete_point = 0;

char memo[100][100];
char txt1[100],txt2[100];

typedef struct{
	double x, y;
	Colors boxColor;	//�����ֿ�ĵ�����ϱ�ʾ���ֿ���ɫ���ݵĽṹ���� 
} Point;

typedef struct{
	double x, y;
	int n;
	char Linecolor[100];	//�����ߵ���ɫ���� 
} linePoint;

typedef struct{
	double x1, y1;
	double x2, y2;
	char str[100];
	Colors boxColor;
} boxPoint;//�洢һ�����box�������������Լ������������������ƴ�ȡ 

extern linkedlistADT g_box , g_tail ;
extern linkedlistADT f_line , f_tail ;
extern Point g_mousepoint;
extern Colors Text;
extern char lineColor[100];

int number_control=0;
int link_main[100],link_sub[100];//�ı���1���ı���2������ Ϊ������main,sub?������ʱ��ƫ�ķ���ͬ 

char link_color[100][10];//�������ߵ���ɫ 

int link_num=0;//Ŀǰ�ı�������ߵ����� 
int box_num=0;//�����������ʾ�ı�������������ڶ�ȡʱ��ѭ�� 
int line_num=0;//��ʾ�ߵĸ��� 
int boxpoint = 1;

void drawBlankDrawingButtons()
{

    double fh=GetFontHeight();
	double x=winwidth/20;
	double y=winheight/8;
	double w=TextStringWidth("�����Ļ")*1.5;
	double h=fh*2; 
	
	char s1[10],s2[10];
	
	//���ı���ť �����߰�ť 
	if ( button(GenUIID(0), x, y, w, h, g_add_point ? "ֹͣ" : "���ı���") )
	{
		g_add_point = !g_add_point;
		f_add_point = 0;
		}
	if ( button(GenUIID(0), x+=w, y, w, h, f_add_point ? "ֹͣ" : "����") )
	{
		f_add_point = !f_add_point;
		g_add_point=0;
	}
	
	//������հ�ť 
	if ( button(GenUIID(0), x+=w, y, w, h, "����") )//������ť 
		deletepoint();
	if ( button(GenUIID(0), x+=w, y, w, h, "�����Ļ") )//�����Ļ��ť 
	{
		ClearAll();
	}
	

	//���ӷ�����ʾ���ְ�ť 
	if ( button(GenUIID(0), x+=w, y, w, h, number_control?"���ؿ��":"��ʾ���") )number_control=!number_control;//��ʾ��Ű�ť 
	
	//�������ı������ʾ�Լ��ı��� 
	x=winwidth/20,y-=(h+fh);//���� 
	drawLabel(x,y+fh*0.7," ���ı����:");//+fh*0.7������� 
	x+=w;
	textbox(GenUIID(0),x+fh/2,y,w-fh,h,txt1,sizeof(txt1));//����Ҫ�����ı���1 
	
	//�����ı������ʾ�Լ��ı��� 
	drawLabel(x+=w,y+fh*0.7," ���ı����:");//+fh*0.7������� 
	x+=w;
	textbox(GenUIID(0),x+fh/2,y,w-fh,h,txt2,sizeof(txt2));//����Ҫ�����ı���2 
	
	//�����ӷ���ť 
	if ( button(GenUIID(0),x+=w,y,w,h,"���ӷ���") )
	{
		sscanf(txt1,"%d",&link_main[link_num]);
		sscanf(txt2,"%d",&link_sub[link_num]);
	
		strcpy(link_color[link_num],lineColor); //����һ�����ߵ���ɫ 
		
		if(link_main[link_num]&&link_sub[link_num])link_num++;//���δ�����򲻶� 
		
		g_add_point=0,f_add_point=0; 
	} 
	
	//���������ӷ���ť 
	if ( button(GenUIID(0),x+=w,y,w,h,"��������") )
	{
		link_main[link_num]=0;
		link_sub[link_num]=0;
		strcpy(link_color[link_num],"Black");
		if(link_num>0)link_num--;
	}
	
	//���򿪱���ͷ��ذ�ť
	x+=w*1.1,y=winheight/8,w*=0.8;
	if(button(GenUIID(0),x,y,w,h,"��")) 
	{
		start_choice=3;
		OpenMemory=2;
		
		g_add_point=0,f_add_point=0; //��ֹ�뿪ҳ�滹�ڻ��� 
		
	}
	if(button(GenUIID(0),x+w,y,w,h,"����"))
	{
		mode_choice=2;
		start_choice=4;
		OpenMemory=2;
		
		g_add_point=0,f_add_point=0; //��ֹ�뿪ҳ�滹�ڻ��� 
		
	}
	if(button(GenUIID(0),x,y-=(fh+h),w*2,h,"����"))
	{
		start_choice=0;
		
		g_add_point=0,f_add_point=0; //��ֹ�뿪ҳ�滹�ڻ��� 
		
	}
	drawbbox();
	drawlline();
	link_box();
}

//���ı���   		//��ÿһ��textboxǰ���� setTextBoxColors ��ָ��p���Colors�ṹ����������ݷ���ȥ���� 
void drawbbox()
{
	linkedlistADT bbox = g_box;
	Point *p;
	
	double temp,x1,y1,x2,y2;
	int t = 1;//ע��memo[1]��ʼ����ı� 
	char ch[5]; 
	
	SetPenSize(1); 
	if (NextNode(g_box,g_box)) //��һ���ڵ�ǿ�
	{
		while(bbox = NextNode(g_box, bbox))//��ʼѭ�� 
		{
			p = (Point*) NodeObj(g_box, bbox);
    		x1=p->x;
    		y1=p->y;//��ڵ������ 
    		if(!(bbox = NextNode(g_box, bbox)))//ż�ڵ㻹û����� 
    		{
    		 	if(g_add_point == 1)
    			{
    				setTextBoxColors(p->boxColor.textframe, p->boxColor.textletter, "Red", "Red", p->boxColor.whetherfill);
    				textbox(GenUIID(0), x1, y1, mouse_x - x1, mouse_y - y1, memo[0], 100);//���������Ŀ� 
    			}
    			break;
    		}
    		else
    		{
    			Point * q = (Point*) NodeObj(g_box, bbox);
				x2=q->x;
    			y2=q->y;//ż�ڵ������
    			
    			//��֤x1,y1����С�� 
				if(x2<x1)temp=x2,x2=x1,x1=temp;
				if(y2<y1)temp=y1,y1=y2,y2=temp;
				
				p->x=x1,p->y=y1,q->x=x2,q->y=y2;//���������ٴ��ȥ 
				
				setTextBoxColors(p->boxColor.textframe, p->boxColor.textletter, "Red", "Red", p->boxColor.whetherfill);
				textbox(GenUIID(t),x1, y1, x2-x1, y2-y1, memo[t++], 100);
				
				//������ 
				sprintf(ch,"%d",t-1);
				if(number_control)
					drawLabel(x1,y2,ch); 
    		}
		}
	}
	box_num=t-1;//�ı������� 
	setTextBoxColors("Blue", "Blue", "Red", "Red", 0);
}

//����		//��movepenǰ����SetPenColor���У������ֿ�ͬ ���ǽṹ���������ַ��������Զ�����һ���ֲ�������strcpy����������ɫ���� 
void drawlline()
{
	double w = 1.0;
	double h = GetFontHeight()*2;
	int t;
	SetPenColor("Black"); 
	linkedlistADT lline = NextNode(f_line, f_line);
	if (lline) {
		linePoint * p = (linePoint*) NodeObj(f_line, lline);
		double lx = p->x;
		double ly = p->y;
		char ccolor[100];
		t = p->n; 
		strcpy(ccolor, p->Linecolor);
	Here:
		SetPenColor(ccolor);
		MovePen(lx, ly);
		while (lline = NextNode(f_line, lline))
		{
			p = (linePoint*) NodeObj(f_line, lline);
			t = p->n;
			if(p->n==1){
				DrawLine(p->x - lx, p->y - ly);
				lx = p->x;
				ly = p->y;
			}else{
				if( lline = NextNode(f_line, lline) ){
					p = (linePoint*) NodeObj(f_line, lline);
					lx = p->x;
		   	    	ly = p->y;
		   	    	strcpy(ccolor, p->Linecolor);
		   	    	t = p->n;
		        	goto Here;
				}
				break;
			}
		}
		if( f_add_point && t==1 ){
		//draw a line to the mouse
		SetPenColor(ccolor);
		DrawLine(g_mousepoint.x - lx, g_mousepoint.y - ly);
		}  	
	}
	SetPenColor("Black");
}


//box��line�ӵ� 
void AddboxPoint(double x, double y, Colors ttext)
{
	Point * p = malloc(sizeof(Point));
	p->x = x;
	p->y = y;
	p->boxColor = ttext;		//����ɫ���ݼӽ��� 
	g_tail = InsertNode(g_box, g_tail, p);
}

void AddlinePoint(double x, double y, int n)
{
	linePoint * p = (linePoint*)malloc(sizeof(linePoint));
	p->x = x;
	p->y = y;
	p->n = n;
	strcpy(p->Linecolor,lineColor);		//����ɫ���ݼӽ��� 
	line_num++;
	f_tail = InsertNode(f_line, f_tail, p);
}

void deletepoint()
{
	//�������� 
	if(f_add_point==1){
		linkedlistADT lline = NextNode(f_line, f_line);
		linkedlistADT aline = NextNode(f_line, f_line);
		if (lline) {
		linePoint * p = (linePoint*) NodeObj(f_line, lline);
		do{
			if(aline = NextNode(f_line, lline)){
				if(!(aline = NextNode(f_line, aline))){
					f_tail = lline;
					f_tail->next = NULL;
					break;
				}
			}else{
				FreeLinkedList(f_line);
				f_tail = NULL;
				f_line = NewLinkedList();
				break;
			}
		}while(lline = NextNode(f_line, lline));
		}	
	}
	
	//�������ı��� 
	if(g_add_point==1)
	{
		linkedlistADT bbox = NextNode(g_box, g_box);
		linkedlistADT abox = NextNode(g_box, g_box);
		if (bbox) 
		{
			Point * p = (Point*) NodeObj(g_box, bbox);
			do{
				if(abox = NextNode(g_box, bbox))
				{
					if(!(abox = NextNode(g_box, abox)))
					{
						g_tail = bbox;
						g_tail->next = NULL;
						break;
					}
				}else
				{
					FreeLinkedList(g_box);
					g_tail = NULL;
					g_box = NewLinkedList();
					break;
				}
			}while(bbox = NextNode(g_box, bbox));
		}
	}
	
	//�������������
	//link_num-- ;
}

//����Ϳ�֮������� 
void link_box()
{
	int i=0,angle,p=1;
	double x1,y1,x2,y2,x3,y3,x4,y4;
	double w1,w2,h1,h2;
	double lx1,ly1,lx2,ly2;//���ߵĿ�ʼ�������� 
	double lw,lh;
	
	for(i=0;i<link_num;i++)
	{
		//�����������������º����ϵ����� �ó�����Ŀ�͸� 
		Point *p1=(Point*)malloc(sizeof(Point)),*p2=(Point*)malloc(sizeof(Point));
		Point *p3=(Point*)malloc(sizeof(Point)),*p4=(Point*)malloc(sizeof(Point));
		p1=(Point*)ithNodeobj(g_box,link_main[i]*2-1);
		p2=(Point*)ithNodeobj(g_box,link_main[i]*2);
		p3=(Point*)ithNodeobj(g_box,link_sub[i]*2-1);
		p4=(Point*)ithNodeobj(g_box,link_sub[i]*2);
		//��һ�� 
		x1=p1->x,y1=p1->y;
		x2=p2->x,y2=p2->y;
		w1=x2-x1,h1=y2-y1;
		//�ڶ��� 
		x3=p3->x,y3=p3->y;
		x4=p4->x,y4=p4->y;
		w2=x4-x3,h2=y4-y3;
		
		//��lx1,ly1,lx2,ly2��ΪҪ���������յ� ����lx1,ly1����Ҫ�Ŀ�����ԭ�򣿻��߷����в�ͬ 
		//ǰ�������߲��غϡ������Ի���ʱ���ж�һ��  
		
		//2��ȫ����1����� 
		if(x4<x1)
		{
			lx1=x1,lx2=x4,ly1=y1+h1/2,ly2=y3+h2/2;
			angle=0;
			p=((ly2>ly1)?1:-1);
		}
		//2��ȫ����1���Ҳ� 
		else if(x2<x3)
		{
			lx1=x2,lx2=x3,ly1=y1+h1/2,ly2=y3+h2/2;
			angle=180;
			p=((ly2>ly1)?-1:1);
		}
		else//2����1��һ���ֺ��򽻴� 
		{
			//2����1������ 
			if(y3>y2)
			{
				lx1=x1+w1/2,lx2=x3+w2/2,ly1=y2,ly2=y3;
				angle=270;
				p=((lx2>lx1)?1:-1);
			}
			//2����1������ 
			else 
			{
				lx1=x1+w1/2,lx2=x3+w2/2,ly1=y1,ly2=y4;
				angle=90;
				p=((lx2>lx1)?-1:1);
			}
		}
		
		lw=((lx2-lx1>0)?lx2-lx1:lx1-lx2);
		lh=((ly2-ly1>0)?ly2-ly1:ly1-ly2);
		
		//��ʼ��
		
		SetPenColor(link_color[i]); //���ߵ���ɫ 
		
		MovePen(lx1,ly1);
		DrawEllipticalArc(lw,lh,angle,p*90);
		//DrawLine(lx2-lx1,ly2-ly1);
	}
}

//������������Լ����� 
void ClearAll()
{
	FreeLinkedList(g_box);
	g_box=NewLinkedList();
	g_tail=NULL;
	
	FreeLinkedList(f_line);
	f_line=NewLinkedList();
	f_tail=NULL;

	link_num=0;
	box_num=0;
	line_num=0;
		
	int i;
	for(i=0;i<100;i++)
	{
		strcpy(memo[i],"");
	}
	
	DisplayClear(); 
} 
