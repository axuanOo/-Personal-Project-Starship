#include "Engine/Math/Capsule2.hpp"
#include "Engine/Core/EngineCommon.hpp"

Capsule2::Capsule2()
{
	m_start = Vec2(0.0f,0.0f);
	m_end = Vec2(0.0f,0.0f);
	radius = 0.0f;
}

Capsule2::Capsule2(Vec2 const& start, Vec2 const& end, float m_radius)
{
	m_start = start;
	m_end = end;
	radius = m_radius;
}

Capsule2::~Capsule2()
{

}

Vec2 Capsule2::GetBoneAABBMinPos() const
{
	return m_start + GetDownDirectionNormalizedVec2() * radius;
}

Vec2 Capsule2::GetBoneAABBMaxPos() const
{
	return m_end + GetUpDirectionNormalizedVec2() * radius;
}

Vec2 Capsule2::GetCenter() const
{
	return (m_start + m_end) * 0.5f;
}

Vec2 Capsule2::GetHalfDimension() const
{
	float halfDimensionX = (m_end - m_start).GetLength() * 0.5f;
	float halfDimensionY = radius;
	return Vec2(halfDimensionX, halfDimensionY);
}

Vec2 Capsule2::GetUpDirectionNormalizedVec2() const
{
	Vec2 startToEnd = m_end - m_start;
	Vec2 startToEndNormalized = startToEnd.GetNormalized();
	return startToEndNormalized.GetRotated90Degrees();
}

Vec2 Capsule2::GetDownDirectionNormalizedVec2() const
{
	Vec2 startToEnd = m_end - m_start;
	Vec2 startToEndNormalized = startToEnd.GetNormalized();
	return startToEndNormalized.GetRotatedMinus90Degrees();
}

void Capsule2::Translate(Vec2 translation)
{
	m_start += translation;
	m_end += translation;
}

void Capsule2::SetCenter(Vec2 newCenter)
{
	Vec2 oldCenter = (m_start + m_end) * 0.5f;
	Vec2 translation = newCenter - oldCenter;
	m_start += translation;
	m_end += translation;
}

void Capsule2::RotateAboutCenter(float rotationDeltaDegrees)
{
	UNUSED(rotationDeltaDegrees);
}
