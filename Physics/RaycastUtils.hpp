#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/Plane3.hpp"
#include "Engine/Math/OBB3.hpp"

struct RaycastResult2D
{
public:

	RaycastResult2D();
	~RaycastResult2D();

	void	Clear();

	// Basic raycast result information (required)
	bool	m_didImpact = false;
	float	m_impactDist = 0.f;
	Vec2	m_impactPos;
	Vec2	m_impactNormal;

	// Original raycast information (optional)
	Vec2	m_rayFwdNormal;
	Vec2	m_rayStartPos;
	float	m_rayMaxLength = 1.f;

private:

};

RaycastResult2D RaycastVsDisc2D( Vec2 startPos, Vec2 fwdNormal, float maxDist, Vec2 discCenter, float discRadius );
RaycastResult2D RaycastVsLineSegment2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, const LineSegment2& lineSegment);
RaycastResult2D RaycastVsAABB2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, const AABB2& aabb);



struct RaycastResult3D 
{
public:

	RaycastResult3D();
	~RaycastResult3D();

	void	Clear();

	bool m_didImpact = false;
	float m_impactDistance = 0.0f;
	Vec3 m_impactPosition;
	Vec3 m_impactNormal;
	
	Vec3 m_rayStartPostion;
	Vec3 m_rayDirection;
	float m_rayLength = 1.0f;
};
RaycastResult3D RaycastVsAABB3D(Vec3 rayStart, Vec3 rayForwardNormal, float rayLength, AABB3 const& box);
RaycastResult3D	RaycastVsSphere3D(Vec3 rayStart, Vec3 rayForwardNormal, float rayLength, Vec3 const& sphereCenter, float sphereRadius );
RaycastResult3D	RaycastVsCylinderZ3D(Vec3 rayStart, Vec3 rayForwardNormal, float rayLength, Vec2 const& centerXY, FloatRange const& minMaxZ, float radiusXY );
RaycastResult3D	RaycastVsPlane3D(Vec3 rayStart, Vec3 rayForwardNormal, float rayLength, Plane3 const& plane);
RaycastResult3D	RaycastVsOBB3D(Vec3 const& rayStart, Vec3 const& rayForwardNormal, float rayLength, OBB3 const& obb);