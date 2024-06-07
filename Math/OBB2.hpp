#pragma once
#include "Engine/Math/Vec2.hpp"

struct OBB2
{
public:

	Vec2		m_center;
	Vec2		m_iBasisNormal;
	Vec2		m_halfDimensions;

	float		m_elasticity = 1.0f;
public:

	OBB2();
	~OBB2();


	void		GetCornerPoints(Vec2* out_fourCornerWorldPositions) const; // for drawing, etc.!
	Vec2		GetLocalPosForWorldPos(Vec2 worldPos) const;
	Vec2		GetWorldPosForLocalPos(Vec2 localPos) const;
	void		RotateAboutCenter(float rotationDeltaDegrees);


private:


};

