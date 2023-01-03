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

extern double  winwidth, winheight;
extern int main_topic,sub_topic;
extern char contents[100][100];
extern int fail_to_draw;
double Fixx;

//鱼骨图绘制 
void drawfishbone()
{
	double fH = GetFontHeight();
	double h = fH*2; 
	double x = 0.7;  
	double y = (winheight+1)/2; 
	double w = winwidth/6;

	int i,j,k;
	int flag;//表明上下 
    int sub[6]={0,0,0,0,0,0};
	int remaincount=sub_topic;
	
	MovePen(x,y);
	DrawArc(1.5*w,135,-45);
	Fixx=GetCurrentX();
	DrawLine(0,-2*(1-sqrt(2)/2)*1.5*w);
	DrawArc(1.5*w,-90,-45);
	MovePen(Fixx,y);
	DrawLine(winwidth*3/5,0);
	DrawLine(w/3,w/2);
	DrawLine(0,-w);
	DrawLine(-w/3,w/2);
	
	if(fail_to_draw==0){
		for(i=1;i<=main_topic;i++){
			flag=(i%2==0)?-1:1;
			MovePen(Fixx-0.5*w+winwidth*3/5/(main_topic+1)*i,y);
			DrawLine(w/3,w/2*flag);
		}
		for(i=1;i<=main_topic;i++){
		    if(remaincount*1.0/(main_topic+1-i)>1) sub[i]=2;
	     	else if(remaincount*1.0/(main_topic+1-i)>0) sub[i]=1;
		    remaincount=remaincount-sub[i];
	    }
		for(i=1;i<=main_topic;i++){
			flag=(i%2==0)?-1:1;
			k=0;
			for(j=1;j<=i;j++) k=k+sub[j];
			if(sub[i]==1){
				MovePen(Fixx-0.5*w+winwidth*3/5/(main_topic+1)*i+w/3+0.4*h,y+w/2*flag+flag*0.6*h);
				DrawLine(w/3,w/2*flag);
			}
			if(sub[i]==2){
				MovePen(Fixx-0.5*w+winwidth*3/5/(main_topic+1)*i+0.4*h+w/3,y+w/2*flag+0.6*h*flag);
				DrawLine(w/3,2*w/3*flag);
				DrawLine(-w/4,-w/2*flag);
				DrawLine(w/4,w/9*flag);
			}
		}		
	}		
}

void drawfishboneText()
{
	double fH = GetFontHeight();
	double h = fH*2; 
	double x = 0.7;  
	double y = (winheight+1)/2; 
	double w = winwidth/6;

	int i,j,k;
	int flag;//表明上下 
    int sub[6]={0,0,0,0,0,0};
	int remaincount=sub_topic;
	
	setTextBoxColors("White", "Blue", "White","Red", 0);
	textbox(GenUIID(0),x+0.2*w,y-0.6*h,0.8*w,1.2*h,contents[0],sizeof(contents[0]));
	setTextBoxColors("Blue", "Blue", "Red","Red", 0);
	
	if(fail_to_draw==0){
		for(i=1;i<=main_topic;i++){
			flag=(i%2==0)?-1:1;
			textbox(GenUIID(i),Fixx-0.5*w+winwidth*3/5/(main_topic+1)*i+w/3,y+w/2*flag-0.6*h,w,1.2*h,contents[i],sizeof(contents[i]));
		}
		for(i=1;i<=main_topic;i++){
		    if(remaincount*1.0/(main_topic+1-i)>1) sub[i]=2;
	     	else if(remaincount*1.0/(main_topic+1-i)>0) sub[i]=1;
		    remaincount=remaincount-sub[i];
	    }
		for(i=1;i<=main_topic;i++){
			flag=(i%2==0)?-1:1;
			k=0;
			for(j=1;j<=i;j++) k=k+sub[j];
			if(sub[i]==1){
				textbox(GenUIID(k),Fixx-0.5*w+winwidth*3/5/(main_topic+1)*i+0.4*h+2*w/3,y+w*flag+flag*0.6*h-0.5*h,0.8*w,h,contents[4+k],sizeof(contents[4+k]));
			}
			if(sub[i]==2){
				textbox(GenUIID(k-1),Fixx-0.5*w+winwidth*3/5/(main_topic+1)*i+0.4*h+w/3+w/3,y+7*w/6*flag+flag*0.6*h-0.5*h,0.8*w,h,contents[4+k-1],sizeof(contents[4+k-1]));
				textbox(GenUIID(k),Fixx-0.5*w+winwidth*3/5/(main_topic+1)*i+0.4*h+w/3+w/3,y+flag*0.6*h+7*w/9*flag-0.5*h,0.8*w,h,contents[4+k],sizeof(contents[4+k]));
			}
		}		
	}	
}
