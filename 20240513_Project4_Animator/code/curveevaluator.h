#ifndef INCLUDED_CURVE_EVALUATOR_H
#define INCLUDED_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)

#include "Curve.h"
#include "vector"
#include "vec.h"
#include "math.h"

//using namespace std;

#define CUBIC 3

class CurveEvaluator
{
public:
	virtual ~CurveEvaluator(void);
	virtual void evaluateCurve(const std::vector<Point>& control_points, 
							   std::vector<Point>& evaluated_curve_points, 
							   const float& animation_length, 
							   const bool& wrap_control_points) const = 0;
	vector<Vec4f> CurveEvaluator::getG_b(const std::vector<Point>& ptvCtrlPts, int iCtrlPtCount, int i) const;
	static float s_fFlatnessEpsilon;
	static int s_iSegCount;
	static int s_inumInterpolation;
};


#endif