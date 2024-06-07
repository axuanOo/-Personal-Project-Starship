#include "Engine/Math/LineSegment2.hpp"
#include "Engine/Core/EngineCommon.hpp"

LineSegment2::LineSegment2()
{
	m_start = Vec2(0.0f, 0.0f);
	m_end = Vec2(0.0f, 0.0f);
}

LineSegment2::LineSegment2(Vec2 const& startPos, Vec2 const& endPos)
	:m_start(startPos),
	m_end(endPos)
{

}

LineSegment2::~LineSegment2()
{

}

void LineSegment2::Translate(Vec2 translation)
{
	m_start += translation;
	m_end += translation;
}

void LineSegment2::SetCenter(Vec2 newCenter)
{
	Vec2 oldCenter = (m_start + m_end) * 0.5f;
	Vec2 translation = newCenter - oldCenter;
	m_start += translation;
	m_end += translation;
}

void LineSegment2::RotateAboutCenter(float rotationDeltaDegrees)
{
	UNUSED(rotationDeltaDegrees);
}
