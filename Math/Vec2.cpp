#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

const Vec2 Vec2::ZERO = Vec2(0.0f, 0.0f);
const Vec2 Vec2::ONE = Vec2(1.0f, 1.0f);

//-----------------------------------------------------------------------------------------------
Vec2::Vec2(const Vec2& copy)
	: x(copy.x)
	, y(copy.y)
{
}


//-----------------------------------------------------------------------------------------------
Vec2::Vec2(float initialX, float initialY)
	: x(initialX)
	, y(initialY)
{
}


//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator + (const Vec2& vecToAdd) const
{
	return Vec2(x + vecToAdd.x, y + vecToAdd.y);
}


//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator-(const Vec2& vecToSubtract) const
{
	return Vec2(x - vecToSubtract.x, y - vecToSubtract.y);
}


//------------------------------------------------------------------------------------------------
const Vec2 Vec2::operator-() const
{
	return Vec2(-x, -y);
}


//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator*(float uniformScale) const
{
	return Vec2(x * uniformScale, y * uniformScale);
}


//------------------------------------------------------------------------------------------------
const Vec2 Vec2::operator*(const Vec2& vecToMultiply) const
{
	return Vec2(x * vecToMultiply.x, y * vecToMultiply.y);
}


//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator/(float inverseScale) const
{
	return Vec2(x / inverseScale, y / inverseScale);
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator+=(const Vec2& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator-=(const Vec2& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator*=(const float uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator/=(const float uniformDivisor)
{
	x /= uniformDivisor;
	y /= uniformDivisor;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator=(const Vec2& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}


//-----------------------------------------------------------------------------------------------
const Vec2 operator*(float uniformScale, const Vec2& vecToScale)
{
	return Vec2(vecToScale.x * uniformScale, vecToScale.y * uniformScale);
}


//-----------------------------------------------------------------------------------------------
bool Vec2::operator==(const Vec2& compare) const
{
	return x == compare.x && y == compare.y;
}


//-----------------------------------------------------------------------------------------------
bool Vec2::operator!=(const Vec2& compare) const
{
	return x != compare.x || y != compare.y;
}



Vec2 const Vec2::MakeFromPolarRadians(float orientationRadians, float length /*= 1.f*/)
{
	return Vec2(length * cosf(orientationRadians), length * sinf(orientationRadians));
}

Vec2 const Vec2::MakeFromPolarDegrees(float orientationDegrees, float length /*= 1.f*/)
{
	float orientationRadians = orientationDegrees * (g_PI / 180.f);
	return Vec2(length * cosf(orientationRadians), length * sinf(orientationRadians));
}


float Vec2::GetLength() const
{
	return sqrtf(x * x + y * y);
}

float Vec2::GetLengthSquared() const
{
	return (x * x + y * y);
}

float Vec2::GetOrientationRadians() const
{
	return atan2f(y, x);
}

float Vec2::GetOrientationDegrees() const
{
	return atan2f(y, x) * (180.f / g_PI);
}

Vec2 const Vec2::GetRotated90Degrees() const
{
	//float originalRadians =atan2f(y, x);
	//float vectorLength = this->GetLength();

	//float rotatedDegrees = (originalRadians * (180.f / g_PI)) + 90.f;
	//float rotatedRadians = rotatedDegrees * (g_PI / 180.f);
	//float cosRotatedTheta = cosf(rotatedRadians);
	//float sinRotatedTheta = sinf(rotatedRadians);
	//return Vec2(vectorLength * cosRotatedTheta, vectorLength * sinRotatedTheta);
	return Vec2(-y, x);
}

Vec2 const Vec2::GetRotatedMinus90Degrees() const
{
	float originalRadians = atan2f(y, x);
	float vectorLength = this->GetLength();

	float rotatedDegrees = (originalRadians * (180.f / g_PI)) - 90.f;
	float rotatedRadians = rotatedDegrees * (g_PI / 180.f);
	float cosRotatedTheta = cosf(rotatedRadians);
	float sinRotatedTheta = sinf(rotatedRadians);

	return Vec2(vectorLength * cosRotatedTheta, vectorLength * sinRotatedTheta);
}

Vec2 const Vec2::GetRotatedRadians(float delatRadians) const
{
	float originalRadians = atan2f(y, x);
	float vectorLength = this->GetLength();

	float rotatedRadians = originalRadians + delatRadians;
	float cosRotatedTheta = cosf(rotatedRadians);
	float sinRotatedTheta = sinf(rotatedRadians);

	return Vec2(vectorLength * cosRotatedTheta, vectorLength * sinRotatedTheta);
}

Vec2 const Vec2::GetRotatedDegrees(float delatDegrees) const
{
	float originalRadians = atan2f(y, x);
	float vectorLength = this->GetLength();

	float rotatedDegrees = (originalRadians * (180.f / g_PI)) + delatDegrees;
	float rotatedRadians = rotatedDegrees * (g_PI / 180.f);
	float cosRotatedTheta = cosf(rotatedRadians);
	float sinRotatedTheta = sinf(rotatedRadians);

	return Vec2(vectorLength * cosRotatedTheta, vectorLength * sinRotatedTheta);
}

Vec2 const Vec2::GetClamped(float maxLength) const
{
	float vectorLength = GetLength();

	if (vectorLength < maxLength)
	{
		return *this;
	}
	else
	{
		float scaleFactor = maxLength / vectorLength;
		return Vec2(x * scaleFactor, y * scaleFactor);
	}
}

Vec2 const Vec2::GetNormalized() const
{
	float vectorLength = GetLength();
	return Vec2(x / vectorLength, y / vectorLength);
}

Vec2 const Vec2::GetReflected(Vec2 const& impactVectorNormalized) const
{
	float length = GetProjectedLength2D(*this, impactVectorNormalized);
	Vec2 thisProjectOnPara = length*impactVectorNormalized;
	Vec2 reflectValue = *this - 2.0f * thisProjectOnPara;
	return reflectValue;
}

void Vec2::SetOrientationRadians(float newOrientationRadians)
{
	float vectorLength = GetLength();
	x = vectorLength * cosf(newOrientationRadians);
	y = vectorLength * sinf(newOrientationRadians);
}

void Vec2::SetOrientationDegrees(float newOrientationDegrees)
{
	float vectorLength = GetLength();
	float newOrientationRadians = newOrientationDegrees * (g_PI / 180.f);
	x = vectorLength * cosf(newOrientationRadians);
	y = vectorLength * sinf(newOrientationRadians);
}

void Vec2::SetPolarRadians(float newOrientationRadians, float newLength)
{
	x = newLength * cosf(newOrientationRadians);
	y = newLength * sinf(newOrientationRadians);
}

void Vec2::SetPolarDegrees(float newOrientationDegrees, float newLength)
{
	float newOrientationRadians = newOrientationDegrees * (g_PI / 180.f);
	x = newLength * cosf(newOrientationRadians);
	y = newLength * sinf(newOrientationRadians);
}

void Vec2::Rotate90Degrees()
{
	float tmp = y;
	y = x;
	x = -tmp;
}

void Vec2::RotateMinus90Degrees()
{
	float tmp = x;
	x = y;
	y = -tmp;
}

void Vec2::RotateRadians(float deltaRadians)
{
	float originalRadians = atan2f(y, x);
	float vectorLength = this->GetLength();

	float rotatedRadians = originalRadians + deltaRadians;
	float cosRotatedTheta = cosf(rotatedRadians);
	float sinRotatedTheta = sinf(rotatedRadians);

	x = cosRotatedTheta * vectorLength;
	y = sinRotatedTheta * vectorLength;
}

void Vec2::RotateDegrees(float deltaDegrees)
{
	float originalRadians = atan2f(y, x);
	float vectorLength = this->GetLength();

	float rotatedDegrees = (originalRadians * (180.f / g_PI)) + deltaDegrees;
	float rotatedRadians = rotatedDegrees * (g_PI / 180.f);
	float cosRotatedTheta = cosf(rotatedRadians);
	float sinRotatedTheta = sinf(rotatedRadians);

	x = vectorLength * cosRotatedTheta;
	y = vectorLength * sinRotatedTheta;
}

void Vec2::SetLength(float newLength)
{
	float originalRadians = atan2f(y, x);
	float cosRotatedTheta = cosf(originalRadians);
	float sinRotatedTheta = sinf(originalRadians);

	x = cosRotatedTheta * newLength;
	y = sinRotatedTheta * newLength;
}

void Vec2::ClampLength(float maxLength)
{
	float vectorLength = GetLength();

	if (vectorLength < maxLength)
	{
		// do nothing
	}
	else
	{
		float scaleFactor = maxLength / vectorLength;
		x = x * scaleFactor;
		y = y * scaleFactor;
	}
}

void Vec2::Normalized()
{
	float vectorLength = GetLength();
	x /= vectorLength;
	y /= vectorLength;
}

void Vec2::Reflect(Vec2 const& vectorToReflect)
{
	float length = GetProjectedLength2D(*this, vectorToReflect);
	Vec2 thisProjectOnPara = length * vectorToReflect;
	Vec2 reflectValue = *this - 2.0f * thisProjectOnPara;
	*this = reflectValue;
}

void Vec2::SetFromText(char const* text)
{
	Strings strings = SplitStringOnDelimiter(text, ',');
	
	GUARANTEE_OR_DIE(strings.size()==2,"You can not input not two char into this function");
	y = static_cast<float>(atof(strings[1].c_str()));
	x = static_cast<float>(atof(strings[0].c_str()));
}

float Vec2::NormalizedAndGetPreviousLength()
{
	float vectorLength = GetLength();
	x /= vectorLength;
	y /= vectorLength;
	return vectorLength;
}

