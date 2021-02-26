

//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "WarpBrush.h"

#include <iostream>

extern float frand();

WarpBrush::WarpBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void WarpBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();

	previous = source;

	glPointSize((float)1);

	Gaussian_filter=new double*[size];
	double sigma = dlg->getWarpStrength();
	// sum is for normalization 
	double sum = 0.0;
	int i, j;



	for (i = 0; i < size; i++) {
		Gaussian_filter[i] = new double[size];
		for (j = 0; j < size; j++) {
			// using gaussian function to generate gaussian filter 

			Gaussian_filter[i][j] = exp(-(pow((i-size/2)*1.0,2.0) + pow((j - size / 2) * 1.0, 2.0)) / (2 * sigma * sigma)) / (M_PI * sigma * sigma * 2);
			sum += Gaussian_filter[i][j];
		}
	}

	//// normalising 
	//for (i = 0; i < size; i++) {
	//	for (j = 0; j < size; j++) {
	//		Gaussian_filter[i][j] /= sum;
	//	}
	//}
	//for (i = 0; i < size; i++) {
	//	for (j = 0; j < size; j++) {
	//		std::cout<<Gaussian_filter[i][j] << ' ';
	//	}
	//}
	BrushMove(source, target);
}

__inline double bilinear(double a, double b, int uv, int u1v, int uv1, int u1v1)
{
	return (double)(uv * (1 - a) * (1 - b) + u1v * a * (1 - b) + uv1 * b * (1 - a) + u1v1 * a * b);
}

void WarpBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("WarpBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize();
	int dx = target.x - previous.x;
	int dy = target.y - previous.y;
	double stD = sqrt(pow(dx, 2.0) + pow(dy, 2.0));
	int count = 0;
	glBegin(GL_POINTS);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			int x = target.x - size / 2 + i;
			int y = target.y - size / 2 + j;
			double d1 = sqrt(pow((x - previous.x) * 1.0, 2.0) + pow((y - previous.y) * 1.0, 2.0));
			double d2 = sqrt(pow((x - target.x) * 1.0, 2.0) + pow((y - target.y) * 1.0, 2.0));
			
			//double horD = d * cos(theta1 - theta2)-stD;
			//if (horD > 0)
			//{
			//	double verD = sqrt(pow(stD, 2.0) - pow(horD, 2.0));
			//	if (verD < d * sin(theta1 - theta2))
			//		continue;
			//}


			if (d1 > size * 3.0 || d2 > size/2)
				continue;
			double pre_x = x - dx * Gaussian_filter[i][j] / Gaussian_filter[size/2][size/2];
			double pre_y = y - dy * Gaussian_filter[i][j] / Gaussian_filter[size / 2][size / 2];
			int x1 = (int)pre_x;
			int y1 = (int)pre_y;

			GLubyte colors[3];
			for (int t = 0; t < 3; t++)
			{
				double color = bilinear(pre_x - x1, pre_y - y1,
					pDoc->m_ucPainting[(pDoc->m_nWidth * y1 + x1) * 3 + t],
					pDoc->m_ucPainting[(pDoc->m_nWidth * y1 + x1 + 1) * 3 + t],
					pDoc->m_ucPainting[(pDoc->m_nWidth * (y1 + 1) + x1) * 3 + t],
					pDoc->m_ucPainting[(pDoc->m_nWidth * (y1 + 1) + x1 + 1) * 3 + t]);
				if (color > 255)
					colors[t] = 255;
				else
					colors[t] = color;
			}

			glColor3ubv(colors);
			glVertex2d(x, y);
		}
	}
	glEnd();

	previous = target;
}

void WarpBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}


