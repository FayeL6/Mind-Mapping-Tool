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
extern int line_num,box_num,link_num;

extern char link_color[100][10];

extern char memo[100][100];


//���ļ��ĵ�ͼ����ѡ�� 
void fileopenchoice(FILE *fp) 
{
	double fH = GetFontHeight();
	double h = fH*2; 
		
	if (fp==NULL) {
		drawBox(0, 0, winwidth, h/1.5, 1, "��ʧ��!",'L' , "White");
	}
	else{
		newload=1;
	    fscanf(fp,"%d\n",&mode_choice);
	    if (mode_choice==1) {
		   fileopentemplate(fp);
	    }
	    if (mode_choice==2) {
	    	fileopenpicture(fp);
	    }
	}	
}

void Bfileopenchoice(FILE *fp) //�������ļ���ѡ�� 
{
	double fH = GetFontHeight();
	double h = fH*2; 
	
	if (fp==NULL) {
		drawBox(0, 0, winwidth, h/1.5, 1, "��ʧ��!",'L' , "White");
	}
	else{
		newload=1;
	    fscanf(fp,"%d\n",&mode_choice);
	    if (mode_choice==1) {
		   fileopentemplate(fp);
	    }
	    if (mode_choice==2) {
	    	Bfileopenpicture(fp);
	    }
	}	
}

//ģ�嵼ͼ�� 
void fileopentemplate(FILE *fp)
{
	double fH = GetFontHeight();
	double h = fH*2; 
	char mode[100];
	int i,j;
	
	fscanf(fp,"%s\n",&mode);
	if (strcmp("isLogicGraphic",mode)==0){
		isLogicGraphic=1;isStructGraphic=0;isFishBone=0;
	}
	if (strcmp("isStructGraphic",mode)==0){
		isLogicGraphic=0;isStructGraphic=1;isFishBone=0;
	}
	if (strcmp("isFishBone",mode)==0){
		isLogicGraphic=0;isStructGraphic=0;isFishBone=1;
	}
	
	fscanf(fp,"%d %d\n",&main_topic,&sub_topic);
	
	fscanf(fp,"%s\n",&contents[0]);
	if(strcmp(contents[0],"##########################################################")==0) strcpy(contents[0],"");

	for(i=1;i<=main_topic;i++){
		fscanf(fp,"%[^\n]\n",&contents[i]);
		if(strcmp(contents[i],"##########################################################")==0) strcpy(contents[i],"");
	}
	for(i=1;i<=sub_topic;i++){
		fscanf(fp,"%[^\n]\n",&contents[i+4]);
		if(strcmp(contents[4+i],"##########################################################")==0) strcpy(contents[4+i],"");
	}
	if(fclose(fp)){
		drawBox(0, 0, winwidth, h/1.5, 1, "��ʧ��!",'L' , "White");
	}
	start_choice=1;
}
//���ı�������ͼ 
void fileopenpicture(FILE *fp)
{
	double x1,y1,x2,y2;
	Point * p1=(Point*)malloc(sizeof(Point)),*p2=(Point*)malloc(sizeof(Point)); 
	int i,t=1;
	char str[3]="#"; 
	double fH = GetFontHeight();
	double h = fH*2; 
	Colors ttext;
	
	ClearAll();
	
	//�����ߵĸ��� 
	fscanf(fp,"%d\n",&line_num);
	linePoint * p;
	int n;  
	double x,y;
	char ccolor[100];
	for(i=0;i<line_num;i++)
	{
		fscanf(fp,"%lf %lf %d %s\n",&x,&y,&n,ccolor);
		p = (linePoint*)malloc(sizeof(linePoint));
		p->x = x;
		p->y = y;
		p->n = n;
		strcpy(p->Linecolor, ccolor);
		f_tail = InsertNode(f_line, f_tail, p);
	}
	
	//���뻭�ߵĸ����Լ������ı�������Щ 
	fscanf(fp,"%d\n",&link_num);
	for(i=0;i<link_num;i++)
	{
		fscanf(fp,"%d %d %s",&link_main[i],&link_sub[i],link_color[i]);
	}
	
	//�����ı���λ���Լ����� 
	fscanf(fp,"%d\n",&box_num);
	while(!feof(fp))
	{	//������� ��ɫ����� ����Text�Text ��Colorsȫ�ֱ���������������ֿ����ɫ���ݣ�����������������ں���ļӵ㣬�ӵ㺯������p��boxColor����Text 
		if (box_num==0) break;
		fscanf(fp,"%lf %lf %lf %lf %s %s %d %[^\n]\n",&x1,&y1,&x2,&y2,ttext.textframe,ttext.textletter,&ttext.whetherfill,memo[t++]);
		if(strcmp(memo[t-1],"#")==0) //�������'#'���ɿ� 
		{
			strcpy(memo[t-1],"");
		}
		
		AddboxPoint(x1,y1,ttext);
		AddboxPoint(x2,y2,ttext);
	}
	
	if(fclose(fp)){
		drawBox(0, 0, winwidth, h/1.5, 1, "��ʧ��!",'L' , "White");
	}
	start_choice=2;
}

void Bfileopenpicture(FILE *fp)//�򿪶��������ɻ�ͼ 
{
	double x1,y1,x2,y2;
	Point * p1=(Point*)malloc(sizeof(Point)),*p2=(Point*)malloc(sizeof(Point));
	int i,t=1; 
	boxPoint box_u[100],*pu=box_u;
	linePoint line_u[100],*pl=line_u;
	double fH = GetFontHeight();
	double h = fH*2;
	Colors ttext;
	
	//ClearAll();
	
	//�����ߵĸ��� 
	fread(&line_num,sizeof(int),1,fp);
	fread(pl,sizeof(linePoint),line_num,fp);
	linePoint * p;
	for(i=0;i<line_num;i++)
	{
		p = (linePoint*)malloc(sizeof(linePoint));
		p->x=line_u[i].x ;
		p->y=line_u[i].y ;
		p->n=line_u[i].n ;
		strcpy(p->Linecolor,line_u[i].Linecolor);
		f_tail = InsertNode(f_line, f_tail, p);
	}
	
	//���뻭�ߵĸ����Լ������ı�������Щ 
	fread(&link_num,sizeof(int),1,fp);
	for(i=0;i<link_num;i++)
	{
		fread(&link_main[i],sizeof(int),1,fp);
		fread(&link_sub[i],sizeof(int),1,fp);
		fread(link_color[i],sizeof(link_color[0]),1,fp);
	}
	
	//�����ı���������ı���λ���Լ����� 
	fread(&box_num,sizeof(int),1,fp);
	fread(pu,sizeof(boxPoint),box_num,fp);//�ȶ���ĩβ��֮����Լ���һ������ȷ�������ٸ� 
	
	for(i=0;i<box_num;i++)
	{
		x1=box_u[i].x1;
		y1=box_u[i].y1;
		x2=box_u[i].x2;
		y2=box_u[i].y2;
		ttext = box_u[i].boxColor;
		strcpy(memo[i+1],box_u[i].str);
		AddboxPoint(x1,y1,ttext);
		AddboxPoint(x2,y2,ttext);
	}
	
	if(fclose(fp)){
		drawBox(0, 0, winwidth, h/1.5, 1, "��ʧ��!",'L' , "White");
	}
	start_choice=2;
}
