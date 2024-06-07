#include "Engine/Math/IntRange.hpp"

const IntRange IntRange::ZERO = IntRange(0, 0);
const IntRange IntRange::ONE = IntRange(1, 1);
const IntRange IntRange::ZERO_TO_ONE = IntRange(0, 1);

IntRange::IntRange()
	:m_min(0)
	,m_max(0)
{

}

IntRange::IntRange(int minValue, int maxValue)
	:m_min(minValue)
	,m_max(maxValue)
{

}

IntRange::~IntRange()
{

}


bool const IntRange::IsOnRange(int value) const
{
	return value >= m_min && value <= m_max;
}

bool const IntRange::IsOverlappingWith(const IntRange& rangeValue) const
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

void IntRange::operator=(const IntRange& compare)
{
	m_min = compare.m_min;
	m_max = compare.m_max;
}

bool IntRange::operator!=(const IntRange& compare) const
{
	return !(m_min == compare.m_min && m_max == compare.m_max);
}

bool IntRange::operator==(const IntRange& compare) const
{
	return (m_min == compare.m_min && m_max == compare.m_max);
}
