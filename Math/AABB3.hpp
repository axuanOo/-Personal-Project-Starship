#pragma once
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/MathUtils.hpp"

enum class AABB3Point 
{
	ALLMINS = 0,
	XMAXONLY,
	YMAXONLY,
	ZMAXONLY,
	XYMAXONLY,
	XZMAXONLY,
	YZMAXONLY,
	ALLMAXS,
	COUNT
};

struct AABB3
{
public:

	Vec3 m_mins;
	Vec3 m_maxs;

public:

	// Construction/Destruction
	~AABB3(){}
	AABB3(){}
	AABB3(AABB3 const& copyFrom);

	void		TransformAABB3(Vec3 const& translation);
	Vec3		GetCenter() const;
	Vec3		GetPositionAtPoint(AABB3Point point) const;
	Vec3		GetDimensions() const;
	
	explicit	AABB3(float minX, float minY, float minZ,float maxX, float maxY, float maxZ);
	explicit	AABB3(Vec3 const& mins, Vec3 const& maxs);

};