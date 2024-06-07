#pragma once
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Matrix44.hpp"

struct Plane3 
{
	Vec3		m_normal;
	float		m_distanceAlongNormal;

	Vec3		GetCenter() const;
	Mat44		GetPlaneOneMatrix() const;
	float		GetDistanceToPlane(Vec3 const& position) const;
};