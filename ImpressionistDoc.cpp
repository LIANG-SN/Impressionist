// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "ImpBrush.h"

// Include individual brush headers here.
#include "PointBrush.h"
#include "LineBrush.h"
#include "ScatterPointBrush.h"
#include "CircleBrush.h"
#include "ScatteredCircleBrush.h"
#include "ScatteredLineBrush.h"
#include "PentagramBrush.h"
#include "BlurSharpBrush.h"
#include "CurveBrush.h"
#include "AlphaMappedBrush.h"
#include <cmath>
#include <string>

#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucBitmap		= NULL;
	m_ucPainting	= NULL;
	m_anotherBitmap = NULL;
	m_edgeBitmap = NULL;
	m_alphaMappedBitmap = NULL;
	m_ucPainting_prev = NULL;
	m_fadedBackgroundBitmap = NULL;
	m_compositeBitmap = NULL;

	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES]				
		= new LineBrush( this, "Lines" );
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]				
		= new CircleBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]	
		= new ScatterPointBrush( this, "Scattered Points" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]		
		= new ScatteredLineBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]	
		= new ScatteredCircleBrush( this, "Scattered Circles" );
	ImpBrush::c_pBrushes[BRUSH_PENTAGRAM]
		= new PentagramBrush(this, "Pentagram");
	ImpBrush::c_pBrushes[BRUSH_BLURORSHARPEN]
		= new BlurSharpBrush(this, "Blur or Sharpen");
	ImpBrush::c_pBrushes[BRUSH_CURVE]
		= new CurveBrush(this, "Curve");
	ImpBrush::c_pBrushes[BRUSH_ALPHA_MAPPED]
		= new AlphaMappedBrush(this, "Alpha Mapped");
	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];

}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
}

void ImpressionistDoc::setLineDirectionChoice(int choice)
{
	ImpBrush::lineDirectionChoice = choice;
}

void ImpressionistDoc::setBlurSharpBrushChoice(int choice)
{
	ImpBrush::BlurSharpBrushChoice = choice;
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

int ImpressionistDoc::getLineWidth() { return m_pUI->getLineWidth(); }
int ImpressionistDoc::getLineAngle() { return m_pUI->getLineAngle(); }
void ImpressionistDoc::setLineAngle(double angle) { m_pUI->setLineAngle(angle); }
double ImpressionistDoc::getAlpha() { return m_pUI->getAlpha(); }
double ImpressionistDoc::getRed(){ return m_pUI->getRed(); }
double ImpressionistDoc::getGreen(){ return m_pUI->getGreen(); }
double ImpressionistDoc::getBlue(){ return m_pUI->getBlue(); }
int ImpressionistDoc::getLevel() { return m_pUI->getLevel(); }
int	ImpressionistDoc::getThreshold() { return m_pUI->getThreshold(); }


void ImpressionistDoc::confirmLastModify()
{
	memcpy(m_ucPainting_prev, m_ucPainting, m_nWidth * m_nHeight * 3);
}
void ImpressionistDoc::undo()
{
	memcpy(m_ucPainting, m_ucPainting_prev, m_nWidth * m_nHeight * 3);
	// todo: review this
	// m_pUI->m_paintView->RestoreContent();
	m_pUI->m_paintView->refresh();
}
bool ImpressionistDoc::isEdge(int x, int y)
{
	if (x < 0 || x > m_nWidth || y < 0 || y > m_nHeight || !m_edgeBitmap)
		return 0;
	return (*(m_edgeBitmap + 3 * (y * m_nWidth + x)) == 255);
}

int ImpressionistDoc::getGradientOfPoint(const int x, const int y, int& Gx, int& Gy)
{
	if (x < 0 || x >= m_nWidth || y < 0 || y > m_nHeight)
		return 0;
	int sobel_x[3][3] =
	{
		{ 1, 0, -1 },
		{ 2, 0, -2 },
		{ 1, 0, -1 }
	};
	Gx = 0;
	Gy = 0;
	for (int a = -1; a <= 1; a++)
	{
		for (int b = -1; b <= 1; b++)
		{
			GLubyte* pixel = GetOriginalPixel(x+a, y+b);
			// formula from tutorial doc page 19
			int pixelValue = 0.299 * pixel[0] + 0.587 * pixel[1] + 0.114 * pixel[2];
			Gx += sobel_x[a + 1][b + 1] * pixelValue;
			Gy += sobel_x[b + 1][a + 1] * pixelValue;
		}
	}
	// debug
	std::string debug = "Gx: " + std::to_string(Gx) + "\n";
	m_pUI->print(debug);
	return int(sqrt(Gx * Gx + Gy * Gy));
}


//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=readBMP(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	// release old storage
	if ( m_ucBitmap ) delete [] m_ucBitmap;
	if ( m_ucPainting ) delete [] m_ucPainting;

	m_ucBitmap		= data;

	// allocate space for draw view
	m_ucPainting	= new unsigned char [width*height*3];
	memset(m_ucPainting, 0, width*height*3);
	m_ucPainting_prev = new unsigned char[width * height * 3];
	memset(m_ucPainting_prev, 0, width * height * 3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+325); // additional 300 for text window

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->showImageChoice(SHOW_ORIGIN_IMAGE);

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();


	return 1;
}
int ImpressionistDoc::loadAnotherImage(char* iname)
{

	// try to open the image to read
	unsigned char* data;
	int				width, height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}
	if (width != m_nWidth || height != m_nHeight)
	{
		fl_alert("Can't load. Image size must be same!");
		delete[] data;
		return 0;
	}


	// release old storage
	if (m_anotherBitmap) delete[] m_anotherBitmap;
	m_anotherBitmap = data;

	return 1;
}
int ImpressionistDoc::loadEdgeImage(char* iname)
{

	// try to open the image to read
	unsigned char* data;
	int				width, height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}
	if (width != m_nWidth || height != m_nHeight)
	{
		fl_alert("Can't load. Image size must be same!");
		delete[] data;
		return 0;
	}

	// release old storage
	if (m_edgeBitmap) delete[] m_edgeBitmap;
	m_edgeBitmap = data;

	return 1;
}

int	ImpressionistDoc::loadMuralImage(char* iname) 
{
	unsigned char* data;
	int				width, height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}
	if (width != m_nWidth || height != m_nHeight)
	{
		fl_alert("Can't load. Image size must be same!");
		delete[] data;
		return 0;
	}
	if (m_ucBitmap) delete[] m_ucBitmap;

	m_ucBitmap = data;


	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);
	m_pUI->m_origView->showImageChoice(SHOW_ORIGIN_IMAGE);

	return 1;
}

int	ImpressionistDoc::loadAlphaMappedImage(char* iname)
{
	unsigned char* data;


	if ((data = readBMP(iname, m_alphaMapedWidth, m_alphaMappedHeight)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	if (m_alphaMappedBitmap) delete[] m_alphaMappedBitmap;

	m_alphaMappedBitmap = data;


	

	return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
void ImpressionistDoc::generateEdgeImage() 
{
	if (m_edgeBitmap) delete[] m_edgeBitmap;
	m_edgeBitmap = new unsigned char[m_nWidth * m_nHeight * 3];


	for (int x = 0; x < m_nWidth; x++) {
		for (int y = 0; y < m_nHeight; y++) {

			int Gx = 0, Gy = 0;

			getGradientOfPoint(x, y, Gx, Gy);

			int threshold = getThreshold();
			if (abs(Gx) >= threshold || abs(Gy) >= threshold) {
				m_edgeBitmap[3 * (y * m_nWidth + x)] = (unsigned char)255;
				m_edgeBitmap[3 * (y * m_nWidth + x) + 1] = (unsigned char)255;
				m_edgeBitmap[3 * (y * m_nWidth + x) + 2] = (unsigned char)255;
			}
			else {
				m_edgeBitmap[3 * (y * m_nWidth + x)] = (unsigned char)0;
				m_edgeBitmap[3 * (y * m_nWidth + x) + 1] = (unsigned char)0;
				m_edgeBitmap[3 * (y * m_nWidth + x) + 2] = (unsigned char)0;
			}

		}
	}


}

void ImpressionistDoc::generateFadedBackground()
{
	if (!m_ucBitmap) return;
	if (m_fadedBackgroundBitmap) delete[] m_fadedBackgroundBitmap;
	m_fadedBackgroundBitmap= new unsigned char[m_nHeight* m_nWidth*3];
	for (int i = 0; i < m_nHeight; i++)
	{
		for (int j = 0; j < m_nWidth; j++)
		{
			for (int t = 0; t < 3; t++)
			{
				*(m_fadedBackgroundBitmap + 3 * (i * m_nWidth + j) + t) = 0;
				*(m_fadedBackgroundBitmap + 3 * (i * m_nWidth + j) + t) += *(m_ucBitmap + 3 * (i * m_nWidth + j) + t) * m_pUI->getFadedRate();
			}
		}
	}
}

void ImpressionistDoc::generatemCompositeBitmap()
{
	if (!m_ucPainting) return;

	if (m_compositeBitmap) delete[] m_compositeBitmap;
	m_compositeBitmap = new unsigned char[m_nHeight * m_nWidth * 3];

	for (int i = 0; i < m_nHeight; i++)
	{
		for (int j = 0; j < m_nWidth; j++)
		{
			int hasPainted = 0;
			for (int t = 0; t < 3; t++)
			{
				if (*(m_ucPainting + 3 * (i * m_nWidth + j) + t) != 0) {
					hasPainted = 1;
					break;
				}

			}

			for (int t = 0; t < 3; t++)
			{
				*(m_compositeBitmap + 3 * (i * m_nWidth + j) + t) = *(m_fadedBackgroundBitmap + 3 * (i * m_nWidth + j) + t) * (1-hasPainted) + *(m_ucPainting + 3 * (i * m_nWidth + j) + t) * hasPainted;
			}
		}
	}
}


int ImpressionistDoc::saveImage(char *iname)
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;
}

int ImpressionistDoc::dissolve_image(char* iname)
{
	// try to open the image to read
	unsigned char* data;
	int				width, height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}
	if (!m_ucBitmap)
	{
		fl_alert("Load an origin image first");
		delete[] data;
		return 0;
	}
	// only can dissolve image with smaller or equal size
	if (width > m_nWidth || height > m_nHeight)
	{
		fl_alert("The loaded file is larger than the origin, choose a smaller one.");
		delete[] data;
		return 0;
	}

	// merge data
	for (int i = 0; i < m_nHeight; i++)
	{
		for (int j = 0; j < m_nWidth; j++)
		{
			if (i < height && j < width)
			{
				for (int t = 0; t < 3; t++)
				{
					*(m_ucBitmap + 3 * (i * m_nWidth + j) + t) *= 0.5;
					*(m_ucBitmap + 3 * (i * m_nWidth + j) + t) += *(data + 3 * (i * width + j) + t) * 0.5;
				
				}
			}
		}
	}

	// display it on origView
	m_pUI->m_origView->refresh();

	return 1;
}
//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	/// move the pointer from original to the  point, each point have 3 value so *3
	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));  
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}

GLubyte* ImpressionistDoc::GetPaintingPixel(int x, int y)
{
	if (x < 0)
		x = 0;
	else if (x >= m_nPaintWidth)
		x = m_nPaintWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nPaintHeight)
		y = m_nPaintHeight - 1;

	return (GLubyte*)(m_ucPainting + 3 * (y * m_nPaintWidth + x));
}


GLubyte* ImpressionistDoc::GetAlphaMappedPixel(int x, int y)
{
	if (x < 0)
		x = 0;
	else if (x >= m_alphaMapedWidth)
		x = m_alphaMapedWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_alphaMappedHeight)
		y = m_alphaMappedHeight - 1;

	return (GLubyte*)(m_alphaMappedBitmap + 3 * (y * m_alphaMapedWidth + x));
}