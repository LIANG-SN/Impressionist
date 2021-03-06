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
#include "WarpBrush.h"
#include <cmath>
#include <string>
#include <iostream>
#include <Windows.h>


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
	m_thumbnailBitmap = NULL;
	m_mattingAlphaBitmap = NULL;

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
	ImpBrush::c_pBrushes[BRUSH_WARP]
		= new WarpBrush(this, "Warp");
	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];
	m_pPaintlyBrush = ImpBrush::c_pBrushes[0];

	matte = NULL;

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
void ImpressionistDoc::setPaintlyBrush(int type)
{
	m_pPaintlyBrush = ImpBrush::c_pBrushes[type];
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
								height+25); // additional 300 for text window

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


int ImpressionistDoc::loadMattingAlphaImage(char* iname)
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

	// release old storage
	if (m_mattingAlphaBitmap) delete[] m_mattingAlphaBitmap;
	m_mattingAlphaBitmap = data;
	memcpy(m_ucPainting, m_mattingAlphaBitmap, width * height * 3);
	m_pUI->m_paintView->refresh();

	return 1;
}

void  ImpressionistDoc::MattingImage()
{
	matte=new Matting(this);
	matte->solveAlpha();
	matte->generateNewAlphaBitmap();
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

int ImpressionistDoc::loadVideo(char* iname)
{
	// try to open the image to read
	unsigned char* data;
	int				width, height;
	avi = new Avi;
	avi->readStart(iname, width, height);

	data = avi->getNextFrame();
	if (data == NULL)
	{
		fl_alert("Can't load video file");
		return 0;
	}

	// reflect the fact of loading the new video
	m_nWidth = width;
	m_nPaintWidth = width;
	m_nHeight = height;
	m_nPaintHeight = height;

	// release old storage
	if (m_ucBitmap) delete[] m_ucBitmap;
	if (m_ucPainting) delete[] m_ucPainting;

	m_ucBitmap = data;

	// allocate space for draw view
	m_ucPainting = new unsigned char[width * height * 3];
	memset(m_ucPainting, 0, width * height * 3);
	m_ucPainting_prev = new unsigned char[width * height * 3];
	memset(m_ucPainting_prev, 0, width * height * 3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(),
		m_pUI->m_mainWindow->y(),
		width * 2,
		height + 25); // additional 300 for text window

// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);
	m_pUI->m_origView->showImageChoice(SHOW_ORIGIN_IMAGE);

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);

	m_pUI->m_paintView->setEventType(AUTO_PAINT);
	m_pUI->m_paintView->setEventTrue();
	m_pUI->m_paintView->refresh();

	
	avi->writeStart("new_video");
	//avi.writeNextFrame(m_ucPainting);

	return 1;
}

bool ImpressionistDoc::processVideo()
{

		unsigned char* temp = avi->getNextFrame();
		if (temp == nullptr)
			return false;

		m_ucBitmap = temp;
	    m_pUI->m_origView->refresh();
	    m_pUI->m_paintView->setEventType(AUTO_PAINT);
	    m_pUI->m_paintView->setEventTrue();
	    m_pUI->m_paintView->refresh();
		//Sleep(10);

	    /*using namespace std;
	    cout << avi->getPeriod() << endl;*/
		return true;

	//avi.writeNextFrame(m_ucPainting);
}
void ImpressionistDoc::videoEnd()
{
	avi->readEnd();
	delete avi;
	avi = nullptr;
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

void ImpressionistDoc::applyWeightedFilter()
{
	confirmLastModify();
	if (m_ucPainting) delete[] m_ucPainting;
	m_ucPainting = new unsigned char [m_nHeight * m_nWidth * 3];

	int size = m_pUI->getFilterSize();
	double* weight = m_pUI->getFilterWeight();

	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{

			int count = 0;
			int color[3]{ 0,0,0 };
			for (int i = -size / 2; i <= size / 2; i++)
			{
				for (int j = -size / 2; j <= size / 2; j++)
				{
					GLubyte* origin_pixel = GetOriginalPixel(x + j, y + i);
					for (int t = 0; t < 3; t++)
					{
						color[t] += origin_pixel[t] * weight[count];
					}
					count++;
				}
			}


			for (int t = 0; t < 3; t++)
			{
				if (m_pUI->getNormalized())
					color[t] = color[t] / (size * size);
				if (color[t] > 255)
					*(m_ucPainting + 3 * (y * m_nWidth + x) + t) = 255;
				else
					*(m_ucPainting + 3 * (y * m_nWidth + x) + t) = color[t];
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

__inline double bilinear(double a, double b, int uv, int u1v, int uv1, int u1v1)
{
	return (double)(uv * (1 - a) * (1 - b) + u1v * a * (1 - b) + uv1 * b * (1 - a) + u1v1 * a * b);
}

void ImpressionistDoc::generateThumbnail(unsigned char* target, int w, int h)
{
	double x_rate = m_nWidth / w;
	double y_rate = m_nHeight / h;
	
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++) {
			double ox = x_rate * x ;
			double oy = y_rate * y ;

			int x1 = (int)ox;
			int y1 = (int)oy;


			for (int i = 0; i < 3; i++)
			{
				
				double color = bilinear(ox - x1, oy - y1,
					m_ucBitmap[(m_nWidth * y1 + x1) * 3 + i],
					m_ucBitmap[(m_nWidth * y1 + x1 + 1) * 3 + i],
					m_ucBitmap[(m_nWidth * (y1 + 1) + x1) * 3 + i],
					m_ucBitmap[(m_nWidth * (y1 + 1) + x1 + 1) * 3 + i]);
				if (color > 255)
					*(target + 3 * (y * w + h) + i) = 255;
				else
					*(target + 3 * (y * w + x) + i) = color;
			}
			
		}
	}
}
void ImpressionistDoc::showMosaicOfThumbnail(int rate,double alpha)
{
	int w = (int) m_nWidth / rate;
	int h = (int) m_nHeight / rate;
	int wr = m_nWidth % w;
	int hr = m_nHeight % h;
	m_thumbnaildWidth = w + wr / rate;
	m_thumbnailHeight = h + hr / rate;
	w_bigger = wr % rate;
	h_bigger = hr % rate;
	


	if (m_thumbnailBitmap) delete[] m_thumbnailBitmap;
	
	m_thumbnailBitmap = new unsigned char[m_thumbnaildWidth * m_thumbnailHeight * 3];


	generateThumbnail(m_thumbnailBitmap, m_thumbnaildWidth, m_thumbnailHeight);


	for (int a = 0; a < rate; a++)
	{
		bool is_big_h = (a < h_bigger);

			
			//int start = 3 * (a * m_nWidth * (m_thumbnailHeight + 1) + b * (m_thumbnaildWidth + 1));

		for (int i = 0; i < m_thumbnailHeight; i++)
		{
			int h_start = (m_thumbnailHeight * a + is_big_h * a + (1 - is_big_h) * h_bigger + i) * m_nWidth * 3;
			//if (m_thumbnailHeight * a + is_big_h * a + (1 - is_big_h) * h_bigger + i >= m_nHeight)
				
			//if (i == 0)
			//	std::cout << m_thumbnailHeight * a + is_big_h * a + (1 - is_big_h) * h_bigger + i<<'\n';

			for (int b = 0; b < rate; b++)
			{
				bool is_big_w = (b < w_bigger);
				//if(m_thumbnaildWidth * b + is_big_w * b + (1 - is_big_w) * w_bigger>= m_nWidth)
				//	count++;
				int w_start = (m_thumbnaildWidth * b + is_big_w * b + (1 - is_big_w) * w_bigger) * 3;
				for (int j = 0; j < m_thumbnaildWidth; j++)
				{

					for (int k = 0; k < 3; k++)
					{
						int color = *(m_thumbnailBitmap + 3 * (i * m_thumbnaildWidth + j) + k) * alpha +
							*(m_ucBitmap + h_start + w_start + j * 3 + k) * (1 - alpha);
						if (color > 255)
							color = 255;
						*(m_ucPainting + h_start + w_start + j * 3 + k) = color;
					
					}
				}

				if (is_big_w)
				{
					for (int k = 0; k < 3; k++)
					{
						int color = *(m_ucPainting + h_start + w_start + (m_thumbnaildWidth - 1) * 3 + k) * alpha
							+ *(m_ucBitmap + h_start + w_start + m_thumbnaildWidth * 3 + k) * (1 - alpha);
						if (color > 255)
							color = 255;
						*(m_ucPainting + h_start + w_start + m_thumbnaildWidth * 3 + k) = color;

					}
				}
			}


		}


		if (is_big_h)
		{
			int h_end = (m_thumbnailHeight * (a + 1) + is_big_h * a + (1 - is_big_h) * h_bigger - 1) * m_nWidth * 3;
			for (int i = 0; i < m_nWidth; i++)
			{
				for (int k = 0; k < 3; k++)
				{
					
					int color = *(m_ucPainting + h_end + m_nWidth * 3 + 3 * i + k) * (1 - alpha) +
						*(m_ucBitmap + h_end + m_thumbnailHeight * m_nWidth * 3 + 3 * i + k) * alpha;
					if (color > 255)
						color = 255;
					* (m_ucPainting + h_end + m_nWidth * 3 + 3 * i + k) = color;
					
				}
			}
		}

			


		
	}

	//std::cout << count << '\n';


	m_pUI->m_paintView->refresh();
}


int ImpressionistDoc::saveImage(char *iname)
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);
	//writeBMP(iname, m_thumbnaildWidth, m_thumbnailHeight, m_ucBitmap);


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
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return (GLubyte*)(m_ucPainting + 3 * (y * m_nWidth + x));
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