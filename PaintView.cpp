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
#include "CurveBrush.h"
#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>
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
		case PAINTLY:
		{
			m_pDoc->setPaintlyBrush(BRUSH_CURVE);
			// set canvas to a special color first
			memset(m_pDoc->m_ucPainting, 255, m_pDoc->m_nPaintWidth * m_pDoc->m_nPaintHeight * 3);
			SaveCurrentContent();
			RestoreContent();

			using namespace std;
			vector<int> R;
			R.push_back(0); // take space for front detect
			int ratio = m_pDoc->m_pUI->getLayerRatio();
			int R1 = m_pDoc->m_pUI->getPaintlyPrecision();
			R.push_back(R1);
			for (int i = 1; i < m_pDoc->m_pUI->getLayer(); i++)
			{
				R1 *= ratio;
				R.push_back(R1);
			}
			
			for (vector<int>::iterator p = R.end() - 1; p != R.begin(); p--)
			{
				paintLayer(*p, m_pDoc->m_pUI->getPaintlyThresh());
			}
			break;
		}
		case MULTIRESOLUTION:
		{
			m_pDoc->setPaintlyBrush(BRUSH_POINTS);
			// set canvas to a special color first
			memset(m_pDoc->m_ucPainting, 255, m_pDoc->m_nPaintWidth * m_pDoc->m_nPaintHeight * 3);
			SaveCurrentContent();
			RestoreContent();

			using namespace std;
			vector<int> R;
			R.push_back(0); // take space for front detect
			int ratio = 2;
			int R1 = 1;
			R.push_back(R1);
			for (int i = 1; i < 5; i++)
			{
				R1 *= ratio;
				R.push_back(R1);
			}

			for (vector<int>::iterator p = R.end() - 1; p != R.begin(); p--)
			{
				paintLayer(*p, 7);
			}
			break;
		}
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

void PaintView::paintLayer(int R, int T)
{
	using namespace std;
	vector<Point> strokes;

	vector<vector<int>> filtered;
	vector<vector<int>> diff;
	for (int row = 0; row < m_nDrawHeight; row++)
	{
		vector<int> rowV, diffV;
		filtered.push_back(rowV);
		diff.push_back(diffV);
		cout << "debug" << endl;
		for (int col = 0; col < m_nDrawWidth; col++)
		{
			GLubyte* origin = m_pDoc->GetOriginalPixel(col, row);
			filtered[row].push_back( 0.299 * (*origin) + 0.587 * (*(origin + 1))
				+ 0.114 * (*(origin + 2)) );
			GLubyte* paintingPix = m_pDoc->GetPaintingPixel(col, row);
			int painting = 0.299 * (*paintingPix) + 0.587 * (*(paintingPix + 1))
				+ 0.114 * (*(paintingPix + 2));
			diff[row].push_back(  abs(filtered[row][col] - painting) );
		}
	}
	float fg = m_pDoc->m_pUI->getPaintlyGridSize();
	int grid = fg * R;
	
	for (int row = 0; row < m_nDrawHeight; row += grid)
	{
		for (int col = 0; col < m_nDrawWidth; col += grid)
		{
			// sum the error in this grid
			int  errorSum = 0;
			vector<vector<int>> error;
			for (int i = 0; i <= grid; i++)
			{
				vector<int> eV;
				error.push_back(eV);
				for (int j = 0; j <= grid; j++)
				{
					if (row - grid / 2 + i >= 0 && row - grid / 2 + i < m_nDrawHeight
						&& col - grid / 2 + j >= 0 && col - grid / 2 + j < m_nDrawWidth)
					{
						error[i].push_back(diff[row - grid / 2 + i][col - grid / 2 + j]);
						errorSum += diff[row - grid / 2 + i][col - grid / 2 + j];
					}
					else
						error[i].push_back(0);

				}
			}
			if (errorSum >= T)
			{
				int x, y, e = 0;
				// find the argument(position) of max error
				for (int i = 0; i < grid; i++)
				{
					vector<int>::iterator p = max_element(error[i].begin(), error[i].end());
					if (e < *p)
					{
						e = *p;
						x = p - error[i].begin() + col - grid / 2;
						y = i + row - grid / 2;
						
					}
				}
				strokes.push_back({ x, y });
			}
		}
	}
	// random
	random_shuffle(strokes.begin(), strokes.end());
	m_pDoc->m_pUI->setSize(R * 2);
	// match the window
						//Point s(x + m_nStartCol, m_nEndRow - y);//original view
						//Point t(x, m_nWindowHeight - y);// paint view
	m_pDoc->m_pPaintlyBrush->BrushBegin({ strokes[0].x + m_nStartCol, m_nEndRow - strokes[0].y }, 
		{ strokes[0].x, m_nWindowHeight - strokes[0].y });
	for (vector<Point>::iterator p = strokes.begin() + 1; p != strokes.end(); p++)
	{
		m_pDoc->m_pPaintlyBrush->BrushMove({ p->x + m_nStartCol, m_nEndRow - p->y }, { p->x, m_nWindowHeight - p->y });
	}
	m_pDoc->m_pPaintlyBrush->BrushEnd({ m_nStartCol, m_nEndRow }, { 0, m_nWindowHeight });
	SaveCurrentContent();
	RestoreContent();
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


