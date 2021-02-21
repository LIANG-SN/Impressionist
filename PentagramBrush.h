#ifndef PENTAGRAMBRUSH_H
#define PENTAGRAMBRUSH_H

#include "ImpBrush.h"

class PentagramBrush : public ImpBrush
{
public:
	PentagramBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif