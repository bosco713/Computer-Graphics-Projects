//
// TriangleBrush.h
//
// The header file for Triangle Brush. 
//

#ifndef TRIANGLEBRUSH_H
#define TRIANGLEBRUSH_H

#pragma once
#include "ImpBrush.h"
class TriangleBrush : public ImpBrush
{
public:
	TriangleBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif