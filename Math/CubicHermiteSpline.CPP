#include "Engine/Math/CubicHermiteSpline.hpp"

CatmullRomSpline::CatmullRomSpline(std::vector<Vec2> points)
:m_points(points)
{
	m_velocity.reserve(points.size());

	for (int i = 0; i < (int)points.size(); i++)
	{
		if (i == 0 || i == (int)points.size() - 1)
		{
			m_velocity.push_back(Vec2::ZERO);
			continue;
		}

		Vec2 velocity = 0.5f * (m_points[i + 1] - m_points[i - 1]);
		m_velocity.push_back(velocity);
	}
}

Vec2 CatmullRomSpline::EvaluateAtParametric(float parametricZeroToOne, int currentBezierIndex)
{
	// If there are n points in the vector, there will be n-1 bezier curve in the spline
	// the time on each segment of the spline is same
	if (currentBezierIndex >= (int)m_points.size())
	{
		return Vec2::ZERO;
	}

	CubicHermitiveCurve2D cubicHemitiveCurve = CubicHermitiveCurve2D(m_points[currentBezierIndex], m_velocity[currentBezierIndex], m_points[currentBezierIndex + 1], m_velocity[currentBezierIndex + 1]);
	
	return cubicHemitiveCurve.EvaluateAtParametric(parametricZeroToOne);
}

float CatmullRomSpline::GetApproximateLength(int currentBezierIndex, int division /*= 64*/)
{
	if (currentBezierIndex >= (int)m_points.size())
	{
		return 0.0f;
	}

	CubicHermitiveCurve2D cubicHemitiveCurve = CubicHermitiveCurve2D(m_points[currentBezierIndex], m_velocity[currentBezierIndex], m_points[currentBezierIndex + 1], m_velocity[currentBezierIndex + 1]);

	return cubicHemitiveCurve.GetApproximateLength(division);
}

Vec2 CatmullRomSpline::EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubdivision /*= 64*/, int currentBezierIndex /*= 0*/)
{
	if (currentBezierIndex >= (int)m_points.size())
	{
		return Vec2::ZERO;
	}

	CubicHermitiveCurve2D cubicHemitiveCurve = CubicHermitiveCurve2D(m_points[currentBezierIndex], m_velocity[currentBezierIndex], m_points[currentBezierIndex + 1], m_velocity[currentBezierIndex + 1]);
	
	return cubicHemitiveCurve.EvaluateAtApproximateDistance(distanceAlongCurve, numSubdivision);
}
