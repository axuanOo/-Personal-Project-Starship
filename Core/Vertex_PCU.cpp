#include "Engine/Core/Vertex_PCU.hpp"


Vertex_PCU::Vertex_PCU(float x, float y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	:m_position(x, y, 0),
	m_color(r, g, b, a),
	m_uvTexCoords(0.f, 0.f)
{

}

Vertex_PCU::Vertex_PCU(Vec3 const& position, Rgba8 const& color, Vec2 const& uvTexCoords) 
	:m_position(position.x,position.y,position.z),
	m_color(color.r,color.g,color.b,color.a),
	m_uvTexCoords(uvTexCoords.x,uvTexCoords.y)
{

}

Vertex_PCU::Vertex_PCU()
{

}

Vertex_PCU::Vertex_PCU(Vec2 const& position, Rgba8 const& color, Vec2 const& uvTexCoords):
	m_color(color.r, color.g, color.b, color.a),
	m_uvTexCoords(uvTexCoords.x, uvTexCoords.y)
{
	m_position = Vec3(position.x, position.y, 0.0f);
}
