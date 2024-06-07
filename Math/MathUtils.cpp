#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/OBB2.hpp"
#include "Engine/Math/OBB3.hpp"
#include "Engine/Math/Plane3.hpp"
#include "Engine/Math/Matrix44.hpp"
#include "Engine/Math/Capsule2.hpp"
#include "Engine/Math/LineSegment2.hpp"
#include "Engine/Core/EngineCommon.hpp"

float ConvertDegreeToRadians(float degrees)
{
	return degrees * (g_PI / 180.f);
}

float ConvertRadiansToDegrees(float radians)
{
	return radians * (180.f / g_PI);
}

float CosDegrees(float degrees)
{
	float radians = degrees * (g_PI / 180.f);
	return cosf(radians);
}

float SinDegrees(float degrees)
{
	float radians = degrees * (g_PI / 180.f);
	return sinf(radians);
}

float Atan2Degrees(float y, float x)
{
	float radians = atan2f(y, x);
	return radians * (180.f / g_PI);
}


float GetShortestAngularDisDegrees(float startDegrees, float endDegrees)
{
	while (startDegrees>360.f)
	{
		startDegrees -= 360.f;
	}

	while (startDegrees < 0.f)
	{
		startDegrees += 360.f;
	}

	while (endDegrees > 360.f)
	{
		endDegrees -= 360.f;
	}

	while (endDegrees < 0.f)
	{
		endDegrees += 360.f;
	}

	float rawValue = endDegrees - startDegrees;

	if (rawValue >= -180.f && rawValue <= 180.f)
	{
		return rawValue;
	}
	else if (rawValue < -180.f)
	{
		return rawValue +360.f;
	}
	else 
	{
		return rawValue - 360.f;
	}
}

float GetTurnedTowardDegrees(float currentDegrees, float goalDegrees, float maxDeltaDegrees)
{
	float shortDegree = GetShortestAngularDisDegrees(currentDegrees, goalDegrees);

	if (fabsf(shortDegree)<= maxDeltaDegrees) 
	{
		return goalDegrees;
	}
	else 
	{
		return currentDegrees + (shortDegree / fabsf(shortDegree)) * maxDeltaDegrees;
	}
}

Vec3 ConstructVec3FromVec2(const Vec2& vec2)
{
	return Vec3(vec2.x, vec2.y, 0.0f);
}

float GetDistance2D(Vec2 const& positionA, Vec2 const& positionB)
{
	float xOffset = positionB.x - positionA.x;
	float yOffset = positionB.y - positionA.y;

	float distance2D = sqrtf((xOffset * xOffset) + (yOffset * yOffset));
	return distance2D;
}

float GetDistanceSquared2D(Vec2 const& positionA, Vec2 const& positionB)
{
	float xOffset = positionB.x - positionA.x;
	float yOffset = positionB.y - positionA.y;

	float distance2D = (xOffset * xOffset) + (yOffset * yOffset);
	return distance2D;
}

float GetDistance3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float xOffset = positionB.x - positionA.x;
	float yOffset = positionB.y - positionA.y;
	float zOffset = positionB.z - positionA.z;

	float distance3D = sqrtf((xOffset * xOffset) + (yOffset * yOffset) + (zOffset * zOffset));
	return distance3D;
}

float GetDistanceSquared3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float xOffset = positionB.x - positionA.x;
	float yOffset = positionB.y - positionA.y;
	float zOffset = positionB.z - positionA.z;

	float distance3D = (xOffset * xOffset) + (yOffset * yOffset) + (zOffset * zOffset);
	return distance3D;
}

float GetDistanceXY3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float xOffset = positionB.x - positionA.x;
	float yOffset = positionB.y - positionA.y;

	float distanceXY3D = sqrtf((xOffset * xOffset) + (yOffset * yOffset));
	return distanceXY3D;
}

float GetDistanceXYSquared3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float xOffset = positionB.x - positionA.x;
	float yOffset = positionB.y - positionA.y;

	float distanceXYSquared3D = (xOffset * xOffset) + (yOffset * yOffset);
	return distanceXYSquared3D;
}



int GetTaxicabDistance2D(IntVec2 const& pointA, IntVec2 const& pointB)
{
	int xOffset = pointA.x - pointB.x;
	int yOffset = pointA.y - pointB.y;

	xOffset = abs(xOffset);
	yOffset = abs(yOffset);

	return xOffset + yOffset;
}

float GetProjectedLength2D(Vec2 const& vectorToProject, Vec2 const& vectorToProjectOnto)
{
	float dotValue = DotProduct2D(vectorToProject, vectorToProjectOnto);
	float lengthofVectorToProjectOnto = vectorToProjectOnto.GetLength();
	
	return dotValue / lengthofVectorToProjectOnto;
}

Vec2 const GetProjectedOnto2D(Vec2 const& vectorToProject, Vec2 const& vectorToProjectOnto)
{
	float length = GetProjectedLength2D(vectorToProject, vectorToProjectOnto);
	return length * vectorToProjectOnto.GetNormalized();
}

Vec3 const GetProjectedOnto3D(Vec3 const& vectorToProject, Vec3 const& vectorToProjectOnto)
{
	float dotProduct = DotProduct3D(vectorToProject, vectorToProjectOnto);
	float projectedLength = dotProduct / vectorToProjectOnto.GetLength();

	return projectedLength * vectorToProjectOnto.GetNormalized();
}

float GetAngleDegreesBetweenVectors2D(Vec2 const& vectorA, Vec2 const& vectorB)
{
	float dotValue = DotProduct2D(vectorA,vectorB);

	float lengthVectorA = vectorA.GetLength();

	float lengthVectorB = vectorB.GetLength();

	float lengthMultiplied = lengthVectorA * lengthVectorB;

	float cosTheta =  dotValue / lengthMultiplied;

	float theta = acosf(cosTheta);

	theta = ConvertRadiansToDegrees(theta);

	return theta;
}

bool IsPointInsideDisc2D(Vec2 const& point, Vec2 const& discCenter, float discRadius)
{
	float distancePointToCenter = GetDistance2D(point, discCenter);

	if (distancePointToCenter < discRadius)
	{
		return true;
	}
	else 
	{
		return false;
	}
}

bool IsPointInsideAABB2D(Vec2 const& point, AABB2 const& box)
{
	float offsetX = point.x - box.GetCenter().x;
	float offsetY = point.y - box.GetCenter().y;

	float absOffsetX = fabsf(offsetX);
	float absOffsetY = fabsf(offsetY);

	float width = box.GetDimensions().x;
	float height = box.GetDimensions().y;

	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;

	return absOffsetX < halfWidth && absOffsetY < halfHeight;
}


bool IsPointInsideAABB2D(Vec2 const& point, Vec2 const& minPos, Vec2 const& maxPos)
{
	Vec2 center = minPos + maxPos;

	float offsetX = point.x - center.x;
	float offsetY = point.y - center.y;

	float absOffsetX = fabsf(offsetX);
	float absOffsetY = fabsf(offsetY);

	float width = maxPos.x - minPos.x;
	float height = maxPos.y - minPos.y;

	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;

	return absOffsetX < halfWidth&& absOffsetY < halfHeight;

}


bool IsPointInsideCapsule2D(Vec2 const& point, Capsule2 const& capsule)
{
	Vec2 iBasis = capsule.m_end - capsule.m_start;
	iBasis.Normalized();

	OBB2 capsuleOBB = OBB2();
	capsuleOBB.m_center = capsule.GetCenter();
	capsuleOBB.m_iBasisNormal = iBasis;
	capsuleOBB.m_halfDimensions = capsule.GetHalfDimension();

	if (IsPointInsideDisc2D(point, capsule.m_start, capsule.radius))
	{
		return true;
	}

	if (IsPointInsideDisc2D(point, capsule.m_end, capsule.radius))
	{
		return true;
	}

	if (IsPointInsideOBB2D(point, capsuleOBB))
	{
		return true;
	}

	return false;
}

bool IsPointInsideCapsule2D(Vec2 const& point, Vec2 const& boneStart, Vec2 const& boneEnd, float radius)
{
	Vec2 startToEnd = boneEnd - boneStart;
	Vec2 startToEndDirectional = startToEnd.GetNormalized();
	
	Vec2 localSpaceUpDirection = startToEndDirectional.GetRotated90Degrees();
	Vec2 localSpaceDownDirection = startToEndDirectional.GetRotatedMinus90Degrees();

	Vec2 minPos = boneStart + localSpaceDownDirection * radius ;
	Vec2 maxPos = boneEnd + localSpaceUpDirection * radius;

	if (IsPointInsideDisc2D(point, boneStart, radius))
	{
		return true;
	}

	if (IsPointInsideDisc2D(point, boneEnd, radius))
	{
		return true;
	}

	if (IsPointInsideAABB2D(point, minPos, maxPos))
	{
		return true;
	}

	return false;
}

bool IsPointInsideOBB2D(Vec2 const& point, OBB2 const& orientedBox)
{
	Vec2 iBasis = orientedBox.m_iBasisNormal;
	Vec2 jBasis = iBasis.GetRotated90Degrees();

	Vec2 centerToPoint = point - orientedBox.m_center;

	float pointOniBasis = DotProduct2D(centerToPoint, iBasis);
	float pointOnjBasis = DotProduct2D(centerToPoint, jBasis);

	if (pointOniBasis >= orientedBox.m_halfDimensions.x)
	{
		return false;
	}
	if (pointOniBasis <= -orientedBox.m_halfDimensions.x)
	{
		return false;
	}
	if (pointOnjBasis >= orientedBox.m_halfDimensions.y)
	{
		return false;
	}
	if (pointOnjBasis <= -orientedBox.m_halfDimensions.y)
	{
		return false;
	}

	return true;
}

bool IsPointInsideOrientedSector2D(Vec2 const& point, Vec2 const& sectorTip, float sectorForwardDegrees, float sectorApertureDegrees, float sectorRadius)
{
	if (!IsPointInsideDisc2D(point, sectorTip, sectorRadius))
	{
		return false;
	}
	else
	{
		Vec2 sectorForwardNormal = Vec2::MakeFromPolarDegrees(sectorForwardDegrees);
		Vec2 sectorTipToPoint = point - sectorTip;
		float angleBetweenSFNAndTTP = GetAngleDegreesBetweenVectors2D(sectorForwardNormal, sectorTipToPoint);

		if (angleBetweenSFNAndTTP >= sectorApertureDegrees * 0.5f)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

bool IsPointInsideDirectedSector2D(Vec2 const& point, Vec2 const& sectorTip, Vec2 const& sectorForwardNormal, float sectorApertureDegrees, float sectorRadius)
{
	if (!IsPointInsideDisc2D(point, sectorTip, sectorRadius))
	{
		return false;
	}
	else
	{
		Vec2 sectorTipToPoint = point - sectorTip;
		float angleBetweenSFNAndTTP = GetAngleDegreesBetweenVectors2D(sectorForwardNormal, sectorTipToPoint);

		if (angleBetweenSFNAndTTP >= sectorApertureDegrees * 0.5f)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

bool DoDiscsOverlap(Vec2 const& centerA, float radiusA, Vec2 const& centerB, float radiusB)
{
	float centerDistance = GetDistance2D(centerA,centerB);

	if (centerDistance >= radiusA + radiusB)
	{
		return false;
	}
	else 
	{
		return true;
	}
}

bool DoSphereOverlap(Vec3 const& centerA, float radiusA, Vec3 const& centerB, float radiusB)
{
	float centerDistance = GetDistance3D(centerA, centerB);

	if (centerDistance >= radiusA + radiusB)
	{
		return false;
	}
	else
	{
		return true;
	}
}



bool DoAABB2Overlap(AABB2 const& aabb1, AABB2 const& aabb2)
{
	bool overlapX = (aabb1.m_mins.x <= aabb2.m_maxs.x && aabb1.m_maxs.x >= aabb2.m_mins.x);

	bool overlapY = (aabb1.m_mins.y <= aabb2.m_maxs.y && aabb1.m_maxs.y >= aabb2.m_mins.y);

	return (overlapX && overlapY);
}

Vec2 const GetNearestPointOnDisc2D(Vec2 const& referencePosition, Vec2 const& discCenter, float discRadius)
{
	float distanceToCenter = GetDistance2D(discCenter, referencePosition);

	if (distanceToCenter <= discRadius)
	{
		return referencePosition;
	}

	Vec2 vectorToCenter = referencePosition - discCenter;

	vectorToCenter *= discRadius / distanceToCenter;

	return discCenter + vectorToCenter;

}

Vec2 const GetNearestPointOnAABB2D(Vec2 const& referencePos,const AABB2& box)
{
	float clampedX = GetClamped(referencePos.x, box.m_mins.x, box.m_maxs.x);
	float clampedY = GetClamped(referencePos.y, box.m_mins.y, box.m_maxs.y);
	return Vec2(clampedX, clampedY);

}

Vec2 const GetNearestPointInfiniteLine2D(Vec2 const& referencePos, LineSegment2 const& infiniteLine)
{
	Vec2 startToPos = referencePos - infiniteLine.m_start;
	Vec2 startToEnd = infiniteLine.m_end - infiniteLine.m_start;
	Vec2 startToEndNormalized = startToEnd.GetNormalized();

	float dotproduct = DotProduct2D(startToPos, startToEndNormalized);

	return infiniteLine.m_start + startToEndNormalized * dotproduct;
}

Vec2 const GetNearestPointInfiniteLine2D(Vec2 const& referencePos, Vec2 const& pointOnLine, Vec2 const& anotherPointOnLine)
{
	Vec2 pointOneToPos = referencePos - pointOnLine;
	Vec2 pointOneToAnother = anotherPointOnLine - pointOnLine;
	Vec2 pointOneToAnotherNormalized = pointOneToAnother.GetNormalized();

	float dotproduct = DotProduct2D(pointOneToPos, pointOneToAnotherNormalized);

	return pointOnLine + pointOneToAnotherNormalized * dotproduct;
}

Vec2 const GetNearestPointLineSegment2D(Vec2 const& referencePos, LineSegment2 const& lineSegment)
{
	Vec2 startToReference = referencePos - lineSegment.m_start;
	Vec2 endToReference = referencePos - lineSegment.m_end;
	Vec2 startToEnd = lineSegment.m_end - lineSegment.m_start;

	float dotproduct1 = DotProduct2D(startToReference, startToEnd);
	float dotproduct2 = DotProduct2D(endToReference, startToEnd);

	if (dotproduct1 <= 0)
	{
		return lineSegment.m_start;
	}

	if (dotproduct2 >= 0 ) 
	{
		return lineSegment.m_end;
	}

	return GetNearestPointInfiniteLine2D(referencePos, lineSegment);
}

Vec2 const GetNearestPointLineSegment2D(Vec2 const& referencePos, Vec2 const& lineSegStart, Vec2 const& lineSegEnd)
{
	Vec2 startToReference = referencePos - lineSegStart;
	Vec2 endToReference = referencePos - lineSegEnd;
	Vec2 startToEnd = lineSegEnd - lineSegStart;

	float dotproduct1 = DotProduct2D(startToReference, startToEnd);
	float dotproduct2 = DotProduct2D(endToReference, startToEnd);

	if (dotproduct1 <= 0)
	{
		return lineSegStart;
	}

	if (dotproduct2 >= 0)
	{
		return lineSegEnd;
	}

	return GetNearestPointInfiniteLine2D(referencePos, lineSegStart, lineSegEnd);
}


Vec2 const GetNearestPointOnCapsule2D(Vec2 const& referencePos, Capsule2 const& capsule)
{
	Vec2 startToReference = referencePos - capsule.m_start;
	Vec2 endToReference = referencePos - capsule.m_end;
	Vec2 startToEnd = capsule.m_end - capsule.m_start;

	float dotproduct1 = DotProduct2D(startToReference, startToEnd);
	float dotproduct2 = DotProduct2D(endToReference, startToEnd);

	if (dotproduct1 <= 0)
	{
		return GetNearestPointOnDisc2D(referencePos, capsule.m_start, capsule.radius);
	}

	if (dotproduct2 >= 0)
	{
		return GetNearestPointOnDisc2D(referencePos, capsule.m_end, capsule.radius);
	}

	OBB2 capsuleOBB = OBB2();
	capsuleOBB.m_center = capsule.GetCenter();
	capsuleOBB.m_iBasisNormal = capsule.GetUpDirectionNormalizedVec2().GetRotatedMinus90Degrees();
	capsuleOBB.m_halfDimensions = capsule.GetHalfDimension();

	return GetNearestPointOnOBB2D(referencePos, capsuleOBB);
}

Vec2 const GetNearestPointOnCapsule2D(Vec2 const& referencePos, Vec2 const& boneStart, Vec2 const& boneEnd, float radius)
{
	Vec2 startToReference = referencePos - boneStart;
	Vec2 endToReference = referencePos - boneEnd;
	Vec2 startToEnd = boneEnd - boneStart;
	Vec2 startToEndNormalized = startToEnd.GetNormalized();

	float dotproduct1 = DotProduct2D(startToReference, startToEnd);
	float dotproduct2 = DotProduct2D(endToReference, startToEnd);

	if (dotproduct1 <= 0)
	{
		return GetNearestPointOnDisc2D(referencePos, boneStart, radius);
	}

	if (dotproduct2 >= 0)
	{
		return GetNearestPointOnDisc2D(referencePos, boneEnd, radius);
	}

	Vec2 localSpaceUpVector = startToEndNormalized.GetRotated90Degrees();
	Vec2 localSpaceDownVector = startToEndNormalized.GetRotatedMinus90Degrees();

	Vec2 aabbMin = boneStart + localSpaceDownVector * radius;
	Vec2 aabbMax = boneEnd + localSpaceUpVector * radius;
	AABB2 capsuleAABB = AABB2(aabbMin, aabbMax);

	return GetNearestPointOnAABB2D(referencePos, capsuleAABB);
}


Vec2 const GetNearestPointOnOBB2D(Vec2 const& referencePos, OBB2 const& orientedBox)
{
	Vec2 iBasis = orientedBox.m_iBasisNormal;
	Vec2 jBasis = iBasis.GetRotated90Degrees();

	Vec2 centerToPoint = referencePos - orientedBox.m_center;

	float pointOniBasis = DotProduct2D(centerToPoint, iBasis);
	float pointOnjBasis = DotProduct2D(centerToPoint, jBasis);

	float clampedPointOniBasis = GetClamped(pointOniBasis, -orientedBox.m_halfDimensions.x, orientedBox.m_halfDimensions.x);
	float clampedPointOnjBasis = GetClamped(pointOnjBasis, -orientedBox.m_halfDimensions.y, orientedBox.m_halfDimensions.y);

	return orientedBox.m_center + iBasis * clampedPointOniBasis + jBasis * clampedPointOnjBasis;
}

bool IsPointInsideAABB3D(Vec3 const& referencePos, AABB3 const& box)
{
	FloatRange xRange = FloatRange(box.m_mins.x, box.m_maxs.x);
	FloatRange yRange = FloatRange(box.m_mins.y, box.m_maxs.y);
	FloatRange zRange = FloatRange(box.m_mins.z, box.m_maxs.z);

	return 
		(referencePos.x>box.m_mins.x&&referencePos.x<box.m_maxs.x)&&
		(referencePos.y>box.m_mins.y&&referencePos.y<box.m_maxs.y)&&
		(referencePos.z>box.m_mins.z&&referencePos.z<box.m_maxs.z);
}

bool IsPointInsideSphere3D(Vec3 const& referencePos, Vec3 const& sphereCenter, float sphereRadius)
{
	float distanceSquare = (sphereCenter - referencePos).GetLengthSquared();
	return distanceSquare < (sphereRadius* sphereRadius);
}


bool IsPointInsideZCylinder(Vec3 const& referencePos, Vec2 cylinderCenterXY, float cylinderRadius, FloatRange cylinderMinMaxZ)
{
	bool isZInsideRange = cylinderMinMaxZ.IsOnRange(referencePos.z);
	bool isXYInsideRange = IsPointInsideDisc2D(Vec2(referencePos.x, referencePos.y), cylinderCenterXY, cylinderRadius);

	return isZInsideRange && isXYInsideRange;
}

bool DoAABBsOverlap3D(AABB3 const& first, AABB3 const& second)
{
	AABB2 topViewFirst = AABB2(Vec2(first.m_mins.x, first.m_mins.y), Vec2(first.m_maxs.x, first.m_maxs.y));
	AABB2 topViewSecond = AABB2(Vec2(second.m_mins.x, second.m_mins.y), Vec2(second.m_maxs.x, second.m_maxs.y));

	AABB2 sideViewFirst = AABB2(Vec2(first.m_mins.x, first.m_mins.z), Vec2(first.m_maxs.x, first.m_maxs.z));
	AABB2 sideViewSecond = AABB2(Vec2(second.m_mins.x, second.m_mins.z), Vec2(second.m_maxs.x, second.m_maxs.z));

	return DoAABB2Overlap(topViewFirst, topViewSecond) && DoAABB2Overlap(sideViewFirst, sideViewSecond);
}

bool DoSpheresOverlap3D(Vec3 const& centerA, float radiusA, Vec3 const& centerB, float radiusB)
{
	float distanceTwoCenter = (centerB - centerA).GetLength();
	return distanceTwoCenter < (radiusA + radiusB);
}

bool DoZCylindersOverlap3D(Vec2 cylinder1CenterXY, float cylinder1Radius, FloatRange cylinder1MinMaxZ, Vec2 cylinder2CenterXY, float cylinder2Radius, FloatRange cylinder2MinMaxZ)
{

	AABB2 sideViewBox1 = AABB2(Vec2(cylinder1CenterXY.x - cylinder1Radius, cylinder1MinMaxZ.m_min), Vec2(cylinder1CenterXY.x + cylinder1Radius, cylinder1MinMaxZ.m_max));
	AABB2 sideViewBox2 = AABB2(Vec2(cylinder2CenterXY.x - cylinder2Radius, cylinder2MinMaxZ.m_min), Vec2(cylinder2CenterXY.x + cylinder2Radius, cylinder2MinMaxZ.m_max));
	
	return DoDiscsOverlap(cylinder1CenterXY, cylinder1Radius, cylinder2CenterXY, cylinder2Radius)&&
		DoAABB2Overlap(sideViewBox1,sideViewBox2);
}

bool DoSphereAndAABBOverlap3D(Vec3 const& sphereCenter, float sphereRadius, AABB3 box)
{
	AABB2 topViewBox = AABB2(Vec2(box.m_mins.x, box.m_mins.y), Vec2(box.m_maxs.x, box.m_maxs.y));
	Vec2 topViewSphereCenter = Vec2(sphereCenter.x, sphereCenter.y);

	AABB2 sideViewBox = AABB2(Vec2(box.m_mins.x, box.m_mins.z), Vec2(box.m_maxs.x, box.m_maxs.z));
	Vec2 sideViewSphereCenter = Vec2(sphereCenter.x, sphereCenter.z);

	Vec2 nearestPointOnAABB2Top = GetNearestPointOnAABB2D(topViewSphereCenter,topViewBox);
	Vec2 nearestPointOnAABB2Side = GetNearestPointOnAABB2D(sideViewSphereCenter, sideViewBox);

	return IsPointInsideDisc2D(nearestPointOnAABB2Top, topViewSphereCenter, sphereRadius)&&
		IsPointInsideDisc2D(nearestPointOnAABB2Side,sideViewSphereCenter,sphereRadius);
}

bool DoZCylinderAndSphereOverlap3D(Vec2 cylinderCenterXY, float cylinderRadius, FloatRange cylinderMinMaxZ, Vec3 sphereCenter, float sphereRadius)
{
	Vec2 topViewSphereCenter = Vec2(sphereCenter.x, sphereCenter.y);

	AABB2 sideViewBox = AABB2(Vec2(cylinderCenterXY.x - cylinderRadius, cylinderMinMaxZ.m_min), Vec2(cylinderCenterXY.x + cylinderRadius, cylinderMinMaxZ.m_max));
	Vec2 sideViewSphereCenter = Vec2(sphereCenter.x, sphereCenter.z);

	Vec2 nearestPointOnAABB2Side = GetNearestPointOnAABB2D(sideViewSphereCenter, sideViewBox);

	return DoDiscsOverlap(cylinderCenterXY,cylinderRadius,topViewSphereCenter,sphereRadius)&&
		IsPointInsideDisc2D(nearestPointOnAABB2Side,sideViewSphereCenter,sphereRadius);
}

bool DoPlaneAndAABBOverlap3D(AABB3 const& aabb, Plane3 const& plane)
{
	Vec3 point1 = aabb.GetPositionAtPoint(AABB3Point::ALLMINS);
	Vec3 point2 = aabb.GetPositionAtPoint(AABB3Point::ALLMAXS);
	Vec3 point3 = aabb.GetPositionAtPoint(AABB3Point::XMAXONLY);
	Vec3 point4 = aabb.GetPositionAtPoint(AABB3Point::YMAXONLY);
	Vec3 point5 = aabb.GetPositionAtPoint(AABB3Point::ZMAXONLY);
	Vec3 point6 = aabb.GetPositionAtPoint(AABB3Point::XYMAXONLY);
	Vec3 point7 = aabb.GetPositionAtPoint(AABB3Point::XZMAXONLY);
	Vec3 point8 = aabb.GetPositionAtPoint(AABB3Point::YZMAXONLY);

	float distance1 = plane.GetDistanceToPlane(point1);
	float distance2 = plane.GetDistanceToPlane(point2);

	float distance3 = plane.GetDistanceToPlane(point3);
	float distance4 = plane.GetDistanceToPlane(point4);

	float distance5 = plane.GetDistanceToPlane(point5);
	float distance6 = plane.GetDistanceToPlane(point6);

	float distance7 = plane.GetDistanceToPlane(point7);
	float distance8 = plane.GetDistanceToPlane(point8);
	
	if (distance1 * distance2 < 0.0f)
	{
		return true;
	}
	if (distance1 * distance3 < 0.0f)
	{
		return true;
	}
	if (distance1 * distance4 < 0.0f)
	{
		return true;
	}
	if (distance1 * distance5 < 0.0f)
	{
		return true;
	}
	if (distance1 * distance6 < 0.0f)
	{
		return true;
	}
	if (distance1 * distance7 < 0.0f)
	{
		return true;
	}
	if (distance1 * distance8 < 0.0f)
	{
		return true;
	}
	return false;
}

bool DoPlaneAndSphereOverlap3D(Vec3 const& centerA, float radiusA, Plane3 const& plane)
{
	float distanceToPlane = plane.GetDistanceToPlane(centerA);

	return fabsf(distanceToPlane) < radiusA;
}

bool DoPlaneAndOBBOverlap3D(OBB3 const& obbBounds, Plane3 const& plane)
{
	Vec3 point1 = obbBounds.GetPointAt(Vec3(0.0f, 0.0f, 0.0f));
	Vec3 point2 = obbBounds.GetPointAt(Vec3(1.0f, 1.0f, 1.0f));
	Vec3 point3 = obbBounds.GetPointAt(Vec3(1.0f, 0.0f, 0.0f));
	Vec3 point4 = obbBounds.GetPointAt(Vec3(0.0f, 1.0f, 0.0f));
	Vec3 point5 = obbBounds.GetPointAt(Vec3(0.0f, 0.0f, 1.0f));
	Vec3 point6 = obbBounds.GetPointAt(Vec3(1.0f, 1.0f, 0.0f));
	Vec3 point7 = obbBounds.GetPointAt(Vec3(1.0f, 0.0f, 1.0f));
	Vec3 point8 = obbBounds.GetPointAt(Vec3(0.0f, 1.0f, 1.0f));
	float distance1 = plane.GetDistanceToPlane(point1);
	float distance2 = plane.GetDistanceToPlane(point2);

	float distance3 = plane.GetDistanceToPlane(point3);
	float distance4 = plane.GetDistanceToPlane(point4);

	float distance5 = plane.GetDistanceToPlane(point5);
	float distance6 = plane.GetDistanceToPlane(point6);

	float distance7 = plane.GetDistanceToPlane(point7);
	float distance8 = plane.GetDistanceToPlane(point8);

	if (distance1 * distance2 < 0.0f)
	{
		return true;
	}
	if (distance1 * distance3 < 0.0f)
	{
		return true;
	}
	if (distance1 * distance4 < 0.0f)
	{
		return true;
	}
	if (distance1 * distance5 < 0.0f)
	{
		return true;
	}
	if (distance1 * distance6 < 0.0f)
	{
		return true;
	}
	if (distance1 * distance7 < 0.0f)
	{
		return true;
	}
	if (distance1 * distance8 < 0.0f)
	{
		return true;
	}
	return false;

}

bool DoZCylinderAndAABBOverlap3D(Vec2 cylinderCenterXY, float cylinderRadius, FloatRange cylinderMinMaxZ, AABB3 box)
{
	bool isOverlapTopView = false;
	bool isOverlapSideView = false;

	// Check if overlap on top view
	
	Vec3 boxCenter = 0.5f * (box.m_mins + box.m_maxs);

	Vec2 topViewBoxCenter = Vec2(boxCenter.x, boxCenter.y);

	Vec2 topViewDimensions = Vec2(box.GetDimensions().x, box.GetDimensions().y);

	AABB2 topViewBox = AABB2(topViewBoxCenter - 0.5f * topViewDimensions, topViewBoxCenter + 0.5f * topViewDimensions);

	Vec2 nearestPointOnAABB2 = GetNearestPointOnAABB2D(cylinderCenterXY, topViewBox);

	isOverlapTopView = IsPointInsideDisc2D(nearestPointOnAABB2, cylinderCenterXY, cylinderRadius);


	// Check if overlap on side view

	Vec2 sideViewBoxCenter = Vec2(boxCenter.x, boxCenter.z);

	Vec2 sideViewDimensions = Vec2(box.GetDimensions().x, box.GetDimensions().z);

	AABB2 sideViewBox = AABB2(sideViewBoxCenter - 0.5f * sideViewDimensions, sideViewBoxCenter + 0.5f * sideViewDimensions);

	AABB2 cylinderSideBox = AABB2(Vec2(cylinderCenterXY.x - cylinderRadius, cylinderMinMaxZ.m_min), Vec2(cylinderCenterXY.x + cylinderRadius, cylinderMinMaxZ.m_max));

	isOverlapSideView = DoAABB2Overlap(sideViewBox, cylinderSideBox);


	return isOverlapSideView && isOverlapTopView;
}

Vec3 const GetNearestPointOnAABB3D(Vec3 const& referencePos, AABB3 const& bounds)
{
	Vec3 nearestPoint;

	nearestPoint.x = GetClamped(referencePos.x, bounds.m_mins.x, bounds.m_maxs.x);
	nearestPoint.y = GetClamped(referencePos.y, bounds.m_mins.y, bounds.m_maxs.y);
	nearestPoint.z = GetClamped(referencePos.z, bounds.m_mins.z, bounds.m_maxs.z);

	return nearestPoint;

}

Vec3 const GetNearestPointOnSphere3D(Vec3 const& referencePos, Vec3 const& sphereCenter, float sphereRadius)
{
	Vec3 nearestPoint;

	Vec3 vectorPointCenter = (referencePos - sphereCenter);
	float distancePointCenter = vectorPointCenter.GetLength();

	distancePointCenter = GetClamped(distancePointCenter,0.0f,sphereRadius);

	nearestPoint = sphereCenter + distancePointCenter * vectorPointCenter.GetNormalized();
	
	return nearestPoint;
}

Vec3 const GetNearestPointOnZCylinder(Vec3 const& referencePos, Vec2 const& cylinderCenterXY, float cylinderRadius, FloatRange cylinderMinMaxZ)
{
	Vec3 nearestPoint;
	
	Vec2 referencePointXY = Vec2(referencePos.x, referencePos.y);

	Vec2 nearestPointXY = GetNearestPointOnDisc2D(referencePointXY, cylinderCenterXY, cylinderRadius);

	float nearestPointZ = GetClamped(referencePos.z, cylinderMinMaxZ.m_min, cylinderMinMaxZ.m_max);

	nearestPoint.x = nearestPointXY.x;
	nearestPoint.y = nearestPointXY.y;
	nearestPoint.z = nearestPointZ;

	return nearestPoint;
}

Vec3 const GetNearestPointOnOBB3D(Vec3 const& referencePos, OBB3 const& obb)
{
	AABB3 bounds;
	bounds.m_mins = Vec3(-obb.GetHalfDimensions().x, -obb.GetHalfDimensions().y, -obb.GetHalfDimensions().z);
	bounds.m_maxs = Vec3(obb.GetHalfDimensions().x, obb.GetHalfDimensions().y, obb.GetHalfDimensions().z);

	Mat44 modelMatrix = obb.GetModelMatrix();

	modelMatrix = modelMatrix.GetOrthonormalInverse();
	
	Vec3 referencePointInLocalSpace = modelMatrix.TransformPosition3D(referencePos);

	Vec3 nearestPointLocalSpace = GetNearestPointOnAABB3D(referencePointInLocalSpace, bounds);

	return obb.GetModelMatrix().TransformPosition3D(nearestPointLocalSpace);
}

Vec3 const GetNearestPointOnPlane3D(Vec3 const& referencePos, Plane3 const& plane)
{
	Vec3 planeCenterToPoint = referencePos - plane.GetCenter();
	Vec3 planeCenterToPointOnNormal = GetProjectedOnto3D(planeCenterToPoint, plane.m_normal);

	return plane.GetCenter() + planeCenterToPoint - planeCenterToPointOnNormal;
}

bool PushDiscOutOfFixedPoint2D(Vec2& mobileDiscCenter, float discRadius, Vec2 const& fixedPoint)
{
	float distancePointCenterSquare = GetDistanceSquared2D(mobileDiscCenter, fixedPoint);
	
	if (distancePointCenterSquare >= discRadius * discRadius)
	{
		return false;
	}
	else 
	{
		Vec2 fixedpointToCenter = mobileDiscCenter - fixedPoint;
		float overlapLength = discRadius - fixedpointToCenter.GetLength();
		fixedpointToCenter.SetLength(overlapLength);
		mobileDiscCenter += fixedpointToCenter;
		return true;
	}
}

bool PushDiscOutOfFixedDisc2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2 const& fixedDiscCenter, float fixedDiscRadius)
{
	float distanceTwoCenterSquare = GetDistanceSquared2D(mobileDiscCenter, fixedDiscCenter);

	if (distanceTwoCenterSquare >= (mobileDiscRadius + fixedDiscRadius)*(mobileDiscRadius + fixedDiscRadius))
	{
		return false;
	}
	else 
	{
		float distanceTwoCenter = GetDistance2D(mobileDiscCenter, fixedDiscCenter);
		Vec2 fixedcenterToCenter = mobileDiscCenter - fixedDiscCenter;
		float overlapLength = mobileDiscRadius + fixedDiscRadius - distanceTwoCenter;
		fixedcenterToCenter.SetLength(overlapLength);
		mobileDiscCenter += fixedcenterToCenter;
		return true;
	}
}

bool PushDiscOutOfEachOther2D(Vec2& aCenter, float aRadius, Vec2& bCenter, float bRadius)
{
	float distanceTwoCenterSquared = GetDistanceSquared2D(aCenter, bCenter);

	if (distanceTwoCenterSquared >= (aRadius + bRadius)*(aRadius + bRadius))
	{
		return false;
	}
	else
	{
		Vec2 bToa = aCenter - bCenter;
		Vec2 aTob = bCenter - aCenter;

		float distanceTwoCenter = GetDistance2D(aCenter, bCenter);
		float overlapLength = aRadius + bRadius - distanceTwoCenter;
		float halfOverlapLength = overlapLength * 0.5f;
	
		bToa.SetLength(halfOverlapLength);
		aTob.SetLength(halfOverlapLength);

		aCenter += bToa;
		bCenter += aTob;

		return true;
	}
}

bool PushDiscOutOfFixedAABB2D(Vec2& mobileDiscCenter, float discRadius, AABB2 const& fixedBox)
{
	if (IsPointInsideAABB2D(mobileDiscCenter,fixedBox)) 
	{
		Vec2 dimensions = fixedBox.GetDimensions();
		Vec2 toCenter = mobileDiscCenter - fixedBox.GetCenter();

		float overlapX = dimensions.x * 0.5f + discRadius - fabsf(toCenter.x);
		float overlapY = dimensions.y * 0.5f + discRadius - fabsf(toCenter.y);
		
		if (mobileDiscCenter.x > fixedBox.GetCenter().x)
		{
			mobileDiscCenter.x += overlapX;
		}

		if (mobileDiscCenter.x < fixedBox.GetCenter().x)
		{
			mobileDiscCenter.x -= overlapX;
		}

		if (mobileDiscCenter.y > fixedBox.GetCenter().y)
		{
			mobileDiscCenter.y += overlapY;
		}

		if (mobileDiscCenter.y < fixedBox.GetCenter().y)
		{
			mobileDiscCenter.y -= overlapY;
		}
		return true;
	}
	else 
	{
		Vec2 nearestPoint = fixedBox.GetNearestPoint(mobileDiscCenter);
		return PushDiscOutOfFixedPoint2D(mobileDiscCenter, discRadius, nearestPoint);
	}
}

void TransformPosition2D(Vec2& posToTransform, float uniformScale, float rotationDegrees, Vec2 const& translation)
{
	float tmp_x = posToTransform.x;
	float tmp_y = posToTransform.y;
	float tmp_radians = atan2f(tmp_y , tmp_x);
	float tmp_length = posToTransform.GetLength();

	// scale part
	//tmp_x *= uniformScale;
	//tmp_y *= uniformScale;
	tmp_length *= uniformScale; 


	// rotation part 
	float rotationRadians = ConvertDegreeToRadians(rotationDegrees);
	tmp_radians += rotationRadians;
	tmp_x = tmp_length * cosf(tmp_radians);
	tmp_y = tmp_length * sinf(tmp_radians);


	// translation part 
	tmp_x += translation.x;
	tmp_y += translation.y;


	// finish part
	posToTransform.x = tmp_x;
	posToTransform.y = tmp_y;
}

void TransformPosition2D(Vec2& posToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation)
{
	float x = posToTransform.x;
	float y = posToTransform.y;

	posToTransform.x = iBasis.x * x + jBasis.x * y + translation.x;
	posToTransform.y = iBasis.y * x + jBasis.y * y + translation.y;
}

void TransformPositionXY3D(Vec3& positionToTransform, float scaleXY, float zRotationDegrees, Vec2 const& translationXY)
{
	float tmp_x = positionToTransform.x;
	float tmp_y = positionToTransform.y;
	float tmp_z = positionToTransform.z;
	float tmp_radians = atan2f(tmp_y, tmp_x);
	float tmp_length = sqrtf((tmp_x * tmp_x) + (tmp_y * tmp_y));

	// scale part
	//tmp_x *= uniformScale;
	//tmp_y *= uniformScale;
	tmp_length *= scaleXY;


	// rotation part 
	float rotationRadians = ConvertDegreeToRadians(zRotationDegrees);
	tmp_radians += rotationRadians;
	tmp_x = tmp_length * cosf(tmp_radians);
	tmp_y = tmp_length * sinf(tmp_radians);


	// translation part 
	tmp_x += translationXY.x;
	tmp_y += translationXY.y;


	// finish part
	positionToTransform.x = tmp_x;
	positionToTransform.y = tmp_y;
	positionToTransform.z = tmp_z;
}

void TransformPositionXY3D(Vec3& posToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation)
{
	float x = posToTransform.x;
	float y = posToTransform.y;

	posToTransform.x = iBasis.x * x + jBasis.x * y + translation.x;
	posToTransform.y = iBasis.y * x + jBasis.y * y + translation.y;
}

float GetClamped(float value, float minValue, float maxValue)
{

	if (value < minValue)
	{
		value = minValue;
	}
	else if (value > maxValue)
	{
		value = maxValue;
	}

	return value;
}

float GetClampedZeroToOne(float value)
{
	if (value < 0.f)
	{
		value = 0.f;
	}
	else if (value > 1.0f)
	{
		value = 1.f;
	}

	return value;
}

float Interpolate(float start, float end, float fractionTowardEnd)
{
	float offset = end - start;

	return start + fractionTowardEnd * offset;
}

Vec2 Interpolate(Vec2& startVec, Vec2& endVec, float fractionTowardEnd)
{
	float inX = Interpolate(startVec.x, endVec.x, fractionTowardEnd);
	float inY = Interpolate(startVec.y, endVec.y, fractionTowardEnd);
	return Vec2(inX, inY);
}

float GetFractionWithinRange(float value, float rangeStart, float rangeEnd)
{
	float offset_total = rangeEnd - rangeStart;
	float offset_value = value - rangeStart;

	return offset_value / offset_total;
}

float RangeMap(float inValue, float inStart, float inEnd, float outStart, float outEnd)
{
	float frac = GetFractionWithinRange(inValue, inStart, inEnd);
	return Interpolate(outStart, outEnd, frac);
}

float RangeMapClamped(float inValue, float inStart, float inEnd, float outStart, float outEnd)
{
	inValue = GetClamped(inValue, inStart, inEnd);
	float frac = GetFractionWithinRange(inValue, inStart, inEnd);
	return Interpolate(outStart, outEnd, frac);
}

float GetMaxOfThreeValues(float value1, float value2, float value3)
{
	return (value1 > value2) ? ((value1 > value3) ? value1 : value3) : ((value2 > value3) ? value2 : value3);
}

float GetMinOfThreeValues(float value1, float value2, float value3)
{
	return (value1 < value2) ? ((value1 < value3) ? value1 : value3) : ((value2 < value3) ? value2 : value3);
}

int RoundDownToInt(float value)
{
	return static_cast<int>(floorf(value));
}

float DotProduct2D(Vec2 const& a, Vec2 const& b)
{
	return ((a.x * b.x) + (a.y * b.y));
}

float DotProduct3D(Vec3 const& a, Vec3 const& b)
{
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

float DotProduct4D(Vec4 const& a, Vec4 const& b)
{
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w));
}

float CrossProduct2D(Vec2 const& a, Vec2 const& b)
{
	return ((a.x * b.y) - (a.y * b.x));
}

Vec3 CrossProduct3D(Vec3 const& a, Vec3 const& b)
{
	return Vec3((a.y * b.z - a.z * b.y), -(a.x * b.z - b.x * a.z), (a.x * b.y - b.x * a.y));
}

float NormalizeByte(unsigned char byteValue)
{
	float singleStep = 1.0f / 255.0f;
	float returnValue = 0.f;

	if (byteValue <= 0)
	{
		returnValue = 0.0f;
	}
	else if (byteValue >= 255)
	{
		returnValue=1.0f;
	}
	else 
	{
		returnValue=static_cast<float>(byteValue) * singleStep;
	}

	return returnValue;
}

unsigned char DenormalizeByte(float value)
{
	float clampValue = GetClampedZeroToOne(value);
	float rawValue = clampValue * 256.0f;
	
	if (rawValue < 0.0f)
	{
		return 0;
	}
	else if (rawValue>255.0f)
	{
		return 255;
	}
	else 
	{
		return static_cast<unsigned char>(rawValue);
	}
	 
}

Mat44 GetBillboardMatrix(BillboardType billboardType, Mat44 const& cameraMatrix, const Vec3& billboardPosition, const Vec2& billboardScale /*= Vec2(1.0f, 1.0f)*/)
{
	UNUSED(billboardScale);

	Mat44 billboardMatrix;

	Vec3 billboardIBasis;
	Vec3 billboardJBasis;
	Vec3 billboardKBasis;

	Vec3 worldX = Vec3(1.0f, 0.0f, 0.0f);
	Vec3 worldZ = Vec3(0.0f, 0.0f, 1.0f);
	Vec3 worldY = Vec3(0.0f, 1.0f, 0.0f);
	
	if (billboardType == BillboardType::FULL_CAMERA_FACING)
	{
		billboardIBasis = (cameraMatrix.GetTranslation3D() - billboardPosition).GetNormalized();
		billboardJBasis;
		billboardKBasis;

		// billboardIBasis and worldZ is not on the same line
		if (fabsf(DotProduct3D(worldZ, billboardIBasis)) < 0.999f)
		{
			billboardJBasis = CrossProduct3D(worldZ, billboardIBasis).GetNormalized();
			billboardKBasis = CrossProduct3D(billboardIBasis, billboardJBasis);
		}
		else 
		{
			billboardKBasis = CrossProduct3D(billboardIBasis, worldY).GetNormalized();
			billboardJBasis = CrossProduct3D(billboardKBasis, billboardIBasis);
		}

	}
	else if (billboardType == BillboardType::FULL_CAMERA_OPPOSING)
	{
		billboardIBasis = -cameraMatrix.GetIBasis3D();
		billboardJBasis = -cameraMatrix.GetJBasis3D();
		billboardKBasis = cameraMatrix.GetKBasis3D();
	}
	else if (billboardType == BillboardType::WORLD_UP_CAMERA_OPPOSING)
	{
		billboardKBasis = worldZ;

		billboardIBasis = -cameraMatrix.GetIBasis3D();
		billboardIBasis.z = 0.0f;
		billboardIBasis = billboardIBasis.GetNormalized();

		billboardJBasis = CrossProduct3D(billboardKBasis, billboardIBasis).GetNormalized();
	}
	else if (billboardType == BillboardType::WORLD_UP_CAMERA_FACING)
	{
		billboardKBasis = worldZ;

		billboardIBasis = (cameraMatrix.GetTranslation3D() - billboardPosition);
		billboardIBasis.z = 0.0f;
		billboardIBasis = billboardIBasis.GetNormalized();

		billboardJBasis = CrossProduct3D(billboardKBasis, billboardIBasis).GetNormalized();

	}

	billboardMatrix.SetIJK3D(billboardIBasis, billboardJBasis, billboardKBasis);

	return billboardMatrix;
}

float SmoothStart2(float x)
{
	return x * x;
}

float SmoothStart3(float x)
{
	return x * x * x;
}

float SmoothStart4(float x)
{
	return (x * x) * (x * x);
}

float SmoothStart5(float x)
{
	return (x * x) * (x * x) * x;
}

float SmoothStart6(float x)
{
	return (x * x) * (x * x) * (x * x);
}

float SmoothStop2(float x)
{
	return 1.0f - (1.0f - x) * (1.0f - x);
}

float SmoothStop3(float x)
{
	return 1.0f - (1.0f - x) * (1.0f - x) * (1.0f - x);
}

float SmoothStop4(float x)
{
	return 1.0f - (1.0f - x) * (1.0f - x) * (1.0f - x)* (1.0f - x);
}

float SmoothStop5(float x)
{
	return 1.0f - (1.0f - x) * (1.0f - x) * (1.0f - x)* (1.0f - x)* (1.0f - x);
}

float SmoothStop6(float x)
{
	return 1.0f - (1.0f - x) * (1.0f - x) * (1.0f - x)* (1.0f - x)* (1.0f - x)* (1.0f - x);
}

float ComputeCubicBezier1D(float A, float B, float C, float D, float t)
{
	float lerpAB = Interpolate(A, B, t);
	float lerpBC = Interpolate(B, C, t);
	float lerpCD = Interpolate(C, D, t);

	float lerpABC = Interpolate(lerpAB, lerpBC, t);
	float lerpBCD = Interpolate(lerpBC, lerpCD, t);

	return Interpolate(lerpABC, lerpBCD, t);
}

float ComputeQuinticBezier1D(float A, float B, float C, float D, float E, float F, float t)
{
	float lerpAB = Interpolate(A, B, t);
	float lerpBC = Interpolate(B, C, t);
	float lerpCD = Interpolate(C, D, t);
	float lerpDE = Interpolate(D, E, t);
	float lerpEF = Interpolate(E, F, t);

	float lerpABC = Interpolate(lerpAB, lerpBC, t);
	float lerpBCD = Interpolate(lerpBC, lerpCD, t);
	float lerpCDE = Interpolate(lerpCD, lerpDE, t);
	float lerpDEF = Interpolate(lerpDE, lerpEF, t);

	float lerpABCD = Interpolate(lerpABC, lerpBCD, t);
	float lerpBCDE = Interpolate(lerpBCD, lerpCDE, t);
	float lerpCDEF = Interpolate(lerpCDE, lerpDEF, t);

	float lerpABCDE = Interpolate(lerpABCD, lerpBCDE, t);
	float lerpBCDEF = Interpolate(lerpBCDE, lerpCDEF, t);

	return Interpolate(lerpABCDE, lerpBCDEF, t);
}

float SmoothStep3(float x)
{
	return 3.0f * (x * x) - 2.0f * (x * x * x);
}

float SmoothStep5(float x)
{
	return 6.0f * (x * x * x * x * x) - 15.0f * (x * x * x * x) + 10.0f * (x * x * x);
}

float Hesitate3(float x)
{
	return 4.0f * (x * x * x) - 6.0f * (x * x) + 3.0f * x;
}

float Hesitate5(float x)
{
	return ComputeQuinticBezier1D(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, x);
}

float CustomCurve(float x)
{
	return 1.0f - cosf(0.5f * g_PI * (2.0f * (x * x) - x * x * x));
}

