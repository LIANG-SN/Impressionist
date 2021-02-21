#ifndef BLURSHARPBRUSH_H
#define BLURSHARPBRUSH_H

#include "ImpBrush.h"

class BlurSharpBrush : public ImpBrush
{
public:
	BlurSharpBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void BlurMove(const Point source, const Point target);
	void SharpenMove(const Point source, const Point target);
	char* BrushName(void);
};

#endif