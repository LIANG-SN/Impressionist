#ifndef IMPBRUSH_H
#define IMPBRUSH_H

//
// ImpBrush.h
//
// The header file of virtual brush. All the other brushes inherit from it.
//

#include <stdlib.h>

// Each brush type has an associated constant.
enum
{
	BRUSH_POINTS = 0,
	BRUSH_LINES,
	BRUSH_CIRCLES,
	BRUSH_SCATTERED_POINTS,
	BRUSH_SCATTERED_LINES,
	BRUSH_SCATTERED_CIRCLES,
	BRUSH_PENTAGRAM,
	BRUSH_BLURORSHARPEN,
	BRUSH_CURVE,
	BRUSH_ALPHA_MAPPED,
	BRUSH_WARP,
	NUM_BRUSH_TYPE // Make sure this stays at the end!
};

// the direction select mode of line brushes
enum
{
	SLIDER_RIGHT_CLICK = 0,
	GRADIENT,
	BRUSH_DIRECTION,
	ANOTHER_GRADIENT,
	NUM_DIRECTION_TYPE
};

enum
{
	BLURRING = 0,
	SHARPENING,
	NUM_BLURSHARP_BRUSH
};

class ImpressionistDoc; // Pre-declaring class

class Point 
{
public:
	Point() {};
	Point(int xx, int yy) { x = xx; y = yy; };

	int x, y;
};

class ImpBrush 
{
protected:
	ImpBrush::ImpBrush( ImpressionistDoc* pDoc = NULL, char* name = NULL );

public:
	// The implementation of your brush should realize these virtual functions
	virtual void BrushBegin( const Point source, const Point target) = 0;
	virtual void BrushMove( const Point source, const Point target ) = 0;
	virtual void BrushEnd( const Point source, const Point target ) = 0;

	virtual void rightMouseBegin(const Point source, const Point target) {};
	virtual void rightMouseMove(const Point source, const Point target) {};
	virtual void rightMouseEnd(const Point source, const Point target) {};
	// according to the source image and the position, determine the draw color
	void SetColor( const Point source );

	// get Doc to communicate with it
	ImpressionistDoc* GetDocument( void );

	// Return the name of the brush (not used in this version).
	char* BrushName( void );
	
	static int			c_nBrushCount;	// How many brushes we have,
	static ImpBrush**	c_pBrushes;		// and what they are.
	static int			lineDirectionChoice;
	static int			BlurSharpBrushChoice;

private:
	ImpressionistDoc*	m_pDoc;

	// Brush's name (not used in this version).
	char*				m_pBrushName;
};

#endif