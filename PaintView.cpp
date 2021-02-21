//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintview.h"
#include "ImpBrush.h"
#include <cmath>
#include <algorithm>

#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static Point	coord;

PaintView::PaintView(int			x,
	int			y,
	int			w,
	int			h,
	const char* l)
	: Fl_Gl_Window(x, y, w, h, l)
{
	m_nWindowWidth = w;
	m_nWindowHeight = h;

}


void PaintView::draw()
{
#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
#endif // !MESA
	// toread
	if (!valid())
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable(GL_DEPTH_TEST);

		ortho();

		glClear(GL_COLOR_BUFFER_BIT);
	}

	glEnable(GL_BLEND);
	/// that means these ¡§weight factors¡¨:k(src) = alpha k(dest) = 1 - alpha
	// final framebuffer color = k(src) * color(src) + k(dest) * color(dest)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Point scrollpos;// = GetScrollPosition();  /// to read
	scrollpos.x = 0;
	scrollpos.y = 0;

	m_nWindowWidth = w();
	m_nWindowHeight = h();

	int drawWidth, drawHeight;
	drawWidth = min(m_nWindowWidth, m_pDoc->m_nPaintWidth);
	drawHeight = min(m_nWindowHeight, m_pDoc->m_nPaintHeight);

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if (startrow < 0) startrow = 0;


	m_pDoc->generateFadedBackground();
	m_pDoc->generatemCompositeBitmap();

	m_pShowBitstart = m_pDoc->m_compositeBitmap +
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth = drawWidth;
	m_nDrawHeight = drawHeight;

	m_nStartRow = startrow;
	m_nEndRow = startrow + drawHeight;
	m_nStartCol = scrollpos.x;
	m_nEndCol = m_nStartCol + drawWidth;

	// toread
	if (m_pDoc->m_ucPainting && !isAnEvent)
	{

		RestoreContent(); // toread

	}

	if (m_pDoc->m_ucPainting && isAnEvent)
	{

		// Clear it after processing.
		isAnEvent = 0;



		Point source(coord.x + m_nStartCol, m_nEndRow - coord.y);//original view
		Point target(coord.x, m_nWindowHeight - coord.y);// paint view
		// todu: updata brush direction in single click mode
		m_pDoc->brushMoveAngle = atan2(target.y - prev.y, target.x - prev.x) / M_PI / 2 * 360;
		// This is the event handler
		switch (eventToDo)
		{
		case LEFT_MOUSE_DOWN:
			if (target.x < m_nDrawWidth && target.y > m_nWindowHeight - m_nDrawHeight) {
				m_pDoc->confirmLastModify();
				m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
			}
	
			break;
		case LEFT_MOUSE_DRAG:
			if (target.x < m_nDrawWidth && target.y > m_nWindowHeight - m_nDrawHeight) {
				m_pDoc->m_pCurrentBrush->BrushMove(source, target);
			}
			break;
		case LEFT_MOUSE_UP:
			m_pDoc->m_pCurrentBrush->BrushEnd(source, target);

			SaveCurrentContent();
			RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
		{
			rightMouseStart = target;
			m_pDoc->m_pCurrentBrush->rightMouseBegin(rightMouseStart, target);
		}
		break;
		case RIGHT_MOUSE_DRAG:
		{
			RestoreContent();
			m_pDoc->m_pCurrentBrush->rightMouseMove(rightMouseStart, target);
		}
		break;
		case RIGHT_MOUSE_UP:
		{
			RestoreContent();
			m_pDoc->m_pCurrentBrush->rightMouseEnd(rightMouseStart, target);
			double dx = target.x - rightMouseStart.x;
			double dy = target.y - rightMouseStart.y;

			m_pDoc->setLineAngle(atan2(dy, dx) / M_PI / 2 * 360);
		}
		break;
		case AUTO_PAINT:
		{
			int numPixels = drawHeight * drawWidth;
			int* pixels = new int[numPixels];
			// to do: optimize this?
			for (int i = 0; i < numPixels; i++)
				pixels[i] = i;
			std::random_shuffle(pixels, pixels + numPixels - 1);
			m_pDoc->m_pCurrentBrush->BrushBegin({ m_nStartCol, m_nEndRow}, {0, m_nWindowHeight});
			for (int i = 0; i < numPixels; i++)
			{
				int x = pixels[i] % drawWidth;
				int y = pixels[i] / drawWidth;
				Point s(x + m_nStartCol, m_nEndRow - y);//original view
				Point t(x, m_nWindowHeight - y);// paint view
				m_pDoc->m_pCurrentBrush->BrushMove(s, t);
			}
			m_pDoc->m_pCurrentBrush->BrushEnd({ m_nStartCol, m_nEndRow }, { 0, m_nWindowHeight });
			SaveCurrentContent();
			RestoreContent();
		}
		break;
		default:
			printf("Unknown event!!\n");
			break;
		}
	}

	glFlush();

#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
#endif // !MESA

}

// log event and input info
int PaintView::handle(int event)
{
	switch (event)
	{
	case FL_ENTER: // mouse enter this widget(without button down)
		redraw();
		break;
	case FL_PUSH:
	{

		if (Fl::event_button() > 1)
			eventToDo = RIGHT_MOUSE_DOWN;
		else
		{
			eventToDo = LEFT_MOUSE_DOWN;
			prev = { coord.x, m_nWindowHeight - coord.y };
		}
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		isAnEvent = 1;

		redraw();
	}
	break;
	case FL_DRAG: // mouse move with botton down
	{
		if (Fl::event_button() > 1)
			eventToDo = RIGHT_MOUSE_DRAG;
		else
		{
			eventToDo = LEFT_MOUSE_DRAG;
			prev = { coord.x, m_nWindowHeight - coord.y };
		}
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		isAnEvent = 1;
		m_pDoc->m_pUI->m_origView->setMarker({ coord.x, m_nWindowHeight - coord.y });
		redraw();
	}
	break;
	case FL_RELEASE:
	{
		if (Fl::event_button() > 1)
			eventToDo = RIGHT_MOUSE_UP;
		else
		{
			eventToDo = LEFT_MOUSE_UP;
			prev = { coord.x, m_nWindowHeight - coord.y };
		}
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		isAnEvent = 1;
		redraw();
	}
	break;
	case FL_MOVE: // mouse move without botton down
	{

		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		m_pDoc->m_pUI->m_origView->setMarker({ coord.x, m_nWindowHeight - coord.y });
	}
	break;
	default:
		return 0;
		break;

	}



	return 1;
}

void PaintView::refresh()
{
	redraw(); // ask fltk to call draw()
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent()
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(GL_FRONT);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);
	// read from front buffer to m_pShowBitstart
	glReadPixels(0,
		m_nWindowHeight - m_nDrawHeight,
		m_nDrawWidth,
		m_nDrawHeight,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		m_pShowBitstart);

	// store your painting
	int width = m_pDoc->m_nWidth;
	int height = m_pDoc->m_nHeight;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int hasPainted = 0;
			for (int t = 0; t < 3; t++)
			{
				if (*(m_pDoc->m_compositeBitmap + 3 * (i * width + j) + t) != *(m_pDoc->m_fadedBackgroundBitmap + 3 * (i * width + j) + t)) {
					hasPainted = 1;
					break;
				}

			}

			for (int t = 0; t < 3; t++)
			{
				*(m_pDoc->m_ucPainting + 3 * (i * width + j) + t) =  *(m_pDoc->m_compositeBitmap + 3 * (i * width + j) + t) * hasPainted;
			}
		}
	}
}


void PaintView::RestoreContent()
{
	glDrawBuffer(GL_BACK);

	glClear(GL_COLOR_BUFFER_BIT);

	glRasterPos2i(0, m_nWindowHeight - m_nDrawHeight);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);

	glDrawPixels(m_nDrawWidth,
		m_nDrawHeight,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		m_pShowBitstart);

	glDrawBuffer(GL_FRONT);
}


