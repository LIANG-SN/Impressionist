


#include "impressionistDoc.h"
#include <iostream>

#ifndef MATTING_H
#define MATTING_H
class Matting
{
public:
	Matting(ImpressionistDoc* p);
	void solveAlpha();
	void generateNewAlphaBitmap();
	double findingDistance(int x1, int y1, int x2, int y2);

private:
	int* newAlphaBitmap = NULL;
	int width;
	int height;
	ImpressionistDoc* pDoc;
};

#endif