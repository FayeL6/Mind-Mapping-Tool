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

extern double  winwidth, winheight;
extern int main_topic,sub_topic;
extern char contents[100][100];
extern int fail_to_draw;

//Ω·ππÕºªÊ÷∆ 
void drawstructgraphic()
{
	double fH = GetFontHeight();
	double h = fH*2; 
	double x = winwidth/2;  
	double y = winheight-1.2; 
	double w = winwidth/4.5;
	int i,j,k;
    int sub[6]={0,0,0,0,0,0};
	int remaincount=sub_topic;
	
	
	MovePen(x,y-h);
	DrawLine(0,-winheight/12);
	DrawLine(-0.5*w-0.25*w*main_topic,0);
	DrawLine(w+0.5*w*main_topic,0);
	
	if(fail_to_draw==0){
		for(i=1;i<=main_topic;i++){
			MovePen(x-0.5*w-0.25*w*main_topic+(w+0.5*w*main_topic)/(main_topic-1)*(i-1),y-h-winheight/12);
			DrawLine(0,-winheight/12);
		}
		for(i=1;i<=main_topic;i++){
		    if(remaincount*1.0/(main_topic+1-i)>1) sub[i]=2;
	     	else if(remaincount*1.0/(main_topic+1-i)>0) sub[i]=1;
		    remaincount=remaincount-sub[i];
	    }
		for(i=1;i<=main_topic;i++){
			k=0;
			for(j=1;j<=i;j++) k=k+sub[j];
			if(sub[i]==1){
				MovePen(x-0.5*w-0.25*w*main_topic+(w+0.5*w*main_topic)/(main_topic-1)*(i-1),y-h-winheight/12*2-1.2*h);
				DrawLine(0,-winheight/12);
			}
			if(sub[i]==2){
				MovePen(x-0.5*w-0.25*w*main_topic+(w+0.5*w*main_topic)/(main_topic-1)*(i-1),y-h-winheight/12*2-1.2*h);
				DrawLine(0,-winheight/12);
				DrawLine(-0.25*w-0.05*w*(4-main_topic),0);
				DrawLine(0.5*w+0.1*w*(4-main_topic),0);
				MovePen(x-0.5*w-0.25*w*main_topic+(w+0.5*w*main_topic)/(main_topic-1)*(i-1)-0.25*w-0.05*w*(4-main_topic),y-h-winheight/12*3-1.2*h);
				DrawLine(0,-winheight/12);
				MovePen(x-0.5*w-0.25*w*main_topic+(w+0.5*w*main_topic)/(main_topic-1)*(i-1)+0.25*w+0.05*w*(4-main_topic),y-h-winheight/12*3-1.2*h);
				DrawLine(0,-winheight/12);
			 }
	    }   			
   }
}

void drawstructgraphicText()
{
	double fH = GetFontHeight();
	double h = fH*2; 
	double x = winwidth/2;  
	double y = winheight-1.2; 
	double w = winwidth/4.5;
	int i,j,k;
    int sub[6]={0,0,0,0,0,0};
	int remaincount=sub_topic;

	textbox(GenUIID(0),x-w/2,y-h,w,2*h,contents[0],sizeof(contents[0]));
	
	if(fail_to_draw==0){
		for(i=1;i<=main_topic;i++){
			textbox(GenUIID(i),x-0.5*w-0.25*w*main_topic+(w+0.5*w*main_topic)/(main_topic-1)*(i-1)-0.4*w,y-h-winheight/12*2-1.2*h,0.8*w,1.2*h,contents[i],sizeof(contents[i]));
		}
		for(i=1;i<=main_topic;i++){
		    if(remaincount*1.0/(main_topic+1-i)>1) sub[i]=2;
	     	else if(remaincount*1.0/(main_topic+1-i)>0) sub[i]=1;
		    remaincount=remaincount-sub[i];
	    }
		for(i=1;i<=main_topic;i++){
			k=0;
			for(j=1;j<=i;j++) k=k+sub[j];
			if(sub[i]==1){
				textbox(GenUIID(k),x-0.5*w-0.25*w*main_topic+(w+0.5*w*main_topic)/(main_topic-1)*(i-1)-0.3*w,y-h-winheight/12*3-1.2*h-h,0.6*w,h,contents[4+k],sizeof(contents[4+k]));
			}
			if(sub[i]==2){
				textbox(GenUIID(k-1),x-0.5*w-0.25*w*main_topic+(w+0.5*w*main_topic)/(main_topic-1)*(i-1)-0.45*w-0.1*w*(4-main_topic),y-h-winheight/12*4-1.2*h-h,0.4*w+0.1*w*(4-main_topic),h,contents[4+k-1],sizeof(contents[4+k-1]));
				textbox(GenUIID(k),x-0.5*w-0.25*w*main_topic+(w+0.5*w*main_topic)/(main_topic-1)*(i-1)+0.05*w,y-h-winheight/12*4-1.2*h-h,0.4*w+0.1*w*(4-main_topic),h,contents[4+k],sizeof(contents[4+k]));
			}
		}
	}				
} 

