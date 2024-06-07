#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"


const Vec3 Vec3::ZERO = Vec3(0.0f, 0.0f,0.0f);
const Vec3 Vec3::ONE = Vec3(1.0f, 1.0f, 1.0f);


Vec3::Vec3(float initialX, float initialY, float initialZ)
	:x(initialX), y(initialY), z(initialZ)
{

}

Vec3::Vec3(const Vec3& copyFrom)
	: x(copyFrom.x)
	, y(copyFrom.y)
	, z(copyFrom.z)
{

}


float Vec3::GetLength() const
{
	return sqrtf((x * x) + (y * y) + (z * z));
}

float Vec3::GetLengthXY() const
{
	return sqrtf((x * x) + (y * y));
}

float Vec3::GetLengthSquared() const
{
	return (x * x + y * y + z * z);
}

float Vec3::GetLengthXYSquared() const
{
	return (x * x + y * y);
}

float Vec3::GetAngleAboutZRadians() const
{
	return atan2f(y, x);
}

float Vec3::GetAngleAboutZDegrees() const
{
	float radian = atan2f(y, x);
	return	radian * (180.f / g_PI);
}


Vec3 const Vec3::GetRotatedAboutZRadians(float deltaRadians) const
{

	float originalRadian = atan2f(y, x);												// Get the original radian around the Z axis
	float originalLength = this->GetLengthXY();										// Get the original length of this vector 


	float rotatedRadian = originalRadian + deltaRadians;							// Get the new radians after the rotation 
	float cosRotatedTheta = cosf(rotatedRadian);										// Get the cosine of the new radians
	float sinRotatedTheta = sinf(rotatedRadian);										// Get the sine of the new radians


	return Vec3(originalLength * cosRotatedTheta, originalLength * sinRotatedTheta, z);
}

Vec3 const Vec3::GetRotatedAboutZDegrees(float deltaDegrees) const
{

	float originalRadian = atan2f(y, x);												// Get the original radian around the Z axis
	float originalLength = this->GetLengthXY();												// Get the original length of this vector 
	float originalDegrees = originalRadian * (180.f / g_PI);						// Get the original degrees of this radians


	float rotatedDegrees = originalDegrees + deltaDegrees;						// Get the new degrees after the rotation 
	float rotatedRadian = rotatedDegrees * (g_PI / 180.f);						// Get the new radians after the rotation 
	float cosRotatedTheta = cosf(rotatedRadian);										// Get the cosine of the new radians
	float sinRotatedTheta = sinf(rotatedRadian);										// Get the sine of the new radians

	return Vec3(originalLength * cosRotatedTheta, originalLength * sinRotatedTheta, z);

}

Vec3 const Vec3::GetClamped(float maxLength) const
{
	float vectorLength = this->GetLength();

	if (vectorLength < maxLength)
	{
		return *this;
	}
	else
	{
		float scaleFactor = maxLength / vectorLength;
		return Vec3(x * scaleFactor, y * scaleFactor, z * scaleFactor);
	}
}

Vec3 const Vec3::GetNormalized() const
{
	float vectorLength = this->GetLength();

	if (vectorLength == 0.0f)
	{
		return Vec3::ZERO;
	}

	float oneDivideLength = 1.0f / vectorLength;
	return Vec3(x * oneDivideLength, y * oneDivideLength, z * oneDivideLength);
}

void Vec3::Normalize()
{
	float vectorLength = GetLength();

	if (vectorLength == 0.0f)
	{
		return;
	}

	float oneDivideLength = 1.0f / vectorLength;
	x = x * oneDivideLength;
	y = y * oneDivideLength;
	z = z * oneDivideLength;
}

void Vec3::SetFromText(char const* text)
{
	Strings strings = SplitStringOnDelimiter(text, ',');

	GUARANTEE_OR_DIE(strings.size() == 3, "You can not input not three char into this function");
	x = static_cast<float>(atof(strings[0].c_str()));
	y = static_cast<float>(atof(strings[1].c_str()));
	z = static_cast<float>(atof(strings[2].c_str()));
}

const Vec3 Vec3::MakeFromPolarRadians(float latitudeRadians, float longitudeRadians, float length /*= 1.0f*/)
{
	float latitudeDegrees = ConvertRadiansToDegrees(latitudeRadians);
	float longitudeDegrees = ConvertRadiansToDegrees(longitudeRadians);

	return MakeFromPolarDegrees(latitudeDegrees, longitudeDegrees, length);
}

const Vec3 Vec3::MakeFromPolarDegrees(float latitudeDegrees, float longitudeDegrees, float length /*= 1.0f*/)
{
	float cosYaw = CosDegrees(longitudeDegrees);
	float sinYaw = SinDegrees(longitudeDegrees);
	float cosPitch = CosDegrees(latitudeDegrees);
	float sinPitch = SinDegrees(latitudeDegrees);

	return Vec3(cosYaw * cosPitch, sinYaw * cosPitch, sinPitch) * length;
}

const Vec3 Vec3::operator + (const Vec3& vecToAdd) const
{
	return Vec3(x + vecToAdd.x, y + vecToAdd.y, z + vecToAdd.z);
}

const Vec3 Vec3::operator-(const Vec3& vecToSubtract) const
{
	return Vec3(x - vecToSubtract.x, y - vecToSubtract.y, z - vecToSubtract.z);
}

const Vec3 Vec3::operator-() const
{
	return Vec3(-x, -y, -z);
}

const Vec3 Vec3::operator*(float uniformScale) const
{
	return Vec3(uniformScale * x, uniformScale * y, uniformScale * z);
}

const Vec3 Vec3::operator*(const Vec3& vecToMultiply) const
{
	return Vec3(x * vecToMultiply.x, y * vecToMultiply.y, z * vecToMultiply.z);
}

const Vec3 operator*(float uniformScale, const Vec3& vecToScale)
{
	return Vec3(uniformScale * vecToScale.x, uniformScale * vecToScale.y, uniformScale * vecToScale.z);
}

const Vec3 Vec3::operator/(float inverseScale) const
{
	return Vec3(x / inverseScale, y / inverseScale, z / inverseScale);
}

void Vec3::operator=(const Vec3& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
}

void Vec3::operator/=(const float uniformDivisor)
{
	x /= uniformDivisor;
	y /= uniformDivisor;
	z /= uniformDivisor;
}

void Vec3::operator*=(const float uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
}

void Vec3::operator-=(const Vec3& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
}

void Vec3::operator+=(const Vec3& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
}

bool Vec3::operator!=(const Vec3& compare) const
{
	return !(x == compare.x && y == compare.y && z == compare.z);
}

bool Vec3::operator==(const Vec3& compare) const
{
	return (x == compare.x && y == compare.y && z == compare.z);
}
