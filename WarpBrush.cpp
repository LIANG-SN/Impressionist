

//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "WarpBrush.h"

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

	//Gaussian_filter=new double*[size];
	//double sigma = dlg->getWarpStrength();
	//// sum is for normalization 
	//double sum = 0.0;
	//int i, j;



	//for (i = 0; i < size; i++) {
	//	Gaussian_filter[i] = new double[size];
	//	for (j = 0; j < size; j++) {
	//		// using gaussian function to generate gaussian filter 
	//		if (sqrt(pow(i - size / 2, 2.0) + pow(j - size / 2, 2.0)) > size / 2)
	//			Gaussian_filter[i][j] = 0;
	//		else
	//			Gaussian_filter[i][j] = exp(-(i * i + j * j) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
	//		sum += Gaussian_filter[i][j];
	//	}
	//}

	//// normalising the Kernel 
	//for (i = 0; i < size; i++) {
	//	for (j = 0; j < size; j++) {
	//		Gaussian_filter[i][j] /= sum;
	//	}
	//}

	BrushMove(source, target);
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
	double dx = target.x - previous.x;
	double dy = target.y - previous.y;
	double d = sqrt(pow(dx, 2.0) + pow(dy, 2.0));
	int count = 0;
	glBegin(GL_POINTS);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (sqrt(pow((i - size / 2)*1.0, 2.0) + pow((j - size / 2)*1.0, 2.0)) > size / 2)
				continue;
			int x = target.x - size / 2 + i;
			int y = target.y - size / 2 + j;
			GLubyte color[3];
			memcpy(color, pDoc->GetPaintingPixel(previous.x - size / 2 + i -dx, previous.y - size / 2 + j -dy), 3);
			memcpy(color, pDoc->GetPaintingPixel(previous.x - size / 2 + i -dx, previous.y - size / 2 + j -dy), 3);
			glColor3ubv(color);
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


