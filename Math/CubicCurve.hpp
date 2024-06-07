#pragma once
#include "Engine/Math/Vec2.hpp"

class CubicHermitiveCurve2D;
class CubicBezierCurve2D 
{
public:
	CubicBezierCurve2D(Vec2 startPos, Vec2 guidePos1, Vec2 guidePos2, Vec2 endPos);
	explicit CubicBezierCurve2D(CubicHermitiveCurve2D const& fromHermite);
	Vec2 EvaluateAtParametric(float parametricZeroToOne);
	float GetApproximateLength(int division =64);
	Vec2 EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubdivision = 64);

	Vec2 m_startPos;
	Vec2 m_endPos;
	Vec2 m_guidePos1;
	Vec2 m_guidePos2;
};

class CubicHermitiveCurve2D 
{
public:
	CubicHermitiveCurve2D(Vec2 startPos, Vec2 startVel, Vec2 endPos, Vec2 endVel);
	explicit CubicHermitiveCurve2D(CubicBezierCurve2D const& fromBezier);
	Vec2 EvaluateAtParametric(float parametricZeroToOne);
	float GetApproximateLength(int division = 64);
	Vec2 EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubdivision = 64);

	Vec2 m_startPos;
	Vec2 m_endPos;
	Vec2 m_startVel;
	Vec2 m_endVel;
};
