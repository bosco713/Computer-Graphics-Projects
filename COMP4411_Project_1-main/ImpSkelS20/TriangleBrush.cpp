//
// TriangleBrush.cpp
//
// The implementation of Triangle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "TriangleBrush.h"
#include "math.h"

extern float frand();

TriangleBrush::TriangleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void TriangleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	//int width = pDoc->getLineWidth();


	//glLineWidth((float)width);

	BrushMove(source, target);
}

void TriangleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("TriangleBrush::BrushMove  document is NULL\n");
		return;
	}
	double alpha = pDoc->getAlpha();
	GLubyte* color = pDoc->getColor();


	// glBegin(GL_POINTS);		// <-- make change to this can change the cursor type
	glBegin(GL_TRIANGLES);			// --> need to give two vertex 
		SetColor(source, alpha, color);

		double size = double(pDoc->getSize());
		double v1_x = double(target.x);
		double v1_y = double(target.y) + 2 * size / 3;
		double v2_x = double(target.x) - (1 / sqrt(3)) * size;
		double v2_y = double(target.y) - double(1 / 3) * size;
		double v3_x = double(target.x) + (1 / sqrt(3)) * size;
		double v3_y = double(target.y) - double(1 / 3) * size;
		glVertex2d(v1_x, v1_y);
		glVertex2d(v2_x, v2_y);
		glVertex2d(v3_x, v3_y);

	glEnd();
}

void TriangleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

