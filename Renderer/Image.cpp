#define STB_IMAGE_IMPLEMENTATION // Exactly one .CPP (this Image.cpp) should #define this before #including stb_image.h
#include "ThirdParty/stb/stb_image.h"
#include "Engine/Renderer/Image.hpp"

Image::Image(char const* imageFilePath)
	:m_imageFilePath(std::string(imageFilePath)),
	m_dimensions(IntVec2(0,0)),
	m_rgbaTexels(std::vector<Rgba8>())

{

	int bytesPerTexel = 0; // This will be filled in for us to indicate how many color components the image had (e.g. 3=RGB=24bit, 4=RGBA=32bit)
	int numComponentsRequested = 0; // don't care; we support 3 (24-bit RGB) or 4 (32-bit RGBA)

	// Load (and decompress) the image RGB(A) bytes from a file on disk into a memory buffer (array of bytes)
	stbi_set_flip_vertically_on_load(1); // We prefer uvTexCoords has origin (0,0) at BOTTOM LEFT
	unsigned char* texelData = stbi_load(imageFilePath, &m_dimensions.x, &m_dimensions.y, &bytesPerTexel, numComponentsRequested);

	// Check if the load was successful
	GUARANTEE_OR_DIE(texelData, Stringf("Failed to load image \"%s\"", imageFilePath));

	m_rgbaTexels.reserve((size_t)(m_dimensions.x * m_dimensions.y));
	Rgba8 currentTexelColor = Rgba8();

	if (bytesPerTexel ==3) 
	{
		for (int i = 0; i < m_dimensions.x * m_dimensions.y * 3; i += 3)
		{
			currentTexelColor.r = texelData[i];
			currentTexelColor.g = texelData[i + 1];
			currentTexelColor.b = texelData[i + 2];
			currentTexelColor.a = 255;
			m_rgbaTexels.push_back(currentTexelColor);
		}
	}
	else 
	{
		for (int i = 0; i < m_dimensions.x * m_dimensions.y * 4; i += 4)
		{
			currentTexelColor.r = texelData[i];
			currentTexelColor.g = texelData[i + 1];
			currentTexelColor.b = texelData[i + 2];
			currentTexelColor.a = texelData[i + 3];
			m_rgbaTexels.push_back(currentTexelColor);
		}
	}
}

Image::Image(IntVec2 size, Rgba8 color)
	:m_dimensions(size)
{
	m_rgbaTexels.resize((size_t)(size.x * size.y));
	
	for (int i = 0; i < m_rgbaTexels.size(); i++)
	{
		m_rgbaTexels[i] = color;
	}
}

std::string const& Image::GetImageFilePath() const
{
	return m_imageFilePath;
}

IntVec2 Image::GetDimensions() const
{
	return m_dimensions;
}

const void* Image::GetRawData() const
{
	return m_rgbaTexels.data();
}

Rgba8 Image::GetTexelColor(IntVec2 const& texelCoords) const
{
	int index = texelCoords.x + (texelCoords.y * m_dimensions.x);

	return m_rgbaTexels[index];
}

void Image::SetTexelColor(IntVec2 const& texelCoords, Rgba8 const& newColor)
{
	int index = texelCoords.x + (texelCoords.y * m_dimensions.x);

	m_rgbaTexels[index] = newColor;
}
