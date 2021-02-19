//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredCircleBrush.h"
#include <cstdlib>
#include <math.h>

#define PI acos(-1)

extern float frand();

ScatteredCircleBrush::ScatteredCircleBrush(ImpressionistDoc* pDoc, char* name) :
	CircleBrush(pDoc, name)
{
}

void ScatteredCircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;



	BrushMove(source, target);
}

void ScatteredCircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredCircleBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize();
	int n = rand() % 5 + 1;


	for (int i = 0; i < n; i++) {
		int l = rand() % 5;
		int a = rand() % 50;
		CircleBrush::BrushMove(Point(source.x + (size * l / 5 / 2) * cos(a * 2.0 * PI / 50), source.y + (size * l / 5 / 2) * sin(a * 2.0 * PI / 50)),
			Point(target.x + (size * l / 5 / 2) * cos(a * 2.0 * PI / 50), target.y + (size * l / 5 / 2) * sin(a * 2.0 * PI / 50)));
	}


	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++) {
			if ((rand() % 4) == 0) {
				glBegin(GL_POINTS);
				SetColor(Point(source.x - size / 2.0 + i, source.y - size / 2.0 + j));
				glVertex2d(target.x - size / 2.0 + i, target.y - size / 2.0 + j);
				glEnd();
			}
		};




}

void ScatteredCircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}