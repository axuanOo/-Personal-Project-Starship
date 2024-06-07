#pragma once
#include <math.h>
//
// Forward type declarations
//
struct Vec2;
struct Vec3;
struct Vec4;
struct IntVec2;
struct AABB2;
struct AABB3;
struct Mat44;
struct LineSegment2 ;
struct Capsule2;
class FloatRange;
struct OBB2;
struct OBB3;
struct Plane3;

constexpr float g_PI = 3.14159265359f;

enum class BillboardType 
{
	NONE=-1,
	WORLD_UP_CAMERA_FACING,
	WORLD_UP_CAMERA_OPPOSING,
	FULL_CAMERA_FACING,
	FULL_CAMERA_OPPOSING,
	COUNT
};

//
// Angle utilities
float		ConvertDegreeToRadians(float degrees);
float		ConvertRadiansToDegrees(float radians);
float		CosDegrees(float degrees);
float		SinDegrees(float degrees);
float		Atan2Degrees(float y, float x);
float		GetShortestAngularDisDegrees(float startDegrees, float endDegrees);
float		GetTurnedTowardDegrees( float currentDegrees, float goalDegrees, float maxDeltaDegrees);
Vec3		ConstructVec3FromVec2(const Vec2& vec2);

//
// Distance & projections utilities
//
float		GetDistance2D(Vec2 const& positionA, Vec2 const& positionB);
float		GetDistanceSquared2D(Vec2 const& positionA, Vec2 const& positionB);
float		GetDistance3D(Vec3 const& positionA, Vec3 const& positionB);
float		GetDistanceSquared3D(Vec3 const& positionA, Vec3 const& positionB);
float		GetDistanceXY3D(Vec3 const& positionA, Vec3 const& positionB);
float		GetDistanceXYSquared3D(Vec3 const& positionA, Vec3 const& positionB);
int			GetTaxicabDistance2D(IntVec2 const& pointA, IntVec2 const& pointB);
float		GetProjectedLength2D(Vec2 const& vectorToProject, Vec2 const& vectorToProjectOnto );
Vec2 const	GetProjectedOnto2D(Vec2 const& vectorToProject, Vec2 const& vectorToProjectOnto);
Vec3 const  GetProjectedOnto3D(Vec3 const& vectorToProject, Vec3 const& vectorToProjectOnto);
float		GetAngleDegreesBetweenVectors2D(Vec2 const& vectorA, Vec2 const& vectorB);

//
// Geometric query utilities
//
bool		IsPointInsideDisc2D( Vec2 const& point, Vec2 const& discCenter, float discRadius);
bool		IsPointInsideAABB2D( Vec2 const& point, AABB2 const& box );
bool		IsPointInsideAABB2D(Vec2 const& point, Vec2 const& minPos, Vec2 const& maxPos);
bool		IsPointInsideCapsule2D( Vec2 const& point, Capsule2 const& capsule);
bool		IsPointInsideCapsule2D( Vec2 const& point, Vec2 const& boneStart, Vec2 const& boneEnd, float radius);
bool		IsPointInsideOBB2D( Vec2 const& point, OBB2 const& orientedBox);
bool		IsPointInsideOrientedSector2D( Vec2 const& point, Vec2 const& sectorTip, float sectorForwardDegrees, float sectorApertureDegrees, float sectorRadius);
bool		IsPointInsideDirectedSector2D(	Vec2 const& point, Vec2 const& sectorTip, Vec2 const& sectorForwardNormal, float sectorApertureDegrees, float sectorRadius);
bool		DoDiscsOverlap(Vec2 const& centerA, float radiusA, Vec2 const& centerB, float radiusB);
bool		DoSphereOverlap(Vec3 const& centerA, float radiusA, Vec3 const& centerB, float radiusB);
bool		DoAABB2Overlap(AABB2 const& aabb1, AABB2 const& aabb2);
bool		PushDiscOutOfFixedPoint2D(Vec2& mobileDiscCenter, float discRadius, Vec2 const& fixedPoint);
bool		PushDiscOutOfFixedDisc2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2 const& fixedDiscCenter, float fixedDiscRadius);
bool		PushDiscOutOfEachOther2D( Vec2& aCenter, float aRadius, Vec2& bCenter, float bRadius);
bool		PushDiscOutOfFixedAABB2D(Vec2& mobileDiscCenter, float discRadius, AABB2 const& fixedBox);
Vec2 const	GetNearestPointOnDisc2D(Vec2 const& referencePosition, Vec2 const& discCenter, float discRadius);
Vec2 const	GetNearestPointOnAABB2D(Vec2 const& referencePos, const AABB2& box);
Vec2 const	GetNearestPointInfiniteLine2D( Vec2 const& referencePos, LineSegment2 const& infiniteLine);
Vec2 const	GetNearestPointInfiniteLine2D( Vec2 const& referencePos, Vec2 const& pointOnLine, Vec2 const& anotherPointOnLine);
Vec2 const	GetNearestPointLineSegment2D( Vec2 const& referencePos, LineSegment2 const& lineSegment );
Vec2 const	GetNearestPointLineSegment2D( Vec2 const& referencePos, Vec2 const& lineSegStart, Vec2 const& lineSegEnd );
Vec2 const  GetNearestPointOnCapsule2D( Vec2 const& referencePos, Capsule2 const& capsule);
Vec2 const	GetNearestPointOnCapsule2D( Vec2 const& referencePos, Vec2 const& boneStart, Vec2 const& boneEnd, float radius );
Vec2 const  GetNearestPointOnOBB2D( Vec2 const& referencePos, OBB2 const& orientedBox );


bool		IsPointInsideAABB3D(Vec3 const& referencePos, AABB3 const& box);
bool		IsPointInsideSphere3D(Vec3 const& referencePos, Vec3 const& sphereCenter, float sphereRadius);
bool		IsPointInsideZCylinder(Vec3 const& referencePos, Vec2 cylinderCenterXY, float cylinderRadius, FloatRange cylinderMinMaxZ);

bool		DoAABBsOverlap3D(AABB3 const& first, AABB3 const& second);
bool		DoSpheresOverlap3D(Vec3 const& centerA, float radiusA, Vec3 const& centerB, float radiusB);
bool		DoZCylindersOverlap3D(Vec2 cylinder1CenterXY, float cylinder1Radius, FloatRange cylinder1MinMaxZ, Vec2 cylinder2CenterXY, float cylinder2Radius, FloatRange cylinder2MinMaxZ);
bool		DoSphereAndAABBOverlap3D(Vec3 const& sphereCenter, float sphereRadius, AABB3 box);
bool		DoZCylinderAndAABBOverlap3D(Vec2 cylinderCenterXY, float cylinderRadius, FloatRange cylinderMinMaxZ, AABB3 box);
bool		DoZCylinderAndSphereOverlap3D(Vec2 cylinderCenterXY, float cylinderRadius, FloatRange cylinderMinMaxZ, Vec3 sphereCenter, float sphereRadius);
bool		DoPlaneAndAABBOverlap3D(AABB3 const& aabb, Plane3 const& plane);
bool		DoPlaneAndSphereOverlap3D(Vec3 const& centerA, float radiusA, Plane3 const& plane);
bool		DoPlaneAndOBBOverlap3D(OBB3 const& obbBounds, Plane3 const& plane);

Vec3 const	GetNearestPointOnAABB3D(Vec3 const& referencePos, AABB3 const& bounds);
Vec3 const	GetNearestPointOnSphere3D(Vec3 const& referencePos, Vec3 const& sphereCenter, float sphereRadius);
Vec3 const	GetNearestPointOnZCylinder(Vec3 const& referencePos, Vec2 const& cylinderCenterXY, float cylinderRadius, FloatRange cylinderMinMaxZ);
Vec3 const	GetNearestPointOnOBB3D(Vec3 const& referencePos, OBB3 const& obb);
Vec3 const	GetNearestPointOnPlane3D(Vec3 const& referencePos, Plane3 const& plane);
//
// Transform utilities
//
void		TransformPosition2D(Vec2& posToTransform, float uniformScale, float rotationDegrees, Vec2 const& translation);
void		TransformPosition2D(Vec2& posToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation);
void		TransformPositionXY3D(Vec3& positionToTransform, float scaleXY, float zRotationDegrees, Vec2 const& translationXY);
void		TransformPositionXY3D(Vec3& posToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation);

// Clamp and lerp
float		GetClamped(	float value, float minValue, float maxValue); 
float		GetClampedZeroToOne( float value );
float		Interpolate( float start, float end, float fractionTowardEnd);
Vec2		Interpolate( Vec2& startVec, Vec2& endVec, float fractionTowardEnd);
float		GetFractionWithinRange( float value, float rangeStart, float rangeEnd );
float		RangeMap( float inValue, float inStart, float inEnd, float outStart, float outEnd);
float		RangeMapClamped( float inValue, float inStart, float inEnd, float outStart, float outEnd);
float		GetMaxOfThreeValues(float value1, float value2, float value3);
float		GetMinOfThreeValues(float value1, float value2, float value3);
int		    RoundDownToInt( float value );

// Dot and Cross
float		DotProduct2D( Vec2 const& a, Vec2 const& b);
float		DotProduct3D(Vec3 const& a, Vec3 const& b);
float		DotProduct4D(Vec4 const& a, Vec4 const& b);
float		CrossProduct2D(Vec2 const& a, Vec2 const& b);
Vec3		CrossProduct3D(Vec3 const& a, Vec3 const& b);

// Normalize byte
float		NormalizeByte(unsigned char byteValue);
unsigned char DenormalizeByte(float value);

// Matrix function
Mat44		GetBillboardMatrix(BillboardType billboardType, Mat44 const& cameraMatrix, const Vec3& billboardPosition, const Vec2& billboardScale);

// Easing Curves
float		SmoothStart2(float x);
float		SmoothStart3(float x);
float		SmoothStart4(float x);
float		SmoothStart5(float x);
float		SmoothStart6(float x);

float		SmoothStop2(float x);
float		SmoothStop3(float x);
float		SmoothStop4(float x);
float		SmoothStop5(float x);
float		SmoothStop6(float x);

float		ComputeCubicBezier1D(float A, float B, float C, float D, float t);
float		ComputeQuinticBezier1D( float A, float B, float C, float D, float E, float F, float t );

float		SmoothStep3(float x);
float		SmoothStep5(float x);
float		Hesitate3(float x);
float		Hesitate5(float x);

float		CustomCurve(float x);