#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/CubicCurve.hpp"
#include <vector>

class CatmullRomSpline 
{
public:
	CatmullRomSpline(std::vector<Vec2> points);
	Vec2 EvaluateAtParametric(float parametricZeroToOne,int currentBezierIndex); // Index of the line segment and begins from zero
	float GetApproximateLength(int currentBezierIndex, int division = 64);
	Vec2 EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubdivision = 64, int currentBezierIndex = 0);
	std::vector<Vec2>	m_points;
	std::vector<Vec2>	m_velocity;
	std::vector<CubicHermitiveCurve2D> m_hemitiveCurves;
};
