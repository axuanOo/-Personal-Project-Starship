#include "Engine/Math/FloatRange.hpp"

const FloatRange ZERO = FloatRange(0.0f,0.0f);
const FloatRange ONE = FloatRange(1.0f,1.0f);
const FloatRange ZERO_TO_ONE = FloatRange(0.0f,1.0f);

FloatRange::FloatRange()
	:m_min(0)
	,m_max(0)
{

}

FloatRange::FloatRange(float minValue, float maxValue)
{
	if (minValue<=maxValue)
	{
		m_min = minValue;
		m_max = maxValue;
	}
	else 
	{
		m_min = maxValue;
		m_max = minValue;
	}
}

FloatRange::~FloatRange()
{

}


bool const FloatRange::IsOnRange(float value) const
{
	return value >= m_min && value <= m_max;
}

bool const FloatRange::IsOverlappingWith(const FloatRange& rangeValue) const
{
	if (rangeValue.m_min > m_min && rangeValue.m_min < m_max)
	{
		return true;
	}

	if (rangeValue.m_max > m_min && rangeValue.m_max < m_max)
	{
		return true;
	}

	if (rangeValue.m_min< m_min && rangeValue.m_max> m_max)
	{
		return true;
	}

	return false;
}

bool const FloatRange::IsInsideRange(const FloatRange& rangeValue) const
{
	return m_min<rangeValue.m_min&& m_max>rangeValue.m_max;
}

void FloatRange::operator=(const FloatRange& compare)
{
	m_min = compare.m_min;
	m_max = compare.m_max;
}

bool FloatRange::operator!=(const FloatRange& compare) const
{
	return !(m_min == compare.m_min && m_max == compare.m_max);
}

bool FloatRange::operator==(const FloatRange& compare) const
{
	return (m_min == compare.m_min && m_max == compare.m_max);
}
