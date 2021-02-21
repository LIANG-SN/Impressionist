

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "PentagramBrush.h"
#include <cmath>


extern float frand();

PentagramBrush::PentagramBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void PentagramBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	

	BrushMove(source, target);
}

void PentagramBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	double R = pDoc->getSize();
	double r = R / 2.62;

	double s18 = sin(18 * M_PI / 180 );
	double c18 = cos(18 * M_PI / 180 );
	double s54 = sin(54 * M_PI / 180 );
	double c54 = cos(54 * M_PI / 180 );

	glBegin(GL_TRIANGLE_FAN);
	SetColor(source); // set color of source(original paint) to gl

	glVertex2d(target.x, target.y);
	glVertex2d(target.x, target.y + R);
	glVertex2d(target.x + r * c54, target.y + r * s54);
	glVertex2d(target.x + R * c18, target.y + R * s18);
	glVertex2d(target.x + r * c18, target.y - r * s18);
	glVertex2d(target.x + R * c54, target.y - R * s54);
	glVertex2d(target.x, target.y - r);
	glVertex2d(target.x - R * c54, target.y - R * s54);
	glVertex2d(target.x - r * c18, target.y - r * s18);
	glVertex2d(target.x - R * c18, target.y + R * s18);
	glVertex2d(target.x - r * c54, target.y + r * s54);
	glVertex2d(target.x, target.y + R);
	glEnd();
}

void PentagramBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

