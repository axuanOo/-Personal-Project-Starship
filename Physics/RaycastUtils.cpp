#include "Engine/Physics/RaycastUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/FloatRange.hpp"

RaycastResult2D::RaycastResult2D()
{

}

RaycastResult2D::~RaycastResult2D()
{

}

void RaycastResult2D::Clear()
{
	m_didImpact = false;
	m_impactDist = 0.f;
	m_impactPos = Vec2(0.0f,0.0f);
	m_impactNormal = Vec2(0.0f, 0.0f);
}

RaycastResult2D RaycastVsDisc2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, Vec2 discCenter, float discRadius)
{
	RaycastResult2D result = RaycastResult2D();

	result.m_rayStartPos = startPos;
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayMaxLength = maxDist;

	Vec2 upNormalLocal = fwdNormal.GetRotated90Degrees();

	Vec2 startToCenter = discCenter - startPos;

	float scProjectedOnUpNormal = DotProduct2D(startToCenter, upNormalLocal); // altitude on vector

	if (scProjectedOnUpNormal >= discRadius)
	{
		return result;
	}

	///// Check if the disc is too early or late here
	//////////////////////////////////////////////////////////////////////////

	float scProjectedOnFwdInsideDisc = sqrtf((discRadius * discRadius) - (scProjectedOnUpNormal * scProjectedOnUpNormal));

	float scProjectedOnFwd = DotProduct2D(startToCenter, fwdNormal);

	if (scProjectedOnFwd - scProjectedOnFwdInsideDisc > maxDist)
	{
		return result;
	}

	if (scProjectedOnFwd < -scProjectedOnFwdInsideDisc)
	{
		return result;
	}

	float distanceToDiscCenter = startToCenter.GetLength();
	if (distanceToDiscCenter < discRadius)
	{
		result.m_didImpact = true;
		result.m_impactDist = 0.0f;
		result.m_impactPos = startPos;
		result.m_impactNormal = -fwdNormal;
		return result;
	}

	result.m_didImpact = true;
	result.m_impactDist = scProjectedOnFwd - scProjectedOnFwdInsideDisc;
	result.m_impactPos = startPos + fwdNormal * (scProjectedOnFwd - scProjectedOnFwdInsideDisc);
	result.m_impactNormal = (result.m_impactPos - discCenter).GetNormalized();
	return result;
}

RaycastResult2D RaycastVsLineSegment2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, const LineSegment2& lineSegment)
{
	RaycastResult2D result = RaycastResult2D();

	Vec2 lineStart = lineSegment.m_start;
	Vec2 lineEnd = lineSegment.m_end;
	Vec2 rayNormal = fwdNormal.GetRotated90Degrees();

	/// First step just figure out if the start and the end of the line are in different sides
	Vec2 RaystartToLineStart = lineStart - startPos;
	Vec2 RaystartToLineEnd = lineEnd - startPos;

	float dotValue1 = DotProduct2D(rayNormal, RaystartToLineStart);
	float dotValue2 = DotProduct2D(rayNormal, RaystartToLineEnd);

	if (dotValue1 * dotValue2 >= 0.0f)
	{
		// The start and the end of this line are in the same side
		return result;
	}

	float fraction = dotValue1 / (dotValue1 - dotValue2);

	Vec2 impactPoint = lineStart + fraction * (lineEnd - lineStart);
	
	Vec2 startToImpact = impactPoint - startPos;

	float distanceStartImpact = DotProduct2D(startToImpact, fwdNormal);

	// If the impact point is too near or too far
	if (distanceStartImpact <= 0.0f || distanceStartImpact >= maxDist)
	{
		return result;
	}

	Vec2 lineStartToLineEnd = lineEnd - lineStart;
	Vec2 impactNormal = lineStartToLineEnd.GetRotated90Degrees().GetNormalized();
	if (dotValue2 < 0.0f)
	{
		impactNormal *= -1.0f;
	}

	result.m_didImpact = true;
	result.m_impactPos = impactPoint;
	result.m_impactNormal = impactNormal;
	result.m_impactDist = distanceStartImpact;

	return result;
}

RaycastResult2D RaycastVsAABB2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, const AABB2& aabb)
{
	RaycastResult2D result = RaycastResult2D();
	
	result.m_rayStartPos = startPos;
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayMaxLength = maxDist;

	// First step: to figure out is the AABB of this ray is overlap with the aabb for testing

	AABB2 rayAABB2 = AABB2();
	rayAABB2.SetCenter(startPos + maxDist * 0.5f * fwdNormal);
	Vec2 aabbDimension = Vec2(fabsf(startPos.x - (startPos + maxDist * fwdNormal).x), fabsf(startPos.y - (startPos + maxDist * fwdNormal).y));
	rayAABB2.SetDimensions(aabbDimension);

	if (!DoAABB2Overlap(rayAABB2, aabb))
	{
		return result;
	}

	if (IsPointInsideAABB2D(startPos, aabb))
	{
		result.m_didImpact = true;
		result.m_impactDist = 0.0f;
		result.m_impactPos = startPos;
		result.m_impactNormal = -fwdNormal;
		return result;
	}

	// Second step: if these two aabb is overlap, calculate the overlap area
	Vec2 rayEndPosition = startPos + maxDist * fwdNormal;

	float oneOverRangeX = 1.0f / (rayEndPosition.x - startPos.x);
	float oneOverRangeY = 1.0f / (rayEndPosition.y - startPos.y);

	float overlapAABBXMinFraction = oneOverRangeX * (aabb.m_mins.x - startPos.x);
	float overlapAABBXMaxFraction = oneOverRangeX * (aabb.m_maxs.x - startPos.x);

	if (overlapAABBXMinFraction > overlapAABBXMaxFraction)
	{
		float tmp = 0.0f;
		tmp = overlapAABBXMaxFraction;
		overlapAABBXMaxFraction = overlapAABBXMinFraction;
		overlapAABBXMinFraction = tmp;
	}

	float overlapAABBYMinFraction = oneOverRangeY * (aabb.m_mins.y - startPos.y);
	float overlapAABBYMaxFraction = oneOverRangeY * (aabb.m_maxs.y - startPos.y);

	if (overlapAABBYMinFraction > overlapAABBYMaxFraction)
	{
		float tmp = 0.0f;
		tmp = overlapAABBYMaxFraction;
		overlapAABBYMaxFraction = overlapAABBYMinFraction;
		overlapAABBYMinFraction = tmp;
	}

	FloatRange overlapXFraction = FloatRange(overlapAABBXMinFraction, overlapAABBXMaxFraction);
	FloatRange overlapYFraction = FloatRange(overlapAABBYMinFraction, overlapAABBYMaxFraction);

	if (!overlapXFraction.IsOverlappingWith(overlapYFraction))
	{
		return result;
	}

	FloatRange finalOverlap = FloatRange();

	if (overlapAABBXMinFraction < overlapAABBYMinFraction) 
	{
		finalOverlap.m_min = overlapAABBYMinFraction;		
	}
	else 
	{
		finalOverlap.m_min = overlapAABBXMinFraction;
	}

	if (overlapAABBXMaxFraction < overlapAABBYMaxFraction)
	{
		finalOverlap.m_max = overlapAABBXMaxFraction;
	}
	else
	{
		finalOverlap.m_max = overlapAABBYMaxFraction;
	}

	result.m_didImpact = true;
	result.m_impactDist = finalOverlap.m_min * maxDist;
	result.m_impactPos = startPos + fwdNormal * finalOverlap.m_min * maxDist;

	if (fabsf(result.m_impactPos.x - aabb.m_mins.x) < 0.00001f)
	{
		result.m_impactNormal = Vec2(-1.0f, 0.0f);
	}
	if (fabsf(result.m_impactPos.x - aabb.m_maxs.x) < 0.00001f)
	{
		result.m_impactNormal = Vec2(1.0f, 0.0f);
	}
	if (fabsf(result.m_impactPos.y - aabb.m_mins.y) < 0.00001f)
	{
		result.m_impactNormal = Vec2(0.0f, -1.0f);
	}
	if (fabsf(result.m_impactPos.y - aabb.m_maxs.y) < 0.00001f)
	{
		result.m_impactNormal = Vec2(0.0f, 1.0f);
	}

	return result;
}

RaycastResult3D RaycastVsAABB3D(Vec3 rayStart, Vec3 rayForwardNormal, float rayLength, AABB3 const& box)
{
	RaycastResult3D result;

	Vec3 rayEnd = rayStart + rayForwardNormal * rayLength;
	Vec3 rayBoxCenter = (rayStart + rayEnd) * 0.5f;

	float dimensionX = fabsf(rayEnd.x - rayStart.x);
	float dimensionY = fabsf(rayEnd.y - rayStart.y);
	float dimensionZ = fabsf(rayEnd.z - rayStart.z);

	Vec3 rayBoxDimension = Vec3(dimensionX, dimensionY, dimensionZ);
	AABB3 rayBox = AABB3(rayBoxCenter - rayBoxDimension * 0.5f, rayBoxCenter + rayBoxDimension * 0.5f);

	if (!DoAABBsOverlap3D(rayBox, box))
	{
		return result;
	}

	if (IsPointInsideAABB3D(rayStart, box))
	{
		result.m_didImpact = true;
		result.m_impactDistance = 0.0f;
		result.m_impactPosition = rayStart;
		result.m_impactNormal = -rayForwardNormal;
		return result;
	}

	float oneOverDimensionX = 1.0f / (rayForwardNormal.x * rayLength);
	float oneOverDimensionY = 1.0f / (rayForwardNormal.y * rayLength);
	float oneOverDimensionZ = 1.0f / (rayForwardNormal.z * rayLength);
									 
	FloatRange tValueAtXRange = FloatRange((box.m_mins.x - rayStart.x) * oneOverDimensionX, (box.m_maxs.x - rayStart.x) * oneOverDimensionX);
	FloatRange tValueAtYRange = FloatRange((box.m_mins.y - rayStart.y) * oneOverDimensionY, (box.m_maxs.y - rayStart.y) * oneOverDimensionY);
	FloatRange tValueAtZRange = FloatRange((box.m_mins.z - rayStart.z) * oneOverDimensionZ, (box.m_maxs.z - rayStart.z) * oneOverDimensionZ);

	if (tValueAtXRange.IsOverlappingWith(tValueAtYRange) && tValueAtXRange.IsOverlappingWith(tValueAtZRange) && tValueAtYRange.IsOverlappingWith(tValueAtZRange))
	{
		float tValueForAllMin = GetMaxOfThreeValues(tValueAtXRange.m_min, tValueAtYRange.m_min, tValueAtZRange.m_min);
		float tValueForAllMax = GetMinOfThreeValues(tValueAtXRange.m_max, tValueAtYRange.m_max, tValueAtZRange.m_max);
		if (tValueForAllMin > 1.0f || tValueForAllMax < 0.0f)
		{
			return result;
		}
		
		tValueForAllMin = GetClampedZeroToOne(tValueForAllMin);
		tValueForAllMax = GetClampedZeroToOne(tValueForAllMax);

		result.m_didImpact = true;
		result.m_impactPosition = rayStart + tValueForAllMin * rayForwardNormal * rayLength;
		result.m_impactDistance = (tValueForAllMax - tValueForAllMin) * rayLength;

		if (tValueForAllMin == tValueAtXRange.m_min)
		{
			if (rayForwardNormal.x > 0.0f)
			{
				result.m_impactNormal = Vec3(-1.0f, 0.0f, 0.0f);
			}
			else 
			{
				result.m_impactNormal = Vec3(1.0f, 0.0f, 0.0f);
			}
		}
		if (tValueForAllMin == tValueAtYRange.m_min)
		{
			if (rayForwardNormal.y > 0.0f)
			{
				result.m_impactNormal = Vec3(0.0f, -1.0f, 0.0f);
			}
			else
			{
				result.m_impactNormal = Vec3(0.0f, 1.0f, 0.0f);
			}
		}
		if (tValueForAllMin == tValueAtZRange.m_min)
		{
			if (rayForwardNormal.z > 0.0f)
			{
				result.m_impactNormal = Vec3(0.0f, 0.0f, -1.0f);
			}
			else
			{
				result.m_impactNormal = Vec3(0.0f, 0.0f, 1.0f);
			}
		}
	}

	return result;
}

RaycastResult3D RaycastVsSphere3D(Vec3 rayStart, Vec3 rayForwardNormal, float rayLength, Vec3 const& sphereCenter, float sphereRadius)
{
	RaycastResult3D result;

	Vec3 startToCenter = sphereCenter - rayStart;

	float startToCenterLengthOnFwd = DotProduct3D(startToCenter, rayForwardNormal);

	// Too far to raycast vs the sphere
	if (startToCenterLengthOnFwd >= rayLength + sphereRadius)
	{
		return result;
	}

	// Too late to raycast vs the sphere
	if (startToCenterLengthOnFwd <= 0.0f)
	{
		return result;
	}

	Vec3 startToCenterOnFwd = startToCenterLengthOnFwd * rayForwardNormal;

	Vec3 startToCenterOnJKBasisPlane = startToCenter - startToCenterOnFwd;

	float startToCenterOnJKBasisPlanceLengthSquare = startToCenterOnJKBasisPlane.GetLengthSquared();

	if (startToCenterOnJKBasisPlanceLengthSquare >= sphereRadius * sphereRadius)
	{
		return result;
	}

	// If the startpoint is inside the sphere
	if (IsPointInsideSphere3D(rayStart,sphereCenter,sphereRadius))
	{
		result.m_didImpact =true;
		result.m_impactDistance = 0.0f;
		result.m_impactPosition = rayStart;
		result.m_impactNormal = rayForwardNormal;
		return result;
	}

	float startToCenterLengthOnFwdInsideDisc = sqrtf((sphereRadius * sphereRadius) - startToCenterOnJKBasisPlanceLengthSquare);

	float impactDistance = startToCenterLengthOnFwd - startToCenterLengthOnFwdInsideDisc;

	if (impactDistance >= rayLength)
	{
		return result;
	}

	result.m_didImpact = true;
	result.m_impactDistance = impactDistance;
	result.m_impactPosition = rayStart + (rayForwardNormal * impactDistance);
	result.m_impactNormal = (result.m_impactPosition - sphereCenter).GetNormalized();

	return result;
}

RaycastResult3D RaycastVsCylinderZ3D(Vec3 rayStart, Vec3 rayForwardNormal, float rayLength, Vec2 const& centerXY, FloatRange const& minMaxZ, float radiusXY)
{
	RaycastResult3D result = RaycastResult3D();

	Vec3 rayEnd = rayStart + rayForwardNormal * rayLength;
	Vec3 rayBoxCenter = (rayStart + rayEnd) * 0.5f;

	float dimensionX = fabsf(rayEnd.x - rayStart.x);
	float dimensionY = fabsf(rayEnd.y - rayStart.y);
	float dimensionZ = fabsf(rayEnd.z - rayStart.z);

	Vec3 rayBoxDimension = Vec3(dimensionX, dimensionY, dimensionZ);
	AABB3 rayBox = AABB3(rayBoxCenter - rayBoxDimension * 0.5f, rayBoxCenter + rayBoxDimension * 0.5f);

	if (!DoZCylinderAndAABBOverlap3D(centerXY,radiusXY,minMaxZ,rayBox))
	{
		return result;
	}

	// First check if this point is inside the ZCylinder

	if (IsPointInsideZCylinder(rayStart, centerXY, radiusXY, minMaxZ))
	{
		result.m_didImpact = true;
		result.m_impactDistance = 0.0f;
		result.m_impactPosition = rayStart;
		result.m_impactNormal = -rayForwardNormal;
		return result;
	}

	// Now this ray has collision with the cylinder and not inside the cylinder
	// Divide the collision check into two different space, first is the topView
	
	Vec2 rayStartXY = Vec2(rayStart.x, rayStart.y);
	Vec2 rayFwdDirectional = Vec2(rayForwardNormal.x,rayForwardNormal.y).GetNormalized();
	float maxDistanceXY = sqrtf((dimensionX * dimensionX) + (dimensionY * dimensionY));
	Vec2 upNormal = rayFwdDirectional.GetRotated90Degrees();
	Vec2 startToCenter = centerXY - rayStartXY;

	float scProjectedOnUpNormal = DotProduct2D(startToCenter, upNormal); // altitude on vector
	if (scProjectedOnUpNormal >= radiusXY)
	{
		return result;
	}

	float scProjectedOnFwdInsideDisc = sqrtf((radiusXY * radiusXY) - (scProjectedOnUpNormal * scProjectedOnUpNormal));
	float scProjectedOnFwd = DotProduct2D(startToCenter, rayFwdDirectional);

	// Make sure we already have two impact point in XY plane
	float maxDistanceDividedByOne = 1.0f / maxDistanceXY;
	float shorterImpactDistance = scProjectedOnFwd - scProjectedOnFwdInsideDisc;
	float longerImpactDistance = scProjectedOnFwd + scProjectedOnFwdInsideDisc;
	FloatRange impactRangeXY = FloatRange(shorterImpactDistance * maxDistanceDividedByOne, longerImpactDistance * maxDistanceDividedByOne);

	// Check the overlap distance on Z axis
	float rayFwdNormalZDividedByOne = 1.0f / (rayEnd.z - rayStart.z);
	float rayTValueAtCylinderMinZ = (minMaxZ.m_min - rayStart.z) * rayFwdNormalZDividedByOne;
	float rayTValueAtCylinderMaxZ = (minMaxZ.m_max - rayStart.z) * rayFwdNormalZDividedByOne;
	FloatRange impactRangeZ = FloatRange(rayTValueAtCylinderMinZ, rayTValueAtCylinderMaxZ);

	// Check if these two foat range is overlap with each other
	if (!impactRangeXY.IsOverlappingWith(impactRangeZ))
	{
		return result;
	}
	
	float commonImpactMinValue = impactRangeXY.m_min > impactRangeZ.m_min ? impactRangeXY.m_min : impactRangeZ.m_min;
	//float commonImpactMaxValue = impactRangeXY.m_max < impactRangeZ.m_max ? impactRangeXY.m_max : impactRangeZ.m_max;

	result.m_didImpact = true;
	result.m_impactDistance = commonImpactMinValue * rayLength;
	result.m_impactPosition = rayStart + rayForwardNormal * result.m_impactDistance;

	if (impactRangeXY.m_min > impactRangeZ.m_min)
	{
		result.m_impactNormal = result.m_impactPosition - Vec3(centerXY.x, centerXY.y, result.m_impactPosition.z);
		result.m_impactNormal = result.m_impactNormal.GetNormalized();
	}
	else if (impactRangeXY.m_min < impactRangeZ.m_min)
	{
		if (rayTValueAtCylinderMinZ < rayTValueAtCylinderMaxZ)
		{
			result.m_impactNormal = Vec3(0.0f, 0.0f, -1.0f);
		}
		else 
		{
			result.m_impactNormal = Vec3(0.0f, 0.0f, 1.0f);
		}
	}

	return result;
}

RaycastResult3D RaycastVsPlane3D(Vec3 rayStart, Vec3 rayForwardNormal, float rayLength, Plane3 const& plane)
{
	RaycastResult3D result = RaycastResult3D();
	
	float distanceStartPlane = plane.GetDistanceToPlane(rayStart);

	float distanceEndPlane = plane.GetDistanceToPlane(rayStart + rayLength * rayForwardNormal);

	if (distanceStartPlane * distanceEndPlane > 0.0f)
	{
		return result;
	}

	float ratio = -distanceStartPlane / distanceEndPlane;

	result.m_didImpact = true;
	result.m_impactDistance = ratio * rayLength / (1.0f + ratio);
	result.m_impactPosition = rayStart + rayForwardNormal * result.m_impactDistance;
	if (distanceEndPlane > 0.0f)
	{
		result.m_impactNormal = plane.m_normal;
	}
	else 
	{
		result.m_impactNormal = -plane.m_normal;
	}
	return result;
}

RaycastResult3D RaycastVsOBB3D(Vec3 const& rayStart, Vec3 const& rayForwardNormal, float rayLength, OBB3 const& obb)
{
	RaycastResult3D result = RaycastResult3D();

	Mat44 inverseModelMatrix = obb.GetModelMatrix().GetOrthonormalInverse();
	
	Vec3 rayStartInOBBLocal = inverseModelMatrix.TransformPosition3D(rayStart);

	Vec3 rayForwardNormalInOBBLocal = inverseModelMatrix.TransformVectorQuantity3D(rayForwardNormal);

	result = RaycastVsAABB3D(rayStartInOBBLocal, rayForwardNormalInOBBLocal, rayLength, obb.GetLocalSpaceAABB3());

	if (result.m_didImpact == false)
	{
		return RaycastResult3D();
	}

	result.m_impactPosition = obb.GetModelMatrix().TransformPosition3D(result.m_impactPosition);
	result.m_impactNormal = obb.GetModelMatrix().TransformVectorQuantity3D(result.m_impactNormal);

	return result;
}

RaycastResult3D::RaycastResult3D()
{
	m_didImpact = false;
}

RaycastResult3D::~RaycastResult3D()
{

}

void RaycastResult3D::Clear()
{
	m_didImpact = false;
	m_impactDistance = 0.f;
	m_impactPosition = Vec3::ZERO;
	m_impactNormal = Vec3::ZERO;

}
