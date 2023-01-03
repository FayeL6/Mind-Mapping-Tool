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
extern int newload;

extern linkedlistADT g_box , g_tail ;
extern linkedlistADT f_line , f_tail ;
extern int number_control;
extern int link_main[100],link_sub[100];
extern char link_color[100][10];
extern int line_num,box_num,link_num;
extern char memo[100][100];

//模板导图保存
void filesavetemplate(FILE *fp)  
{
	double fH = GetFontHeight();
	double h = fH*2; 
	int i,j;
	
	if (fp==NULL) {
		drawBox(0, 0, winwidth, h/1.5, 1, "保存失败!",'L' , "White");
	}
	fprintf(fp,"%d",1); fprintf(fp,"\n");//输入思维导图是模板绘制（1）还是自由绘图 （2）
	if(isLogicGraphic) fputs("isLogicGraphic",fp);
	if(isStructGraphic) fputs("isStructGraphic",fp);
	if(isFishBone) fputs("isFishBone",fp);
	fprintf(fp,"\n");
	
	fprintf(fp,"%d %d\n",main_topic,sub_topic);
	
	if (strcmp(contents[0],"")==0){
		fputs("##########################################################",fp);fputs("\n",fp);
	}
	else	fprintf(fp,"%s\n",contents[0]);
	for(i=1;i<=main_topic;i++){
         if (strcmp(contents[i],"")==0){
		      fputs("##########################################################",fp);fputs("\n",fp);
	     }
	     else	fprintf(fp,"%s\n",contents[i]);
	}
	for(i=1;i<=sub_topic;i++){
		if (strcmp(contents[4+i],"")==0){
		fputs("##########################################################",fp);fputs("\n",fp);
	}
	else	fprintf(fp,"%s\n",contents[4+i]);
	}
	
	if(fclose(fp)){
		drawBox(0, 0, winwidth, h/1.5, 1, "保存失败!",'L' , "White");
	}
	else {
		drawBox(0, 0, winwidth, h/1.5, 1, "保存成功!",'L' , "White");
	} 
	
}

void filesavepicture(FILE *fp)//自由绘图保存 
{
	linkedlistADT poly=g_box,poly2,poly_line=f_line;
	int i,t=1;
	double fH = GetFontHeight();
	double h = fH*2; 
	
	if (fp==NULL) {
		drawBox(0, 0, winwidth, h/1.5, 1, "保存失败!",'L' , "White");
	}
	fprintf(fp,"%d",2); fprintf(fp,"\n");//输入思维导图是模板绘制（1）还是自由绘图 （2）
	//保存线的个数以及线的位置
	fprintf(fp,"%d\n",line_num);
	if(NextNode(f_line,f_line))
	{
		linePoint * p =(linePoint*)malloc(sizeof(linePoint));
		
		while(poly_line=NextNode(f_line,poly_line))
		{
			p = (linePoint*) NodeObj(f_line, poly_line);
			fprintf(fp,"%lf %lf %d %s\n",p->x,p->y,p->n,p->Linecolor);
		}
	}
	 
	//将连线的个数以及所连方框数字存入 
	fprintf(fp,"%d\n",link_num);
	for(i=0;i<link_num;i++)
	{
		fprintf(fp,"%d %d %s\n",link_main[i],link_sub[i],link_color[i]);
	}
	
	//将方框个数，方框的坐标和文字存进去 
	fprintf(fp,"%d\n",box_num);
	poly2=NextNode(g_box,g_box);
	if(NextNode(g_box,poly2)&&poly2)
	{
		Point * p1 =(Point*)malloc(sizeof(Point));
		Point * p2 =(Point*)malloc(sizeof(Point));
		
		while(poly=NextNode(g_box,poly))
		{
			p1 = (Point*) NodeObj(g_box, poly);
			if(poly=NextNode(g_box,poly))
			{
				p2 = (Point*) NodeObj(g_box, poly);
				if(strcmp(memo[t],"")==0) //若是空则输入进去'#' 
				{
					strcpy(memo[t],"#");   //把 边框颜色 文字颜色 是否填充 放进去 看了文本文件是成功保存进去的 
					fprintf(fp,"%lf %lf %lf %lf %s %s %d %s\n",p1->x,p1->y,p2->x,p2->y,(p1->boxColor).textframe,(p1->boxColor).textletter,(p1->boxColor).whetherfill,memo[t++]);
					//fprintf(fp,"%lf %lf %lf %lf %s\n",p1->x,p1->y,p2->x,p2->y,memo[t++]);
					strcpy(memo[t-1],"");
				}
				else 
					fprintf(fp,"%lf %lf %lf %lf %s %s %d %s\n",p1->x,p1->y,p2->x,p2->y,(p1->boxColor).textframe,(p1->boxColor).textletter,(p1->boxColor).whetherfill,memo[t++]);
					//fprintf(fp,"%lf %lf %lf %lf %s\n",p1->x,p1->y,p2->x,p2->y,memo[t++]);
			}
		}
	}
	if(fclose(fp)){
		drawBox(0, 0, winwidth, h/1.5, 1, "保存失败!",'L' , "White");
	}
	else {
		drawBox(0, 0, winwidth, h/1.5, 1, "保存成功!",'L' , "White");
	} 
}
 
//自由绘图二进制保存 
void Bfilesavepicture(FILE *fp)
{
	linkedlistADT poly=g_box,poly2,poly_line=f_line;
	boxPoint box_u[100],*pu=box_u;//box_u存储文本框各个参数,pu指针指向它的首地址 
	linePoint line_u[100],*lu=line_u;
	int i,t=0,t2=0; 
	double fH = GetFontHeight();
	double h = fH*2; 
	
	if (fp==NULL) {
		drawBox(0, 0, winwidth, h/1.5, 1, "保存失败!",'L' , "White");
	}
	fprintf(fp,"%d",2); fprintf(fp,"\n");//输入思维导图是模板绘制（1）还是自由绘图 （2）
	
	//存入线的个数和位置 
	fwrite(&line_num,sizeof(int),1,fp);
	if(NextNode(f_line,f_line))
	{
		linePoint * p =(linePoint*)malloc(sizeof(linePoint));
		while(poly_line=NextNode(f_line,poly_line))
		{
			p = (linePoint*) NodeObj(f_line, poly_line);
			line_u[t2].x=p->x;
			line_u[t2].y=p->y;
			line_u[t2].n=p->n;
			strcpy(line_u[t2].Linecolor, p->Linecolor);
			t2++;
		}
	}
	fwrite(lu,sizeof(linePoint),t2,fp);
	
	//输入连线的个数和框的数字进去
	fwrite(&link_num,sizeof(int),1,fp);
	for(i=0;i<link_num;i++)
	{
		fwrite(&link_main[i],sizeof(int),1,fp);
		fwrite(&link_sub[i],sizeof(int),1,fp);
		
		fwrite(link_color[i],sizeof(link_color[i]),1,fp);//存入连线的颜色 
		
	}
	
	//将方框个数，方框的坐标和文字存进去 
	fwrite(&box_num,sizeof(int),1,fp);
	poly2=NextNode(g_box,g_box);
	if(NextNode(g_box,poly2)&&poly2)//前两个节点非空开始存储 
	{
		Point * p1 =(Point*)malloc(sizeof(Point));
		Point * p2 =(Point*)malloc(sizeof(Point));
		
		while(poly=NextNode(g_box,poly))
		{
			p1 = (Point*) NodeObj(g_box, poly);//奇数节点 
			if(poly=NextNode(g_box,poly))
			{
				p2 = (Point*) NodeObj(g_box, poly);//偶数节点 
				
				//存入box_u[]这个结构数组里面一起存进去 
				box_u[t].x1=p1->x;
				box_u[t].y1=p1->y;
				box_u[t].x2=p2->x;
				box_u[t].y2=p2->y;
				box_u[t].boxColor = p1->boxColor;
				strcpy(box_u[t].str,memo[t+1]);//字符是从memo[1]开始存 
				t++;
			}
		}
	}
	fwrite(pu,sizeof(boxPoint),t,fp);//从首地址开始输出t个值
	
	if(fclose(fp)){
		drawBox(0, 0, winwidth, h/1.5, 1, "保存失败!",'L' , "White");
	}
	else {
		drawBox(0, 0, winwidth, h/1.5, 1, "保存成功!",'L' , "White");
	} 
}
