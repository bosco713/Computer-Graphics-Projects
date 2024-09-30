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
#include "cstring"
#include "math.h"


#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6


#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;
static Point	coord;

PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l)
						: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;
	m_bPrintFullImage = false;
}


void PaintView::draw()
{
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA

	if(!valid())
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		glClear( GL_COLOR_BUFFER_BIT );
	}

	// Enabling transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;
	

	if ( m_pDoc->m_ucPainting && !isAnEvent) 
	{
		RestoreContent();

	}

	if ( m_pDoc->m_ucPainting && isAnEvent) 
	{

		// Clear it after processing.
		isAnEvent	= 0;	

		Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		Point target( coord.x, m_nWindowHeight - coord.y );
		
		// This is the event handler
		switch (eventToDo) 
		{
		case LEFT_MOUSE_DOWN:
			m_pDoc->m_pCurrentBrush->BrushBegin( source, target );
			if ((m_pDoc->m_pCurrentBrush == ImpBrush::c_pBrushes[BRUSH_LINES] || m_pDoc->m_pCurrentBrush == ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES])) {
				if (m_pDoc->m_nStrokeDirection == STROKE_DIRECTION_BRUSH_DIRECTION) {
					m_pDoc->m_pFirstLeftClick = source;
				} else if (m_pDoc->m_nStrokeDirection == STROKE_DIRECTION_GRADIENT) {
					double Dx, Dy;
					m_pDoc->getGradient(source, &Dx, &Dy);
					m_pDoc->setLineAngle(Dx, Dy);
					m_pDoc->m_pCurrentBrush->BrushMove(source, target);
				}
			}
			break;
		case LEFT_MOUSE_DRAG:
			if (m_pDoc->m_pCurrentBrush == ImpBrush::c_pBrushes[BRUSH_LINES] || m_pDoc->m_pCurrentBrush == ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES])
			{
				if (m_pDoc->m_nStrokeDirection == STROKE_DIRECTION_BRUSH_DIRECTION) {
					m_pDoc->m_pSecondLeftClick = source;
					m_pDoc->setLineAngle(m_pDoc->m_pFirstLeftClick, m_pDoc->m_pSecondLeftClick);
					m_pDoc->m_pFirstLeftClick = m_pDoc->m_pSecondLeftClick;
				}
				else if (m_pDoc->m_nStrokeDirection == STROKE_DIRECTION_GRADIENT) {
					double Dx, Dy;
					m_pDoc->getGradient(source, &Dx, &Dy);
					m_pDoc->setLineAngle(Dx, Dy);
				}
			}
			m_pDoc->m_pCurrentBrush->BrushMove( source, target );
			break;
		case LEFT_MOUSE_UP:
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );

			SaveCurrentContent();
			RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
			m_pDoc->m_pFirstRightClick = source;
			break;
		case RIGHT_MOUSE_DRAG:
			
			RestoreContent();

			m_pDoc->m_pSecondRightClick = source;

			// Set the drawing color to red
			glColor3f(1.0, 0.0, 0.0); // Red

			// Draw line from first right click to current mouse position (temporary line)
			glLineWidth(1); // Set line width
			glBegin(GL_LINES);
				glVertex2i(m_pDoc->m_pFirstRightClick.x, m_pDoc->m_pFirstRightClick.y);
				glVertex2i(m_pDoc->m_pSecondRightClick.x, m_pDoc->m_pSecondRightClick.y);
			glEnd();
			
			break;
		case RIGHT_MOUSE_UP:
			RestoreContent();
			if (m_pDoc->m_pCurrentBrush == ImpBrush::c_pBrushes[BRUSH_LINES] || m_pDoc->m_pCurrentBrush == ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES])
			{
				m_pDoc->m_pSecondRightClick = source;
				m_pDoc->setLineAngle(m_pDoc->m_pFirstRightClick, m_pDoc->m_pSecondRightClick);
				m_pDoc->setSize(sqrt(pow((m_pDoc->m_pFirstRightClick.x - m_pDoc->m_pSecondRightClick.x), 2) + pow((m_pDoc->m_pFirstRightClick.y - m_pDoc->m_pSecondRightClick.y), 2)));
			}
			
			break;

		default:
			printf("Unknown event!!\n");		
			break;
		}
	}

	if (m_bPrintFullImage) 
	{
		PaintFullImage(m_nSpacingSize);
		m_bPrintFullImage = false;
		glFlush();
	}

	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA

}


int PaintView::handle(int event)
{
	switch(event)
	{
	case FL_ENTER:
	    redraw();
		break;
	case FL_PUSH:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DOWN;
		else
			eventToDo=LEFT_MOUSE_DOWN;
		isAnEvent=1;
		redraw();
		break;
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DRAG;
		else
			eventToDo=LEFT_MOUSE_DRAG;
		isAnEvent=1;
		redraw();

		// Update the OriginalView marker position
		m_pDoc->m_pUI->getOriginalView()->setMarkerPos(coord.x, coord.y);
		break;
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		redraw();
		break;
	case FL_MOVE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();

		// Update the OriginalView marker position
		m_pDoc->m_pUI->getOriginalView()->setMarkerPos(coord.x, coord.y);
		break;
	default:
		return 0;
		break;

	}



	return 1;
}

void PaintView::refresh()
{
	redraw();
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

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );
}


void PaintView::RestoreContent()
{
	glDrawBuffer(GL_BACK);

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);

//	glDrawBuffer(GL_FRONT);
}

void PaintView::PaintFullImage(int spacingSize) 
{
	int drawWidth, drawHeight;
	drawWidth = min(m_nWindowWidth, m_pDoc->m_nPaintWidth);
	drawHeight = min(m_nWindowHeight, m_pDoc->m_nPaintHeight);

	int startrow = m_pDoc->m_nPaintHeight - drawHeight;
	if (startrow < 0) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting +
		3 * (m_pDoc->m_nPaintWidth * startrow);

	m_nDrawWidth = drawWidth;
	m_nDrawHeight = drawHeight;

	m_nStartRow = startrow;
	m_nEndRow = startrow + drawHeight;
	m_nStartCol = 0;
	m_nEndCol = m_nStartCol + drawWidth;
	Point source(coord.x + m_nStartCol, m_nEndRow - coord.y);
	Point target(coord.x, m_nWindowHeight - coord.y);
	// int Width = w();
	// int Height = h();
	// Point source{ 0,0 };
	// Point target{ /*m_nDraw*/Width,0};
	// int Width = m_pDoc->m_nWidth;
	// int Height = m_pDoc->m_nHeight;
	m_pDoc->m_pUI->setSize(m_pDoc->getSize());
	for (int y = 0; y < /*m_nDraw*/m_nDrawHeight; y = y + spacingSize) {
		for (int x = 0; x < /*m_nDraw*/m_nDrawWidth; x = x + spacingSize) {
			source = Point{ x + m_nStartCol, m_nEndRow - y };
			target = Point{ x, m_nWindowHeight - y };
			m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
			m_pDoc->m_pCurrentBrush->BrushEnd(source, target);
		}
	}
	glFlush();
}

void PaintView::SetPaintFullImageBool(int spacingSize) 
{
	m_bPrintFullImage = true;
	m_nSpacingSize = spacingSize;
	redraw();
}
