#include "graphics.h"
#include <windows.h>
#include <winuser.h>

double canvasRect[4] = {0.5, 5.5, 0.5, 5.5};

//±³¾°¿ò 
int InCanvasRect(double x, double y)
{
	return (x >= canvasRect[0] && x <= canvasRect[1] &&
			y >= canvasRect[2] && y <= canvasRect[3] );
}

void SetCanvasRect()
{
	double w = GetWindowWidth();
	double h = GetWindowHeight();
	canvasRect[0] = 0.5;
	canvasRect[1] = w-0.5;
	canvasRect[2] = 1.5;
	canvasRect[3] = h - 0.5;
}

void displayCanvasRect()
{
	SetCanvasRect();
	double dx = canvasRect[1] - canvasRect[0];
	double dy = canvasRect[3] - canvasRect[2];
	SetPenColor("Blue");
	MovePen(canvasRect[0],canvasRect[2]);
	DrawLine(dx,0);
	DrawLine(0,dy);
	DrawLine(-dx,0);
	DrawLine(0,-dy);
}

