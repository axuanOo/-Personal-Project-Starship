#pragma once
#include "Engine/Core/EngineCommon.hpp"
class TileHeatMap
{
public:
	TileHeatMap(IntVec2 const& dimensions);
	~TileHeatMap();

	float		GetHeatValueByIndex(int index) const;

	void		ResetAllValues();
	void		SetAllValues(float value);
	void		SetValueByIndex(int index, float value);
	void		AddValueByIndex(int index,float valueToAdd);
	int			GetIndexByPosition(Vec2 const& position,float tileSize) const;
	float		GetHighestNonSpecialValue(float specialValue) const;
	int			GetValuesSize() const;

	void		AddVertsForDebugDraw( std::vector<Vertex_PCU>& verts, AABB2 bounds, FloatRange valueRange=FloatRange(0.f,1.f), Rgba8 lowColor=Rgba8(0,0,0,100), Rgba8 highColor=Rgba8(255,255,255,100), float specialValue=999999.f, Rgba8 specialColor=Rgba8(0,0,255) );
private:
	IntVec2				m_dimensions;
	std::vector<float>	m_values;
};