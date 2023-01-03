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
	Colors boxColor;	//在文字框的点里加上表示文字框颜色数据的结构变量 
} Point;

typedef struct{
	double x, y;
	int n;
	char Linecolor[100];	//加上线的颜色变量 
} linePoint;

typedef struct{
	double x1, y1;
	double x2, y2;
	char str[100];
	Colors boxColor;
} boxPoint;//存储一组关于box左下右上坐标以及里面的数，方便二进制存取 

extern linkedlistADT g_box , g_tail ;
extern linkedlistADT f_line , f_tail ;
extern Point g_mousepoint;
extern Colors Text;
extern char lineColor[100];

int number_control=0;
int link_main[100],link_sub[100];//文本框1和文本框2的连线 为何区分main,sub?画弧线时候偏的方向不同 

char link_color[100][10];//加了连线的颜色 

int link_num=0;//目前文本框间连线的数量 
int box_num=0;//现在用这个表示文本框的数量，便于读取时候循环 
int line_num=0;//表示线的个数 
int boxpoint = 1;

void drawBlankDrawingButtons()
{

    double fh=GetFontHeight();
	double x=winwidth/20;
	double y=winheight/8;
	double w=TextStringWidth("清空屏幕")*1.5;
	double h=fh*2; 
	
	char s1[10],s2[10];
	
	//画文本框按钮 ，画线按钮 
	if ( button(GenUIID(0), x, y, w, h, g_add_point ? "停止" : "画文本框") )
	{
		g_add_point = !g_add_point;
		f_add_point = 0;
		}
	if ( button(GenUIID(0), x+=w, y, w, h, f_add_point ? "停止" : "画线") )
	{
		f_add_point = !f_add_point;
		g_add_point=0;
	}
	
	//撤销清空按钮 
	if ( button(GenUIID(0), x+=w, y, w, h, "撤销") )//撤销按钮 
		deletepoint();
	if ( button(GenUIID(0), x+=w, y, w, h, "清空屏幕") )//清空屏幕按钮 
	{
		ClearAll();
	}
	

	//连接方框，显示数字按钮 
	if ( button(GenUIID(0), x+=w, y, w, h, number_control?"隐藏框号":"显示框号") )number_control=!number_control;//显示框号按钮 
	
	//画子主文本框号提示以及文本框 
	x=winwidth/20,y-=(h+fh);//换行 
	drawLabel(x,y+fh*0.7," 主文本框号:");//+fh*0.7让其对齐 
	x+=w;
	textbox(GenUIID(0),x+fh/2,y,w-fh,h,txt1,sizeof(txt1));//读入要连的文本框1 
	
	//画子文本框号提示以及文本框 
	drawLabel(x+=w,y+fh*0.7," 子文本框号:");//+fh*0.7让其对齐 
	x+=w;
	textbox(GenUIID(0),x+fh/2,y,w-fh,h,txt2,sizeof(txt2));//读入要连的文本框2 
	
	//画连接方框按钮 
	if ( button(GenUIID(0),x+=w,y,w,h,"连接方框") )
	{
		sscanf(txt1,"%d",&link_main[link_num]);
		sscanf(txt2,"%d",&link_sub[link_num]);
	
		strcpy(link_color[link_num],lineColor); //加了一个连线的颜色 
		
		if(link_main[link_num]&&link_sub[link_num])link_num++;//如果未输入则不读 
		
		g_add_point=0,f_add_point=0; 
	} 
	
	//画撤销连接方框按钮 
	if ( button(GenUIID(0),x+=w,y,w,h,"撤销连接") )
	{
		link_main[link_num]=0;
		link_sub[link_num]=0;
		strcpy(link_color[link_num],"Black");
		if(link_num>0)link_num--;
	}
	
	//画打开保存和返回按钮
	x+=w*1.1,y=winheight/8,w*=0.8;
	if(button(GenUIID(0),x,y,w,h,"打开")) 
	{
		start_choice=3;
		OpenMemory=2;
		
		g_add_point=0,f_add_point=0; //防止离开页面还在绘制 
		
	}
	if(button(GenUIID(0),x+w,y,w,h,"保存"))
	{
		mode_choice=2;
		start_choice=4;
		OpenMemory=2;
		
		g_add_point=0,f_add_point=0; //防止离开页面还在绘制 
		
	}
	if(button(GenUIID(0),x,y-=(fh+h),w*2,h,"返回"))
	{
		start_choice=0;
		
		g_add_point=0,f_add_point=0; //防止离开页面还在绘制 
		
	}
	drawbbox();
	drawlline();
	link_box();
}

//画文本框   		//在每一个textbox前加上 setTextBoxColors 把指针p里的Colors结构变量里的数据放上去就行 
void drawbbox()
{
	linkedlistADT bbox = g_box;
	Point *p;
	
	double temp,x1,y1,x2,y2;
	int t = 1;//注意memo[1]开始存放文本 
	char ch[5]; 
	
	SetPenSize(1); 
	if (NextNode(g_box,g_box)) //第一个节点非空
	{
		while(bbox = NextNode(g_box, bbox))//开始循环 
		{
			p = (Point*) NodeObj(g_box, bbox);
    		x1=p->x;
    		y1=p->y;//奇节点的坐标 
    		if(!(bbox = NextNode(g_box, bbox)))//偶节点还没画情况 
    		{
    		 	if(g_add_point == 1)
    			{
    				setTextBoxColors(p->boxColor.textframe, p->boxColor.textletter, "Red", "Red", p->boxColor.whetherfill);
    				textbox(GenUIID(0), x1, y1, mouse_x - x1, mouse_y - y1, memo[0], 100);//画跟随鼠标的框 
    			}
    			break;
    		}
    		else
    		{
    			Point * q = (Point*) NodeObj(g_box, bbox);
				x2=q->x;
    			y2=q->y;//偶节点的坐标
    			
    			//保证x1,y1是最小的 
				if(x2<x1)temp=x2,x2=x1,x1=temp;
				if(y2<y1)temp=y1,y1=y2,y2=temp;
				
				p->x=x1,p->y=y1,q->x=x2,q->y=y2;//交换结束再存回去 
				
				setTextBoxColors(p->boxColor.textframe, p->boxColor.textletter, "Red", "Red", p->boxColor.whetherfill);
				textbox(GenUIID(t),x1, y1, x2-x1, y2-y1, memo[t++], 100);
				
				//画数字 
				sprintf(ch,"%d",t-1);
				if(number_control)
					drawLabel(x1,y2,ch); 
    		}
		}
	}
	box_num=t-1;//文本框总数 
	setTextBoxColors("Blue", "Blue", "Red", "Red", 0);
}

//画线		//在movepen前加上SetPenColor就行，与文字框不同 不是结构变量而是字符串，所以定义了一个局部变量用strcpy保存点里的颜色数据 
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


//box与line加点 
void AddboxPoint(double x, double y, Colors ttext)
{
	Point * p = malloc(sizeof(Point));
	p->x = x;
	p->y = y;
	p->boxColor = ttext;		//把颜色数据加进来 
	g_tail = InsertNode(g_box, g_tail, p);
}

void AddlinePoint(double x, double y, int n)
{
	linePoint * p = (linePoint*)malloc(sizeof(linePoint));
	p->x = x;
	p->y = y;
	p->n = n;
	strcpy(p->Linecolor,lineColor);		//把颜色数据加进来 
	line_num++;
	f_tail = InsertNode(f_line, f_tail, p);
}

void deletepoint()
{
	//撤销画线 
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
	
	//撤销画文本框 
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
	
	//撤销画序号连线
	//link_num-- ;
}

//画框和框之间的连线 
void link_box()
{
	int i=0,angle,p=1;
	double x1,y1,x2,y2,x3,y3,x4,y4;
	double w1,w2,h1,h2;
	double lx1,ly1,lx2,ly2;//画线的开始结束坐标 
	double lw,lh;
	
	for(i=0;i<link_num;i++)
	{
		//读入这两个方框左下和右上的坐标 得出方框的宽和高 
		Point *p1=(Point*)malloc(sizeof(Point)),*p2=(Point*)malloc(sizeof(Point));
		Point *p3=(Point*)malloc(sizeof(Point)),*p4=(Point*)malloc(sizeof(Point));
		p1=(Point*)ithNodeobj(g_box,link_main[i]*2-1);
		p2=(Point*)ithNodeobj(g_box,link_main[i]*2);
		p3=(Point*)ithNodeobj(g_box,link_sub[i]*2-1);
		p4=(Point*)ithNodeobj(g_box,link_sub[i]*2);
		//第一个 
		x1=p1->x,y1=p1->y;
		x2=p2->x,y2=p2->y;
		w1=x2-x1,h1=y2-y1;
		//第二个 
		x3=p3->x,y3=p3->y;
		x4=p4->x,y4=p4->y;
		w2=x4-x3,h2=y4-y3;
		
		//让lx1,ly1,lx2,ly2变为要画的起点和终点 并且lx1,ly1是主要的框，区分原因？弧线方向有不同 
		//前提是两者不重合——可以画的时候判断一下  
		
		//2框全部在1框左侧 
		if(x4<x1)
		{
			lx1=x1,lx2=x4,ly1=y1+h1/2,ly2=y3+h2/2;
			angle=0;
			p=((ly2>ly1)?1:-1);
		}
		//2框全部在1框右侧 
		else if(x2<x3)
		{
			lx1=x2,lx2=x3,ly1=y1+h1/2,ly2=y3+h2/2;
			angle=180;
			p=((ly2>ly1)?-1:1);
		}
		else//2框与1框一部分横向交错 
		{
			//2框在1框上面 
			if(y3>y2)
			{
				lx1=x1+w1/2,lx2=x3+w2/2,ly1=y2,ly2=y3;
				angle=270;
				p=((lx2>lx1)?1:-1);
			}
			//2框在1框下面 
			else 
			{
				lx1=x1+w1/2,lx2=x3+w2/2,ly1=y1,ly2=y4;
				angle=90;
				p=((lx2>lx1)?-1:1);
			}
		}
		
		lw=((lx2-lx1>0)?lx2-lx1:lx1-lx2);
		lh=((ly2-ly1>0)?ly2-ly1:ly1-ly2);
		
		//开始画
		
		SetPenColor(link_color[i]); //连线的颜色 
		
		MovePen(lx1,ly1);
		DrawEllipticalArc(lw,lh,angle,p*90);
		//DrawLine(lx2-lx1,ly2-ly1);
	}
}

//清空所有链表以及内容 
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
