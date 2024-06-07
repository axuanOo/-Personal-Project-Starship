#include "Engine/Math/AABB3.hpp"

AABB3::AABB3(AABB3 const& copyFrom)
	:m_mins(copyFrom.m_mins),
	m_maxs(copyFrom.m_maxs)
{
}

AABB3::AABB3(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	m_mins.x = minX;
	m_mins.y = minY;
	m_mins.z = minZ;

	m_maxs.x = maxX;
	m_maxs.y = maxY;
	m_maxs.z = maxZ;

}

AABB3::AABB3(Vec3 const& mins, Vec3 const& maxs)
	:m_mins(mins),
	m_maxs(maxs)
{
}

void AABB3::TransformAABB3(Vec3 const& translation)
{
	m_mins += translation;
	m_maxs += translation;
}

Vec3 AABB3::GetCenter() const
{
	return 0.5f * (m_mins + m_maxs);
}

Vec3 AABB3::GetPositionAtPoint(AABB3Point point) const
{

	switch (point)
	{
	case AABB3Point::ALLMINS:
		return m_mins;
	case AABB3Point::XMAXONLY:
		return Vec3(m_maxs.x, m_mins.y, m_mins.z);
	case AABB3Point::YMAXONLY:
		return Vec3(m_mins.x, m_maxs.y, m_mins.z);
	case AABB3Point::ZMAXONLY:
		return Vec3(m_mins.x, m_mins.y, m_maxs.z);
	case AABB3Point::XYMAXONLY:
		return Vec3(m_maxs.x, m_maxs.y, m_mins.z);
	case AABB3Point::XZMAXONLY:
		return Vec3(m_maxs.x, m_mins.y, m_maxs.z);
	case AABB3Point::YZMAXONLY:
		return Vec3(m_mins.x, m_maxs.y, m_maxs.z);
	case AABB3Point::ALLMAXS:
		return m_maxs;
	}

	return Vec3::ZERO;
}

Vec3 AABB3::GetDimensions() const
{
	return m_maxs - m_mins;
}
