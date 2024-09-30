//
// ScatteredCircleBrush.cpp
//
// The implementation of Scattered Circle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredCircleBrush.h"
#include "math.h"

extern float frand();

ScatteredCircleBrush::ScatteredCircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredCircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();
	// int size = 1;


	glPointSize((float)size);

	BrushMove(source, target);
}

void ScatteredCircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredCircleBrush::BrushMove  document is NULL\n");
		return;
	}
	double alpha = pDoc->getAlpha();
	GLubyte* color = pDoc->getColor();

	for (int height = 0; height < (pDoc->getSize() + 1) / 2; ++height) {
		if (rand() % 2)
		{
			glBegin(GL_TRIANGLE_FAN);		// logic: use size 1 circle (?), randomly paint pixel within the square
			double offset = rand() % pDoc->getSize() - double(pDoc->getSize() / 2);
			SetColor(Point(source.x + offset, source.y - height), alpha, color);		// pick one centre point using rand

			double radius = double(pDoc->getSize()) / 2;
			double triangle_degree = 1;
			glVertex2d(target.x + offset, target.y);

			for (int num = 0; num < 360 / triangle_degree; ++num) {
				double v1_x = target.x + offset + radius * cos(triangle_degree * num * M_PI / 180);
				double v1_y = target.y - height + radius * sin(triangle_degree * num * M_PI / 180);
				double v2_x = target.x + offset + radius * cos(triangle_degree * (num + 1) * M_PI / 180);
				double v2_y = target.y - height + radius * sin(triangle_degree * (num + 1) * M_PI / 180);

				glVertex2d(v1_x, v1_y);
				glVertex2d(v2_x, v2_y);
			}

			glEnd;
		}
		if (rand() % 2)
		{
			glBegin(GL_TRIANGLE_FAN);		// logic: use size 1 circle (?), randomly paint pixel within the square
			double offset = rand() % pDoc->getSize() - double(pDoc->getSize() / 2);
			SetColor(Point(source.x + offset, source.y - height), alpha, color);		// pick one centre point using rand

			double radius = double(pDoc->getSize()) / 2;
			double triangle_degree = 1;
			glVertex2d(target.x + offset, target.y);

			for (int num = 0; num < 360 / triangle_degree; ++num) {
				double v1_x = target.x + offset + radius * cos(triangle_degree * num * M_PI / 180);
				double v1_y = target.y + height + radius * sin(triangle_degree * num * M_PI / 180);
				double v2_x = target.x + offset + radius * cos(triangle_degree * (num + 1) * M_PI / 180);
				double v2_y = target.y + height + radius * sin(triangle_degree * (num + 1) * M_PI / 180);

				glVertex2d(v1_x, v1_y);
				glVertex2d(v2_x, v2_y);
			}

			glEnd;
		}
	}

	glEnd();
}

void ScatteredCircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}



