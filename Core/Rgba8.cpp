#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/MathUtils.hpp"

const Rgba8 Rgba8::RED = Rgba8(255, 0, 0, 255);
const Rgba8 Rgba8::GREEN = Rgba8(0, 255, 0, 255);
const Rgba8 Rgba8::YELLOW = Rgba8(255, 255, 0, 255);
const Rgba8 Rgba8::BLUE = Rgba8(0, 0, 255, 255);
const Rgba8 Rgba8::ORANGE = Rgba8(255, 128, 0, 255);
const Rgba8 Rgba8::WHITE = Rgba8(255, 255, 255, 255);
const Rgba8 Rgba8::GREY = Rgba8(192, 192, 192, 192);
const Rgba8 Rgba8::BLACK = Rgba8(0, 0, 0, 255);
const Rgba8 Rgba8::TRANSPARENT_GREY = Rgba8(192, 192, 192, 100);
const Rgba8 Rgba8::MAGENTA = Rgba8(255, 0, 255, 255);
Rgba8::Rgba8(unsigned char redByte, unsigned char greenByte, unsigned char blueByte, unsigned char aByte)
	:r(redByte), g(greenByte), b(blueByte), a(aByte)
{

}

Rgba8::Rgba8(unsigned char redByte, unsigned char greenByte, unsigned char blueByte)
	:r(redByte), g(greenByte), b(blueByte), a(255)
{

}

Rgba8::Rgba8()
{

}

Rgba8 Rgba8::Interpolate(Rgba8 start, Rgba8 end, float fractionofEnd)
{
	float newr = ::Interpolate(::NormalizeByte(start.r), ::NormalizeByte(end.r), fractionofEnd);
	float newg = ::Interpolate(::NormalizeByte(start.g), ::NormalizeByte(end.g), fractionofEnd);
	float newb = ::Interpolate(::NormalizeByte(start.b), ::NormalizeByte(end.b), fractionofEnd);
	float newa = ::Interpolate(::NormalizeByte(start.a), ::NormalizeByte(end.a), fractionofEnd);

	return Rgba8(::DenormalizeByte(newr), ::DenormalizeByte(newg), ::DenormalizeByte(newb), ::DenormalizeByte(newa));
}

float Rgba8::NormalizeByte(unsigned char colorValue) const
{
	return (float)(colorValue) / 255.0f;
}

unsigned char Rgba8::DenormalizeByte(float value)
{
	return (unsigned char)(value * 255.0f);
}

void Rgba8::SetFromText(char const* text)
{
	Strings strings = SplitStringOnDelimiter(text, ',');

	GUARANTEE_OR_DIE(strings.size() == 3||strings.size() == 4, "Parameter number doesn't match");

	if (strings.size() == 3)
	{
		r = static_cast<unsigned char>(atoi(strings[0].c_str()));
		g = static_cast<unsigned char>(atoi(strings[1].c_str()));
		b = static_cast<unsigned char>(atoi(strings[2].c_str()));
		a = static_cast<unsigned char>(255);
	}

	if (strings.size() == 4)
	{
		r = static_cast<unsigned char>(atoi(strings[0].c_str()));
		g = static_cast<unsigned char>(atoi(strings[1].c_str()));
		b = static_cast<unsigned char>(atoi(strings[2].c_str()));
		a = static_cast<unsigned char>(atoi(strings[3].c_str()));
	}
}

bool Rgba8::operator==(const Rgba8& compare) const
{
	return (compare.r == r) && (compare.b == b) && (compare.g == g) && (compare.a == a);
}

const Rgba8 Rgba8::operator*(float uniformScale) const
{
	float rValue = uniformScale * static_cast<float>(r);
	float gValue = uniformScale * static_cast<float>(g);
	float bValue = uniformScale * static_cast<float>(b);

	return Rgba8(static_cast<unsigned char>(rValue), static_cast<unsigned char>(gValue), static_cast<unsigned char>(bValue));
}

void Rgba8::GetAsFloats(float* colorAsFloats) const
{
	colorAsFloats[0] = NormalizeByte(r);
	colorAsFloats[1] = NormalizeByte(g);
	colorAsFloats[2] = NormalizeByte(b);
	colorAsFloats[3] = NormalizeByte(a);
}

