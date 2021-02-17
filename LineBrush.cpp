//
// LineBrush.cpp
//
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "LineBrush.h"
#include <cmath>
#include <iostream>
extern float frand();

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void LineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;


	int width = pDoc->getLineWidth();


	glLineWidth((float)width);

	BrushMove(source, target);
}

void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	int size = pDoc->getSize(); // use size control the length of line
	double angle;
	switch (lineDirectionChoice)
	{
	case SLIDER_RIGHT_CLICK:
		angle = pDoc->getLineAngle();
		break;
	case BRUSH_DIRECTION:
		angle = pDoc->brushMoveAngle;
		break;
	case GRADIENT:
		int sobel_x[3][3] =
		{
			{ 1, 0, -1 },
			{ 2, 0, -2 },
			{ 1, 0, -1 }
		};
		int Gx = 0, Gy = 0;
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				GLubyte* pixel = pDoc->GetOriginalPixel(source.x + i, source.y + j);
				// formula from tutorial doc page 19
				int pixelValue = 0.299 * pixel[0] + 0.587 * pixel[1] + 0.114 * pixel[2];
				Gx += sobel_x[i + 1][j + 1] * pixelValue;
				Gy += sobel_x[j + 1][i + 1] * pixelValue;
			}
		}
		angle = atan2(Gx, Gy) / M_PI / 2.0 * 360; // prependicular to gradient

	}
	double rad = angle / 360.0 * 2 * M_PI;
	double x_length = size * cos(rad);
	double y_length = size * sin(rad);
	glBegin(GL_LINES);
	SetColor(source); // set color of source(original paint) to gl
	glVertex2d(target.x - x_length / 2, target.y - y_length / 2);
	glVertex2d(target.x + x_length / 2, target.y + y_length / 2);

	glEnd();
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

void LineBrush::rightMouseBegin(const Point start, const Point target)
{
	glLineWidth(0.5f);
	glColor3f(1, 0, 0);
	rightMouseMove(start, target);
}

void LineBrush::rightMouseMove(const Point start, const Point target)
{
	if (GetDocument() == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	glBegin(GL_LINES);

	glVertex2d(start.x, start.y);
	glVertex2d(target.x, target.y);

	glEnd();
}

void LineBrush::rightMouseEnd(const Point source, const Point target)
{
}