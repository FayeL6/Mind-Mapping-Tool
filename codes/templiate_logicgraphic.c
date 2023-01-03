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


//Âß¼­Í¼»æÖÆ 
void drawlogicgraphic()
{
	double fH = GetFontHeight();
	double h = fH*2; 
	double x = 0.7;  
	double y = (winheight+1)/2; 
	double w = winwidth/4.5;
	int i,j,k;
    int sub[6]={0,0,0,0,0,0};
	int remaincount=sub_topic;
	

	MovePen(x+w,y);
	DrawLine(w/3,0);
	DrawLine(0,winheight/4);
	DrawLine(0,-winheight/2);
	
	if(fail_to_draw==0){
		for(i=1;i<=main_topic;i++){
			MovePen(x+w+w/3,y+winheight/4-winheight/(2*main_topic-2)*(i-1));
			DrawLine(w/3,0);
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
				MovePen(x+w+2*w/3+0.8*w,y+winheight/4-winheight/(2*main_topic-2)*(i-1));
				DrawLine(w/3,0);
			}
			if(sub[i]==2){
				MovePen(x+w+2*w/3+0.8*w,y+winheight/4-winheight/(2*main_topic-2)*(i-1));
				DrawLine(w/3,0);
				DrawLine(0,winheight/24);
				DrawLine(0,-winheight/12);
				MovePen(x+w+w+0.8*w,y+winheight/4-winheight/(2*main_topic-2)*(i-1)+winheight/24);
				DrawLine(w/3,0);
				MovePen(x+w+w+0.8*w,y+winheight/4-winheight/(2*main_topic-2)*(i-1)-winheight/24);
				DrawLine(w/3,0);
			}
		}		
	}		
}

void drawlogicgraphicText()
{
	double fH = GetFontHeight();
	double h = fH*2; 
	double x = 0.7;  
	double y = (winheight+1)/2; 
	double w = winwidth/4.5;
	int i,j,k;
    int sub[6]={0,0,0,0,0,0};
	int remaincount=sub_topic;
	
	textbox(GenUIID(0),x,y-h,w,2*h,contents[0],sizeof(contents[0]));
	if(fail_to_draw==0){
		for(i=1;i<=main_topic;i++){
			textbox(GenUIID(i),x+w+2*w/3,y+winheight/4-winheight/(2*main_topic-2)*(i-1)-0.6*h,0.8*w,1.2*h,contents[i],sizeof(contents[i]));
		}
		for(i=1;i<=main_topic;i++){
		    if(remaincount*1.0/(main_topic+1-i)>1) sub[i]=2;
	     	else if(remaincount*1.0/(main_topic+1-i)>0) sub[i]=1;
		    remaincount=remaincount-sub[i];
	    }
	    for(i=1;i<=main_topic;i++){
			k=0;
			for(j=1;j<=i;j++) k=k+sub[j];
		    if (sub[i]==1){
			    textbox(GenUIID(k),x+w+w+0.8*w,y+winheight/4-winheight/(2*main_topic-2)*(i-1)-0.5*h,0.6*w,h,contents[4+k],sizeof(contents[4+k]));
		    }
		    if (sub[i]==2){
			    textbox(GenUIID(k-1),x+w+w+0.8*w+w/3,y+winheight/4-winheight/(2*main_topic-2)*(i-1)+winheight/24-0.5*h,0.6*w,h,contents[4+k-1],sizeof(contents[4+k-1]));
			    textbox(GenUIID(k),x+w+w+0.8*w+w/3,y+winheight/4-winheight/(2*main_topic-2)*(i-1)-winheight/24-0.5*h,0.6*w,h,contents[4+k],sizeof(contents[4+k]));
		    }
		} 
	}
}

