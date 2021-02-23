#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "AlphaMappedBrush.h"

extern float frand();

AlphaMappedBrush::AlphaMappedBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void AlphaMappedBrush::BrushBegin(const Point source, const Point target)
{


	glPointSize((float)0.4);

	BrushMove(source, target);
}

void AlphaMappedBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	if (pDoc->m_alphaMappedBitmap == NULL) {
		printf("No alpha mapped image.\n");
		return;
	}



	int w = pDoc->m_alphaMapedWidth;
	int h = pDoc->m_alphaMappedHeight;
	double a = 0.5;
	// TODU: add a slider to change the likelihood?
	glBegin(GL_POINTS);
	for (int x = 0; x < w; x += 1)
	{
		for (int y = 0; y < h; y += 1)
		{
			
			unsigned char  alphaColor[3];
			memcpy(alphaColor, pDoc->GetAlphaMappedPixel(x, y), 3);
			double a = (alphaColor[0] + alphaColor[1] + alphaColor[2]) / 765.0f;

			GLubyte color[4];
			memcpy(color, pDoc->GetOriginalPixel(source), 3);

			color[3] = GLubyte(255 *a);
			glColor4ubv(color);
			//glColor3ubv(temp);
			glVertex2d(target.x - w / 2 + x, target.y - h / 2 + y);

		}
	}
	glEnd();
}

void AlphaMappedBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

