
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "BlurSharpBrush.h"
#include <cmath>
extern float frand();

BlurSharpBrush::BlurSharpBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void BlurSharpBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;





	BrushMove(source, target);
}

void BlurSharpBrush::BrushMove(const Point source, const Point target)
{
	
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	if (pDoc == NULL) {
		printf("BlurSharpBrush::BrushMove  document is NULL\n");
		return;
	}

	switch (BlurSharpBrushChoice)
	{
	case BLURRING:
		BlurMove(source, target);
		break;
	case SHARPENING:
		SharpenMove(source, target);
		break;
	}
}

void BlurSharpBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}


void BlurSharpBrush::BlurMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	int filter_size = pDoc->getLevel()*2-1;
	int size = pDoc->getSize();

	
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			int x = source.x - size / 2 + i - filter_size / 2;
			int y = source.y - size / 2 + j - filter_size / 2;
			double sum[3] = { 0.0,0.0,0.0 };
			int count = 0;

			for (int k = 0; k < filter_size; k++) {
				for (int w = 0; w < filter_size; w++) {
					GLubyte temp[3];
					memcpy(temp, pDoc->GetOriginalPixel(Point(x + k, y + w)), 3);
					sum[0] += temp[0]/255.0;
					sum[1] += temp[1]/255.0;
					sum[2] += temp[2]/255.0;
					count++;
				}
			}

			glBegin(GL_POINTS);

				GLubyte colors[4];
				colors[0] = GLubyte(sum[0] / count * 255);
				colors[1] = GLubyte(sum[1] / count * 255);
				colors[2] = GLubyte(sum[2] / count * 255);
				colors[0] = colors[0] * pDoc->getRed();
				colors[1] = colors[1] * pDoc->getGreen();
				colors[2] = colors[2] * pDoc->getBlue();
				colors[3] = GLubyte(255 * pDoc->getAlpha());
				glColor4ubv(colors);
				glVertex2d(target.x - size / 2 + i, target.y - size / 2 + j);

			glEnd();

		}
	}

	
}
void BlurSharpBrush::SharpenMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	int strength = pDoc->getLevel();
	int size = pDoc->getSize();

	int sobel_x[3][3] =
	{
		{ 1, 0, -1 },
		{ 2, 0, -2 },
		{ 1, 0, -1 }
	};

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			int Gx = 0, Gy = 0;
			int x = source.x - size / 2 + i;
			int y = source.y - size / 2 + j;

			for (int a = -1; a <= 1; i++)
			{
				for (int b = -1; b <= 1; j++)
				{
					GLubyte* pixel = pDoc->GetOriginalPixel(x , y);
					// formula from tutorial doc page 19
					int pixelValue = 0.299 * pixel[0] + 0.587 * pixel[1] + 0.114 * pixel[2];
					Gx += sobel_x[a + 1][b + 1] * pixelValue;
					Gy += sobel_x[b + 1][a + 1] * pixelValue;
				}
			}
		}
	}


	


}
