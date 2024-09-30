//
// LineBrush.cpp
//
// The implementation of Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "LineBrush.h"
#include "math.h"

extern float frand();

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void LineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int width = pDoc->getLineWidth();


	glLineWidth((float)width);

	BrushMove(source, target);
}

void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("LineBrush::BrushMove  document is NULL\n");
		return;
	}
	double alpha = pDoc->getAlpha();
	GLubyte* color = pDoc->getColor();


	// glBegin(GL_POINTS);		// <-- make change to this can change the cursor type
	glBegin(GL_LINES);			// --> need to give two vertex 
		SetColor(source, alpha, color);

		double length_of_half_line = double (pDoc->getSize()) /2;
		double line_angle = ((double)pDoc->getLineAngle()) * M_PI / 180;
		glVertex2d(target.x - length_of_half_line * cos(line_angle), target.y - length_of_half_line * sin(line_angle));
		glVertex2d(target.x + length_of_half_line * cos(line_angle), target.y + length_of_half_line * sin(line_angle));

	glEnd();
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

