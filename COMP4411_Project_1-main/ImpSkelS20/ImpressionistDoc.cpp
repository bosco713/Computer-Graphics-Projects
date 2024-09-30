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
#include "CircleBrush.h"
#include "ScatteredPointBrush.h"
#include "ScatteredLineBrush.h"
#include "ScatteredCircleBrush.h"
#include "TriangleBrush.h"


#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucBitmap		= NULL;
	m_ucPainting	= NULL;


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
		= new ScatteredPointBrush( this, "Scattered Points" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]		
		= new ScatteredLineBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]	
		= new ScatteredCircleBrush( this, "Scattered Circles" );
	ImpBrush::c_pBrushes[BRUSH_TRIANGLES]
		= new TriangleBrush(this, "Triangles");

	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];
	// make one of the stroke direction current
	m_nStrokeDirection = STROKE_DIRECTION_SLIDER_OR_RIGHT_MOUSE;

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

//---------------------------------------------------------
// Called by the UI when the user changes the stroke direction type.
// type: one of the defined stroke direction types.
//---------------------------------------------------------
void ImpressionistDoc::setStrokeDirectionType(int type)
{
	m_nStrokeDirection = type;
}


//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

//---------------------------------------------------------
// Returns the line width of the line brushes.
//---------------------------------------------------------
int ImpressionistDoc::getLineWidth()
{
	return m_pUI->getLineWidth();
}

//---------------------------------------------------------
// Returns the line angle of the line brushes.
//---------------------------------------------------------
int ImpressionistDoc::getLineAngle()
{
	return m_pUI->getLineAngle();
}

//---------------------------------------------------------
// Calculate the gradient and returns as a point (vector from origin).
//---------------------------------------------------------
void ImpressionistDoc::getGradient(Point point, double* Dx, double* Dy)
{
	GLubyte TopPixel[3];
	GLubyte RightPixel[3];
	GLubyte BottomPixel[3];
	GLubyte LeftPixel[3];
	memcpy(TopPixel, this->GetOriginalPixel(Point{ point.x, point.y - 1 }), 3);
	memcpy(RightPixel, this->GetOriginalPixel(Point{ point.x + 1, point.y }), 3);
	memcpy(BottomPixel, this->GetOriginalPixel(Point{ point.x, point.y + 1 }), 3);
	memcpy(LeftPixel, this->GetOriginalPixel(Point{ point.x - 1, point.y }), 3);
	*Dy = ((TopPixel[0] - BottomPixel[0]) + (TopPixel[1] - BottomPixel[1]) + (TopPixel[2] - BottomPixel[2])) / 2;
	*Dx = ((RightPixel[0] - LeftPixel[0]) + (RightPixel[0] - LeftPixel[0]) + (RightPixel[0] - LeftPixel[0])) / 2;
}

//---------------------------------------------------------
// Sets the line angle of the line brushes using two points.
//---------------------------------------------------------
void ImpressionistDoc::setLineAngle(Point point_1, Point point_2) {
	m_pUI->setLineAngle(point_1, point_2);
}

//---------------------------------------------------------
// Sets the line angle of the line brushes using gradient.
//---------------------------------------------------------
void ImpressionistDoc::setLineAngle(double Dx, double Dy) {
	m_pUI->setLineAngle(Dx, Dy);
}

//---------------------------------------------------------
// Sets the size of the brushes.
//---------------------------------------------------------
void ImpressionistDoc::setSize(double size) {
	m_pUI->setSize(size);
}

//---------------------------------------------------------
// Returns the alpha values brushes.
//---------------------------------------------------------
double ImpressionistDoc::getAlpha()
{
	return m_pUI->getAlpha();
}

//---------------------------------------------------------
// Returns the rgb color values.
//---------------------------------------------------------
GLubyte* ImpressionistDoc::getColor()
{
	return m_pUI->getColor();
}

void ImpressionistDoc::printFullImageDoc(int spacingSize) {
	m_pUI->m_paintView->SetPaintFullImageBool(spacingSize);
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

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();


	return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) 
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

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

//----------------------------------------------------------------
// Swap contents in two windows
// This is called by the UI when the swap views menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::swapViews() {
	if (!m_ucBitmap || !m_ucPainting) return 0;

	int totalBytes = m_nPaintWidth * m_nPaintHeight * 3;

	unsigned char* temp = new unsigned char[totalBytes];

	if (!temp) {
		fl_alert("Unable to allocate memory for swapping.");
		return 0;
	}


	memcpy(temp, m_ucBitmap, totalBytes);
	memcpy(m_ucBitmap, m_ucPainting, totalBytes);
	memcpy(m_ucPainting, temp, totalBytes);

	delete[] temp;

	m_pUI->m_origView->refresh();
	m_pUI->m_paintView->refresh();

	return 1;
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

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}

