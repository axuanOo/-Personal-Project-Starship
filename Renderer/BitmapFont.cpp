#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Core/VertexUtils.hpp"
BitmapFont::BitmapFont(char const* fontFilePathNameWithNoExtension, Texture& fontTexture)
{
	m_fontGlyphsSpriteSheet = new SpriteSheet(fontTexture,IntVec2(16,16));
	m_fontFilePathNameWithNoExtension = std::string(fontFilePathNameWithNoExtension);
}

Texture const& BitmapFont::GetTexture()
{
	return m_fontGlyphsSpriteSheet->GetTexture();
}

void BitmapFont::AddVertsForText2D(std::vector<Vertex_PCU>& vertexArray, Vec2 const& textMins, const float cellHeight, std::string const& text, Rgba8 const& tint /*= Rgba8::WHITE*/, float cellAspect /*= 1.f*/)
{
	int vertsNum = (int)text.size() * 6;
	vertexArray.reserve(vertsNum);

	float cellWidth = cellHeight * cellAspect;

	Vec2 currentTextPosition = textMins;
	
	for (int charIndex = 0; charIndex < text.size(); ++charIndex)
	{
		int asciiValue = static_cast<int>(text[charIndex]);
		const SpriteDefinition& spriteOfLetter = m_fontGlyphsSpriteSheet->GetSpriteDef(asciiValue);

		Vec2 leftBottomPoint = currentTextPosition + Vec2(-0.5f * cellWidth, -0.5f * cellHeight);
		Vec2 rightBottomPoint = currentTextPosition + Vec2(0.5f * cellWidth, -0.5f * cellHeight);
		Vec2 leftTopPoint = currentTextPosition + Vec2(-0.5f * cellWidth, 0.5f * cellHeight);
		Vec2 rightTopPoint = currentTextPosition + Vec2(0.5f * cellWidth, 0.5f * cellHeight);

		AABB2 uvBoundsOfSpriteDef = spriteOfLetter.GetUVs();

		Vec2 leftBottomUVs = uvBoundsOfSpriteDef.GetPointAtUV(Vec2(0.0f, 0.0f));
		Vec2 rightBottomUVs = uvBoundsOfSpriteDef.GetPointAtUV(Vec2(1.0f, 0.0f));
		Vec2 leftTopUVs = uvBoundsOfSpriteDef.GetPointAtUV(Vec2(0.0f, 1.0f));
		Vec2 rightTopUVs = uvBoundsOfSpriteDef.GetPointAtUV(Vec2(1.0f, 1.0f));

		Vertex_PCU leftBottomPCU = Vertex_PCU(Vec3(leftBottomPoint.x,leftBottomPoint.y,0.0f),tint,leftBottomUVs);
		Vertex_PCU rightBottomPCU = Vertex_PCU(Vec3(rightBottomPoint.x, rightBottomPoint.y, 0.0f), tint, rightBottomUVs);
		Vertex_PCU leftTopPCU = Vertex_PCU(Vec3(leftTopPoint.x, leftTopPoint.y, 0.0f), tint, leftTopUVs);
		Vertex_PCU rightTopPCU = Vertex_PCU(Vec3(rightTopPoint.x, rightTopPoint.y, 0.0f), tint, rightTopUVs);

		vertexArray.push_back(leftBottomPCU);
		vertexArray.push_back(rightBottomPCU);
		vertexArray.push_back(leftTopPCU);

		vertexArray.push_back(leftTopPCU);
		vertexArray.push_back(rightBottomPCU);
		vertexArray.push_back(rightTopPCU);

		currentTextPosition += Vec2(cellWidth, 0.0f);
	}
}

void BitmapFont::AddVertsForTextInBox2D(std::vector<Vertex_PCU>& vertexArray, AABB2 const& box, float cellHeight, std::string const& text, Rgba8 const& tint /*= Rgba8::WHITE*/, float cellAspect /*= 1.f*/, Vec2 const& alignment /*= Vec2(.5f, .5f)*/, TextBoxMode mode /*= TextBoxMode::SHRINK_TO_FIT*/, int maxGlyphsToDraw /*= 99999999*/)
{
	int vertsNum = (int)text.size() * 6;
	vertexArray.reserve(vertsNum);

	float cellWidth = cellHeight * cellAspect;

	Strings stringsInLines = SplitStringOnDelimiter(text,'\n');
	
	int maxElement = (int)stringsInLines[0].size();

	for (int stringIndex = 0; stringIndex < stringsInLines.size(); ++stringIndex)
	{
		if (stringsInLines[stringIndex].size() > maxElement)
		{
			maxElement = (int)stringsInLines[stringIndex].size();
		}
	}

	
	for (int stringIndex = 0; stringIndex < stringsInLines.size(); ++stringIndex)
	{
		// Set the beginning string position at the top of the text box
		Vec2 startLinePosition = box.GetPointAtUV(Vec2(0.0f, 1.0f));
		startLinePosition += (float)stringIndex * Vec2(0.0f, -cellHeight);
		startLinePosition += 0.5f * Vec2(cellWidth, -cellHeight);

		const std::string& stringThisLine = stringsInLines[stringIndex];

		float lineWidth = cellWidth * stringThisLine.size();
		float lineHeight = cellHeight * stringsInLines.size(); // Fixed value

		float lineAndBoxHorizontalOffset = box.GetDimensions().x - lineWidth;
		float lineAndBoxVerticalOffset = box.GetDimensions().y - lineHeight;

		startLinePosition += alignment.x * Vec2(lineAndBoxHorizontalOffset, 0.0f);
		startLinePosition += alignment.y * Vec2(0.0f, -lineAndBoxVerticalOffset);

		///---------------------------------------------------------------------------------------
		int maxLettersToDraw = maxGlyphsToDraw;

		for (int charIndex = 0; charIndex < stringThisLine.size(); ++charIndex)
		{
			if (maxLettersToDraw<=0) 
			{
				break;
			}

			Vec2 charPosition = startLinePosition + (float)charIndex * Vec2(cellWidth, 0.0f);

			const char& letter = stringThisLine[charIndex];

			int asciiValue = static_cast<int>(letter);

			const SpriteDefinition& spriteOfLetter = m_fontGlyphsSpriteSheet->GetSpriteDef(asciiValue);


			Vec2 leftBottomPoint = charPosition + Vec2(-0.5f * cellWidth, -0.5f * cellHeight);
			Vec2 rightBottomPoint = charPosition + Vec2(0.5f * cellWidth, -0.5f * cellHeight);
			Vec2 leftTopPoint = charPosition + Vec2(-0.5f * cellWidth, 0.5f * cellHeight);
			Vec2 rightTopPoint = charPosition + Vec2(0.5f * cellWidth, 0.5f * cellHeight);


			if (mode == TextBoxMode::SHRINK_TO_FIT)
			{
				float maxWidth = cellWidth * maxElement;
				float maxHeight = cellHeight * stringsInLines.size();

				float widthShrinkStrength = box.GetDimensions().x / maxWidth;
				float heightShrinkStrength = box.GetDimensions().y / maxHeight;

				float shrinkParameter = widthShrinkStrength < heightShrinkStrength ? widthShrinkStrength : heightShrinkStrength;

				if (shrinkParameter <= 1.0f)
				{
					Vec2 pivot = box.GetPointAtUV(Vec2(alignment.x, 1.0f - alignment.y));

					Vec2 pivotToPositionLB = Vec2(leftBottomPoint.x - pivot.x, leftBottomPoint.y - pivot.y);
					Vec2 pivotToPositionRB = Vec2(rightBottomPoint.x - pivot.x, rightBottomPoint.y - pivot.y);
					Vec2 pivotToPositionLT = Vec2(leftTopPoint.x - pivot.x, leftTopPoint.y - pivot.y);
					Vec2 pivotToPositionRT = Vec2(rightTopPoint.x - pivot.x, rightTopPoint.y - pivot.y);

					float pivotToPositionLengthLB = pivotToPositionLB.GetLength();
					float pivotToPositionLengthRB = pivotToPositionRB.GetLength();
					float pivotToPositionLengthLT = pivotToPositionLT.GetLength();
					float pivotToPositionLengthRT = pivotToPositionRT.GetLength();

					pivotToPositionLB.SetLength(pivotToPositionLengthLB * shrinkParameter);
					pivotToPositionRB.SetLength(pivotToPositionLengthRB * shrinkParameter);
					pivotToPositionLT.SetLength(pivotToPositionLengthLT * shrinkParameter);
					pivotToPositionRT.SetLength(pivotToPositionLengthRT * shrinkParameter);

					Vec2 tmpPosition2DLB = pivot + pivotToPositionLB;
					Vec2 tmpPosition2DRB = pivot + pivotToPositionRB;
					Vec2 tmpPosition2DLT = pivot + pivotToPositionLT;
					Vec2 tmpPosition2DRT = pivot + pivotToPositionRT;

					leftBottomPoint.x = tmpPosition2DLB.x;
					leftBottomPoint.y = tmpPosition2DLB.y;
					rightBottomPoint.x = tmpPosition2DRB.x;
					rightBottomPoint.y = tmpPosition2DRB.y;
					leftTopPoint.x = tmpPosition2DLT.x;
					leftTopPoint.y = tmpPosition2DLT.y;
					rightTopPoint.x = tmpPosition2DRT.x;
					rightTopPoint.y = tmpPosition2DRT.y;
				}
			}

			Vec3 leftBottomPoint3D = Vec3(leftBottomPoint.x, leftBottomPoint.y, 0.0f);
			Vec3 rightBottomPoint3D = Vec3(rightBottomPoint.x, rightBottomPoint.y, 0.0f);
			Vec3 leftTopPoint3D = Vec3(leftTopPoint.x, leftTopPoint.y, 0.0f);
			Vec3 rightTopPoint3D = Vec3(rightTopPoint.x, rightTopPoint.y, 0.0f);

			AABB2 uvBoundsOfSpriteDef = spriteOfLetter.GetUVs();

			Vec2 leftBottomUVs = uvBoundsOfSpriteDef.GetPointAtUV(Vec2(0.0f, 0.0f));
			Vec2 rightBottomUVs = uvBoundsOfSpriteDef.GetPointAtUV(Vec2(1.0f, 0.0f));
			Vec2 leftTopUVs = uvBoundsOfSpriteDef.GetPointAtUV(Vec2(0.0f, 1.0f));
			Vec2 rightTopUVs = uvBoundsOfSpriteDef.GetPointAtUV(Vec2(1.0f, 1.0f));

			Vertex_PCU leftBottomPCU = Vertex_PCU(leftBottomPoint3D, tint, leftBottomUVs);
			Vertex_PCU rightBottomPCU = Vertex_PCU(rightBottomPoint3D, tint, rightBottomUVs);
			Vertex_PCU leftTopPCU = Vertex_PCU(leftTopPoint3D, tint, leftTopUVs);
			Vertex_PCU rightTopPCU = Vertex_PCU(rightTopPoint3D, tint, rightTopUVs);

			vertexArray.push_back(leftBottomPCU);
			vertexArray.push_back(rightBottomPCU);
			vertexArray.push_back(rightTopPCU);

			vertexArray.push_back(leftBottomPCU);
			vertexArray.push_back(rightTopPCU);
			vertexArray.push_back(leftTopPCU);

			maxLettersToDraw -= 1;
		}
	}

}

void BitmapFont::AddVertsForText3DAtOriginXForward(std::vector<Vertex_PCU>& vertexArray, float cellHeight, std::string const& text, Rgba8 const& tint /*= Rgba8::WHITE*/, float cellAspect /*= 1.f*/, Vec2 const& alignment /*= Vec2(.5f, .5f)*/, TextBoxMode mode /*= TextBoxMode::SHRINK_TO_FIT*/, int maxGlyphsToDraw /*= 99999999*/)
{
	UNUSED(maxGlyphsToDraw);
	UNUSED(mode);
	UNUSED(alignment);

	int vertsNum = (int)text.size() * 6;
	vertexArray.reserve(vertsNum);

	float cellWidth = cellHeight * cellAspect;

	Vec3 currentTextPosition = Vec3(0.0f, 0.0f, 0.0f);

	for (int charIndex = 0; charIndex < text.size(); ++charIndex)
	{
		int asciiValue = static_cast<int>(text[charIndex]);
		const SpriteDefinition& spriteOfLetter = m_fontGlyphsSpriteSheet->GetSpriteDef(asciiValue);

		Vec3 leftBottomPoint = currentTextPosition + Vec3(0.0f, 0.0f, 0.0f);
		Vec3 rightBottomPoint = currentTextPosition + Vec3(cellWidth, 0.0f, 0.0f);
		Vec3 leftTopPoint = currentTextPosition + Vec3(0.0f,cellHeight,0.0f);
		Vec3 rightTopPoint = currentTextPosition + Vec3(cellWidth, cellHeight, 0.0f);

		AABB2 uvBoundsOfSpriteDef = spriteOfLetter.GetUVs();

		Vec2 leftBottomUVs = uvBoundsOfSpriteDef.GetPointAtUV(Vec2(0.0f, 0.0f));
		Vec2 rightBottomUVs = uvBoundsOfSpriteDef.GetPointAtUV(Vec2(1.0f, 0.0f));
		Vec2 leftTopUVs = uvBoundsOfSpriteDef.GetPointAtUV(Vec2(0.0f, 1.0f));
		Vec2 rightTopUVs = uvBoundsOfSpriteDef.GetPointAtUV(Vec2(1.0f, 1.0f));

		Vertex_PCU leftBottomPCU = Vertex_PCU(Vec3(leftBottomPoint.x, leftBottomPoint.y, 0.0f), tint, leftBottomUVs);
		Vertex_PCU rightBottomPCU = Vertex_PCU(Vec3(rightBottomPoint.x, rightBottomPoint.y, 0.0f), tint, rightBottomUVs);
		Vertex_PCU leftTopPCU = Vertex_PCU(Vec3(leftTopPoint.x, leftTopPoint.y, 0.0f), tint, leftTopUVs);
		Vertex_PCU rightTopPCU = Vertex_PCU(Vec3(rightTopPoint.x, rightTopPoint.y, 0.0f), tint, rightTopUVs);

		vertexArray.push_back(leftBottomPCU);
		vertexArray.push_back(rightBottomPCU);
		vertexArray.push_back(leftTopPCU);

		vertexArray.push_back(leftTopPCU);
		vertexArray.push_back(rightBottomPCU);
		vertexArray.push_back(rightTopPCU);

		currentTextPosition += Vec3(cellWidth, 0.0f, 0.0f);
	}
	AABB2 bounds = GetVertexBounds2D(vertexArray);
	Mat44 tranformMatrix;
	tranformMatrix.AppendZRotation(90.0f);
	tranformMatrix.AppendXRotation(90.0f);
	tranformMatrix.AppendTranslation2D(-bounds.GetCenter());
	TransformVertexArray3D(vertexArray, tranformMatrix);
}

float BitmapFont::GetTextWidth(float cellHeight, std::string const& text, float cellAspect /*= 1.f*/)
{
	float singleWidth = cellHeight * cellAspect;

	return singleWidth * text.size();
}

float BitmapFont::GetGlyphAspect(int glyphUnicode) const
{
	UNUSED(glyphUnicode);
	return 1.0f;
}
