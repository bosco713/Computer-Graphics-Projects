//
// ScatteredLineBrush.cpp
//
// The implementation of Scattered Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredLineBrush.h"
#include "math.h"

extern float frand();

ScatteredLineBrush::ScatteredLineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredLineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int width = pDoc->getLineWidth();

	glLineWidth((float)width);

	BrushMove(source, target);
}

void ScatteredLineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredLineBrush::BrushMove  document is NULL\n");
		return;
	}
	double alpha = pDoc->getAlpha();
	GLubyte* color = pDoc->getColor();

	glBegin(GL_LINES);		// logic: use size 1 lines, randomly paint pixel within the square
	double length_of_half_line = double(pDoc->getSize() / 2);
	double line_angle = ((double)pDoc->getLineAngle()) * M_PI / 180;

	for (int height = 0; height < (pDoc->getSize() + 1) / 2; ++height) {
		if (rand() % 2)
		{
			
			double offset = rand() % pDoc->getSize() - length_of_half_line;
			SetColor(Point(source.x + offset, source.y - height), alpha, color);		// pick one centre point using rand
			glVertex2d(target.x + offset - length_of_half_line * cos(line_angle), target.y - height - length_of_half_line * sin(line_angle));	// draw the line
			glVertex2d(target.x + offset + length_of_half_line * cos(line_angle), target.y - height + length_of_half_line * sin(line_angle));	// draw the line
		}
		if (rand() % 2)
		{
			double offset = rand() % pDoc->getSize() - length_of_half_line;
			SetColor(Point(source.x + offset, source.y + height), alpha, color);
			glVertex2d(target.x + offset - length_of_half_line * cos(line_angle), target.y + height - length_of_half_line * sin(line_angle));
			glVertex2d(target.x + offset + length_of_half_line * cos(line_angle), target.y + height + length_of_half_line * sin(line_angle));
		}
	}

	glEnd();
}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}


