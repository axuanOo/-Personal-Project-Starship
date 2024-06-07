#include "Engine/Input/AnalogJoystick.hpp"


Vec2 AnalogJoystick::GetPosition() const
{
	return m_correctedPosition ;
}

float AnalogJoystick::GetMagnitude() const
{
	return m_correctedPosition.GetLength();
}

float AnalogJoystick::GetOrientationDegrees() const
{
	return m_correctedPosition.GetOrientationDegrees();
}

Vec2 AnalogJoystick::GetRawUncorrectedPosition() const
{
	return m_rawPosition;
}

float AnalogJoystick::GetInnerDeadZoneFraction() const
{
	return m_innerDeadZoneFraction;
}

float AnalogJoystick::GetOuterDeadZoneFraction() const
{
	return m_outerDeadZoneFraction;
}

void AnalogJoystick::Reset()
{
	m_rawPosition = Vec2(0.f,0.f);
	m_correctedPosition = Vec2(0.f, 0.f);
	m_innerDeadZoneFraction = 0.00f;
	m_outerDeadZoneFraction = 1.00f;
}

void AnalogJoystick::SetDeadZoneThresholds(float normalizedInnerDeadZoneThreshold, float normalizedOuterDeadZoneThreshold)
{
	m_innerDeadZoneFraction = normalizedInnerDeadZoneThreshold;
	m_outerDeadZoneFraction = normalizedOuterDeadZoneThreshold;
}

void AnalogJoystick::UpdatePosition(float rawNormalizedX, float rawNormalizedY)
{
	m_rawPosition = Vec2(rawNormalizedX, rawNormalizedY);

	float x = m_rawPosition.x;
	float y = m_rawPosition.y;

	if (m_rawPosition.GetLength() < m_innerDeadZoneFraction)
	{
		x = 0.f;
		y = 0.f;
	}

	if (m_rawPosition.GetLength() > m_outerDeadZoneFraction)
	{
		x = m_rawPosition.GetNormalized().x;
		y = m_rawPosition.GetNormalized().y;
	}

	m_correctedPosition = Vec2(x, y);
}
