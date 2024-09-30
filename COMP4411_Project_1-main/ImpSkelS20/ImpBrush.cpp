//
// ImpBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ImpBrush.h"


// Static class member initializations
int			ImpBrush::c_nBrushCount	= 0;
ImpBrush**	ImpBrush::c_pBrushes	= NULL;

ImpBrush::ImpBrush(ImpressionistDoc*	pDoc, 
				   char*				name) :
					m_pDoc(pDoc), 
					m_pBrushName(name)
{
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* ImpBrush::GetDocument(void)
{
	return m_pDoc;
}

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
char* ImpBrush::BrushName(void)
{
	return m_pBrushName;
}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample 
// the color from
//----------------------------------------------------
void ImpBrush::SetColor (const Point source, double alpha, GLubyte* color)
{
	ImpressionistDoc* pDoc = GetDocument();

	GLubyte sourceColor[3];

	memcpy (sourceColor, pDoc->GetOriginalPixel( source ), 3 );

	GLubyte finalColor[3];
	finalColor[0] = static_cast<GLubyte>((sourceColor[0] * color[0] / 255));
	finalColor[1] = static_cast<GLubyte>((sourceColor[1] * color[1] / 255));
	finalColor[2] = static_cast<GLubyte>((sourceColor[2] * color[2] / 255));

	glColor4ub(finalColor[0], finalColor[1], finalColor[2], static_cast<GLubyte>(alpha * 255));
}