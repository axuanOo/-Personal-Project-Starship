#pragma once
#include "Engine/Math/Vec3.hpp"

struct Mat44;
struct AABB3;

struct OBB3 
{
public:
	OBB3();
	~OBB3();
	explicit OBB3(Vec3 const& position, Vec3 const& ibasis, Vec3 const& jBasis, Vec3 const& kBasis, Vec3 const& halfDimensions);

	Vec3	GetCenter() const;
	Vec3	GetIBasis() const;
	Vec3	GetJBasis() const;
	Vec3	GetKBasis() const;
	Vec3	GetHalfDimensions() const;
	Mat44	GetModelMatrix() const;
	AABB3	GetLocalSpaceAABB3() const;
	Vec3	GetPointAt(Vec3 const& pointLocalCoordinatesZeroToOne) const;
	void	SetOBBPosition(Vec3 const& position);
private:
	Vec3	m_position;
	Vec3	m_iBasis;
	Vec3	m_jBasis;
	Vec3	m_kBasis;
	Vec3	m_halfDimensions;
};