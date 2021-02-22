#pragma once

#include "ImpBrush.h"

class CurveBrush : public ImpBrush
{
public:
	CurveBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);

};
