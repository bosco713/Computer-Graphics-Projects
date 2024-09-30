#include "bsplinecurveevaluator.h"
#include <assert.h>

void BSplineCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	// TODO
	ptvEvaluatedCurvePts.clear();

	int iCtrlPtCount = ptvCtrlPts.size();

	vector<Vec4f> convert = vector<Vec4f>{
		Vec4f(1.0f, 4.0f, 1.0f, 0.0f),
		Vec4f(0.0f, 4.0f, 2.0f, 0.0f),
		Vec4f(0.0f, 2.0f, 4.0f, 0.0f),
		Vec4f(0.0f, 1.0f, 4.0f, 1.0f) };

	vector<Vec4f> M_b = vector<Vec4f>{
		Vec4f(-1.0f, 3.0f, -3.0f, 1.0f),
		Vec4f(3.0f, -6.0f, 3.0f, 0.0f),
		Vec4f(-3.0f, 3.0f, 0.0f, 0.0f),
		Vec4f(1.0f, 0.0f, 0.0f, 0.0f) };

	if (iCtrlPtCount <= CUBIC) {	// at least have 2 points --> consider two case: 2pts, and 3pts
		if (!bWrap) {
			if (iCtrlPtCount == 2) {
				ptvEvaluatedCurvePts.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());
			}
			else {
				unwrappedFirstHalf(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength);
				unwrappedLastHalf(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength);
			}
			unwrapped(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength);
		}
		else {
			if (iCtrlPtCount == 2) {
				int count = 0;
				count = wrappedSecondOneThird(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength, count);
				count = wrappedSecondHalf(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength, count);
			}
			else {
				int count = 0;
				count = wrappedFirstOneThird(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength, count);
				count = wrappedSecondOneThird(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength, count);
				count = wrappedLastOneThird(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength, count);
			}
		}
		return;
	}

	for (int i = 0; i < iCtrlPtCount - CUBIC; i+=1) {
		vector<Vec4f> G_b = getG_b(ptvCtrlPts, iCtrlPtCount, i);
		Vec4f conv_x = Vec4f{ convert[0] * G_b[0], convert[1] * G_b[0], convert[2] * G_b[0], convert[3] * G_b[0] }/6;
		Vec4f conv_y = Vec4f{ convert[0] * G_b[1], convert[1] * G_b[1], convert[2] * G_b[1], convert[3] * G_b[1] }/6;
		G_b.clear();
		G_b.push_back(conv_x);
		G_b.push_back(conv_y);
		Vec4f x = Vec4f{ M_b[0] * G_b[0], M_b[1] * G_b[0], M_b[2] * G_b[0], M_b[3] * G_b[0] };
		Vec4f y = Vec4f{ M_b[0] * G_b[1], M_b[1] * G_b[1], M_b[2] * G_b[1], M_b[3] * G_b[1] };
		for (int j = 0; j < s_inumInterpolation; ++j) {
			float interpolation = (float)j / (float)s_inumInterpolation;
			Vec4f T = { (float)pow(interpolation, 3.0), (float)pow(interpolation, 2.0), interpolation, 1.0 };
			Point BSplinePoint = Point{ (float)(x * T), (float)(y * T) };
			ptvEvaluatedCurvePts.push_back(BSplinePoint);
		}
	}


	if (!bWrap) {
		unwrappedFirstHalf(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength);
		unwrappedLastHalf(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength);
		unwrapped(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength);
	}
	else {
		int count = 0;
		count = wrappedFirstOneThird(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength, count);
		count = wrappedSecondOneThird(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength, count);
		count = wrappedLastOneThird(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength, count);
	}
}

void BSplineCurveEvaluator::unwrapped(const std::vector<Point>& ptvCtrlPts, std::vector<Point>& ptvEvaluatedCurvePts, float fAniLength) const
{
	ptvEvaluatedCurvePts.push_back(ptvCtrlPts[ptvCtrlPts.size() - 1]);
	ptvEvaluatedCurvePts.push_back(Point{ fAniLength, ptvCtrlPts[ptvCtrlPts.size() - 1].y });
	ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin(), ptvCtrlPts[0]);
	ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin(), Point{ 0, ptvCtrlPts[0].y });
}

void BSplineCurveEvaluator::unwrappedFirstHalf(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength) const {
	vector<Vec4f> convert = vector<Vec4f>{
		Vec4f(1.0f, 4.0f, 1.0f, 0.0f),
		Vec4f(0.0f, 4.0f, 2.0f, 0.0f),
		Vec4f(0.0f, 2.0f, 4.0f, 0.0f),
		Vec4f(0.0f, 1.0f, 4.0f, 1.0f) };

	vector<Vec4f> M_b = vector<Vec4f>{
		Vec4f(-1.0f, 3.0f, -3.0f, 1.0f),
		Vec4f(3.0f, -6.0f, 3.0f, 0.0f),
		Vec4f(-3.0f, 3.0f, 0.0f, 0.0f),
		Vec4f(1.0f, 0.0f, 0.0f, 0.0f) };
	vector<Vec4f> G_b = vector<Vec4f>{
		Vec4f{ptvCtrlPts[0].x, ptvCtrlPts[0].x, ptvCtrlPts[1].x, ptvCtrlPts[2].x},
		Vec4f{ptvCtrlPts[0].y, ptvCtrlPts[0].y, ptvCtrlPts[1].y, ptvCtrlPts[2].y}
	};
	Vec4f conv_x = Vec4f{ convert[0] * G_b[0], convert[1] * G_b[0], convert[2] * G_b[0], convert[3] * G_b[0] } / 6;
	Vec4f conv_y = Vec4f{ convert[0] * G_b[1], convert[1] * G_b[1], convert[2] * G_b[1], convert[3] * G_b[1] } / 6;
	G_b.clear();
	G_b.push_back(conv_x);
	G_b.push_back(conv_y);
	Vec4f x = Vec4f{ M_b[0] * G_b[0], M_b[1] * G_b[0], M_b[2] * G_b[0], M_b[3] * G_b[0] };
	Vec4f y = Vec4f{ M_b[0] * G_b[1], M_b[1] * G_b[1], M_b[2] * G_b[1], M_b[3] * G_b[1] };
	for (int j = 0; j < s_inumInterpolation; ++j) {
		float interpolation = (float)j / (float)s_inumInterpolation;
		Vec4f T = { (float)pow(interpolation, 3.0), (float)pow(interpolation, 2.0), interpolation, 1.0 };
		Point BSplinePoint = Point{ (float)(x * T), (float)(y * T) };
		ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + j, BSplinePoint);
	}
}

void BSplineCurveEvaluator::unwrappedLastHalf(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength) const {
	vector<Vec4f> convert = vector<Vec4f>{
		Vec4f(1.0f, 4.0f, 1.0f, 0.0f),
		Vec4f(0.0f, 4.0f, 2.0f, 0.0f),
		Vec4f(0.0f, 2.0f, 4.0f, 0.0f),
		Vec4f(0.0f, 1.0f, 4.0f, 1.0f) };

	vector<Vec4f> M_b = vector<Vec4f>{
		Vec4f(-1.0f, 3.0f, -3.0f, 1.0f),
		Vec4f(3.0f, -6.0f, 3.0f, 0.0f),
		Vec4f(-3.0f, 3.0f, 0.0f, 0.0f),
		Vec4f(1.0f, 0.0f, 0.0f, 0.0f) };
	vector<Vec4f> G_b = vector<Vec4f>{
		Vec4f{ptvCtrlPts[ptvCtrlPts.size() - 3].x, ptvCtrlPts[ptvCtrlPts.size() - 2].x, ptvCtrlPts[ptvCtrlPts.size() - 1].x, ptvCtrlPts[ptvCtrlPts.size() - 1].x},
		Vec4f{ptvCtrlPts[ptvCtrlPts.size() - 3].y, ptvCtrlPts[ptvCtrlPts.size() - 2].y, ptvCtrlPts[ptvCtrlPts.size() - 1].y, ptvCtrlPts[ptvCtrlPts.size() - 1].y}
	};
	Vec4f conv_x = Vec4f{ convert[0] * G_b[0], convert[1] * G_b[0], convert[2] * G_b[0], convert[3] * G_b[0] } / 6;
	Vec4f conv_y = Vec4f{ convert[0] * G_b[1], convert[1] * G_b[1], convert[2] * G_b[1], convert[3] * G_b[1] } / 6;
	G_b.clear();
	G_b.push_back(conv_x);
	G_b.push_back(conv_y);
	Vec4f x = Vec4f{ M_b[0] * G_b[0], M_b[1] * G_b[0], M_b[2] * G_b[0], M_b[3] * G_b[0] };
	Vec4f y = Vec4f{ M_b[0] * G_b[1], M_b[1] * G_b[1], M_b[2] * G_b[1], M_b[3] * G_b[1] };
	for (int j = 0; j < s_inumInterpolation; ++j) {
		float interpolation = (float)j / (float)s_inumInterpolation;
		Vec4f T = { (float)pow(interpolation, 3.0), (float)pow(interpolation, 2.0), interpolation, 1.0 };
		Point BSplinePoint = Point{ (float)(x * T), (float)(y * T) };
		ptvEvaluatedCurvePts.push_back(BSplinePoint);
	}
}

int BSplineCurveEvaluator::wrappedSecondHalf(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const int& count) const {
	// Third iteration
	vector<Vec4f> convert = vector<Vec4f>{
		Vec4f(1.0f, 4.0f, 1.0f, 0.0f),
		Vec4f(0.0f, 4.0f, 2.0f, 0.0f),
		Vec4f(0.0f, 2.0f, 4.0f, 0.0f),
		Vec4f(0.0f, 1.0f, 4.0f, 1.0f) };

	vector<Vec4f> M_b = vector<Vec4f>{
		Vec4f(-1.0f, 3.0f, -3.0f, 1.0f),
		Vec4f(3.0f, -6.0f, 3.0f, 0.0f),
		Vec4f(-3.0f, 3.0f, 0.0f, 0.0f),
		Vec4f(1.0f, 0.0f, 0.0f, 0.0f) };
	float maxMinusLastX = fAniLength - ptvCtrlPts[ptvCtrlPts.size() - 1].x;
	vector<Vec4f> G_b = vector<Vec4f>{
		Vec4f{ 0, maxMinusLastX + ptvCtrlPts[0].x, maxMinusLastX + ptvCtrlPts[1].x, fAniLength + ptvCtrlPts[0].x },
		Vec4f{ ptvCtrlPts[ptvCtrlPts.size() - 1].y, ptvCtrlPts[0].y, ptvCtrlPts[1].y, ptvCtrlPts[0].y }
	};
	Vec4f conv_x = Vec4f{ convert[0] * G_b[0], convert[1] * G_b[0], convert[2] * G_b[0], convert[3] * G_b[0] } / 6;
	Vec4f conv_y = Vec4f{ convert[0] * G_b[1], convert[1] * G_b[1], convert[2] * G_b[1], convert[3] * G_b[1] } / 6;
	G_b.clear();
	G_b.push_back(conv_x);
	G_b.push_back(conv_y);
	Vec4f x = Vec4f{ M_b[0] * G_b[0], M_b[1] * G_b[0], M_b[2] * G_b[0], M_b[3] * G_b[0] };
	Vec4f y = Vec4f{ M_b[0] * G_b[1], M_b[1] * G_b[1], M_b[2] * G_b[1], M_b[3] * G_b[1] };
	int localCount = count;
	for (int j = 0; j <= s_inumInterpolation; ++j) {
		float interpolation = (float)j / (float)s_inumInterpolation;
		Vec4f T = { (float)pow(interpolation, 3.0), (float)pow(interpolation, 2.0), interpolation, 1.0 };
		Point BSplinePointA, BSplinePointB;
		float tempX = (float)(x * T);
		float tempY = (float)(y * T);
		if (tempX < maxMinusLastX) {
			tempX += ptvCtrlPts[ptvCtrlPts.size() - 1].x;
			BSplinePointA = Point{ tempX, tempY };
			ptvEvaluatedCurvePts.push_back(BSplinePointA);
			// cout << "BezierPointA = " << BezierPointA.x << ", " << BezierPointA.y << endl;
		}
		else {
			if (!localCount) {	// draw the last point on the screen ends
				BSplinePointA = Point{ fAniLength, tempY };
				ptvEvaluatedCurvePts.push_back(BSplinePointA);
				BSplinePointB = Point{ 0, tempY };
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + localCount, BSplinePointB);
			}
			else {
				tempX -= maxMinusLastX;
				BSplinePointB = Point{ tempX, tempY };
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + localCount, BSplinePointB);
				// cout << "BezierPointB = " << BezierPointB.x << ", " << BezierPointB.y << endl;
			}
			localCount++;
		}
	}
	return localCount;
}

int BSplineCurveEvaluator::wrappedFirstOneThird(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength, 
	const int& count) const {
	vector<Vec4f> convert = vector<Vec4f>{
		Vec4f(1.0f, 4.0f, 1.0f, 0.0f),
		Vec4f(0.0f, 4.0f, 2.0f, 0.0f),
		Vec4f(0.0f, 2.0f, 4.0f, 0.0f),
		Vec4f(0.0f, 1.0f, 4.0f, 1.0f) };

	vector<Vec4f> M_b = vector<Vec4f>{
		Vec4f(-1.0f, 3.0f, -3.0f, 1.0f),
		Vec4f(3.0f, -6.0f, 3.0f, 0.0f),
		Vec4f(-3.0f, 3.0f, 0.0f, 0.0f),
		Vec4f(1.0f, 0.0f, 0.0f, 0.0f) };
	float maxMinusThiLastX = fAniLength - ptvCtrlPts[ptvCtrlPts.size() - 3].x;
	float maxMinusSecLastX = fAniLength - ptvCtrlPts[ptvCtrlPts.size() - 2].x;
	float maxMinusLastX = fAniLength - ptvCtrlPts[ptvCtrlPts.size() - 1].x;
	// First iteration
	vector<Vec4f> G_b = vector<Vec4f>{
		Vec4f{0, maxMinusThiLastX - maxMinusSecLastX, maxMinusThiLastX - maxMinusLastX, maxMinusThiLastX + ptvCtrlPts[0].x},
		Vec4f{ptvCtrlPts[ptvCtrlPts.size() - 3].y, ptvCtrlPts[ptvCtrlPts.size() - 2].y, ptvCtrlPts[ptvCtrlPts.size() - 1].y, ptvCtrlPts[0].y}
	};
	Vec4f conv_x = Vec4f{ convert[0] * G_b[0], convert[1] * G_b[0], convert[2] * G_b[0], convert[3] * G_b[0] } / 6;
	Vec4f conv_y = Vec4f{ convert[0] * G_b[1], convert[1] * G_b[1], convert[2] * G_b[1], convert[3] * G_b[1] } / 6;
	G_b.clear();
	G_b.push_back(conv_x);
	G_b.push_back(conv_y);
	Vec4f x = Vec4f{ M_b[0] * G_b[0], M_b[1] * G_b[0], M_b[2] * G_b[0], M_b[3] * G_b[0] };
	Vec4f y = Vec4f{ M_b[0] * G_b[1], M_b[1] * G_b[1], M_b[2] * G_b[1], M_b[3] * G_b[1] };
	int localCount = count;
	for (int j = 0; j <= s_inumInterpolation; ++j) {
		float interpolation = (float)j / (float)s_inumInterpolation;
		Vec4f T = { (float)pow(interpolation, 3.0), (float)pow(interpolation, 2.0), interpolation, 1.0 };
		Point BSplinePointA, BSplinePointB;
		float tempX = (float)(x * T);
		float tempY = (float)(y * T);
		if (tempX < maxMinusThiLastX) {
			tempX += ptvCtrlPts[ptvCtrlPts.size() - 3].x;
			BSplinePointA = Point{ tempX, tempY };
			ptvEvaluatedCurvePts.push_back(BSplinePointA);
			// cout << "BezierPointA = " << BezierPointA.x << ", " << BezierPointA.y << endl;
		}
		else {
			if (!localCount) {	// draw the last point on the screen ends
				BSplinePointA = Point{ fAniLength, tempY };
				ptvEvaluatedCurvePts.push_back(BSplinePointA);
				BSplinePointB = Point{ 0, tempY };
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + localCount, BSplinePointB);
			}
			else {
				tempX -= maxMinusThiLastX;
				BSplinePointB = Point{ tempX, tempY };
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + localCount, BSplinePointB);
				// cout << "BezierPointB = " << BezierPointB.x << ", " << BezierPointB.y << endl;
			}
			localCount++;
		}
	}
	return localCount;
}

int BSplineCurveEvaluator::wrappedSecondOneThird(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const int& count) const {
	// Second iteration
	vector<Vec4f> convert = vector<Vec4f>{
		Vec4f(1.0f, 4.0f, 1.0f, 0.0f),
		Vec4f(0.0f, 4.0f, 2.0f, 0.0f),
		Vec4f(0.0f, 2.0f, 4.0f, 0.0f),
		Vec4f(0.0f, 1.0f, 4.0f, 1.0f) };

	vector<Vec4f> M_b = vector<Vec4f>{
		Vec4f(-1.0f, 3.0f, -3.0f, 1.0f),
		Vec4f(3.0f, -6.0f, 3.0f, 0.0f),
		Vec4f(-3.0f, 3.0f, 0.0f, 0.0f),
		Vec4f(1.0f, 0.0f, 0.0f, 0.0f) };
	float maxMinusSecLastX = fAniLength - ptvCtrlPts[ptvCtrlPts.size() - 2].x;
	float maxMinusLastX = fAniLength - ptvCtrlPts[ptvCtrlPts.size() - 1].x;
	vector<Vec4f> G_b = vector<Vec4f>{
		Vec4f{ 0, maxMinusSecLastX - maxMinusLastX, maxMinusSecLastX + ptvCtrlPts[0].x, maxMinusSecLastX + ptvCtrlPts[1].x },
		Vec4f{ ptvCtrlPts[ptvCtrlPts.size() - 2].y, ptvCtrlPts[ptvCtrlPts.size() - 1].y, ptvCtrlPts[0].y, ptvCtrlPts[1].y }
	};
	Vec4f conv_x = Vec4f{ convert[0] * G_b[0], convert[1] * G_b[0], convert[2] * G_b[0], convert[3] * G_b[0] } / 6;
	Vec4f conv_y = Vec4f{ convert[0] * G_b[1], convert[1] * G_b[1], convert[2] * G_b[1], convert[3] * G_b[1] } / 6;
	G_b.clear();
	G_b.push_back(conv_x);
	G_b.push_back(conv_y);
	Vec4f x = Vec4f{ M_b[0] * G_b[0], M_b[1] * G_b[0], M_b[2] * G_b[0], M_b[3] * G_b[0] };
	Vec4f y = Vec4f{ M_b[0] * G_b[1], M_b[1] * G_b[1], M_b[2] * G_b[1], M_b[3] * G_b[1] };
	int localCount = count;
	for (int j = 0; j <= s_inumInterpolation; ++j) {
		float interpolation = (float)j / (float)s_inumInterpolation;
		Vec4f T = { (float)pow(interpolation, 3.0), (float)pow(interpolation, 2.0), interpolation, 1.0 };
		Point BSplinePointA, BSplinePointB;
		float tempX = (float)(x * T);
		float tempY = (float)(y * T);
		if (tempX < maxMinusSecLastX) {
			tempX += ptvCtrlPts[ptvCtrlPts.size() - 2].x;
			BSplinePointA = Point{ tempX, tempY };
			ptvEvaluatedCurvePts.push_back(BSplinePointA);
			// cout << "BezierPointA = " << BezierPointA.x << ", " << BezierPointA.y << endl;
		}
		else {
			if (!localCount) {	// draw the last point on the screen ends
				BSplinePointA = Point{ fAniLength, tempY };
				ptvEvaluatedCurvePts.push_back(BSplinePointA);
				BSplinePointB = Point{ 0, tempY };
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + localCount, BSplinePointB);
			}
			else {
				tempX -= maxMinusSecLastX;
				BSplinePointB = Point{ tempX, tempY };
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + localCount, BSplinePointB);
				// cout << "BezierPointB = " << BezierPointB.x << ", " << BezierPointB.y << endl;
			}
			localCount++;
		}
	}
	return localCount;
}

int BSplineCurveEvaluator::wrappedLastOneThird(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const int& count) const {
	// Third iteration
	vector<Vec4f> convert = vector<Vec4f>{
		Vec4f(1.0f, 4.0f, 1.0f, 0.0f),
		Vec4f(0.0f, 4.0f, 2.0f, 0.0f),
		Vec4f(0.0f, 2.0f, 4.0f, 0.0f),
		Vec4f(0.0f, 1.0f, 4.0f, 1.0f) };

	vector<Vec4f> M_b = vector<Vec4f>{
		Vec4f(-1.0f, 3.0f, -3.0f, 1.0f),
		Vec4f(3.0f, -6.0f, 3.0f, 0.0f),
		Vec4f(-3.0f, 3.0f, 0.0f, 0.0f),
		Vec4f(1.0f, 0.0f, 0.0f, 0.0f) };
	float maxMinusLastX = fAniLength - ptvCtrlPts[ptvCtrlPts.size() - 1].x;
	vector<Vec4f> G_b = vector<Vec4f>{
		Vec4f{ 0, maxMinusLastX + ptvCtrlPts[0].x, maxMinusLastX + ptvCtrlPts[1].x, maxMinusLastX + ptvCtrlPts[2].x },
		Vec4f{ ptvCtrlPts[ptvCtrlPts.size() - 1].y, ptvCtrlPts[0].y, ptvCtrlPts[1].y, ptvCtrlPts[2].y } 
	};
	Vec4f conv_x = Vec4f{ convert[0] * G_b[0], convert[1] * G_b[0], convert[2] * G_b[0], convert[3] * G_b[0] } / 6;
	Vec4f conv_y = Vec4f{ convert[0] * G_b[1], convert[1] * G_b[1], convert[2] * G_b[1], convert[3] * G_b[1] } / 6;
	G_b.clear();
	G_b.push_back(conv_x);
	G_b.push_back(conv_y);
	Vec4f x = Vec4f{ M_b[0] * G_b[0], M_b[1] * G_b[0], M_b[2] * G_b[0], M_b[3] * G_b[0] };
	Vec4f y = Vec4f{ M_b[0] * G_b[1], M_b[1] * G_b[1], M_b[2] * G_b[1], M_b[3] * G_b[1] };
	int localCount = count;
	for (int j = 0; j <= s_inumInterpolation; ++j) {
		float interpolation = (float)j / (float)s_inumInterpolation;
		Vec4f T = { (float)pow(interpolation, 3.0), (float)pow(interpolation, 2.0), interpolation, 1.0 };
		Point BSplinePointA, BSplinePointB;
		float tempX = (float)(x * T);
		float tempY = (float)(y * T);
		if (tempX < maxMinusLastX) {
			tempX += ptvCtrlPts[ptvCtrlPts.size() - 1].x;
			BSplinePointA = Point{ tempX, tempY };
			ptvEvaluatedCurvePts.push_back(BSplinePointA);
			// cout << "BezierPointA = " << BezierPointA.x << ", " << BezierPointA.y << endl;
		}
		else {
			if (!localCount) {	// draw the last point on the screen ends
				BSplinePointA = Point{ fAniLength, tempY };
				ptvEvaluatedCurvePts.push_back(BSplinePointA);
				BSplinePointB = Point{ 0, tempY };
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + localCount, BSplinePointB);
			}
			else {
				tempX -= maxMinusLastX;
				BSplinePointB = Point{ tempX, tempY };
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + localCount, BSplinePointB);
				// cout << "BezierPointB = " << BezierPointB.x << ", " << BezierPointB.y << endl;
			}
			localCount++;
		}
	}
	return localCount;
}
