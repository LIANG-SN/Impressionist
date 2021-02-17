#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatterPointBrush.h"

extern float frand();

ScatterPointBrush::ScatterPointBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatterPointBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;


	glPointSize((float)0.4);

	BrushMove(source, target);
}

void ScatterPointBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	int size = pDoc->getSize();

	// TODU: add a slider to change the likelihood?
	glBegin(GL_POINTS);
	for (int x = -size/2; x < size/2; x += 1)
	{
		for (int y = - size / 2; y < size / 2; y += 1)
		{
			Point scatter(source.x + x, source.y + y);
			SetColor(scatter);
			int likelihood = rand() % 5;
			if(likelihood <= 1) // likelihood = 2 / 5
				glVertex2d(target.x + x, target.y + y);
		}
	}
	glEnd();
}

void ScatterPointBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

