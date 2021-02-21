
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



	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			int Gx = 0, Gy = 0;
			int x = source.x - size / 2 + i;
			int y = source.y - size / 2 + j;
			pDoc->getGradientOfPoint(x, y, Gx, Gy);

			GLubyte temp[4];
			memcpy(temp, pDoc->GetOriginalPixel(x, y), 3);
			temp[3] = GLubyte(255 * pDoc->getAlpha());

			if (Gx > 100 || Gy > 100) {
				double h, s, v;
				double temp2[4];

				dlg->m_ColorChooser->rgb2hsv(temp[0] / 255, temp[1] / 255, temp[2] / 255, h, s, v);
				v += strength * 5 / 255;
				v = ((int)(v * 255) % 255) / 255;
				dlg->m_ColorChooser->rgb2hsv(h, s, v, temp2[0], temp2[1], temp2[2]);

				GLubyte colors[4];
				colors[0] = temp2[0] * pDoc->getRed();
				colors[1] = temp2[1] * pDoc->getGreen();
				colors[2] = temp2[2] * pDoc->getBlue();
				colors[3] = GLubyte(255 * pDoc->getAlpha());
				glColor4ubv(colors);
			}
			else
				glColor4ubv(temp);

			glBegin(GL_POINTS);
			glVertex2d(target.x - size / 2 + i, target.y - size / 2 + j);
			glEnd();
		}
	}


	


}
