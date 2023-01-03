#ifndef _blankdrawing_h
#define _blankdrawing_h
#include "color.h"

typedef struct{
	double x, y;
	Colors boxColor;
} Point;

typedef struct{
	double x, y;
	int n;
	char Linecolor[100];
} linePoint;

typedef struct{
	double x1, y1;
	double x2, y2;
	char str[100];
	Colors boxColor;
} boxPoint;

void drawBlankDrawingButtons();
void drawbbox();
void drawlline();
void AddboxPoint(double x, double y, Colors ttext);
void AddlinePoint(double x, double y, int n);
void deletepoint();
void link_box();
void ClearAll();

#endif
