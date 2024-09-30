// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"

class ImpressionistUI;

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void		setUI(ImpressionistUI* ui);		// Assign the UI to use

	int			loadImage(char *iname);			// called by the UI to load image
	int			saveImage(char *iname);			// called by the UI to save image


	int			clearCanvas();									// called by the UI to clear the drawing canvas
	int			swapViews();									// called by the UI to swap contents in two windows
	void		setBrushType(int type);							// called by the UI to set the brushType
	void		setStrokeDirectionType(int type);				// called by the UI to set the stroke direction
	int			getSize();										// get the UI size
	int			getLineWidth();									// get the UI Line Width
	int			getLineAngle();									// get the UI Line Angle
	double		getAlpha();										// get the UI alpha value
	GLubyte*	getColor();										// get the UI color value
	void		setSize(double size);							// set the UI size
	void		getGradient(Point point_1, double* Dx, double* Dy);			// get the gradient and return as a point (vector from origin)
	void		setLineAngle(Point point_1, Point point_2);		// set the UI line angle value
	void		setLineAngle(double Dx, double Dy);					// set the line angle using gradient
	char*		getImageName();									// get the current image name
	void		printFullImageDoc(int spacingSize);				// prepare the environment to print full image in the paint view

	

// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	
	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;
	unsigned char*	m_ucPainting;

	// Points to store the left and right click locations
	Point			m_pFirstLeftClick{ 0,0 };
	Point			m_pSecondLeftClick{ 0,0 };
	Point			m_pFirstRightClick{ 0,0 };
	Point			m_pSecondRightClick{ 1,0 };	// default set to 1,0 so the initial angle is always 0


	// The current active brush.
	ImpBrush*			m_pCurrentBrush;	
	// The current stroke direction.
	int m_nStrokeDirection;
	// Size of the brush.
	int m_nSize;							

	ImpressionistUI*	m_pUI;

// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );  


private:
	char			m_imageName[256];

};

extern void MessageBox(char *message);

#endif
