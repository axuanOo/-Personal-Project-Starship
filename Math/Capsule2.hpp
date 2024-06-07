#pragma once
#include "Engine/Math/Vec2.hpp"

struct Capsule2
{
public:

	Vec2 m_start;
	Vec2 m_end;
	float radius; 
	float		m_elasticity = 1.0f;

public:

	Capsule2();
	Capsule2(Vec2 const& start, Vec2 const& end , float m_radius);
	~Capsule2();

	Vec2	GetBoneAABBMinPos() const;
	Vec2	GetBoneAABBMaxPos() const;
	Vec2	GetCenter() const;
	Vec2	GetHalfDimension() const;
	Vec2	GetUpDirectionNormalizedVec2() const;
	Vec2	GetDownDirectionNormalizedVec2() const;

	void	Translate(Vec2 translation);
	void	SetCenter(Vec2 newCenter);
	void	RotateAboutCenter(float rotationDeltaDegrees);

};

