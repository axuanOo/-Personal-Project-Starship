#pragma once
#include "Engine/Core/Vertex_PCU.hpp"
#include <vector>

//
// Forward type declarations
//
struct Vec2;
struct Vec3;
struct IntVec2;
struct AABB2;
struct AABB3;
struct Mat44;
struct LineSegment2;
struct Capsule2;
struct OBB2;
struct OBB3;

void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts,Capsule2 const& capsule, Rgba8 const& color);
void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, Vec2 const& boneStart, Vec2 const& boneEnd, float radius, Rgba8 const& color);
void AddVertsForHalfDisc(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, Rgba8 const& color, float startDegree);
void AddVertsForDisc2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, Rgba8 const& color);
void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, Rgba8 const& color, Vec2 const& uvAtMins = Vec2(0.0f, 0.0f), Vec2 const& uvAtMaxs = Vec2(1.0f, 1.0f));
void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, Rgba8 const& color, AABB2 const& uvBox);
void AddVertsForOBB2D(std::vector<Vertex_PCU>& verts, OBB2 const& box , Rgba8 const& color);
void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, Vec2 const& start, Vec2 const& end, float thickness, Rgba8 const& color);
void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, LineSegment2 const& lineSegment, float thickness, Rgba8 const& color);
void AddVertsForArrow2D( std::vector<Vertex_PCU>& verts, Vec2 tailPos, Vec2 tipPos, float arrowSize, float lineThickness, Rgba8 const& color );

void AddVertsForQuad3D( std::vector<Vertex_PCU>& verts,
	const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Vec3& topLeft,
	const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);

void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts,
	std::vector<unsigned int>& indexes,
	const Vec3& bottomLeft, const Vec3& bottomRight,
	const Vec3& topRight, const Vec3& topLeft,
	const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);

void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts,
	std::vector<unsigned int>& indexes,
	const Vec3& bottomLeft, const Vec3& bottomRight, 
	const Vec3& topRight, const Vec3& topLeft,
	const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE, int subdivisionX = 1, int subdivisionY = 1);

void AddVertsForAABB3D(std::vector<Vertex_PCU>& verts,
	const AABB3& bounds, const Rgba8& color = Rgba8::WHITE,
	const AABB2& UVs = AABB2::ZERO_TO_ONE);

void AddVertsForAABB3D(std::vector<Vertex_PCUTBN>& verts,
	std::vector<unsigned int>& indexes,
	const AABB3& bounds, const Rgba8& color = Rgba8::WHITE,
	const AABB2& UVs = AABB2::ZERO_TO_ONE);

void AddVertsForSphere3D(std::vector<Vertex_PCU>& verts,
	const Vec3& center, float radius, const Rgba8& color = Rgba8::WHITE,
	const AABB2& UVs = AABB2::ZERO_TO_ONE, int numLatitudeSlices = 8,int numLongtitudeSlices = 16);

void AddVertsForOBB3D(std::vector<Vertex_PCU>& verts,
	OBB3 const& obb, const Rgba8& color = Rgba8::WHITE,
	const AABB2& UVs = AABB2::ZERO_TO_ONE);

void TransformVertexArray3D(std::vector<Vertex_PCU>&verts, const Mat44& transform);
AABB2 GetVertexBounds2D(const std::vector<Vertex_PCU>& verts);

void AddVertsForCylinder3D(std::vector<Vertex_PCU>& verts, const Vec3& start, const Vec3& end, float radius, const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE, int numSlices = 8);

void AddVertsForCone3D(std::vector<Vertex_PCU>& verts, const Vec3& start, const Vec3& end, float radius, const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE, int numSlices = 8);

void AddVertsForArrow3D(std::vector<Vertex_PCU>& verts, const Vec3& start, const Vec3& end, float radius, float arrowAspect = 0.2f, const Rgba8& color = Rgba8::WHITE, const Rgba8& coneColor = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE, int numSlices = 8);

void TransformVertexArrayXY3D(int numVerts, Vertex_PCU* verts, float uniformScaleXY
	, float rotationDegreesAboutZ, Vec2 const& translationXY);


