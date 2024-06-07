#pragma once
#include "Game/GameCommon.hpp"

class UIElements
{
public:

	UIElements();
	UIElements(Vec2 position, float orientationDegrees, Rgba8 color);
	~UIElements();

	void			StartUp();

	void			RenderPlayership(const Vec2& position,const float orientationDegrees,const float scale) const;
	void			RenderHeart(const Vec2& position,const float orientationDegrees,const float scale) const;
	void			RenderHpBar(const Vec2& position,const float orientationDegrees,const float scale,float value) const;
	void			RenderCanvasBox(const Vec2& position, const float hdw, const float width) const;
	void			RenderMissile(const Vec2& position, const float orientationDegrees, const float scale, const unsigned char a) const;
	void			RenderAttractModeShip() const;
	void			RenderAttractModePlayButton() const;
	void			RenderText(std::string const& text, Vec2 const& startMins, float cellHeight, const Rgba8& color) const;

	Rgba8			GetColor() const;
	Vec2			GetPosition() const;
	float			GetColorAlpha() const;
	float			GetOrientationDegrees() const;

	void			SetColor(Rgba8& color);
	void			SetPosition(Vec2& position);
	void			SetColorAlpha(unsigned char deltaAlpha);
	void			SetOrientationDegrees(float orientationDegrees);

private:

	Rgba8			m_color;
	float			m_orientationDegrees;
	Vec2			m_position;

};
