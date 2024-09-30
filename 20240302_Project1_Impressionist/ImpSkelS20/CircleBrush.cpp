//
// CircleBrush.cpp
//
// The implementation of Circle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "CircleBrush.h"
#include "math.h"

extern float frand();

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void CircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize((float)size);

	BrushMove(source, target);
}

void CircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("CircleBrush::BrushMove  document is NULL\n");
		return;
	}
	double alpha = pDoc->getAlpha();
	GLubyte* color = pDoc->getColor();

	// glBegin(GL_POINTS);		// <-- make change to this can change the cursor type
	glBegin(GL_TRIANGLE_FAN);		// use 360 triangles to draw one circle	
		SetColor(source, alpha, color);

		double radius = double (pDoc->getSize())/2;
		double triangle_degree = 1;
		glVertex2d(target.x, target.y);

		for (int num = 0; num < 360 / triangle_degree; ++num) {
			double v1_x = target.x + radius * cos(triangle_degree * num * M_PI / 180);
			double v1_y = target.y + radius * sin(triangle_degree * num * M_PI / 180);
			double v2_x = target.x + radius * cos(triangle_degree * (num + 1) * M_PI / 180);
			double v2_y = target.y + radius * sin(triangle_degree * (num + 1) * M_PI / 180);

			glVertex2d(v1_x, v1_y);
			glVertex2d(v2_x, v2_y);
		}

	glEnd();
}

void CircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}


