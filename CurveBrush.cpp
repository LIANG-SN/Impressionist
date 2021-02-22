//
// CurveBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "CurveBrush.h"
#include <math.h>
#include <string>
#define PI acos(-1)

extern float frand();

CurveBrush::CurveBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void CurveBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();
	glPointSize(size);

	
	BrushMove(source, target);
}

void CurveBrush::BrushMove(const Point source, const Point target)
{
	// debug 
	using namespace std;
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("CurveBrush::BrushMove  document is NULL\n");
		return;
	}

	float radius = (float)pDoc->getSize() / 2;
	glBegin(GL_POINTS);
	SetColor(source);
	glVertex2d(target.x, target.y);
	int x = target.x, y = target.y;
	double lastDx = 0, lastDy = 0;
	GLubyte* startPix = pDoc->GetOriginalPixel(source);
	int brushValue = 0.299 * startPix[0] + 0.587 * startPix[1] + 0.114 * startPix[2];
	for (int i = 1; i <= pDoc->m_pUI->getMaxStrokeLength(); i++)
	{
		int Gx = 0, Gy = 0;
		int gradientMag = pDoc->getGradientOfPoint(x, y, Gx, Gy);
		GLubyte* originPix = pDoc->GetOriginalPixel(x, y);
		GLubyte* paintPix = pDoc->GetPaintingPixel(x, y);

		int originValue = 0.299 * originPix[0] + 0.587 * originPix[1] + 0.114 * originPix[2];
		int paintValue = 0.299 * paintPix[0] + 0.587 * paintPix[1] + 0.114 * paintPix[2];
		if (i > pDoc->m_pUI->getMinStrokeLength() &&
			abs(originValue - paintValue) < abs(originValue - brushValue))
			break;
		if (gradientMag == 0)
			break;
		double dx = -Gy, dy = Gx;
		if (lastDx * dx + lastDy * dy < 0)
		{
			dx *= -1; dy *= -1;
		}
		float fc = pDoc->m_pUI->getCurveFilter();
		dx = fc * dx + (1 - fc) * lastDx;  dy = fc * dy + (1 - fc) * lastDy;
		dx /= sqrt(dx * dx + dy * dy); dy /= sqrt(dx * dx + dy * dy); // normalize
		x += (int)(radius * dx); y += (int)(radius * dy);
		lastDx = dx; lastDy = dy;

		glVertex2d(x, y);
	}
	glEnd();
}

void CurveBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}