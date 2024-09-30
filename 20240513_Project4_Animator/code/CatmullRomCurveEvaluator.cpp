#include "CatmullRomCurveEvaluator.h"
#include <assert.h>

void CatmullRomCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	// TODO
	ptvEvaluatedCurvePts.clear();

	int iCtrlPtCount = ptvCtrlPts.size();

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

	vector<Vec4f> M_b = vector<Vec4f>{
		Vec4f(-1.0f, 3.0f, -3.0f, 1.0f),
		Vec4f(2.0f, -5.0f, 4.0f, -1.0f),
		Vec4f(-1.0f, 0.0f, 1.0f, 0.0f),
		Vec4f(0.0f, 2.0f, 0.0f, 0.0f) };

	for (int i = 0; i < iCtrlPtCount - CUBIC; i++) {
		vector<Vec4f> G_b = getG_b(ptvCtrlPts, iCtrlPtCount, i);
		Vec4f x = Vec4f{ M_b[0] * G_b[0], M_b[1] * G_b[0], M_b[2] * G_b[0], M_b[3] * G_b[0] }/2;
		Vec4f y = Vec4f{ M_b[0] * G_b[1], M_b[1] * G_b[1], M_b[2] * G_b[1], M_b[3] * G_b[1] }/2;
		for (int j = 0; j < s_inumInterpolation; ++j) {
			float interpolation = (float)j / (float)s_inumInterpolation;
			Vec4f T = { (float)pow(interpolation, 3.0), (float)pow(interpolation, 2.0), interpolation, 1.0 };
			Point CatmullRomPoint = Point{ (float)(x * T), (float)(y * T) };
			ptvEvaluatedCurvePts.push_back(CatmullRomPoint);
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

void CatmullRomCurveEvaluator::unwrapped(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength) const {
	ptvEvaluatedCurvePts.push_back(Point{ fAniLength, ptvCtrlPts[ptvCtrlPts.size() - 1].y });
	ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin(), Point{ 0, ptvCtrlPts[0].y });
}

void CatmullRomCurveEvaluator::unwrappedFirstHalf(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength) const {
	vector<Vec4f> M_b = vector<Vec4f>{
		Vec4f(-1.0f, 3.0f, -3.0f, 1.0f),
		Vec4f(2.0f, -5.0f, 4.0f, -1.0f),
		Vec4f(-1.0f, 0.0f, 1.0f, 0.0f),
		Vec4f(0.0f, 2.0f, 0.0f, 0.0f) };

	vector<Vec4f> G_b = vector<Vec4f>{
		Vec4f{ptvCtrlPts[0].x, ptvCtrlPts[0].x, ptvCtrlPts[1].x, ptvCtrlPts[2].x},
		Vec4f{ptvCtrlPts[0].y, ptvCtrlPts[0].y, ptvCtrlPts[1].y, ptvCtrlPts[2].y}
	};
	Vec4f x = Vec4f{ M_b[0] * G_b[0], M_b[1] * G_b[0], M_b[2] * G_b[0], M_b[3] * G_b[0] }/2;
	Vec4f y = Vec4f{ M_b[0] * G_b[1], M_b[1] * G_b[1], M_b[2] * G_b[1], M_b[3] * G_b[1] }/2;
	for (int j = 0; j < s_inumInterpolation; ++j) {
		float interpolation = (float)j / (float)s_inumInterpolation;
		Vec4f T = { (float)pow(interpolation, 3.0), (float)pow(interpolation, 2.0), interpolation, 1.0 };
		Point CatmullRomPoint = Point{ (float)(x * T), (float)(y * T) };
		ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + j, CatmullRomPoint);
	}
}

void CatmullRomCurveEvaluator::unwrappedLastHalf(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength) const {
	vector<Vec4f> M_b = vector<Vec4f>{
		Vec4f(-1.0f, 3.0f, -3.0f, 1.0f),
		Vec4f(2.0f, -5.0f, 4.0f, -1.0f),
		Vec4f(-1.0f, 0.0f, 1.0f, 0.0f),
		Vec4f(0.0f, 2.0f, 0.0f, 0.0f) };
	vector<Vec4f> G_b = vector<Vec4f>{
		Vec4f{ptvCtrlPts[ptvCtrlPts.size() - 3].x, ptvCtrlPts[ptvCtrlPts.size() - 2].x, ptvCtrlPts[ptvCtrlPts.size() - 1].x, ptvCtrlPts[ptvCtrlPts.size() - 1].x},
		Vec4f{ptvCtrlPts[ptvCtrlPts.size() - 3].y, ptvCtrlPts[ptvCtrlPts.size() - 2].y, ptvCtrlPts[ptvCtrlPts.size() - 1].y, ptvCtrlPts[ptvCtrlPts.size() - 1].y}
	};
	Vec4f x = Vec4f{ M_b[0] * G_b[0], M_b[1] * G_b[0], M_b[2] * G_b[0], M_b[3] * G_b[0] }/2;
	Vec4f y = Vec4f{ M_b[0] * G_b[1], M_b[1] * G_b[1], M_b[2] * G_b[1], M_b[3] * G_b[1] }/2;
	for (int j = 0; j < s_inumInterpolation; ++j) {
		float interpolation = (float)j / (float)s_inumInterpolation;
		Vec4f T = { (float)pow(interpolation, 3.0), (float)pow(interpolation, 2.0), interpolation, 1.0 };
		Point CatmullRomPoint = Point{ (float)(x * T), (float)(y * T) };
		ptvEvaluatedCurvePts.push_back(CatmullRomPoint);
	}
}

int CatmullRomCurveEvaluator::wrappedSecondHalf(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const int& count) const {
	// Third iteration
	vector<Vec4f> M_b = vector<Vec4f>{
		Vec4f(-1.0f, 3.0f, -3.0f, 1.0f),
		Vec4f(2.0f, -5.0f, 4.0f, -1.0f),
		Vec4f(-1.0f, 0.0f, 1.0f, 0.0f),
		Vec4f(0.0f, 2.0f, 0.0f, 0.0f) };
	float maxMinusLastX = fAniLength - ptvCtrlPts[ptvCtrlPts.size() - 1].x;
	vector<Vec4f> G_b = vector<Vec4f>{
		Vec4f{ 0, maxMinusLastX + ptvCtrlPts[0].x, maxMinusLastX + ptvCtrlPts[1].x, fAniLength + ptvCtrlPts[0].x },
		Vec4f{ ptvCtrlPts[ptvCtrlPts.size() - 1].y, ptvCtrlPts[0].y, ptvCtrlPts[1].y, ptvCtrlPts[0].y }
	};
	Vec4f x = Vec4f{ M_b[0] * G_b[0], M_b[1] * G_b[0], M_b[2] * G_b[0], M_b[3] * G_b[0] }/2;
	Vec4f y = Vec4f{ M_b[0] * G_b[1], M_b[1] * G_b[1], M_b[2] * G_b[1], M_b[3] * G_b[1] }/2;
	int localCount = count;
	for (int j = 0; j <= s_inumInterpolation; ++j) {
		float interpolation = (float)j / (float)s_inumInterpolation;
		Vec4f T = { (float)pow(interpolation, 3.0), (float)pow(interpolation, 2.0), interpolation, 1.0 };
		Point CatmullRomPointA, CatmullRomPointB;
		float tempX = (float)(x * T);
		float tempY = (float)(y * T);
		if (tempX < maxMinusLastX) {
			tempX += ptvCtrlPts[ptvCtrlPts.size() - 1].x;
			CatmullRomPointA = Point{ tempX, tempY };
			ptvEvaluatedCurvePts.push_back(CatmullRomPointA);
			// cout << "BezierPointA = " << BezierPointA.x << ", " << BezierPointA.y << endl;
		}
		else {
			if (!localCount) {	// draw the last point on the screen ends
				CatmullRomPointA = Point{ fAniLength, tempY };
				ptvEvaluatedCurvePts.push_back(CatmullRomPointA);
				CatmullRomPointB = Point{ 0, tempY };
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + localCount, CatmullRomPointB);
			}
			else {
				tempX -= maxMinusLastX;
				CatmullRomPointB = Point{ tempX, tempY };
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + localCount, CatmullRomPointB);
				// cout << "BezierPointB = " << BezierPointB.x << ", " << BezierPointB.y << endl;
			}
			localCount++;
		}
	}
	return localCount;
}

int CatmullRomCurveEvaluator::wrappedFirstOneThird(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const int& count) const {
	vector<Vec4f> M_b = vector<Vec4f>{
		Vec4f(-1.0f, 3.0f, -3.0f, 1.0f),
		Vec4f(2.0f, -5.0f, 4.0f, -1.0f),
		Vec4f(-1.0f, 0.0f, 1.0f, 0.0f),
		Vec4f(0.0f, 2.0f, 0.0f, 0.0f) };
	float maxMinusThiLastX = fAniLength - ptvCtrlPts[ptvCtrlPts.size() - 3].x;
	float maxMinusSecLastX = fAniLength - ptvCtrlPts[ptvCtrlPts.size() - 2].x;
	float maxMinusLastX = fAniLength - ptvCtrlPts[ptvCtrlPts.size() - 1].x;
	// First iteration
	vector<Vec4f> G_b = vector<Vec4f>{
		Vec4f{0, maxMinusThiLastX - maxMinusSecLastX, maxMinusThiLastX - maxMinusLastX, maxMinusThiLastX + ptvCtrlPts[0].x},
		Vec4f{ptvCtrlPts[ptvCtrlPts.size() - 3].y, ptvCtrlPts[ptvCtrlPts.size() - 2].y, ptvCtrlPts[ptvCtrlPts.size() - 1].y, ptvCtrlPts[0].y}
	};
	Vec4f x = Vec4f{ M_b[0] * G_b[0], M_b[1] * G_b[0], M_b[2] * G_b[0], M_b[3] * G_b[0] }/2;
	Vec4f y = Vec4f{ M_b[0] * G_b[1], M_b[1] * G_b[1], M_b[2] * G_b[1], M_b[3] * G_b[1] }/2;
	int localCount = count;
	for (int j = 0; j <= s_inumInterpolation; ++j) {
		float interpolation = (float)j / (float)s_inumInterpolation;
		Vec4f T = { (float)pow(interpolation, 3.0), (float)pow(interpolation, 2.0), interpolation, 1.0 };
		Point CatmullRomPointA, CatmullRomPointB;
		float tempX = (float)(x * T);
		float tempY = (float)(y * T);
		if (tempX < maxMinusThiLastX) {
			tempX += ptvCtrlPts[ptvCtrlPts.size() - 3].x;
			CatmullRomPointA = Point{ tempX, tempY };
			ptvEvaluatedCurvePts.push_back(CatmullRomPointA);
			// cout << "BezierPointA = " << BezierPointA.x << ", " << BezierPointA.y << endl;
		}
		else {
			if (!localCount) {	// draw the last point on the screen ends
				CatmullRomPointA = Point{ fAniLength, tempY };
				ptvEvaluatedCurvePts.push_back(CatmullRomPointA);
				CatmullRomPointB = Point{ 0, tempY };
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + localCount, CatmullRomPointB);
			}
			else {
				tempX -= maxMinusThiLastX;
				CatmullRomPointB = Point{ tempX, tempY };
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + localCount, CatmullRomPointB);
				// cout << "BezierPointB = " << BezierPointB.x << ", " << BezierPointB.y << endl;
			}
			localCount++;
		}
	}
	return localCount;
}

int CatmullRomCurveEvaluator::wrappedSecondOneThird(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const int& count) const {
	// Second iteration
	vector<Vec4f> M_b = vector<Vec4f>{
		Vec4f(-1.0f, 3.0f, -3.0f, 1.0f),
		Vec4f(2.0f, -5.0f, 4.0f, -1.0f),
		Vec4f(-1.0f, 0.0f, 1.0f, 0.0f),
		Vec4f(0.0f, 2.0f, 0.0f, 0.0f) };
	float maxMinusSecLastX = fAniLength - ptvCtrlPts[ptvCtrlPts.size() - 2].x;
	float maxMinusLastX = fAniLength - ptvCtrlPts[ptvCtrlPts.size() - 1].x;
	vector<Vec4f> G_b = vector<Vec4f>{
		Vec4f{ 0, maxMinusSecLastX - maxMinusLastX, maxMinusSecLastX + ptvCtrlPts[0].x, maxMinusSecLastX + ptvCtrlPts[1].x },
		Vec4f{ ptvCtrlPts[ptvCtrlPts.size() - 2].y, ptvCtrlPts[ptvCtrlPts.size() - 1].y, ptvCtrlPts[0].y, ptvCtrlPts[1].y }
	};
	Vec4f x = Vec4f{ M_b[0] * G_b[0], M_b[1] * G_b[0], M_b[2] * G_b[0], M_b[3] * G_b[0] }/2;
	Vec4f y = Vec4f{ M_b[0] * G_b[1], M_b[1] * G_b[1], M_b[2] * G_b[1], M_b[3] * G_b[1] }/2;
	int localCount = count;
	for (int j = 0; j <= s_inumInterpolation; ++j) {
		float interpolation = (float)j / (float)s_inumInterpolation;
		Vec4f T = { (float)pow(interpolation, 3.0), (float)pow(interpolation, 2.0), interpolation, 1.0 };
		Point CatmullRomPointA, CatmullRomPointB;
		float tempX = (float)(x * T);
		float tempY = (float)(y * T);
		if (tempX < maxMinusSecLastX) {
			tempX += ptvCtrlPts[ptvCtrlPts.size() - 2].x;
			CatmullRomPointA = Point{ tempX, tempY };
			ptvEvaluatedCurvePts.push_back(CatmullRomPointA);
			// cout << "BezierPointA = " << BezierPointA.x << ", " << BezierPointA.y << endl;
		}
		else {
			if (!localCount) {	// draw the last point on the screen ends
				CatmullRomPointA = Point{ fAniLength, tempY };
				ptvEvaluatedCurvePts.push_back(CatmullRomPointA);
				CatmullRomPointB = Point{ 0, tempY };
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + localCount, CatmullRomPointB);
			}
			else {
				tempX -= maxMinusSecLastX;
				CatmullRomPointB = Point{ tempX, tempY };
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + localCount, CatmullRomPointB);
				// cout << "BezierPointB = " << BezierPointB.x << ", " << BezierPointB.y << endl;
			}
			localCount++;
		}
	}
	return localCount;
}

int CatmullRomCurveEvaluator::wrappedLastOneThird(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const int& count) const {
	// Third iteration
	vector<Vec4f> M_b = vector<Vec4f>{
		Vec4f(-1.0f, 3.0f, -3.0f, 1.0f),
		Vec4f(2.0f, -5.0f, 4.0f, -1.0f),
		Vec4f(-1.0f, 0.0f, 1.0f, 0.0f),
		Vec4f(0.0f, 2.0f, 0.0f, 0.0f) };
	float maxMinusLastX = fAniLength - ptvCtrlPts[ptvCtrlPts.size() - 1].x;
	vector<Vec4f> G_b = vector<Vec4f>{
		Vec4f{ 0, maxMinusLastX + ptvCtrlPts[0].x, maxMinusLastX + ptvCtrlPts[1].x, maxMinusLastX + ptvCtrlPts[2].x },
		Vec4f{ ptvCtrlPts[ptvCtrlPts.size() - 1].y, ptvCtrlPts[0].y, ptvCtrlPts[1].y, ptvCtrlPts[2].y }
	};
	Vec4f x = Vec4f{ M_b[0] * G_b[0], M_b[1] * G_b[0], M_b[2] * G_b[0], M_b[3] * G_b[0] }/2;
	Vec4f y = Vec4f{ M_b[0] * G_b[1], M_b[1] * G_b[1], M_b[2] * G_b[1], M_b[3] * G_b[1] }/2;
	int localCount = count;
	for (int j = 0; j <= s_inumInterpolation; ++j) {
		float interpolation = (float)j / (float)s_inumInterpolation;
		Vec4f T = { (float)pow(interpolation, 3.0), (float)pow(interpolation, 2.0), interpolation, 1.0 };
		Point CatmullRomPointA, CatmullRomPointB;
		float tempX = (float)(x * T);
		float tempY = (float)(y * T);
		if (tempX < maxMinusLastX) {
			tempX += ptvCtrlPts[ptvCtrlPts.size() - 1].x;
			CatmullRomPointA = Point{ tempX, tempY };
			ptvEvaluatedCurvePts.push_back(CatmullRomPointA);
			// cout << "BezierPointA = " << BezierPointA.x << ", " << BezierPointA.y << endl;
		}
		else {
			if (!localCount) {	// draw the last point on the screen ends
				CatmullRomPointA = Point{ fAniLength, tempY };
				ptvEvaluatedCurvePts.push_back(CatmullRomPointA);
				CatmullRomPointB = Point{ 0, tempY };
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + localCount, CatmullRomPointB);
			}
			else {
				tempX -= maxMinusLastX;
				CatmullRomPointB = Point{ tempX, tempY };
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + localCount, CatmullRomPointB);
				// cout << "BezierPointB = " << BezierPointB.x << ", " << BezierPointB.y << endl;
			}
			localCount++;
		}
	}
	return localCount;
}