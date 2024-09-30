//
// ScatteredPointBrush.cpp
//
// The implementation of Scattered Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredPointBrush.h"

extern float frand();

ScatteredPointBrush::ScatteredPointBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredPointBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	// int size = pDoc->getSize();
	int size = 1;


	glPointSize((float)size);

	BrushMove(source, target);
}

void ScatteredPointBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredPointBrush::BrushMove  document is NULL\n");
		return;
	}
	double alpha = pDoc->getAlpha();
	GLubyte* color = pDoc->getColor();

	glBegin(GL_POINTS);		// logic: use size 1 points, randomly paint pixel within the square

	for (int width = 0; width < (pDoc->getSize() + 1) / 2; ++width) {
		for (int height = 0; height < (pDoc->getSize() + 1) / 2; ++height) {
			if (rand() % 2) 
			{
				SetColor(Point (source.x - width, source.y - height), alpha, color);
				glVertex2d(target.x - width, target.y - height);
			}
			if (rand() % 2)
			{
				SetColor(Point (source.x - width, source.y + height), alpha, color);
				glVertex2d(target.x - width, target.y + height);
			}
			if (rand() % 2)
			{
				SetColor(Point (source.x + width, source.y - height), alpha, color);
				glVertex2d(target.x + width, target.y - height);
			}
			if (rand() % 2)
			{
				SetColor(Point (source.x + width, source.y + height), alpha, color);
				glVertex2d(target.x + width, target.y + height);
			}
		}
	}

	glEnd();
}

void ScatteredPointBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

