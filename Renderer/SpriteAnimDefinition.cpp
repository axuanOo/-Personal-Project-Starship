#include "Engine/Renderer/SpriteAnimDefinition.hpp"
#include "Engine/Renderer/Renderer.hpp"


SpriteAnimDefinition::SpriteAnimDefinition(const SpriteSheet& sheet, int startSpriteIndex, int endSpriteIndex, float durationSeconds, SpriteAnimPlaybackType playbackType /*= SpriteAnimPlaybackType::LOOP*/)
	:m_spriteSheet(sheet),
	m_startSpriteIndex(startSpriteIndex),
	m_endSpriteIndex(endSpriteIndex),
	m_playbackType(playbackType),
	m_durationSeconds(durationSeconds)
{

}

const SpriteDefinition& SpriteAnimDefinition::GetSpriteDefAtTime(float seconds) const
{
	float nonRepeatAnimationLength = (m_endSpriteIndex - m_startSpriteIndex + 1) * m_durationSeconds;

	int index = 0;
	int currentTimeFrameIndex = 0;
	int oneSectionFrameNum = m_endSpriteIndex - m_startSpriteIndex + 1;
	int pingpongFrameNum = (oneSectionFrameNum - 1) * 2;

	switch (m_playbackType)
	{
		case SpriteAnimPlaybackType::ONCE:

			if (seconds < 0.0f)
			{
				return m_spriteSheet.GetSpriteDef(m_startSpriteIndex);
			}

			if (seconds > nonRepeatAnimationLength)
			{
				return m_spriteSheet.GetSpriteDef(m_endSpriteIndex);
			}

			index = RoundDownToInt(seconds / m_durationSeconds);
			break;

		case SpriteAnimPlaybackType::LOOP:

			if (seconds < 0.0f)
			{
				currentTimeFrameIndex = RoundDownToInt(fmodf(seconds, nonRepeatAnimationLength));
				index = currentTimeFrameIndex + oneSectionFrameNum;
			}
			else 
			{
				currentTimeFrameIndex = static_cast<int>(seconds / m_durationSeconds);
				index = currentTimeFrameIndex % oneSectionFrameNum;
			}


			break;

		case SpriteAnimPlaybackType::PINGPONG:

			if (seconds < 0.0f)
			{
				seconds *= -1.0f;
			}

			currentTimeFrameIndex = static_cast<int>(seconds / m_durationSeconds);
			index = currentTimeFrameIndex % pingpongFrameNum;

			if (index > m_endSpriteIndex)
			{
				index = pingpongFrameNum - index;
			}
			break;
	}

	index += m_startSpriteIndex;
	return m_spriteSheet.GetSpriteDef(index);

}

float SpriteAnimDefinition::GetOneClipTotalLength() const
{
	return (m_endSpriteIndex - m_startSpriteIndex + 1) * m_durationSeconds;
}

SpriteAnimPlaybackType SpriteAnimDefinition::GetAnimPlayBackType() const
{
	return m_playbackType;
}

