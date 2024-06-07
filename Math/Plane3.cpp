#include "Engine/Math/Plane3.hpp"
#include "Engine/Math/MathUtils.hpp"

Vec3 Plane3::GetCenter() const
{
	return m_normal * m_distanceAlongNormal;
}

Mat44 Plane3::GetPlaneOneMatrix() const
{
	Mat44 modelMatrix;
	
	Vec3 jBasis;
	Vec3 kBasis;

	if (m_normal == Vec3(0.0f, 0.0f, 1.0f))
	{
		jBasis = Vec3(0.0f, 1.0f, 0.0f);
		kBasis = CrossProduct3D(m_normal, jBasis).GetNormalized();
		jBasis = CrossProduct3D(kBasis,m_normal);
	}
	else 
	{
		kBasis = Vec3(0.0f, 0.0f, 1.0f);
		jBasis = CrossProduct3D(kBasis, m_normal).GetNormalized();
		kBasis = CrossProduct3D(m_normal, jBasis);
	}

	modelMatrix.SetIJKT3D(m_normal, jBasis, kBasis, GetCenter());

	return modelMatrix;
}

float Plane3::GetDistanceToPlane(Vec3 const& position) const
{
	return m_distanceAlongNormal - DotProduct3D(position, m_normal);
}
