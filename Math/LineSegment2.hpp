#pragma once
#include "Engine/Math/Vec2.hpp"

struct LineSegment2
{
public:

	Vec2		m_start;
	Vec2		m_end;
	float		m_elasticity = 1.0f;

public:
	
	LineSegment2();
	LineSegment2(Vec2 const& startPos, Vec2 const& endPos);
	~LineSegment2();

	void	Translate(Vec2 translation);
	void	SetCenter(Vec2 newCenter);
	void	RotateAboutCenter(float rotationDeltaDegrees);


private:

};

