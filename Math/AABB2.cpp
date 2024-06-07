#include "Engine/Math/AABB2.hpp"

const AABB2 AABB2::ZERO_TO_ONE = AABB2(Vec2(0.0f, 0.0f), Vec2(1.0f, 1.0f));

AABB2::AABB2(AABB2 const& copyFrom)
	:m_mins(copyFrom.m_mins),
	m_maxs(copyFrom.m_maxs)
{

}

AABB2::AABB2(float minX, float minY, float maxX, float maxY)
{
	m_mins.x = minX;
	m_mins.y = minY;
	m_maxs.x = maxX;
	m_maxs.y = maxY;

}

AABB2::AABB2(Vec2 const& mins, Vec2 const& maxs)
	: m_mins(mins),
	m_maxs(maxs)
{

}

bool AABB2::IsPointInside(Vec2 const& point) const
{
	bool isPointXInside = point.x <= m_mins.x || point.x >= m_maxs.x ? false : true;
	bool isPointYInside = point.y <= m_mins.y || point.y >= m_maxs.y ? false : true;

	return isPointXInside && isPointYInside;
}

Vec2 const AABB2::GetCenter() const
{
	return(m_mins + m_maxs) * 0.5f;
}

Vec2 const AABB2::GetDimensions() const
{
	//float f_minx = static_cast<float>(m_mins.x);
	//float f_maxx = static_cast<float>(m_maxs.x);
	//float f_miny = static_cast<float>(m_mins.y);
	//float f_maxy = static_cast<float>(m_maxs.y);
	float abs_x = m_maxs.x - m_mins.x;
	float abs_y = m_maxs.y - m_mins.y;
	return Vec2(abs_x, abs_y);
}

Vec2 const AABB2::GetNearestPoint(Vec2 const& referencePosition) const
{
	float nearestX = referencePosition.x;
	float nearestY = referencePosition.y;

	nearestX = GetClamped(nearestX, m_mins.x, m_maxs.x);
	nearestY = GetClamped(nearestY, m_mins.y, m_maxs.y);

	return Vec2(nearestX, nearestY);
}

Vec2 const AABB2::GetPointAtUV(Vec2 const& uv) const
{
	float axis_X = m_mins.x + ((uv.x) * GetDimensions().x);
	float axis_Y = m_mins.y + ((uv.y) * GetDimensions().y);

	return Vec2(axis_X, axis_Y);
}

Vec2 const AABB2::GetUVForPoint(Vec2 const& point) const
{

	float x_length = m_maxs.x - m_mins.x;
	float y_length = m_maxs.y - m_mins.y;

	float uv_lengthX = point.x - m_mins.x;
	float uv_lengthY = point.y - m_mins.y;

	return Vec2(uv_lengthX / x_length, uv_lengthY / y_length);

}

void AABB2::Translate(Vec2 const& translationToApply)
{
	m_mins += translationToApply;
	m_maxs += translationToApply;
}

void AABB2::SetCenter(Vec2 const& newCenter)
{
	Vec2 originalCenter = GetCenter();
	Vec2 translation = newCenter - originalCenter;
	Translate(translation);
}

void AABB2::SetDimensions(Vec2 const& newDimensions)
{
	Vec2  center = GetCenter();
	m_mins = Vec2(center.x - newDimensions.x * 0.5f, center.y - newDimensions.y * 0.5f);
	m_maxs = Vec2(center.x + newDimensions.x * 0.5f, center.y + newDimensions.y * 0.5f);
}

void AABB2::StretchToIncludePoint(Vec2 const& point)
{
	if (point.x < m_mins.x)
	{
		m_mins.x = point.x;
	}
	else if (point.x > m_maxs.x)
	{
		m_maxs.x = point.x;
	}

	if (point.y < m_mins.y)
	{
		m_mins.y = point.y;
	}
	else if (point.y > m_maxs.y)
	{
		m_maxs.y = point.y;
	}
}
