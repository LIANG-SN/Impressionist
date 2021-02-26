// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"

#include "Avi.h"

#include "Matting.h"


class ImpressionistUI;

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *iname);			// called by the UI to load image
	int		saveImage(char *iname);			// called by the UI to save image

	int		loadAnotherImage(char* iname);
	int		loadEdgeImage(char* iname);
	int		loadMuralImage(char* iname);
	int		loadAlphaMappedImage(char* iname);
	int		loadVideo(char* iname);

	int		loadMattingAlphaImage(char* iname);
	void    MattingImage();

	void	generateEdgeImage();

	void	applyWeightedFilter();

	void	generateFadedBackground();
	void	generatemCompositeBitmap();

	void	generateThumbnail(unsigned char* target, int w, int h);
	void	showMosaicOfThumbnail(int rate, double alpha);

	int     clearCanvas();                  // called by the UI to clear the drawing canvas
	void	setBrushType(int type);			// called by the UI to set the brushType
	void	setPaintlyBrush(int type);
	int		getSize();						// get the UI size: brush size
	void	setSize(int size);				// set the UI size ??
	char*	getImageName();					// get the current image name
	int		getLineWidth();
	int		getLineAngle();
	void    setLineAngle(double angle);
	double  getAlpha();
	double	getRed();
	double	getGreen();
	double	getBlue();
	int		getLevel();
	int		getThreshold();
	void	setLineDirectionChoice(int choice); // set the line direction choice to brush
	void    setBlurSharpBrushChoice(int choice);
	void    confirmLastModify();
	void	undo();
	int     dissolve_image(char* iname);
	bool    isEdge(const int x, const int y);
	int 	getGradientOfPoint(const int x, const int y, int& Gx, int& Gy);
	bool	processVideo();
	void    videoEnd();

// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	
	// Dimensions of the alpha mapped image.
	int				m_alphaMapedWidth,
					m_alphaMappedHeight;
	// Dimensions of the thumbnail image.
	int				m_thumbnaildWidth,
					m_thumbnailHeight;
	int				w_bigger,
					h_bigger;

	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;
	unsigned char*	m_ucPainting;
	unsigned char*  m_ucPainting_prev; // prev bitmap for undo
	unsigned char*  m_anotherBitmap;
	unsigned char*  m_edgeBitmap;
	unsigned char*  m_alphaMappedBitmap;
	unsigned char*  m_fadedBackgroundBitmap;
	unsigned char*  m_compositeBitmap;

	unsigned char*  m_vedioBitmap;

	unsigned char*  m_thumbnailBitmap;

	unsigned char*  m_mattingAlphaBitmap;

	// The current active brush.
	ImpBrush*		m_pCurrentBrush;
	ImpBrush*		m_pPaintlyBrush;

	Matting*		matte;

	// Size of the brush.
	int m_nSize;	

	ImpressionistUI*	m_pUI;

	Avi*					avi;

	double brushMoveAngle;

// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );  
	// Get the color of the painting picture at the specified coord
	GLubyte* GetPaintingPixel(int x, int y);
	GLubyte* GetAlphaMappedPixel(int x, int y);

private:
	char			m_imageName[256];

};

extern void MessageBox(char *message);

#endif
