#include "beziercurveevaluator.h"
#include <assert.h>

void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	// TODO

	ptvEvaluatedCurvePts.clear();

	int iCtrlPtCount = ptvCtrlPts.size();

	vector<Vec4f> M_b = vector<Vec4f>{
		Vec4f(-1.0f, 3.0f, -3.0f, 1.0f),
		Vec4f(3.0f, -6.0f, 3.0f, 0.0f),
		Vec4f(-3.0f, 3.0f, 0.0f, 0.0f),
		Vec4f(1.0f, 0.0f, 0.0f, 0.0f) };

	if (iCtrlPtCount <= CUBIC) {
		if (!bWrap) {
			ptvEvaluatedCurvePts.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());
			unwrapped(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength);
		}
		else {
			if (iCtrlPtCount == 2) {
				ptvEvaluatedCurvePts.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());
				float mid_y = (ptvCtrlPts[0].y + ptvCtrlPts[1].y)/2;
				ptvEvaluatedCurvePts.push_back(Point(fAniLength, mid_y));
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin(), Point(0, mid_y));
			}
			else {
				vector<Vec4f> G_b = vector<Vec4f>{
				Vec4f{ptvCtrlPts[0].x, ptvCtrlPts[1].x, ptvCtrlPts[2].x, fAniLength + ptvCtrlPts[0].x},
				Vec4f{ptvCtrlPts[0].y, ptvCtrlPts[1].y, ptvCtrlPts[2].y,  ptvCtrlPts[0].y}
				};
				Vec4f x = Vec4f{ M_b[0] * G_b[0], M_b[1] * G_b[0], M_b[2] * G_b[0], M_b[3] * G_b[0] };
				Vec4f y = Vec4f{ M_b[0] * G_b[1], M_b[1] * G_b[1], M_b[2] * G_b[1], M_b[3] * G_b[1] };
				int count = 0;
				for (int j = 0; j <= s_inumInterpolation; ++j) {
					float interpolation = (float)j / (float)s_inumInterpolation;
					Vec4f T = { (float)pow(interpolation, 3.0), (float)pow(interpolation, 2.0), interpolation, 1.0 };
					float tempX = (float)(x * T);
					float tempY = (float)(y * T);
					if (tempX < fAniLength) {
						Point BezierPoint = Point{ tempX, tempY };
						// cout << "BezierPoint = " << BezierPoint.x << ", " << BezierPoint.y << endl;
						ptvEvaluatedCurvePts.push_back(BezierPoint);
					}
					else {
						if (!count) {
							Point BezierPoint = Point{ fAniLength, tempY };
							// cout << "BezierPoint = " << BezierPoint.x << ", " << BezierPoint.y << endl;
							ptvEvaluatedCurvePts.push_back(BezierPoint);
							BezierPoint = Point{ 0, tempY };
							// cout << "BezierPoint = " << BezierPoint.x << ", " << BezierPoint.y << endl;
							ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + count, BezierPoint);
						}
						else {
							tempX -= fAniLength;
							Point BezierPoint = Point{ tempX, tempY };
							// cout << "BezierPoint = " << BezierPoint.x << ", " << BezierPoint.y << endl;
							ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + count, BezierPoint);
						}
						count++;
					}
				}
			}
			// wrapped(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength);
		}
	}

	int i = 0;
	for (i = 0; i < iCtrlPtCount - CUBIC; i += CUBIC) {
		vector<Vec4f> G_b = getG_b(ptvCtrlPts, iCtrlPtCount, i);
		Vec4f x = Vec4f{ M_b[0] * G_b[0], M_b[1] * G_b[0], M_b[2] * G_b[0], M_b[3] * G_b[0] };
		Vec4f y = Vec4f{ M_b[0] * G_b[1], M_b[1] * G_b[1], M_b[2] * G_b[1], M_b[3] * G_b[1] };
		for (int j = 0; j <= s_inumInterpolation; ++j) {
			float interpolation = (float)j / (float)s_inumInterpolation;
			Vec4f T = { (float)pow(interpolation, 3.0), (float)pow(interpolation, 2.0), interpolation, 1.0 };
			Point BezierPoint = Point{ (float)(x * T), (float)(y * T) };
			// cout << "BezierPoint = " << BezierPoint.x << ", " << BezierPoint.y << endl;
			ptvEvaluatedCurvePts.push_back(BezierPoint);
		}
	}
	int pointsLeft = iCtrlPtCount - i - 1;
	if (!bWrap) {
		for (; i < iCtrlPtCount; ++i) {
			ptvEvaluatedCurvePts.push_back(ptvCtrlPts[i]);
		}
		unwrapped(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength);
	}
	else {
		if (pointsLeft == 0) {
			float mid_y = (ptvCtrlPts[0].y + ptvCtrlPts[ptvCtrlPts.size() - 1].y)/2;
			ptvEvaluatedCurvePts.push_back(Point(fAniLength, mid_y));
			ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin(), Point(0, mid_y));
		}
		else if (pointsLeft == 1) {
			ptvEvaluatedCurvePts.push_back(ptvCtrlPts[ptvCtrlPts.size() - 1]);
			float mid_y = (ptvCtrlPts[0].y + ptvCtrlPts[ptvCtrlPts.size() - 1].y)/2;
			ptvEvaluatedCurvePts.push_back(Point(fAniLength, mid_y));
			ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin(), Point(0, mid_y));
		}
		else {	// == 2
			vector<Vec4f> G_b = vector<Vec4f>{
				Vec4f{ptvCtrlPts[i].x, ptvCtrlPts[i + 1].x, ptvCtrlPts[i + 2].x, fAniLength + ptvCtrlPts[0].x},
				Vec4f{ptvCtrlPts[i].y, ptvCtrlPts[i + 1].y, ptvCtrlPts[i + 2].y,  ptvCtrlPts[0].y}
			};
			Vec4f x = Vec4f{ M_b[0] * G_b[0], M_b[1] * G_b[0], M_b[2] * G_b[0], M_b[3] * G_b[0] };
			Vec4f y = Vec4f{ M_b[0] * G_b[1], M_b[1] * G_b[1], M_b[2] * G_b[1], M_b[3] * G_b[1] };
			int count = 0;
			for (int j = 0; j <= s_inumInterpolation; ++j) {
				float interpolation = (float)j / (float)s_inumInterpolation;
				Vec4f T = { (float)pow(interpolation, 3.0), (float)pow(interpolation, 2.0), interpolation, 1.0 };
				float tempX = (float)(x * T);
				float tempY = (float)(y * T);
				if (tempX < fAniLength) {
					Point BezierPoint = Point{ tempX, tempY };
					// cout << "BezierPoint = " << BezierPoint.x << ", " << BezierPoint.y << endl;
					ptvEvaluatedCurvePts.push_back(BezierPoint);
				}
				else {
					if (!count) {
						Point BezierPoint = Point{ fAniLength, tempY };
						// cout << "BezierPoint = " << BezierPoint.x << ", " << BezierPoint.y << endl;
						ptvEvaluatedCurvePts.push_back(BezierPoint);
						BezierPoint = Point{ 0, tempY };
						// cout << "BezierPoint = " << BezierPoint.x << ", " << BezierPoint.y << endl;
						ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + count, BezierPoint);
					}
					else {
						tempX -= fAniLength;
						Point BezierPoint = Point{ tempX, tempY };
						// cout << "BezierPoint = " << BezierPoint.x << ", " << BezierPoint.y << endl;
						ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + count, BezierPoint);
					}
					count++;
				}
			}
		}
	}
}

void BezierCurveEvaluator::unwrapped(const std::vector<Point>& ptvCtrlPts, std::vector<Point>& ptvEvaluatedCurvePts, float fAniLength) const
{
	ptvEvaluatedCurvePts.push_back(Point{ fAniLength, ptvCtrlPts[ptvCtrlPts.size() - 1].y });
	ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin(), Point{ 0, ptvCtrlPts[0].y });
}

void BezierCurveEvaluator::wrapped(const std::vector<Point>& ptvCtrlPts, std::vector<Point>& ptvEvaluatedCurvePts, float fAniLength) const {
	vector<Vec4f> M_b = vector<Vec4f>{
		Vec4f(-1.0f, 3.0f, -3.0f, 1.0f),
		Vec4f(3.0f, -6.0f, 3.0f, 0.0f),
		Vec4f(-3.0f, 3.0f, 0.0f, 0.0f),
		Vec4f(1.0f, 0.0f, 0.0f, 0.0f) };
	
	float mid_y = (ptvCtrlPts[ptvCtrlPts.size() - 1].y + ptvCtrlPts[0].y) / 2.0;
	float maxMinusLastX = fAniLength - ptvCtrlPts[ptvCtrlPts.size() - 1].x;
	vector<Vec4f> G_b = vector<Vec4f>{
		Vec4f{0, 0, maxMinusLastX + ptvCtrlPts[0].x, maxMinusLastX + ptvCtrlPts[0].x},
		Vec4f{ptvCtrlPts[ptvCtrlPts.size() - 1].y, mid_y, mid_y, ptvCtrlPts[0].y},
	};
	Vec4f x = Vec4f{ M_b[0] * G_b[0], M_b[1] * G_b[0], M_b[2] * G_b[0], M_b[3] * G_b[0] };
	Vec4f y = Vec4f{ M_b[0] * G_b[1], M_b[1] * G_b[1], M_b[2] * G_b[1], M_b[3] * G_b[1] };
	int halfTotalInterpolation = s_inumInterpolation / 2;
	int count = 0;
	for (int j = 0; j <= s_inumInterpolation; ++j) {
		float interpolation = (float)j / (float)s_inumInterpolation;
		Vec4f T = { (float)pow(interpolation, 3.0), (float)pow(interpolation, 2.0), interpolation, 1.0 };
		Point BezierPointA, BezierPointB;
		float tempX = (float)(x * T);
		float tempY = (float)(y * T);
		int jMinusHalf = j - halfTotalInterpolation;
		if (tempX < maxMinusLastX) {
			tempX += ptvCtrlPts[ptvCtrlPts.size() - 1].x;
			BezierPointA = Point{ tempX, tempY };
			ptvEvaluatedCurvePts.push_back(BezierPointA);
			// cout << "BezierPointA = " << BezierPointA.x << ", " << BezierPointA.y << endl;
		}
		else {
			if (!count) {	// draw the last point on the screen ends
				BezierPointA = Point{ fAniLength, tempY };
				ptvEvaluatedCurvePts.push_back(BezierPointA);
				BezierPointB = Point{ 0, tempY };
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + count, BezierPointB);
			}
			else {
				tempX -= maxMinusLastX;
				BezierPointB = Point{ tempX, tempY };
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() + count, BezierPointB);
				// cout << "BezierPointB = " << BezierPointB.x << ", " << BezierPointB.y << endl;
			}
			count++;
		}
	}
}