#include "CurveEvaluator.h"

float CurveEvaluator::s_fFlatnessEpsilon = 0.00001f;
int CurveEvaluator::s_iSegCount = 16;
int CurveEvaluator::s_inumInterpolation = 19;

CurveEvaluator::~CurveEvaluator(void)
{
}

vector<Vec4f> CurveEvaluator::getG_b(const std::vector<Point>& ptvCtrlPts, int iCtrlPtCount, int i) const {
	Point ctrlPoint_1 = Point{ ptvCtrlPts[i].x, ptvCtrlPts[i].y };
	Point ctrlPoint_2 = Point{ ptvCtrlPts[i + 1].x, ptvCtrlPts[i + 1].y };
	Point ctrlPoint_3 = Point{ ptvCtrlPts[i + 2].x, ptvCtrlPts[i + 2].y };
	Point ctrlPoint_4 = Point{ ptvCtrlPts[i + 3].x, ptvCtrlPts[i + 3].y };
	vector<Vec4f> G_b = vector<Vec4f>{
		Vec4f{ctrlPoint_1.x, ctrlPoint_2.x, ctrlPoint_3.x, ctrlPoint_4.x},
		Vec4f{ctrlPoint_1.y, ctrlPoint_2.y, ctrlPoint_3.y, ctrlPoint_4.y}
	};
	return G_b;
}


