#include "Engine/Math/CubicCurve.hpp"
#include "Engine/Math/MathUtils.hpp"

CubicBezierCurve2D::CubicBezierCurve2D(Vec2 startPos, Vec2 guidePos1, Vec2 guidePos2, Vec2 endPos)
:m_startPos(startPos),
m_endPos(endPos),
m_guidePos1(guidePos1),
m_guidePos2(guidePos2)
{
	
}

CubicBezierCurve2D::CubicBezierCurve2D(CubicHermitiveCurve2D const& fromHermite)
:m_startPos(fromHermite.m_startPos),
m_endPos(fromHermite.m_endPos)
{
	m_guidePos1 = fromHermite.m_startPos + 0.3333333333f * (fromHermite.m_startVel);
	m_guidePos2 = fromHermite.m_endPos - 0.3333333333f * (fromHermite.m_endVel);
}

Vec2 CubicBezierCurve2D::EvaluateAtParametric(float parametricZeroToOne)
{
	Vec2 lerpPosAB = Interpolate(m_startPos, m_guidePos1,parametricZeroToOne);
	Vec2 lerpPosBC = Interpolate(m_guidePos1,m_guidePos2,parametricZeroToOne);
	Vec2 lerpPosCD = Interpolate(m_guidePos2,m_endPos,parametricZeroToOne);

	Vec2 lerpPosABC = Interpolate(lerpPosAB, lerpPosBC, parametricZeroToOne);
	Vec2 lerpPosBCD = Interpolate(lerpPosBC, lerpPosCD, parametricZeroToOne);

	return Interpolate(lerpPosABC, lerpPosBCD, parametricZeroToOne);
}

float CubicBezierCurve2D::GetApproximateLength(int division)
{
	float singleStepFraction = 1.0f / division;

	float approximateLength = 0.0f;

	for (int i = 0; i < division; i++)
	{
		Vec2 pos1 = EvaluateAtParametric(singleStepFraction * i);
		Vec2 pos2 = EvaluateAtParametric(singleStepFraction * (i + 1));

		approximateLength += GetDistance2D(pos1, pos2);
	}
	return approximateLength;
}

Vec2 CubicBezierCurve2D::EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubdivision)
{
	float singleStepFraction = 1.0f / numSubdivision;
	float currentDistance = distanceAlongCurve;
	Vec2 finalPosition;

	for (int i = 0; i < numSubdivision; i++)
	{
		Vec2 pos1 = EvaluateAtParametric(singleStepFraction * i);
		Vec2 pos2 = EvaluateAtParametric(singleStepFraction * (i + 1));

		float distancePos12 = GetDistance2D(pos1, pos2);
		currentDistance -= distancePos12;

		if (currentDistance < 0.0f)
		{
			finalPosition = pos1 + (distancePos12 + currentDistance) * ((pos2 - pos1).GetNormalized());
			break;
		}
	}

	return finalPosition;
}

CubicHermitiveCurve2D::CubicHermitiveCurve2D(Vec2 startPos, Vec2 startVel, Vec2 endPos, Vec2 endVel)
:m_startPos(startPos),
m_endPos(endPos),
m_startVel(startVel),
m_endVel(endVel)
{
}

CubicHermitiveCurve2D::CubicHermitiveCurve2D(CubicBezierCurve2D const& fromBezier)
	:m_startPos(fromBezier.m_startPos),
	m_endPos(fromBezier.m_endPos) 
{
	m_startVel = 3.0f * (fromBezier.m_guidePos1 - fromBezier.m_startPos);
	m_endVel = 3.0f * (fromBezier.m_endPos - fromBezier.m_guidePos2);
}

Vec2 CubicHermitiveCurve2D::EvaluateAtParametric(float parametricZeroToOne)
{
	CubicBezierCurve2D bezier = CubicBezierCurve2D(*this);
	return bezier.EvaluateAtParametric(parametricZeroToOne);
}

float CubicHermitiveCurve2D::GetApproximateLength(int division)
{
	CubicBezierCurve2D bezier = CubicBezierCurve2D(*this);
	return bezier.GetApproximateLength(division);
}

Vec2 CubicHermitiveCurve2D::EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubdivision)
{
	CubicBezierCurve2D bezier = CubicBezierCurve2D(*this);
	return bezier.EvaluateAtApproximateDistance(distanceAlongCurve,numSubdivision);
}
