#include "Game/GameCommon.hpp"
#include "Game/App.hpp"

Window*			g_theWindow = nullptr;
Renderer*		g_theRenderer = nullptr;
InputSystem*	g_theInput = nullptr;
AudioSystem*	g_theAudio = nullptr;
Camera*			g_theCamera = nullptr;
App*			g_theApp = nullptr;


void DebugDrawRing(Vec2 const& Center, float radius, float thickness, Rgba8 const& color)
{
	float halfThickness = 0.5f * thickness;
	float innerRadius = radius - halfThickness;
	float outerRadius = radius + halfThickness;
	Vec3 center = Vec3(Center.x, Center.y, 0.f);

	constexpr int NUM_SIDES = 32;
	constexpr int NUM_TRIS = NUM_SIDES * 2;
	constexpr int NUM_VERTS = NUM_TRIS * 3;
	constexpr float DEGREES_PER_SIDE = 360.f / static_cast<float>(NUM_SIDES);

	Vertex_PCU verts[NUM_VERTS];
	for (int sideNum = 0; sideNum < NUM_SIDES; sideNum++)
	{
		float startDegrees = DEGREES_PER_SIDE * static_cast<float>(sideNum);
		float endDegrees = DEGREES_PER_SIDE * static_cast<float>(sideNum+1);

		float cosStart = CosDegrees(startDegrees);
		float sinStart = SinDegrees(startDegrees);
		float cosEnd = CosDegrees(endDegrees);
		float sinEnd = SinDegrees(endDegrees);

		Vec3 innerStartPos = Vec3(innerRadius * cosStart, innerRadius * sinStart, 0.f);

		Vec3 outerStartPos = Vec3(outerRadius * cosStart, outerRadius * sinStart, 0.f);

		Vec3 innerEndPos = Vec3(innerRadius * cosEnd, innerRadius * sinEnd, 0.f);

		Vec3 outerEndPos = Vec3(outerRadius * cosEnd, outerRadius * sinEnd, 0.f);
			

		int vertIndexA = (6 * sideNum) + 0;
		int vertIndexB = (6 * sideNum) + 1;
		int vertIndexC = (6 * sideNum) + 2;
		int vertIndexD = (6 * sideNum) + 3;
		int vertIndexE = (6 * sideNum) + 4;
		int vertIndexF = (6 * sideNum) + 5;

		verts[vertIndexA].m_position = center + innerEndPos;
		verts[vertIndexB].m_position = center + innerStartPos;
		verts[vertIndexC].m_position = center + outerStartPos;
		verts[vertIndexA].m_color = color;
		verts[vertIndexB].m_color = color;
		verts[vertIndexC].m_color = color;

		verts[vertIndexD].m_position = center + innerEndPos;
		verts[vertIndexE].m_position = center + outerStartPos;
		verts[vertIndexF].m_position = center + outerEndPos;
		verts[vertIndexD].m_color = color;
		verts[vertIndexE].m_color = color;
		verts[vertIndexF].m_color = color;
	}
	g_theRenderer->DrawVertexArray(NUM_VERTS, verts);
}

void DebugDrawLine(Vec2 const& startPos, Vec2 const& endPos, float thickness,Rgba8 const& color)
{
	Vec2 direction = endPos - startPos;
	direction.Normalized();

	float thick_r = thickness * 0.5f;

	Vec2 upDirection = direction;
	upDirection.Rotate90Degrees();

	Vec2 startUpperPos = startPos + upDirection * thick_r;
	Vec2 startLowerPos = startPos - upDirection * thick_r;
	Vec2 endUpperPos = endPos + upDirection * thick_r;
	Vec2 endLowerPos = endPos - upDirection * thick_r;

	Vertex_PCU linePoints[6]
	{
		Vertex_PCU(startUpperPos.x, startUpperPos.y,	color.r, color.g, color.b, 255),
		Vertex_PCU(startLowerPos.x, startLowerPos.y,	color.r, color.g, color.b, 255),
		Vertex_PCU(endUpperPos.x, endUpperPos.y,		color.r, color.g, color.b, 255),

		Vertex_PCU(endUpperPos.x, endUpperPos.y,		color.r, color.g, color.b, 255),
		Vertex_PCU(startLowerPos.x, startLowerPos.y,	color.r, color.g, color.b, 255),
		Vertex_PCU(endLowerPos.x, endLowerPos.y,		color.r, color.g, color.b, 255),

	};
	g_theRenderer->DrawVertexArray(6, linePoints);
}

void DebugDrawGlow(Vec2 const& Center, float radius, float thickness, Rgba8 const& color)
{
	float halfThickness = 0.5f * thickness;
	float innerRadius = radius - halfThickness;
	float outerRadius = radius + halfThickness;
	Vec3 center = Vec3(Center.x, Center.y, 0.f);

	constexpr int NUM_SIDES = 36;
	constexpr int NUM_TRIS = NUM_SIDES * 3;
	constexpr int NUM_VERTS = NUM_TRIS * 3;
	constexpr float DEGREES_PER_SIDE = 360.f / static_cast<float>(NUM_SIDES);

	Vertex_PCU verts[NUM_VERTS];
	for (int sideNum = 0; sideNum < NUM_SIDES; sideNum++)
	{
		float startDegrees = DEGREES_PER_SIDE * static_cast<float>(sideNum);
		float endDegrees = DEGREES_PER_SIDE * static_cast<float>(sideNum + 1);

		float cosStart = CosDegrees(startDegrees);
		float sinStart = SinDegrees(startDegrees);
		float cosEnd = CosDegrees(endDegrees);
		float sinEnd = SinDegrees(endDegrees);

		Vec3 innerStartPos = Vec3(innerRadius * cosStart, innerRadius * sinStart, 0.f);

		Vec3 outerStartPos = Vec3(outerRadius * cosStart, outerRadius * sinStart, 0.f);

		Vec3 innerEndPos = Vec3(innerRadius * cosEnd, innerRadius * sinEnd, 0.f);

		Vec3 outerEndPos = Vec3(outerRadius * cosEnd, outerRadius * sinEnd, 0.f);


		int vertIndexA = (9 * sideNum) + 0;
		int vertIndexB = (9 * sideNum) + 1;
		int vertIndexC = (9 * sideNum) + 2;
		int vertIndexD = (9 * sideNum) + 3;
		int vertIndexE = (9 * sideNum) + 4;
		int vertIndexF = (9 * sideNum) + 5;

		int vertIndexG = (9 * sideNum) + 6;
		int vertIndexH = (9 * sideNum) + 7;
		int vertIndexI = (9 * sideNum) + 8;
		
		verts[vertIndexA].m_position = center + innerEndPos;
		verts[vertIndexB].m_position = center + innerStartPos;
		verts[vertIndexC].m_position = center + outerStartPos;
		verts[vertIndexA].m_color = color;
		verts[vertIndexB].m_color = color;
		verts[vertIndexC].m_color = color;
		verts[vertIndexA].m_color.a = 30;
		verts[vertIndexB].m_color.a = 30;
		verts[vertIndexC].m_color.a = 10;

		verts[vertIndexD].m_position = center + innerEndPos;
		verts[vertIndexE].m_position = center + outerStartPos;
		verts[vertIndexF].m_position = center + outerEndPos;
		verts[vertIndexD].m_color = color;
		verts[vertIndexE].m_color = color;
		verts[vertIndexF].m_color = color;
		verts[vertIndexD].m_color.a = 30;
		verts[vertIndexE].m_color.a = 10;
		verts[vertIndexF].m_color.a = 10;

		verts[vertIndexG].m_position = center + innerEndPos;
		verts[vertIndexH].m_position = center + innerStartPos;
		verts[vertIndexI].m_position = center;
		verts[vertIndexG].m_color = color;
		verts[vertIndexH].m_color = color;
		verts[vertIndexI].m_color = color;
		verts[vertIndexG].m_color.a = 30;
		verts[vertIndexH].m_color.a = 30;
		verts[vertIndexI].m_color.a = 40;

	}
	g_theRenderer->DrawVertexArray(NUM_VERTS, verts);
}

