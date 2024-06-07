#include "Engine/Math/OBB3.hpp"
#include "Engine/Math/Matrix44.hpp"
#include "Engine/Math/AABB3.hpp"

OBB3::OBB3()
{
}

OBB3::~OBB3()
{
}

OBB3::OBB3(Vec3 const& position, Vec3 const& ibasis, Vec3 const& jBasis, Vec3 const& kBasis, Vec3 const& halfDimensions)
	:m_position(position),
	m_iBasis(ibasis),
	m_jBasis(jBasis),
	m_kBasis(kBasis),
	m_halfDimensions(halfDimensions)
{
}

Vec3 OBB3::GetCenter() const
{
	return m_position;
}

Vec3 OBB3::GetIBasis() const
{
	return m_iBasis;
}

Vec3 OBB3::GetJBasis() const
{
	return m_jBasis;
}

Vec3 OBB3::GetKBasis() const
{
	return m_kBasis;
}

Vec3 OBB3::GetHalfDimensions() const
{
	return m_halfDimensions;
}

Mat44 OBB3::GetModelMatrix() const
{
	Mat44 modelMatrix;

	modelMatrix.SetIJKT3D(m_iBasis, m_jBasis, m_kBasis, m_position);

	return modelMatrix;
}

AABB3 OBB3::GetLocalSpaceAABB3() const
{
	AABB3 bounds;

	bounds.m_mins = -m_halfDimensions;
	bounds.m_maxs = m_halfDimensions;

	return bounds;
}

Vec3 OBB3::GetPointAt(Vec3 const& pointLocalCoordinatesZeroToOne) const
{
	Vec3 offset = pointLocalCoordinatesZeroToOne - Vec3(.5f, .5f, .5f);

	return m_position 
		+ (offset.x * m_iBasis * m_halfDimensions.x * 2.0f) 
		+ (offset.y * m_jBasis * m_halfDimensions.y * 2.0f) 
		+ (offset.z * m_kBasis * m_halfDimensions.z * 2.0f);
}

void OBB3::SetOBBPosition(Vec3 const& position)
{
	m_position = position;
}
