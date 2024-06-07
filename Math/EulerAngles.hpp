#pragma once
#include "Engine/Math/Vec3.hpp"

struct Mat44;

struct EulerAngles
{
public:
	EulerAngles() = default;
	EulerAngles(float yawDegrees, float pitchDegrees, float rollDegrees);
	void GetVectors_XFwd_YLeft_ZUp(Vec3& out_forwardIBasis, Vec3& out_leftJBasis, Vec3& out_upKBasis) const;
	Mat44 GetMatrix_XFwd_YLeft_ZUp() const;

	Vec3 GetForwardIBasis() const;
	Vec3 GetLeftJBasis() const;
	Vec3 GetUpKBasis() const;

public:
	float m_yawDegrees = 0.f;
	float m_pitchDegrees = 0.f;
	float m_rollDegrees = 0.f;

};


