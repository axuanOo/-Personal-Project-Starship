#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

const IntVec2 IntVec2::ZERO = IntVec2(0,0);

IntVec2::~IntVec2()
{

}

IntVec2::IntVec2()
{

}

IntVec2::IntVec2(const IntVec2& copyFrom)
	: x(copyFrom.x)
	 ,y(copyFrom.y)
{

}

IntVec2::IntVec2(int initialX, int initialY)
	:x(initialX)
	,y(initialY)
{

}

float IntVec2::GetLength() const
{
	float f_x = static_cast<float>(x);
	float f_y = static_cast<float>(y);

	return sqrtf((f_x * f_x) + (f_y * f_y));
}

int IntVec2::GetTaxicabLength() const
{
	return abs(x) + abs(y);
}

int IntVec2::GetLengthSquared() const
{
	return (x * x) + (y * y);
}

float IntVec2::GetOrientationRadians() const
{
	float f_x = static_cast<float>(x);
	float f_y = static_cast<float>(y);
	return atan2f(f_y, f_x);
}

float IntVec2::GetOrientationDegrees() const
{
	float f_x = static_cast<float>(x);
	float f_y = static_cast<float>(y);
	return atan2f(f_y, f_x) * (180.f / g_PI);
}

IntVec2 const IntVec2::GetRotated90Degrees() const
{
	return  IntVec2(-y, x);
}

IntVec2 const IntVec2::GetRotatedMinus90Degrees() const
{
	return  IntVec2(y, -x);
}

void IntVec2::Rotate90Degrees()
{
	int tmp = x;
	x = -y;
	y = tmp;
}

void IntVec2::RotateMinus90Degrees()
{
	int tmp = x;
	x = y;
	y = -tmp;
}

void IntVec2::SetFromText(char const* text)
{
	Strings strings = SplitStringOnDelimiter(text, ',');

	GUARANTEE_OR_DIE(strings.size() == 2, "You can not input not two char into this function");
	x = atoi(strings[0].c_str());
	y = atoi(strings[1].c_str());
}

bool IntVec2::operator!=(const IntVec2& compare) const
{
	return !(x == compare.x) || !(y == compare.y);
}

IntVec2 IntVec2::operator-(const IntVec2& vecToSub) const
{
	return IntVec2(x - vecToSub.x, y - vecToSub.y);
}

void IntVec2::operator+=(const IntVec2& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
}

IntVec2 IntVec2::operator+(const IntVec2& vecToAdd) const
{
	return IntVec2(x + vecToAdd.x, y + vecToAdd.y);
}

bool IntVec2::operator==(const IntVec2& compare) const
{
	return x == compare.x && y == compare.y;
}

void IntVec2::operator=(const IntVec2& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}
