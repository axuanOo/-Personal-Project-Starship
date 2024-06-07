#pragma once

struct Rgba8
{
public:
	unsigned char   r = 255;
	unsigned char   g = 255;
	unsigned char   b = 255;
	unsigned char   a = 255;

	static const Rgba8 RED;
	static const Rgba8 GREEN;
	static const Rgba8 YELLOW;
	static const Rgba8 BLUE;
	static const Rgba8 ORANGE;
	static const Rgba8 WHITE;
	static const Rgba8 GREY;
	static const Rgba8 BLACK;
	static const Rgba8 MAGENTA;
	static const Rgba8 TRANSPARENT_GREY;

	Rgba8();
	Rgba8(unsigned char redByte, unsigned char greenByte, unsigned char blueByte);
	Rgba8(unsigned char redByte, unsigned char greenByte, unsigned char blueByte, unsigned char aByte);
	static Rgba8 Interpolate( Rgba8 start, Rgba8 end, float fractionofEnd );
	float NormalizeByte(unsigned char colorValue) const; // convert from 0-255 to 0-1
	unsigned char DenormalizeByte(float value);

	void SetFromText(char const* text);

	bool operator==(const Rgba8& compare) const;		// rgb8 == rgb8
	const Rgba8	operator*(float uniformScale) const;			// vec2 * float

	void GetAsFloats(float* colorAsFloats) const;
}; 
