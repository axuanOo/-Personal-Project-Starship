#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Capsule2.hpp"
#include "Engine/Math/LineSegment2.hpp"
#include "Engine/Math/OBB2.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/Matrix44.hpp"
#include "Engine/Math/OBB3.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"


void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, Capsule2 const& capsule, Rgba8 const& color)
{
	int NUM_TRIANGLES = 32;

	int NUM_VERTS_DISC = NUM_TRIANGLES * 6;
	int NUM_VERTS_AABB = 6;
	int NUM_VERTS = NUM_VERTS_DISC + NUM_VERTS_AABB;

	float SINGLE_ANGLE_STEP = 180.0f / NUM_TRIANGLES;

	verts.reserve(NUM_VERTS);

	float startAngle = 0.0f;

	Vec3 UpDirectionLocal = Vec3(0.0f, 1.0f, 0.0f) * capsule.radius;
	Vec3 DownDirectionLocal = Vec3(0.0f, -1.0f, 0.0f) * capsule.radius;

	float startToEndDistance = (capsule.m_end - capsule.m_start).GetLength();
	float orientDegree = (capsule.m_end - capsule.m_start).GetOrientationDegrees();

	Vec3 capsuleCenterLocal = Vec3(0.0f, 0.0f, 0.0f);
	Vec3 capsuleStartLocal = Vec3(-0.5f * startToEndDistance, 0.0f, 0.0f);
	Vec3 capsuleEndLocal = Vec3(0.5f * startToEndDistance, 0.0f, 0.0f);

	Vertex_PCU boneStartCenterPCU = Vertex_PCU(ConstructVec3FromVec2(capsule.GetCenter()), color, Vec2(0.0f, 0.0f));
	Vertex_PCU boneEndCenterPCU = Vertex_PCU(capsuleEndLocal.GetRotatedAboutZDegrees(orientDegree)+ConstructVec3FromVec2(capsule.GetCenter()), color, Vec2(0.0f, 0.0f));

	Vec3 capsuleAABB_LeftBottom = capsuleStartLocal + DownDirectionLocal;
	Vec3 capsuleAABB_LeftTop = capsuleStartLocal + UpDirectionLocal;
	Vec3 capsuleAABB_RightBottom = capsuleEndLocal + DownDirectionLocal;
	Vec3 capsuleAABB_RightTop = capsuleEndLocal + UpDirectionLocal;

	capsuleAABB_LeftBottom = capsuleAABB_LeftBottom.GetRotatedAboutZDegrees(orientDegree) + ConstructVec3FromVec2(capsule.GetCenter());
	capsuleAABB_LeftTop = capsuleAABB_LeftTop.GetRotatedAboutZDegrees(orientDegree)+ ConstructVec3FromVec2(capsule.GetCenter());
	capsuleAABB_RightBottom = capsuleAABB_RightBottom.GetRotatedAboutZDegrees(orientDegree)+ ConstructVec3FromVec2(capsule.GetCenter());
	capsuleAABB_RightTop = capsuleAABB_RightTop.GetRotatedAboutZDegrees(orientDegree)+ ConstructVec3FromVec2(capsule.GetCenter());

	Vertex_PCU PCU_LeftBottom = Vertex_PCU(capsuleAABB_LeftBottom, color, Vec2(0.0f, 0.0f));
	Vertex_PCU PCU_LeftTop = Vertex_PCU(capsuleAABB_LeftTop, color, Vec2(0.0f, 0.0f));
	Vertex_PCU PCU_RightBottom = Vertex_PCU(capsuleAABB_RightBottom, color, Vec2(0.0f, 0.0f));
	Vertex_PCU PCU_RightTop = Vertex_PCU(capsuleAABB_RightTop, color, Vec2(0.0f, 0.0f));

	verts.push_back(PCU_LeftBottom);
	verts.push_back(PCU_RightBottom);
	verts.push_back(PCU_RightTop);

	verts.push_back(PCU_LeftBottom);
	verts.push_back(PCU_RightTop);
	verts.push_back(PCU_LeftTop);


	for (int i = 0; i < NUM_TRIANGLES; i++)
	{
		Vec3 startLow = UpDirectionLocal.GetRotatedAboutZDegrees(startAngle) + capsuleStartLocal;
		startLow = startLow.GetRotatedAboutZDegrees(orientDegree) + ConstructVec3FromVec2(capsule.GetCenter());
		Vec3 endLow = DownDirectionLocal.GetRotatedAboutZDegrees(startAngle) + capsuleEndLocal;
		endLow = endLow.GetRotatedAboutZDegrees(orientDegree) + ConstructVec3FromVec2(capsule.GetCenter());
		Vertex_PCU lowerEdgeOfTriangleStart = Vertex_PCU(startLow, color, Vec2(1.0f, 0.0f));
		Vertex_PCU lowerEdgeOfTriangleEnd = Vertex_PCU(endLow, color, Vec2(1.0f, 0.0f));

		startAngle += SINGLE_ANGLE_STEP;

		Vec3 startHigh = UpDirectionLocal.GetRotatedAboutZDegrees(startAngle) + capsuleStartLocal;
		startHigh = startHigh.GetRotatedAboutZDegrees(orientDegree) + ConstructVec3FromVec2(capsule.GetCenter());
		Vec3 endHigh = DownDirectionLocal.GetRotatedAboutZDegrees(startAngle) + capsuleEndLocal;
		endHigh = endHigh.GetRotatedAboutZDegrees(orientDegree) + ConstructVec3FromVec2(capsule.GetCenter());

		Vertex_PCU higherEdgeOfTriangleStart = Vertex_PCU(startHigh, color, Vec2(1.0f, 0.0f));
		Vertex_PCU higherEdgeOfTriangleEnd = Vertex_PCU(endHigh, color, Vec2(1.0f, 0.0f));

		verts.push_back(boneStartCenterPCU);
		verts.push_back(lowerEdgeOfTriangleStart);
		verts.push_back(higherEdgeOfTriangleStart);

		verts.push_back(boneEndCenterPCU);
		verts.push_back(lowerEdgeOfTriangleEnd);
		verts.push_back(higherEdgeOfTriangleEnd);
	}

}

void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, Vec2 const& boneStart, Vec2 const& boneEnd, float radius, Rgba8 const& color)
{
	int NUM_TRIANGLES = 32;

	int NUM_VERTS_DISC = NUM_TRIANGLES * 6;
	int NUM_VERTS_AABB = 6;
	int NUM_VERTS = NUM_VERTS_DISC + NUM_VERTS_AABB;

	float SINGLE_ANGLE_STEP = 180.0f / NUM_TRIANGLES;

	verts.reserve(NUM_VERTS);

	float startAngle = 0.0f;

	Vec3 UpDirectionLocal = Vec3(0.0f, 1.0f, 0.0f) * radius;
	Vec3 DownDirectionLocal = Vec3(0.0f, -1.0f, 0.0f) * radius;

	float startToEndDistance = (boneEnd - boneStart).GetLength();

	Vec3 capsuleCenterLocal = Vec3(0.0f, 0.0f, 0.0f);
	Vec3 capsuleStartLocal = Vec3(-0.5f * startToEndDistance, 0.0f, 0.0f);
	Vec3 capsuleEndLocal = Vec3(0.5f * startToEndDistance, 0.0f, 0.0f);

	Vertex_PCU boneStartCenterPCU = Vertex_PCU(capsuleStartLocal, color, Vec2(0.0f, 0.0f));
	Vertex_PCU boneEndCenterPCU = Vertex_PCU(capsuleEndLocal, color, Vec2(0.0f, 0.0f));

	Vec3 capsuleAABB_LeftBottom = capsuleStartLocal + DownDirectionLocal;
	Vec3 capsuleAABB_LeftTop = capsuleStartLocal + UpDirectionLocal;
	Vec3 capsuleAABB_RightBottom = capsuleEndLocal + DownDirectionLocal;
	Vec3 capsuleAABB_RightTop = capsuleEndLocal + UpDirectionLocal;

	Vertex_PCU PCU_LeftBottom = Vertex_PCU(capsuleAABB_LeftBottom, color, Vec2(0.0f, 0.0f));
	Vertex_PCU PCU_LeftTop = Vertex_PCU(capsuleAABB_LeftTop, color, Vec2(0.0f, 0.0f));
	Vertex_PCU PCU_RightBottom = Vertex_PCU(capsuleAABB_RightBottom, color, Vec2(0.0f, 0.0f));
	Vertex_PCU PCU_RightTop = Vertex_PCU(capsuleAABB_RightTop, color, Vec2(0.0f, 0.0f));

	verts.push_back(PCU_LeftBottom);
	verts.push_back(PCU_LeftTop);
	verts.push_back(PCU_RightBottom);

	verts.push_back(PCU_RightTop);
	verts.push_back(PCU_LeftTop);
	verts.push_back(PCU_RightBottom);


	for (int i = 0; i < NUM_TRIANGLES; i++)
	{
		Vertex_PCU lowerEdgeOfTriangleStart = Vertex_PCU(UpDirectionLocal.GetRotatedAboutZDegrees(startAngle) + capsuleStartLocal, color, Vec2(1.0f, 0.0f));
		Vertex_PCU lowerEdgeOfTriangleEnd = Vertex_PCU(DownDirectionLocal.GetRotatedAboutZDegrees(startAngle) + capsuleEndLocal, color, Vec2(1.0f, 0.0f));

		startAngle += SINGLE_ANGLE_STEP;

		Vertex_PCU higherEdgeOfTriangleStart = Vertex_PCU(UpDirectionLocal.GetRotatedAboutZDegrees(startAngle) + capsuleStartLocal, color, Vec2(1.0f, 0.0f));
		Vertex_PCU higherEdgeOfTriangleEnd = Vertex_PCU(DownDirectionLocal.GetRotatedAboutZDegrees(startAngle) + capsuleEndLocal, color, Vec2(1.0f, 0.0f));

		verts.push_back(lowerEdgeOfTriangleStart);
		verts.push_back(boneStartCenterPCU);
		verts.push_back(higherEdgeOfTriangleStart);

		verts.push_back(lowerEdgeOfTriangleEnd);
		verts.push_back(boneEndCenterPCU);
		verts.push_back(higherEdgeOfTriangleEnd);
	}

	Vec2 capsuleCenterWorld = (boneStart + boneEnd) * 0.5f;
	float orientDegree = (boneEnd - boneStart).GetOrientationDegrees();
	TransformVertexArrayXY3D(NUM_VERTS, verts.data(), 1.0f, orientDegree, capsuleCenterWorld);
}

void AddVertsForHalfDisc(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, Rgba8 const& color, float startDegree)
{
	int NUM_TRIANGLES = 32;
	int NUM_VERTS = NUM_TRIANGLES * 3;
	float SINGLE_ANGLE_STEP = 180.0f / NUM_TRIANGLES;

	verts.reserve(NUM_VERTS);

	float currentAngle = startDegree;

	Vec3 rightDirectionLocal = Vec3(1.0f, 0.0f, 0.0f) * radius;
	Vec3 circleCenterLocal = Vec3(0.0f, 0.0f, 0.0f);

	Vertex_PCU circleCenterPCU = Vertex_PCU(circleCenterLocal, color, Vec2(0.0f, 0.0f));

	for (int i = 0; i < NUM_TRIANGLES; i++)
	{
		Vertex_PCU lowerEdgeOfTriangle = Vertex_PCU(rightDirectionLocal.GetRotatedAboutZDegrees(currentAngle), color, Vec2(1.0f, 0.0f));

		currentAngle += SINGLE_ANGLE_STEP;

		Vertex_PCU higherEdgeOfTriangle = Vertex_PCU(rightDirectionLocal.GetRotatedAboutZDegrees(currentAngle), color, Vec2(1.0f, 0.0f));

		verts.push_back(lowerEdgeOfTriangle);
		verts.push_back(circleCenterPCU);
		verts.push_back(higherEdgeOfTriangle);
	}

	TransformVertexArrayXY3D(NUM_VERTS, verts.data(), 1.0f, 0.0f, center);
}

void AddVertsForDisc2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, Rgba8 const& color)
{
	int NUM_TRIANGLES = 64;
	//int NUM_VERTS = NUM_TRIANGLES * 3;
	float SINGLE_ANGLE_STEP = 360.0f / NUM_TRIANGLES;

	//verts.reserve(NUM_VERTS);

	float currentAngle = 0.0f;

	Vec3 rightDirectionLocal = Vec3(1.0f, 0.0f, 0.0f) * radius;
	Vec3 circleCenterLocal = Vec3(center.x, center.y, 0.0f);

	Vertex_PCU circleCenterPCU = Vertex_PCU(circleCenterLocal, color, Vec2(0.0f, 0.0f));

	for (int i = 0; i < NUM_TRIANGLES; i++)
	{
		Vertex_PCU lowerEdgeOfTriangle = Vertex_PCU(rightDirectionLocal.GetRotatedAboutZDegrees(currentAngle) + circleCenterLocal, color, Vec2(1.0f, 0.0f));
		currentAngle += SINGLE_ANGLE_STEP;
		Vertex_PCU higherEdgeOfTriangle = Vertex_PCU(rightDirectionLocal.GetRotatedAboutZDegrees(currentAngle) + circleCenterLocal, color, Vec2(1.0f, 0.0f));

		verts.push_back(circleCenterPCU);
		verts.push_back(lowerEdgeOfTriangle);
		verts.push_back(higherEdgeOfTriangle);
	}
}

void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, Rgba8 const& color, Vec2 const& uvAtMins, Vec2 const& uvAtMaxs)
{
	Vec2 leftBottomUV = uvAtMins;
	Vec2 rightBottomUV = Vec2(uvAtMaxs.x, uvAtMins.y);
	Vec2 leftTopUV = Vec2(uvAtMins.x, uvAtMaxs.y);
	Vec2 rightTopUV = uvAtMaxs;

	Vec3 leftBottomPointPosition = Vec3(bounds.GetPointAtUV(Vec2(0.0f, 0.0f)).x, bounds.GetPointAtUV(Vec2(0.0f, 0.0f)).y, 0.0f);
	Vec3 rightBottomPointPosition = Vec3(bounds.GetPointAtUV(Vec2(1.0f, 0.0f)).x, bounds.GetPointAtUV(Vec2(1.0f, 0.0f)).y, 0.0f);
	Vec3 leftTopPointPosition = Vec3(bounds.GetPointAtUV(Vec2(0.0f, 1.0f)).x, bounds.GetPointAtUV(Vec2(0.0f, 1.0f)).y, 0.0f);
	Vec3 rightTopPointPosition = Vec3(bounds.GetPointAtUV(Vec2(1.0f, 1.0f)).x, bounds.GetPointAtUV(Vec2(1.0f, 1.0f)).y, 0.0f);

	Vertex_PCU LBVert = Vertex_PCU(leftBottomPointPosition, color, leftBottomUV);
	Vertex_PCU RBVert = Vertex_PCU(rightBottomPointPosition, color, rightBottomUV);
	Vertex_PCU LTVert = Vertex_PCU(leftTopPointPosition, color, leftTopUV);
	Vertex_PCU RTVert = Vertex_PCU(rightTopPointPosition, color, rightTopUV);

	verts.push_back(LBVert);
	verts.push_back(RBVert);
	verts.push_back(RTVert);

	verts.push_back(LBVert);
	verts.push_back(RTVert);
	verts.push_back(LTVert);

}

void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, Rgba8 const& color, AABB2 const& uvBox)
{
	Vec2 leftBottomUV = uvBox.GetPointAtUV(Vec2::ZERO);
	Vec2 rightBottomUV = uvBox.GetPointAtUV(Vec2(1.0f, 0.0f));
	Vec2 leftTopUV = uvBox.GetPointAtUV(Vec2(0.0f, 1.0f));
	Vec2 rightTopUV = uvBox.GetPointAtUV(Vec2(1.0f, 1.0f));

	Vec3 leftBottomPointPosition = Vec3(bounds.GetPointAtUV(Vec2(0.0f, 0.0f)).x, bounds.GetPointAtUV(Vec2(0.0f, 0.0f)).y, 0.0f);
	Vec3 rightBottomPointPosition = Vec3(bounds.GetPointAtUV(Vec2(1.0f, 0.0f)).x, bounds.GetPointAtUV(Vec2(1.0f, 0.0f)).y, 0.0f);
	Vec3 leftTopPointPosition = Vec3(bounds.GetPointAtUV(Vec2(0.0f, 1.0f)).x, bounds.GetPointAtUV(Vec2(0.0f, 1.0f)).y, 0.0f);
	Vec3 rightTopPointPosition = Vec3(bounds.GetPointAtUV(Vec2(1.0f, 1.0f)).x, bounds.GetPointAtUV(Vec2(1.0f, 1.0f)).y, 0.0f);

	Vertex_PCU LBVert = Vertex_PCU(leftBottomPointPosition, color, leftBottomUV);
	Vertex_PCU RBVert = Vertex_PCU(rightBottomPointPosition, color, rightBottomUV);
	Vertex_PCU LTVert = Vertex_PCU(leftTopPointPosition, color, leftTopUV);
	Vertex_PCU RTVert = Vertex_PCU(rightTopPointPosition, color, rightTopUV);

	verts.push_back(LBVert);
	verts.push_back(RBVert);
	verts.push_back(RTVert);

	verts.push_back(LBVert);
	verts.push_back(RTVert);
	verts.push_back(LTVert);
}

void AddVertsForOBB2D(std::vector<Vertex_PCU>& verts, OBB2 const& box, Rgba8 const& color)
{
	verts.reserve(6);

	Vec2 jBasisNormal = box.m_iBasisNormal.GetRotated90Degrees();

	float boxHalfDimensionX = box.m_halfDimensions.x;
	float boxHalfDimensionY = box.m_halfDimensions.y;

	Vec2 OBB_LeftBottom = box.m_center - (boxHalfDimensionX * box.m_iBasisNormal) - (jBasisNormal * boxHalfDimensionY);
	Vec2 OBB_RightBottom =  box.m_center + (boxHalfDimensionX * box.m_iBasisNormal) - (jBasisNormal * boxHalfDimensionY);
	Vec2 OBB_LeftTop = box.m_center - (boxHalfDimensionX * box.m_iBasisNormal) + (jBasisNormal * boxHalfDimensionY);
	Vec2 OBB_RightTop =  box.m_center + (boxHalfDimensionX * box.m_iBasisNormal) + (jBasisNormal * boxHalfDimensionY);

	Vertex_PCU OBB_LB_PCU = Vertex_PCU(ConstructVec3FromVec2(OBB_LeftBottom), color, Vec2(0.0f, 0.0f));
	Vertex_PCU OBB_RB_PCU = Vertex_PCU(ConstructVec3FromVec2(OBB_RightBottom), color, Vec2(1.0f, 0.0f));
	Vertex_PCU OBB_LT_PCU = Vertex_PCU(ConstructVec3FromVec2(OBB_LeftTop), color, Vec2(0.0f, 1.0f));
	Vertex_PCU OBB_RT_PCU = Vertex_PCU(ConstructVec3FromVec2(OBB_RightTop), color, Vec2(1.0f, 1.0f));

	verts.push_back(OBB_LB_PCU);
	verts.push_back(OBB_RB_PCU);
	verts.push_back(OBB_RT_PCU);

	verts.push_back(OBB_LB_PCU);
	verts.push_back(OBB_RT_PCU);
	verts.push_back(OBB_LT_PCU);
}

void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, Vec2 const& start, Vec2 const& end, float thickness, Rgba8 const& color)
{
	verts.reserve(6);

	Vec2 startToEndNormalized = (end - start).GetNormalized();

	Vec2 GetUpDirectionalVector = startToEndNormalized.GetRotated90Degrees();
	Vec2 GetDownDirectionalVector = startToEndNormalized.GetRotatedMinus90Degrees();

	Vec2 leftBottomUV = Vec2(0.0f, 0.0f);
	Vec2 rightBottomUV = Vec2(1.0f, 0.0f);
	Vec2 leftTopUV = Vec2(0.0f, 1.0f);
	Vec2 rightTopUV = Vec2(1.0f, 1.0f);

	Vec2 leftBottomPointPosition = start + GetDownDirectionalVector * 0.5f * thickness;
	Vec2 rightBottomPointPosition = end + GetDownDirectionalVector * 0.5f * thickness;
	Vec2 leftTopPointPosition = start + GetUpDirectionalVector * 0.5f * thickness;
	Vec2 rightTopPointPosition = end + GetUpDirectionalVector * 0.5f * thickness;

	Vertex_PCU RBVert = Vertex_PCU(Vec3(rightBottomPointPosition.x, rightBottomPointPosition.y, 0.0f), color, rightBottomUV);
	Vertex_PCU LBVert = Vertex_PCU(Vec3(leftBottomPointPosition.x, leftBottomPointPosition.y, 0.0f), color, leftBottomUV);
	Vertex_PCU LTVert = Vertex_PCU(Vec3(leftTopPointPosition.x, leftTopPointPosition.y, 0.0f), color, leftTopUV);
	Vertex_PCU RTVert = Vertex_PCU(Vec3(rightTopPointPosition.x, rightTopPointPosition.y, 0.0f), color, rightTopUV);

	verts.push_back(LBVert);
	verts.push_back(RBVert);
	verts.push_back(RTVert);

	verts.push_back(LBVert);
	verts.push_back(RTVert);
	verts.push_back(LTVert);

}

void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, LineSegment2 const& lineSegment, float thickness, Rgba8 const& color)
{
	verts.reserve(6);

	Vec2 startToEndNormalized = (lineSegment.m_end - lineSegment.m_start).GetNormalized();

	Vec2 GetUpDirectionalVector = startToEndNormalized.GetRotated90Degrees();
	Vec2 GetDownDirectionalVector = startToEndNormalized.GetRotatedMinus90Degrees();

	Vec2 leftBottomUV = Vec2(0.0f, 0.0f);
	Vec2 rightBottomUV = Vec2(1.0f, 0.0f);
	Vec2 leftTopUV = Vec2(0.0f, 1.0f);
	Vec2 rightTopUV = Vec2(1.0f, 1.0f);

	Vec2 leftBottomPointPosition = lineSegment.m_start + GetDownDirectionalVector * 0.5f * thickness;
	Vec2 rightBottomPointPosition = lineSegment.m_end + GetDownDirectionalVector * 0.5f * thickness;
	Vec2 leftTopPointPosition = lineSegment.m_start + GetUpDirectionalVector * 0.5f * thickness;
	Vec2 rightTopPointPosition = lineSegment.m_end + GetUpDirectionalVector * 0.5f * thickness;

	Vertex_PCU RBVert = Vertex_PCU(Vec3(rightBottomPointPosition.x, rightBottomPointPosition.y, 0.0f), color, rightBottomUV);
	Vertex_PCU LBVert = Vertex_PCU(Vec3(leftBottomPointPosition.x, leftBottomPointPosition.y, 0.0f), color, leftBottomUV);
	Vertex_PCU LTVert = Vertex_PCU(Vec3(leftTopPointPosition.x, leftTopPointPosition.y, 0.0f), color, leftTopUV);
	Vertex_PCU RTVert = Vertex_PCU(Vec3(rightTopPointPosition.x, rightTopPointPosition.y, 0.0f), color, rightTopUV);

	verts.push_back(LBVert);
	verts.push_back(RBVert);
	verts.push_back(RTVert);

	verts.push_back(LBVert);
	verts.push_back(RTVert);
	verts.push_back(LTVert);
}


void AddVertsForArrow2D(std::vector<Vertex_PCU>& verts, Vec2 tailPos, Vec2 tipPos, float arrowSize, float lineThickness, Rgba8 const& color)
{
	verts.reserve(18);

	Vec2 fwdNormal = (tailPos - tipPos).GetNormalized();

	Vec2 fwdPlus45 = fwdNormal.GetRotatedDegrees(45.0f) * arrowSize;
	Vec2 fwdMinus45 = fwdNormal.GetRotatedDegrees(-45.0f) * arrowSize;

	AddVertsForLineSegment2D(verts, tipPos, tailPos, lineThickness, color);
	AddVertsForLineSegment2D(verts, tipPos, tipPos + fwdPlus45, lineThickness, color);
	AddVertsForLineSegment2D(verts, tipPos, tipPos + fwdMinus45, lineThickness, color);
}

void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Vec3& topLeft, const Rgba8& color /*= Rgba8::WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	verts.reserve(6);

	Vertex_PCU bottomLeftVertex = Vertex_PCU(bottomLeft, color, UVs.GetPointAtUV(Vec2(0.0f, 0.0f)));
	Vertex_PCU bottomRightVertex = Vertex_PCU(bottomRight, color, UVs.GetPointAtUV(Vec2(1.0f, 0.0f)));
	Vertex_PCU topLeftVertex = Vertex_PCU(topLeft, color, UVs.GetPointAtUV(Vec2(0.0f, 1.0f)));
	Vertex_PCU topRightVertex = Vertex_PCU(topRight, color, UVs.GetPointAtUV(Vec2(1.0f, 1.0f)));

	verts.push_back(bottomLeftVertex);
	verts.push_back(bottomRightVertex);
	verts.push_back(topRightVertex);

	verts.push_back(bottomLeftVertex);
	verts.push_back(topRightVertex);
	verts.push_back(topLeftVertex);

}

void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Vec3& topLeft, const Rgba8& color /*= Rgba8::WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/, int subdivisionX, int subdivisionY)
{
	Vec3 bottomLeftNormal = CrossProduct3D((bottomRight - bottomLeft), (topRight - bottomLeft)).GetNormalized();
	Vec3 bottomRightNormal = CrossProduct3D((topRight - bottomRight), (bottomLeft - bottomRight)).GetNormalized();
	Vec3 topRightNormal = CrossProduct3D((bottomLeft - topRight), (bottomRight - topRight)).GetNormalized();
	Vec3 topLeftNormal = CrossProduct3D((bottomLeft - topLeft), (topRight - topLeft)).GetNormalized();

	if (subdivisionX == 1 && subdivisionY == 1)
	{
		int vertexesSize = (int)verts.size();

		Vertex_PCUTBN bottomLeftVertex = Vertex_PCUTBN(bottomLeft, color, UVs.GetPointAtUV(Vec2(0.0f, 0.0f)), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), bottomLeftNormal);
		Vertex_PCUTBN bottomRightVertex = Vertex_PCUTBN(bottomRight, color, UVs.GetPointAtUV(Vec2(1.0f, 0.0f)), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), bottomRightNormal);
		Vertex_PCUTBN topRightVertex = Vertex_PCUTBN(topRight, color, UVs.GetPointAtUV(Vec2(1.0f, 1.0f)), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), topRightNormal);
		Vertex_PCUTBN topLeftVertex = Vertex_PCUTBN(topLeft, color, UVs.GetPointAtUV(Vec2(0.0f, 1.0f)), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), topLeftNormal);

		verts.push_back(bottomLeftVertex);
		verts.push_back(bottomRightVertex);
		verts.push_back(topRightVertex);
		verts.push_back(topLeftVertex);


		indexes.push_back(0 + vertexesSize);
		indexes.push_back(1 + vertexesSize);
		indexes.push_back(2 + vertexesSize);

		indexes.push_back(0 + vertexesSize);
		indexes.push_back(2 + vertexesSize);
		indexes.push_back(3 + vertexesSize);
	}
	else 
	{
		float subdivisionStepX = 1.0f / subdivisionX;
		float subdivisionStepY = 1.0f / subdivisionY;

		Vec3 localRight = bottomRight - bottomLeft;
		Vec3 localUp = topLeft - bottomLeft;

		for (int i = 0; i < subdivisionX; i++)
		{
			for (int j = 0; j < subdivisionY; j++)
			{
				int vertexesSize = (int)verts.size();

				Vec3 localLeftBottom = bottomLeft + (float)i * subdivisionStepX * localRight + (float)j * subdivisionStepY *localUp;
				Vec3 localRightBottom = bottomLeft + ((float)(i + 1) *subdivisionStepX * localRight) + (float)j *subdivisionStepY * localUp;
				Vec3 localTopRight = bottomLeft + ((float)(i + 1) *subdivisionStepX * localRight) + ((float)(j + 1) *subdivisionStepY * localUp);
				Vec3 localTopLeft = bottomLeft + (float)i *subdivisionStepX * localRight + ((float)(j + 1) *subdivisionStepY * localUp);

				Vertex_PCUTBN bottomLeftVertex = Vertex_PCUTBN(localLeftBottom, color, UVs.GetPointAtUV(Vec2(0.0f, 0.0f)), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), bottomLeftNormal);
				Vertex_PCUTBN bottomRightVertex = Vertex_PCUTBN(localRightBottom, color, UVs.GetPointAtUV(Vec2(1.0f, 0.0f)), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), bottomRightNormal);
				Vertex_PCUTBN topRightVertex = Vertex_PCUTBN(localTopRight, color, UVs.GetPointAtUV(Vec2(1.0f, 1.0f)), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), topRightNormal);
				Vertex_PCUTBN topLeftVertex = Vertex_PCUTBN(localTopLeft, color, UVs.GetPointAtUV(Vec2(0.0f, 1.0f)), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), topLeftNormal);

				verts.push_back(bottomLeftVertex);
				verts.push_back(bottomRightVertex);
				verts.push_back(topRightVertex);
				verts.push_back(topLeftVertex);


				indexes.push_back(0 + vertexesSize);
				indexes.push_back(1 + vertexesSize);
				indexes.push_back(2 + vertexesSize);

				indexes.push_back(0 + vertexesSize);
				indexes.push_back(2 + vertexesSize);
				indexes.push_back(3 + vertexesSize);

			}
		}

		
	}

}

void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Vec3& topLeft, const Rgba8& color /*= Rgba8::WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	int vertexesSize = (int)verts.size();

	Vertex_PCU bottomLeftVertex = Vertex_PCU(bottomLeft, color, UVs.GetPointAtUV(Vec2(0.0f, 0.0f)));
	Vertex_PCU bottomRightVertex = Vertex_PCU(bottomRight, color, UVs.GetPointAtUV(Vec2(1.0f, 0.0f)));
	Vertex_PCU topRightVertex = Vertex_PCU(topRight, color, UVs.GetPointAtUV(Vec2(1.0f, 1.0f)));
	Vertex_PCU topLeftVertex = Vertex_PCU(topLeft, color, UVs.GetPointAtUV(Vec2(0.0f, 1.0f)));

	verts.push_back(bottomLeftVertex);
	verts.push_back(bottomRightVertex);
	verts.push_back(topRightVertex);
	verts.push_back(topLeftVertex);

	indexes.push_back(0 + vertexesSize);
	indexes.push_back(1 + vertexesSize);
	indexes.push_back(2 + vertexesSize);

	indexes.push_back(0 + vertexesSize);
	indexes.push_back(2 + vertexesSize);
	indexes.push_back(3 + vertexesSize);
}

void AddVertsForAABB3D(std::vector<Vertex_PCU>& verts, const AABB3& bounds, const Rgba8& color /*= Rgba8::WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	verts.reserve(36);

	Vec3 min = bounds.m_mins;
	Vec3 max = bounds.m_maxs;

	Vec3 box[] =
	{
		min,
		Vec3(max.x,min.y,min.z),
		Vec3(max.x,min.y,max.z),
		Vec3(min.x,min.y,max.z),
		Vec3(min.x,max.y,min.z),
		Vec3(max.x,max.y,min.z),
		max,
		Vec3(min.x,max.y,max.z)
	};

	AddVertsForQuad3D(verts, box[0], box[1], box[2], box[3], color, UVs);
	AddVertsForQuad3D(verts, box[1], box[5], box[6], box[2],color,UVs);
	AddVertsForQuad3D(verts, box[5], box[4], box[7], box[6],color,UVs);
	AddVertsForQuad3D(verts, box[4], box[0], box[3], box[7],color,UVs);
	AddVertsForQuad3D(verts, box[7], box[3], box[2], box[6],color,UVs);
	AddVertsForQuad3D(verts, box[5], box[1], box[0], box[4], color, UVs);
}


void AddVertsForAABB3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const AABB3& bounds, const Rgba8& color /*= Rgba8::WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	verts.reserve(36);

	Vec3 min = bounds.m_mins;
	Vec3 max = bounds.m_maxs;

	Vec3 box[] =
	{
		min,
		Vec3(max.x,min.y,min.z),
		Vec3(max.x,min.y,max.z),
		Vec3(min.x,min.y,max.z),
		Vec3(min.x,max.y,min.z),
		Vec3(max.x,max.y,min.z),
		max,
		Vec3(min.x,max.y,max.z)
	};

	AddVertsForQuad3D(verts, indexes, box[0], box[1], box[2], box[3], color, UVs);
	AddVertsForQuad3D(verts, indexes, box[1], box[5], box[6], box[2], color, UVs);
	AddVertsForQuad3D(verts, indexes, box[5], box[4], box[7], box[6], color, UVs);
	AddVertsForQuad3D(verts, indexes, box[4], box[0], box[3], box[7], color, UVs);
	AddVertsForQuad3D(verts, indexes, box[7], box[3], box[2], box[6], color, UVs);
	AddVertsForQuad3D(verts, indexes, box[5], box[1], box[0], box[4], color, UVs);
}


void AddVertsForSphere3D(std::vector<Vertex_PCU>& verts, const Vec3& center, float radius, const Rgba8& color /*= Rgba8::WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/, int numLatitudeSlices /*= 8*/, int numLongtitudeSlices /*= 8*/)
{
	verts.reserve(100000);

	float currentLongitude = 0.0f;
	float currentLatitude = -90.0f;

	int longitudeNum = numLongtitudeSlices;

	float singleStepDegreesLong = 360.0f / longitudeNum;
	float singleStepDegreesLati = 180.0f / numLatitudeSlices;

	float UVPercentage = 1.0f / numLatitudeSlices;

	Vec3 BLPosition;
	Vec3 BRPosition;
	Vec3 TRPosition;
	Vec3 TLPosition;

	Vertex_PCU BLVert;
	Vertex_PCU BRVert;
	Vertex_PCU TRVert;
	Vertex_PCU TLVert;

	for (int longtitudeIndex = 0; longtitudeIndex < longitudeNum; longtitudeIndex++)
	{
		currentLatitude = -90.0f;

		for (int latitudetitudeIndex = 0; latitudetitudeIndex < numLatitudeSlices; latitudetitudeIndex++)
		{
			BLPosition = Vec3::MakeFromPolarDegrees(currentLatitude, currentLongitude, radius);
			BRPosition = Vec3::MakeFromPolarDegrees(currentLatitude, currentLongitude + singleStepDegreesLong, radius);
			TRPosition = Vec3::MakeFromPolarDegrees(currentLatitude + singleStepDegreesLati, currentLongitude + singleStepDegreesLong, radius);
			TLPosition = Vec3::MakeFromPolarDegrees(currentLatitude + singleStepDegreesLati, currentLongitude, radius);

			BLPosition += center;
			BRPosition += center;
			TRPosition += center;
			TLPosition += center;

			BLVert = Vertex_PCU(BLPosition, color, UVs.GetPointAtUV(Vec2(longtitudeIndex * 0.5f * UVPercentage, latitudetitudeIndex * UVPercentage)));
			BRVert = Vertex_PCU(BRPosition, color, UVs.GetPointAtUV(Vec2((longtitudeIndex + 1) * 0.5f * UVPercentage, latitudetitudeIndex * UVPercentage)));
			TRVert = Vertex_PCU(TRPosition, color, UVs.GetPointAtUV(Vec2((longtitudeIndex + 1) * 0.5f * UVPercentage, (latitudetitudeIndex + 1) * UVPercentage)));
			TLVert = Vertex_PCU(TLPosition, color, UVs.GetPointAtUV(Vec2(longtitudeIndex * 0.5f * UVPercentage, (latitudetitudeIndex + 1) * UVPercentage)));

			verts.push_back(BLVert);
			verts.push_back(BRVert);
			verts.push_back(TRVert);

			verts.push_back(BLVert);
			verts.push_back(TRVert);
			verts.push_back(TLVert);

			currentLatitude += singleStepDegreesLati;
		}

		currentLongitude += singleStepDegreesLong;
	}

}

void AddVertsForOBB3D(std::vector<Vertex_PCU>& verts, OBB3 const& obb, const Rgba8& color /*= Rgba8::WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	verts.reserve(36);

	Vec3 box[] =
	{
		obb.GetPointAt(Vec3(0.0f,0.0f,0.0f)),
		obb.GetPointAt(Vec3(1.0f,0.0f,0.0f)),
		obb.GetPointAt(Vec3(1.0f,0.0f,1.0f)),
		obb.GetPointAt(Vec3(0.0f,0.0f,1.0f)),
		obb.GetPointAt(Vec3(0.0f,1.0f,0.0f)),
		obb.GetPointAt(Vec3(1.0f,1.0f,0.0f)),
		obb.GetPointAt(Vec3(1.0f,1.0f,1.0f)),
		obb.GetPointAt(Vec3(0.0f,1.0f,1.0f))
	};

	AddVertsForQuad3D(verts, box[0], box[1], box[2], box[3], color, UVs);
	AddVertsForQuad3D(verts, box[4], box[0], box[3], box[7], color, UVs);
	AddVertsForQuad3D(verts, box[5], box[4], box[7], box[6], color, UVs);
	AddVertsForQuad3D(verts, box[1], box[5], box[6], box[2], color, UVs);
	AddVertsForQuad3D(verts, box[7], box[3], box[2], box[6], color, UVs);
	AddVertsForQuad3D(verts, box[5], box[1], box[0], box[4], color, UVs);

}

void TransformVertexArray3D(std::vector<Vertex_PCU>& verts, const Mat44& transform)
{
	for (int i = 0; i < (int)verts.size(); i++)
	{
		verts[i].m_position = transform.TransformPosition3D(verts[i].m_position);
	}
}

AABB2 GetVertexBounds2D(const std::vector<Vertex_PCU>& verts)
{
	float minX = FLT_MAX;
	float minY = FLT_MAX;
	float maxX = -FLT_MAX;
	float maxY = -FLT_MAX;

	for (int i = 0; i < (int)verts.size(); i++)
	{
		if (verts[i].m_position.x < minX)
		{
			minX = verts[i].m_position.x;
		}
		if (verts[i].m_position.x > maxX)
		{
			maxX = verts[i].m_position.x;
		}
		if (verts[i].m_position.y < minY)
		{
			minY = verts[i].m_position.y;
		}
		if (verts[i].m_position.y > maxY)
		{
			maxY = verts[i].m_position.y;
		}
	}

	return AABB2(minX, minY, maxX, maxY);
}

void AddVertsForCylinder3D(std::vector<Vertex_PCU>& verts, const Vec3& start, const Vec3& end, float radius, const Rgba8& color, const AABB2& UVs, int numSlices)
{
	std::vector<Vertex_PCU> localVerts;
	localVerts.reserve(1000);
	Vec3 worldZBasis = Vec3(0.0f, 0.0f, 1.0f);
	Vec3 worldYBasis = Vec3(0.0f, 1.0f, 0.0f);
	
	Vec3 cylinderIBasis = (end - start).GetNormalized();
	Vec3 cylinderJBasis;
	Vec3 cylinderKBasis;

	if (fabsf(DotProduct3D(worldZBasis, cylinderIBasis)) < 0.999f)
	{
		cylinderJBasis = CrossProduct3D(worldZBasis, cylinderIBasis);
		cylinderKBasis = CrossProduct3D(cylinderIBasis, cylinderJBasis);
	}
	else 
	{
		cylinderKBasis = CrossProduct3D(cylinderIBasis, worldYBasis);
		cylinderJBasis = CrossProduct3D(cylinderKBasis, cylinderIBasis);
	}

	Mat44 transformMatrix = Mat44();
	transformMatrix.SetIJKT3D(cylinderIBasis, cylinderJBasis, cylinderKBasis, start);

	float SINGLE_UV_STEP = 1.0f / numSlices;
	float SINGLE_ANGLE_STEP = 360.0f * SINGLE_UV_STEP;
	float currentAngle = 0.0f;
	float currentUVForSideView = 0.0f;

	Vec2 rightDirectionStandard = Vec2(0.0f, 0.5f);
	Vec2 rightDirectionForUV = Vec2(0.0f, 0.5f);

	Vec3 rightDirectionLocal = Vec3(0.0f, -1.0f, 0.0f) * radius;
	Vec3 upDiretionLocal = Vec3(1.0f, 0.0f, 0.0f) * (end - start).GetLength();
	Vec3 startCenterLocal = Vec3(0.0f, 0.0f, 0.0f);
	Vec3 endCenterLocal = startCenterLocal + upDiretionLocal;

	Vertex_PCU startCenterLocalPCU = Vertex_PCU(startCenterLocal, color, UVs.GetPointAtUV(Vec2(0.5f, 0.5f)));
	Vertex_PCU endCenterLocalPCU = Vertex_PCU(endCenterLocal, color, UVs.GetPointAtUV(Vec2(0.5f, 0.5f)));

	Vec3 startLowPoint;
	Vec3 startHighPoint;
	Vec3 endLowPoint;
	Vec3 endHighPoint;

	Vertex_PCU PCU_startLowPointForBottom;
	Vertex_PCU PCU_startHighPointForBottom;
	Vertex_PCU PCU_endLowPointForTop;
	Vertex_PCU PCU_endHighPointForTop;

	Vertex_PCU PCU_startLowPointForSideView;
	Vertex_PCU PCU_startHighPointForSideView;
	Vertex_PCU PCU_endLowPointForSideView;
	Vertex_PCU PCU_endHighPointForSideView;

	for (int i = 0; i < numSlices; i++)
	{
		Mat44 rotationAroundXMatrix;
		Mat44 rotationAroundWorldZForUV;

		rotationAroundXMatrix.AppendXRotation(currentAngle);
		rotationAroundWorldZForUV.AppendZRotation(currentAngle);

		startLowPoint = rotationAroundXMatrix.TransformPosition3D(rightDirectionLocal);
		endLowPoint = startLowPoint + upDiretionLocal;
		rightDirectionForUV = rotationAroundWorldZForUV.TransformPosition2D(rightDirectionStandard);

		PCU_startLowPointForBottom = Vertex_PCU(startLowPoint, color, UVs.GetPointAtUV(Vec2(0.5f, 0.5f) + rightDirectionForUV));
		PCU_startLowPointForSideView = Vertex_PCU(startLowPoint, color, UVs.GetPointAtUV(Vec2(currentUVForSideView, 0.0f)));
		PCU_endLowPointForSideView = Vertex_PCU(endLowPoint, color, UVs.GetPointAtUV(Vec2(currentUVForSideView, 1.0f)));
		PCU_endLowPointForTop = Vertex_PCU(endLowPoint, color, UVs.GetPointAtUV(Vec2(0.5f, 0.5f) + rightDirectionForUV));


		rotationAroundXMatrix.AppendXRotation(SINGLE_ANGLE_STEP);
		rotationAroundWorldZForUV.AppendZRotation(SINGLE_ANGLE_STEP);

		startHighPoint = rotationAroundXMatrix.TransformPosition3D(rightDirectionLocal);
		rightDirectionForUV = rotationAroundWorldZForUV.TransformVectorQuantity2D(rightDirectionStandard);
		endHighPoint = startHighPoint +upDiretionLocal;

		PCU_startHighPointForBottom = Vertex_PCU(startHighPoint, color,  UVs.GetPointAtUV(Vec2(0.5f, 0.5f) + rightDirectionForUV));
		PCU_startHighPointForSideView = Vertex_PCU(startHighPoint, color, UVs.GetPointAtUV(Vec2(currentUVForSideView + SINGLE_UV_STEP, 0.0f)));	
		PCU_endHighPointForSideView = Vertex_PCU(endHighPoint, color,  UVs.GetPointAtUV(Vec2(currentUVForSideView + SINGLE_UV_STEP, 1.0f)));
		PCU_endHighPointForTop = Vertex_PCU(endHighPoint, color,  UVs.GetPointAtUV(Vec2(0.5f, 0.5f) + rightDirectionForUV));

		localVerts.push_back(startCenterLocalPCU);
		localVerts.push_back(PCU_startHighPointForBottom);
		localVerts.push_back(PCU_startLowPointForBottom);
		
		localVerts.push_back(PCU_startHighPointForSideView);
		localVerts.push_back(PCU_endLowPointForSideView);
		localVerts.push_back(PCU_startLowPointForSideView);
		
		localVerts.push_back(PCU_startHighPointForSideView);
		localVerts.push_back(PCU_endHighPointForSideView);
		localVerts.push_back(PCU_endLowPointForSideView);
		
		localVerts.push_back(endCenterLocalPCU);
		localVerts.push_back(PCU_endLowPointForTop);
		localVerts.push_back(PCU_endHighPointForTop);

		currentAngle+=SINGLE_ANGLE_STEP;
		currentUVForSideView += SINGLE_UV_STEP;
	}

	TransformVertexArray3D(localVerts, transformMatrix);

	for (Vertex_PCU vert : localVerts)
	{
		verts.push_back(vert);
	}
}


void AddVertsForCone3D(std::vector<Vertex_PCU>& verts, const Vec3& start, const Vec3& end, float radius, const Rgba8& color, const AABB2& UVs, int numSlices)
{
	verts.reserve(1000);
	Vec3 worldZBasis = Vec3(0.0f, 0.0f, 1.0f);
	Vec3 worldYBasis = Vec3(0.0f, 1.0f, 0.0f);

	Vec3 cylinderIBasis = (end - start).GetNormalized();
	Vec3 cylinderJBasis;
	Vec3 cylinderKBasis;

	if (fabsf(DotProduct3D(worldZBasis, cylinderIBasis)) < 0.999f)
	{
		cylinderJBasis = CrossProduct3D(worldZBasis, cylinderIBasis);
		cylinderKBasis = CrossProduct3D(cylinderIBasis, cylinderJBasis);
	}
	else
	{
		cylinderKBasis = CrossProduct3D(cylinderIBasis, worldYBasis);
		cylinderJBasis = CrossProduct3D(cylinderKBasis, cylinderIBasis);
	}

	Mat44 transformMatrix = Mat44();
	transformMatrix.SetIJKT3D(cylinderIBasis, cylinderJBasis, cylinderKBasis, start);

	float SINGLE_UV_STEP = 1.0f / numSlices;
	float SINGLE_ANGLE_STEP = 360.0f * SINGLE_UV_STEP;
	float currentAngle = 0.0f;
	float currentUVForSideView = 0.0f;

	Vec2 rightDirectionStandard = Vec2(0.0f, 0.5f);
	Vec2 rightDirectionForUV = Vec2(0.0f, 0.5f);

	Vec3 rightDirectionLocal = Vec3(0.0f, -1.0f, 0.0f) * radius;
	Vec3 upDiretionLocal = Vec3(1.0f, 0.0f, 0.0f) * (end - start).GetLength();
	Vec3 startCenterLocal = Vec3(0.0f, 0.0f, 0.0f);
	Vec3 endCenterLocal = startCenterLocal + upDiretionLocal;

	Vertex_PCU startCenterLocalPCU = Vertex_PCU(startCenterLocal, color, UVs.GetPointAtUV(Vec2(0.5f, 0.5f)));
	Vertex_PCU endCenterLocalPCU = Vertex_PCU(endCenterLocal, color, UVs.GetPointAtUV(Vec2(0.5f, 0.5f)));

	Vec3 startLowPoint;
	Vec3 startHighPoint;

	Vertex_PCU PCU_startLowPointForBottom;
	Vertex_PCU PCU_startHighPointForBottom;

	Vertex_PCU PCU_startLowPointForSideView;
	Vertex_PCU PCU_startHighPointForSideView;
	for (int i = 0; i < numSlices; i++)
	{
		Mat44 rotationAroundXMatrix;
		Mat44 rotationAroundWorldZForUV;

		rotationAroundXMatrix.AppendXRotation(currentAngle);
		rotationAroundWorldZForUV.AppendZRotation(currentAngle);

		startLowPoint = rotationAroundXMatrix.TransformPosition3D(rightDirectionLocal);
		rightDirectionForUV = rotationAroundWorldZForUV.TransformPosition2D(rightDirectionStandard);

		PCU_startLowPointForBottom = Vertex_PCU(startLowPoint, color, UVs.GetPointAtUV(Vec2(0.5f, 0.5f) + rightDirectionForUV));
		PCU_startLowPointForSideView = Vertex_PCU(startLowPoint, color, UVs.GetPointAtUV(Vec2(currentUVForSideView, 0.0f)));


		rotationAroundXMatrix.AppendXRotation(SINGLE_ANGLE_STEP);
		rotationAroundWorldZForUV.AppendZRotation(SINGLE_ANGLE_STEP);

		startHighPoint = rotationAroundXMatrix.TransformPosition3D(rightDirectionLocal);
		rightDirectionForUV = rotationAroundWorldZForUV.TransformVectorQuantity2D(rightDirectionStandard);

		PCU_startHighPointForBottom = Vertex_PCU(startHighPoint, color, UVs.GetPointAtUV(Vec2(0.5f, 0.5f) + rightDirectionForUV));
		PCU_startHighPointForSideView = Vertex_PCU(startHighPoint, color, UVs.GetPointAtUV(Vec2(currentUVForSideView + SINGLE_UV_STEP, 0.0f)));

		verts.push_back(startCenterLocalPCU);
		verts.push_back(PCU_startHighPointForBottom);
		verts.push_back(PCU_startLowPointForBottom);


		verts.push_back(PCU_startHighPointForSideView);
		verts.push_back(endCenterLocalPCU);
		verts.push_back(PCU_startLowPointForSideView);

		currentAngle += SINGLE_ANGLE_STEP;
		currentUVForSideView += SINGLE_UV_STEP;
	}

	TransformVertexArray3D(verts, transformMatrix);

}

void AddVertsForArrow3D(std::vector<Vertex_PCU>& verts, const Vec3& start, const Vec3& end, float radius, float arrowAspect /*= 0.2f*/, const Rgba8& color /*= Rgba8::WHITE*/, const Rgba8& coneColor, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/, int numSlices /*= 8*/)
{
	UNUSED(UVs);

	verts.reserve(1000);
	Vec3 worldZBasis = Vec3(0.0f, 0.0f, 1.0f);
	Vec3 worldYBasis = Vec3(0.0f, 1.0f, 0.0f);

	Vec3 cylinderIBasis = (end - start).GetNormalized();
	Vec3 cylinderJBasis;
	Vec3 cylinderKBasis;

	if (fabsf(DotProduct3D(worldZBasis, cylinderIBasis)) < 0.999f)
	{
		cylinderJBasis = CrossProduct3D(worldZBasis, cylinderIBasis);
		cylinderKBasis = CrossProduct3D(cylinderIBasis, cylinderJBasis);
	}
	else
	{
		cylinderKBasis = CrossProduct3D(cylinderIBasis, worldYBasis);
		cylinderJBasis = CrossProduct3D(cylinderKBasis, cylinderIBasis);
	}

	Mat44 transformMatrix = Mat44();
	transformMatrix.SetIJKT3D(cylinderIBasis, cylinderJBasis, cylinderKBasis, start);

	float SINGLE_ANGLE_STEP = 360.0f / numSlices;
	float currentAngle = 0.0f;

	Vec3 rightDirectionLocal = Vec3(0.0f, -1.0f, 0.0f) * radius;
	Vec3 rightConeDirectionLocal = Vec3(0.0f, -1.0f, 0.0f) * radius * 2.0f;
	Vec3 upDiretionLocal = Vec3(1.0f, 0.0f, 0.0f) * (end - start).GetLength();
	Vec3 startCenterLocal = Vec3(0.0f, 0.0f, 0.0f);
	Vec3 endCenterLocal = startCenterLocal + upDiretionLocal;

	Vertex_PCU startCenterLocalPCU = Vertex_PCU(startCenterLocal, color, Vec2(1.0f, 1.0f));
	Vertex_PCU endCenterLocalPCU = Vertex_PCU(endCenterLocal, coneColor, Vec2(1.0f, 0.0f));

	Vec3 startLowPoint;
	Vec3 startHighPoint;
	Vec3 endLowPoint;
	Vec3 endHighPoint;

	Vec3 endLowPointLongRadius;
	Vec3 endHightPointLongRadius;
	Vec3 coneHighPoint;

	Vertex_PCU PCU_startLowPoint;
	Vertex_PCU PCU_startHighPoint;
	Vertex_PCU PCU_endLowPoint;
	Vertex_PCU PCU_endHighPoint;

	Vertex_PCU PCU_endLowPointLongRadius;
	Vertex_PCU PCU_endHightPointLongRadius;
	Vertex_PCU PCU_coneHighPoint;

	for (int i = 0; i < numSlices; i++)
	{
		Mat44 rotationAroundXMatrix;
		rotationAroundXMatrix.AppendXRotation(currentAngle);
		startLowPoint = rotationAroundXMatrix.TransformPosition3D(rightDirectionLocal);
		Vec3 startLowPointLongRadius = rotationAroundXMatrix.TransformPosition3D(rightConeDirectionLocal);
		PCU_startLowPoint = Vertex_PCU(startLowPoint, color, Vec2(0.0f, 0.0f));

		rotationAroundXMatrix.AppendXRotation(SINGLE_ANGLE_STEP);
		startHighPoint = rotationAroundXMatrix.TransformPosition3D(rightDirectionLocal);
		Vec3 startHighPointLongRadius = rotationAroundXMatrix.TransformPosition3D(rightConeDirectionLocal);
		PCU_startHighPoint = Vertex_PCU(startHighPoint, color, Vec2(1.0f, 0.0f));

		verts.push_back(startCenterLocalPCU);
		verts.push_back(PCU_startHighPoint);
		verts.push_back(PCU_startLowPoint);

		endLowPoint = startLowPoint + upDiretionLocal;
		PCU_endLowPoint = Vertex_PCU(endLowPoint, color, Vec2(0.0f, 1.0f));
		endHighPoint = startHighPoint + upDiretionLocal;
		PCU_endHighPoint = Vertex_PCU(endHighPoint, color, Vec2(1.0f, 1.0f));

		endLowPointLongRadius = startLowPointLongRadius + upDiretionLocal;
		PCU_endLowPointLongRadius = Vertex_PCU(endLowPointLongRadius, coneColor, Vec2(0.0f, 1.0f));
		endHightPointLongRadius = startHighPointLongRadius + upDiretionLocal;
		PCU_endHightPointLongRadius = Vertex_PCU(endHightPointLongRadius, coneColor, Vec2(1.0f, 1.0f));

		coneHighPoint = endCenterLocal + upDiretionLocal * arrowAspect;
		PCU_coneHighPoint = Vertex_PCU(coneHighPoint, coneColor, Vec2(1.0f, 1.0f));

		verts.push_back(PCU_startHighPoint);
		verts.push_back(PCU_endLowPoint);
		verts.push_back(PCU_startLowPoint);

		verts.push_back(PCU_startHighPoint);
		verts.push_back(PCU_endHighPoint);
		verts.push_back(PCU_endLowPoint);

		verts.push_back(endCenterLocalPCU);
		verts.push_back(PCU_endLowPoint);
		verts.push_back(PCU_endHighPoint);

		//verts.push_back(endCenterLocalPCU);
		//verts.push_back(PCU_endHightPointLongRadius);
		//verts.push_back(PCU_endLowPointLongRadius);

		// Shadow for base of the cone in the arrow
		Vertex_PCU endCenterLocalPCUShadow = Vertex_PCU(endCenterLocal, coneColor * 0.8f, Vec2(1.0f, 0.0f));
		Vertex_PCU endLowPointPCUShadow = Vertex_PCU(endLowPointLongRadius, coneColor * 0.8f, Vec2(0.0f, 1.0f));
		Vertex_PCU endHighPointPCUShadow = Vertex_PCU(endHightPointLongRadius, coneColor * 0.8f, Vec2(0.0f, 1.0f));

		verts.push_back(endCenterLocalPCUShadow);
		verts.push_back(endHighPointPCUShadow);
		verts.push_back(endLowPointPCUShadow);

		verts.push_back(PCU_endHightPointLongRadius);
		verts.push_back(PCU_coneHighPoint);
		verts.push_back(PCU_endLowPointLongRadius);

		currentAngle += SINGLE_ANGLE_STEP;
	}

	TransformVertexArray3D(verts, transformMatrix);

}

void TransformVertexArrayXY3D(int numVerts, Vertex_PCU* verts, float uniformScaleXY, float rotationDegreesAboutZ, Vec2 const& translationXY)
{
	for (int vertIndex = 0; vertIndex < numVerts; vertIndex++)
	{
		Vec2 vertPositionXY(verts[vertIndex].m_position.x, verts[vertIndex].m_position.y);
		float tmp_z = verts[vertIndex].m_position.z;
		float tmp_length = vertPositionXY.GetLength();


		vertPositionXY.SetLength(uniformScaleXY * tmp_length);				// Set scale of the vector
		vertPositionXY.RotateDegrees(rotationDegreesAboutZ);				// Rotate vector around the Z axis
		vertPositionXY += translationXY;									// Translate vector position


		verts[vertIndex].m_position.x = vertPositionXY.x;
		verts[vertIndex].m_position.y = vertPositionXY.y;
		verts[vertIndex].m_position.z = tmp_z;
	}
}

