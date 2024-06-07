#pragma once
#include "Engine//Math/Vec3.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Core/Rgba8.hpp"

struct Vertex_PCU
{

public:
	Vertex_PCU();
	Vec3   m_position;
	Rgba8  m_color;
	Vec2   m_uvTexCoords;

public:

	Vertex_PCU(float x, float y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);		// default constructor (do nothing)
	explicit Vertex_PCU(Vec3 const& position, Rgba8 const& color, Vec2 const& uvTexCoords);					// explicit constructor

	explicit Vertex_PCU(Vec2 const& position, Rgba8 const& color, Vec2 const& uvTexCoords);					// explicit constructor

}; 

struct Vertex_PCUTBN 
{
public:
	Vertex_PCUTBN() = default;
	Vertex_PCUTBN(Vec3 const& position, Rgba8 const& color, Vec2 const& uvTexCoords,
		Vec3 const& tangent, Vec3 const& bitangent, Vec3 const& normal)
		:m_position(position),
		m_color(color),
		m_uvTexCoords(uvTexCoords),
		m_tangent(tangent),
		m_bitangent(bitangent),
		m_normal(normal)
	{};

	Vertex_PCUTBN(float px, float py, float pz,
		unsigned char r, unsigned char g, unsigned char b, unsigned char a,
		float u, float v,
		float tx, float ty, float tz,
		float bx, float by, float bz,
		float nx, float ny, float nz)
		: m_position(px, py, pz)
		, m_color(r, g, b, a)
		, m_uvTexCoords(u, v)
		, m_tangent(tx, ty, tz)
		, m_bitangent(bx, by, bz)
		, m_normal(nx, ny, nz)
	{};

	Vec3    m_position;
	Rgba8   m_color;
	Vec2    m_uvTexCoords;

	Vec3	m_tangent;
	Vec3	m_bitangent;
	Vec3	m_normal;

};
