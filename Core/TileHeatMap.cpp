#include "Engine/Core/TileHeatMap.hpp"

TileHeatMap::TileHeatMap(IntVec2 const& dimensions)
{
	m_dimensions = dimensions;
	int valueSize = (int)(dimensions.x * dimensions.y);
	m_values.reserve(static_cast<size_t>(valueSize));
	m_values.assign(valueSize, 0.0f);
}

TileHeatMap::~TileHeatMap()
{

}

float TileHeatMap::GetHeatValueByIndex(int index) const
{
	return m_values[index];
}

void TileHeatMap::ResetAllValues()
{
	for (auto& element : m_values)
	{
		element = 0.0f;
	}
}

void TileHeatMap::SetAllValues(float value)
{
	for (auto& element : m_values)
	{
		element = value;
	}
}

void TileHeatMap::SetValueByIndex(int index, float value)
{
	m_values[index] = value;
}

void TileHeatMap::AddValueByIndex(int index, float valueToAdd)
{
	m_values[index] += valueToAdd;
}

int TileHeatMap::GetIndexByPosition(Vec2 const& position, float tileSize) const
{
	int tilePosX = static_cast<int>(RoundDownToInt(position.x) / tileSize);
	int tilePosY = static_cast<int>(RoundDownToInt(position.y) / tileSize);
	return m_dimensions.y * tilePosX + (tilePosY);
}

float TileHeatMap::GetHighestNonSpecialValue(float specialValue) const
{
	float highestValue = 0.0f;

	for (int valueIndex = 1; valueIndex < m_values.size(); ++valueIndex)
	{
		if (m_values[valueIndex] == specialValue)
		{
			continue;
		}

		if (m_values[valueIndex] > highestValue)
		{
			highestValue = m_values[valueIndex];
		}
	}

	return highestValue;
}

int TileHeatMap::GetValuesSize() const
{
	return (int)m_values.size();
}

void TileHeatMap::AddVertsForDebugDraw(std::vector<Vertex_PCU>& verts, AABB2 bounds, FloatRange valueRange, Rgba8 lowColor, Rgba8 highColor, float specialValue, Rgba8 specialColor)
{
	int vertsNum = m_dimensions.x * m_dimensions.y * 6;
	verts.reserve(vertsNum);

	Rgba8 color = Rgba8();

	Vec2 leftBottomUV = Vec2(0.0f,0.0f);
	Vec2 rightBottomUV = Vec2(1.0f,0.0f);
	Vec2 leftTopUV = Vec2(0.0f,1.0f);
	Vec2 rightTopUV = Vec2(1.0f,1.0f);

	Vec3 leftBottomPointPosition = Vec3(bounds.GetPointAtUV(Vec2(0.0f, 0.0f)).x, bounds.GetPointAtUV(Vec2(0.0f, 0.0f)).y, 0.0f);
	Vec3 rightBottomPointPosition = Vec3(bounds.GetPointAtUV(Vec2(1.0f, 0.0f)).x, bounds.GetPointAtUV(Vec2(1.0f, 0.0f)).y, 0.0f);
	Vec3 leftTopPointPosition = Vec3(bounds.GetPointAtUV(Vec2(0.0f, 1.0f)).x, bounds.GetPointAtUV(Vec2(0.0f, 1.0f)).y, 0.0f);
	Vec3 rightTopPointPosition = Vec3(bounds.GetPointAtUV(Vec2(1.0f, 1.0f)).x, bounds.GetPointAtUV(Vec2(1.0f, 1.0f)).y, 0.0f);

	int tileIndex = GetIndexByPosition(bounds.GetCenter(), bounds.GetDimensions().x);
	float value = m_values[tileIndex];

	if (value != specialValue)
	{
		value = RangeMapClamped(value, valueRange.m_min, valueRange.m_max, 0.0f, 1.0f);
		color = color.Interpolate(lowColor, highColor, value);
	}
	else 
	{
		color = specialColor;
	}

	Vertex_PCU LBVert = Vertex_PCU(leftBottomPointPosition, color, leftBottomUV);
	Vertex_PCU RBVert = Vertex_PCU(rightBottomPointPosition, color, rightBottomUV);
	Vertex_PCU LTVert = Vertex_PCU(leftTopPointPosition, color, leftTopUV);
	Vertex_PCU RTVert = Vertex_PCU(rightTopPointPosition, color, rightTopUV);

	verts.push_back(LBVert);
	verts.push_back(RBVert);
	verts.push_back(LTVert);

	verts.push_back(LTVert);
	verts.push_back(RTVert);
	verts.push_back(RBVert);
}
