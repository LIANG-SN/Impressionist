//
// paintview.h
//
// The header file for painting view of the input images
//

#ifndef PAINTVIEW_H
#define PAINTVIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>

class ImpressionistDoc;

enum
{
   LEFT_MOUSE_DOWN = 0,
   LEFT_MOUSE_DRAG,
   LEFT_MOUSE_UP,
   RIGHT_MOUSE_DOWN,
   RIGHT_MOUSE_DRAG,
   RIGHT_MOUSE_UP,
   AUTO_PAINT
};

class PaintView : public Fl_Gl_Window
{
public:
	PaintView(int x, int y, int w, int h, const char* l);
	void draw();
	int handle(int event);

	void refresh();
	
	void resizeWindow(int width, int height);

	void SaveCurrentContent();

	void RestoreContent();

	ImpressionistDoc *m_pDoc;

	void setEventType(int event) { eventToDo = event; }
	void setEventTrue() { isAnEvent = 1; }

private:
	GLvoid* m_pPaintBitstart;
	GLvoid* m_pShowBitstart;
	int		m_nDrawWidth,
			m_nDrawHeight,
			m_nStartRow, 
			m_nEndRow,
			m_nStartCol, 
			m_nEndCol,
			m_nWindowWidth, 
			m_nWindowHeight;
	int		eventToDo{ 0 };
	int		isAnEvent{ 0 }; // The better way may be to maintain a message queue for it ?
	Point rightMouseStart;
	Point prev;
};

#endif