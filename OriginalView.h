//
// originalview.h
//
// The header file for original view of the input images
//

#ifndef ORIGINALVIEW_H
#define ORIGINALVIEW_H

#include <FL/Fl.H>

#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdlib.h>

#include "ImpBrush.h"

class ImpressionistDoc;

enum
{
	SHOW_ORIGIN_IMAGE = 0,
	SHOW_ANOTHER_IMAGE,
	SHOW_EDGE_IMAGE,
};
class OriginalView : public Fl_Gl_Window
{
public:
	OriginalView(int x, int y, int w, int h, const char *l);
	
	void draw();
	void refresh();

	void resizeWindow(int width, int height);

	void setMarker(const Point cursor) { marker = cursor; redraw(); }

	void showImageChoice(int choice) { imageShow = choice; redraw(); }
	
	ImpressionistDoc*	m_pDoc;

private:
	int	m_nWindowWidth, 
		m_nWindowHeight;
	Point marker;
	int imageShow{ SHOW_ORIGIN_IMAGE };

};

#endif