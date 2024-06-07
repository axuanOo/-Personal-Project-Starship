#include "Game/UIElements.hpp"

extern Renderer* g_theRenderer;


UIElements::UIElements()
{

}

UIElements::UIElements(Vec2 position, float orientationDegrees, Rgba8 color)
{
	m_position = position;
	m_orientationDegrees = orientationDegrees;
	m_color = color;
}

void UIElements::StartUp()
{
	m_position = Vec2(100.f, 50.f);
	m_orientationDegrees = 0.f;
	m_color = Rgba8(255, 255, 255, 255);
}

void UIElements::RenderPlayership(const Vec2& position, const float orientationDegrees,const float scale) const
{
	Vertex_PCU shipVertsGlobal[]
	{

		Vertex_PCU(2.5f, -0.5f,		m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(3.0, 0.f,		m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(2.5f, 0.5f,		m_color.r, m_color.g, m_color.b, m_color.a),

		Vertex_PCU(0.f,1.25f,		m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(2.5f,0.0f,		m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(2.5f, 0.5f,		m_color.r, m_color.g, m_color.b, m_color.a),

		Vertex_PCU(0.f,-1.25f,		m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(2.5f,0.0f,		m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(0.f,1.25f,		m_color.r, m_color.g, m_color.b, m_color.a),

		Vertex_PCU(0.f,-1.25f,		m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(2.5f, -0.5f,		m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(2.5f,0.0f,		m_color.r, m_color.g, m_color.b, m_color.a),

		Vertex_PCU(-1.8f,3.f,		m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(-1.8f, 1.75f,	128,128,128, m_color.a),
		Vertex_PCU(0.f,1.25f,		128,128,128, m_color.a),

		Vertex_PCU(-1.8f,-1.75f,	m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(0.f,1.25f,		m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(-1.8f, 1.75f,	m_color.r, m_color.g, m_color.b, m_color.a),

		Vertex_PCU(0.f,1.25f,		m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(-1.8f,-1.75f,	m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(0.f,-1.25f,		m_color.r, m_color.g, m_color.b, m_color.a),

		Vertex_PCU(-1.8f,-3.f,		m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(0.f,-1.25f,		128,128,128, m_color.a),
		Vertex_PCU(-1.8f,-1.75f,	128,128,128, m_color.a),

		Vertex_PCU(-2.2f,1.0f,		96,	96,	96, 255),
		Vertex_PCU(-1.8f,1.75f,		96,	96,	96, 255),
		Vertex_PCU(-2.2f,1.75f,		96,	96,	96, 255),

		Vertex_PCU(-2.2f,1.0f,		96,	96,	96, 255),
		Vertex_PCU(-1.8f,1.f,		96,	96,	96, 255),
		Vertex_PCU(-1.8f,1.75f,		96,	96,	96, 255),

		Vertex_PCU(-3.0f,-1.f,		64, 64, 64, 180),
		Vertex_PCU(-1.8f,1.f,		64, 64, 64, 180),
		Vertex_PCU(-3.0f,1.f,		64, 64, 64, 180),

		Vertex_PCU(-3.0f,-1.f,		64, 64, 64, 180),
		Vertex_PCU(-1.8f,-1.f,		64, 64, 64, 180),
		Vertex_PCU(-3.0f,1.f,		64, 64, 64, 180),

		Vertex_PCU(-2.2f,-1.75f,	96,	96,	96, 255),
		Vertex_PCU(-1.8f,-1.f,		96,	96,	96, 255),
		Vertex_PCU(-2.2f,-1.f,		96,	96,	96, 255),

		Vertex_PCU(-2.2f,-1.75f,	96,	96,	96, 255),
		Vertex_PCU(-1.8f,-1.75f,	96,	96,	96, 255),
		Vertex_PCU(-1.8f,-1.f,		96,	96,	96, 255),

	};
	int vertNum = sizeof(shipVertsGlobal) / sizeof(shipVertsGlobal[0]);

	TransformVertexArrayXY3D(vertNum, shipVertsGlobal, scale, orientationDegrees, position);
	g_theRenderer->DrawVertexArray(vertNum, shipVertsGlobal);
	return;
}


void UIElements::RenderHeart(const Vec2& position, const float orientationDegrees, const float scale) const
{
	Vertex_PCU shipVertsGlobal[]
	{
		Vertex_PCU(-2.2f,3.2f,		255, 0, 0, m_color.a),
		Vertex_PCU(0.0f, 0.0f,		255, 0, 0, m_color.a),
		Vertex_PCU(2.2f, 3.2f,		255, 0, 0, m_color.a),

		Vertex_PCU(-2.2f,3.2f,		255, 0, 0, m_color.a),
		Vertex_PCU(0.0f, 3.2f,		255, 0, 0, m_color.a),
		Vertex_PCU(-1.0f, 4.4f,		255, 0, 0, m_color.a),

		Vertex_PCU(0.0f, 3.2f,		255, 0, 0, m_color.a),
		Vertex_PCU(2.2f,3.2f,		255, 0, 0, m_color.a),
		Vertex_PCU(1.0f, 4.4f,		255, 0, 0, m_color.a),
	};
	int vertNum = sizeof(shipVertsGlobal) / sizeof(shipVertsGlobal[0]);
	TransformVertexArrayXY3D(vertNum, shipVertsGlobal, scale, orientationDegrees, position);
	g_theRenderer->DrawVertexArray(vertNum, shipVertsGlobal);
	return;
}

void UIElements::RenderHpBar(const Vec2& position, const float orientationDegrees, const float scale,float value) const
{
	Vertex_PCU shipVertsGlobal[]
	{
		Vertex_PCU(-7.0f,0.0f,							128, 128,128, m_color.a),
		Vertex_PCU(7.0f, 0.0f,							128, 128,128, m_color.a),
		Vertex_PCU(7.0f,3.0f,							128, 128,128, m_color.a),

		Vertex_PCU(-7.0f, 0.0f,							128, 128,128, m_color.a),
		Vertex_PCU(7.0f, 3.0f,							128, 128,128, m_color.a),
		Vertex_PCU(-7.0f, 3.0f,							128, 128,128, m_color.a),

		Vertex_PCU(value, 0.5f,							255, 0,0, m_color.a),
		Vertex_PCU(value,2.5f,							255, 0,0, m_color.a),
		Vertex_PCU(-6.5f, 2.5f,							255, 0,0, m_color.a),
									
		Vertex_PCU(-6.5f,0.5f,							255, 0,0, m_color.a),
		Vertex_PCU(value, 0.5f,							255, 0,0, m_color.a),
		Vertex_PCU(-6.5f, 2.5f,							255, 0,0, m_color.a),

	};
	int vertNum = sizeof(shipVertsGlobal) / sizeof(shipVertsGlobal[0]);
	TransformVertexArrayXY3D(vertNum, shipVertsGlobal, scale, orientationDegrees, position);
	g_theRenderer->DrawVertexArray(vertNum, shipVertsGlobal);
	return;
}

void UIElements::RenderCanvasBox(const Vec2& position, const float hdw, const float width) const
{
	float height = hdw * width;
	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;
	Vertex_PCU shipVertsGlobal[6]
	{
		Vertex_PCU(halfWidth, halfHeight,	90, 90, 90, static_cast<unsigned char>(0.5f*m_color.a)),
		Vertex_PCU(-halfWidth, halfHeight,	90, 90, 90, static_cast<unsigned char>(0.5f*m_color.a)),
		Vertex_PCU(-halfWidth, -halfHeight,	90, 90, 90,  static_cast<unsigned char>(0.5f*m_color.a)),
													
		Vertex_PCU(halfWidth, halfHeight,	90, 90, 90,  static_cast<unsigned char>(0.5f*m_color.a)),
		Vertex_PCU(halfWidth, -halfHeight,	90, 90, 90,  static_cast<unsigned char>(0.5f*m_color.a)),
		Vertex_PCU(-halfWidth, -halfHeight,	90, 90, 90,  static_cast<unsigned char>(0.5f*m_color.a)),
	};
	TransformVertexArrayXY3D(6, shipVertsGlobal, 1.0f, m_orientationDegrees, position);
	g_theRenderer->DrawVertexArray(6, shipVertsGlobal);
	return;
}

void UIElements::RenderMissile(const Vec2& position, const float orientationDegrees, const float scale , const unsigned char a) const
{
	Vertex_PCU MissileVertsGlobal[]
	{
		Vertex_PCU(4.f, 0.f,	255, 0, 0, a),
		Vertex_PCU(3.f, 0.6f,	255, 0, 0, a),
		Vertex_PCU(3.f, -0.6f,	255, 0, 0, a),

		Vertex_PCU(-2.f, 0.6f,	192,192,192, a),
		Vertex_PCU(3.f, 0.6f,	192,192,192, a),
		Vertex_PCU(3.f, -0.6f,	192,192,192, a),

		Vertex_PCU(-2.f,0.6f,	192,192,192, a),
		Vertex_PCU(-2.f, -0.6f,	192,192,192, a),
		Vertex_PCU(3.f, -0.6f,	192,192,192, a),

		Vertex_PCU(1.f, 0.6f,	96,96,96, a),
		Vertex_PCU(-1.f, 0.6f,	96,96,96, a),
		Vertex_PCU(-1.f, 1.f,	96,96,96, a),

		Vertex_PCU(1.f, -0.6f,	96,96,96, a),
		Vertex_PCU(-1.f, -0.6f,	96,96,96, a),
		Vertex_PCU(-1.f, -1.f,	96,96,96, a),

	};
	int vertNum = sizeof(MissileVertsGlobal) / sizeof(MissileVertsGlobal[0]);
	TransformVertexArrayXY3D(vertNum, MissileVertsGlobal, scale, orientationDegrees, position);
	g_theRenderer->DrawVertexArray(vertNum, MissileVertsGlobal);

	Vertex_PCU FlameVertsGlobal[3]
	{
		Vertex_PCU(-2.f, 0.6f,	255, 255, 51, a),
		Vertex_PCU(-2.f, -0.6f,	255, 255, 51, a),
		Vertex_PCU(-3.5f,0.f,	255, 0, 0, 0),

	};

	TransformVertexArrayXY3D(3, FlameVertsGlobal, scale, orientationDegrees, position);
	g_theRenderer->DrawVertexArray(3, FlameVertsGlobal);
}

void UIElements::RenderAttractModeShip() const
{
	Vertex_PCU shipVertsGlobal[]
	{
		Vertex_PCU(2.5f, -0.5f,		m_color.r, m_color.g, m_color.b, 255),
		Vertex_PCU(3.0, 0.f,		m_color.r, m_color.g, m_color.b, 255),
		Vertex_PCU(2.5f, 0.5f,		m_color.r, m_color.g, m_color.b, 255),

		Vertex_PCU(0.f,1.25f,		m_color.r, m_color.g, m_color.b, 255),
		Vertex_PCU(2.5f,0.0f,		m_color.r, m_color.g, m_color.b, 255),
		Vertex_PCU(2.5f, 0.5f,		m_color.r, m_color.g, m_color.b, 255),

		Vertex_PCU(0.f,-1.25f,		m_color.r, m_color.g, m_color.b, 255),
		Vertex_PCU(2.5f,0.0f,		m_color.r, m_color.g, m_color.b, 255),
		Vertex_PCU(0.f,1.25f,		m_color.r, m_color.g, m_color.b, 255),

		Vertex_PCU(0.f,-1.25f,		m_color.r, m_color.g, m_color.b, 255),
		Vertex_PCU(2.5f, -0.5f,		m_color.r, m_color.g, m_color.b, 255),
		Vertex_PCU(2.5f,0.0f,		m_color.r, m_color.g, m_color.b, 255),

		Vertex_PCU(-1.8f, 1.75f,	128,128,128,					255),
		Vertex_PCU(0.f,1.25f,		128,128,128,					255),
		Vertex_PCU(-1.8f,3.f,		m_color.r, m_color.g, m_color.b, 255),

		Vertex_PCU(-1.8f,-1.75f,	m_color.r, m_color.g, m_color.b, 255),
		Vertex_PCU(0.f,1.25f,		m_color.r, m_color.g, m_color.b, 255),
		Vertex_PCU(-1.8f, 1.75f,	m_color.r, m_color.g, m_color.b, 255),

		Vertex_PCU(-1.8f,-1.75f,	m_color.r, m_color.g, m_color.b, 255),
		Vertex_PCU(0.f,-1.25f,		m_color.r, m_color.g, m_color.b, 255),
		Vertex_PCU(0.f,1.25f,		m_color.r, m_color.g, m_color.b, 255),

		Vertex_PCU(-1.8f,-3.f,		m_color.r, m_color.g, m_color.b, 255),
		Vertex_PCU(0.f,-1.25f,		128,128,128, 255),
		Vertex_PCU(-1.8f,-1.75f,	128,128,128, 255),

		Vertex_PCU(-2.2f,1.0f,		96,	96,	96, 255),
		Vertex_PCU(-1.8f,1.75f,		96,	96,	96, 255),
		Vertex_PCU(-2.2f,1.75f,		96,	96,	96, 255),

		Vertex_PCU(-2.2f,1.0f,		96,	96,	96, 255),
		Vertex_PCU(-1.8f,1.f,		96,	96,	96, 255),
		Vertex_PCU(-1.8f,1.75f,		96,	96,	96, 255),

		Vertex_PCU(-3.0f,-1.f,		64, 64, 64, 255),
		Vertex_PCU(-1.8f,1.f,		64, 64, 64, 255),
		Vertex_PCU(-3.0f,1.f,		64, 64, 64, 255),

		Vertex_PCU(-3.0f,-1.f,		64, 64, 64, 255),
		Vertex_PCU(-1.8f,-1.f,		64, 64, 64, 255),
		Vertex_PCU(-3.0f,1.f,		64, 64, 64, 255),

		Vertex_PCU(-2.2f,-1.75f,	96,	96,	96, 255),
		Vertex_PCU(-1.8f,-1.f,		96,	96,	96, 255),
		Vertex_PCU(-2.2f,-1.f,		96,	96,	96, 255),

		Vertex_PCU(-2.2f,-1.75f,	96,	96,	96, 255),
		Vertex_PCU(-1.8f,-1.75f,	96,	96,	96, 255),
		Vertex_PCU(-1.8f,-1.f,		96,	96,	96, 255),

		Vertex_PCU(2.3f, 0.6f,		64, 64, 64, 255),
		Vertex_PCU(2.5f, -0.5f,		64, 64, 64, 255),
		Vertex_PCU(2.5f, 0.5f,		64, 64, 64, 255),

		Vertex_PCU(2.3f,-0.6f,		64, 64, 64, 255),
		Vertex_PCU(2.5f, -0.5f,		64, 64, 64, 255),
		Vertex_PCU(2.3f,0.6f,		64, 64, 64, 255),

		Vertex_PCU(1.0f,-0.5f,		51, 255, 255, 255),
		Vertex_PCU(2.0f,0.0f,		51, 255, 255, 255),
		Vertex_PCU(1.0f,0.5f,		51, 255, 255, 255),

		Vertex_PCU(0.25f,1.0f,		255, 255, 255, 255),
		Vertex_PCU(1.0f,-0.5f,		255, 255, 255, 255),
		Vertex_PCU(1.0f,0.5f,		255, 255, 255, 255),

		Vertex_PCU(0.25f,-1.0f,		255, 255, 255, 255),
		Vertex_PCU(1.0f,-0.5f,		255, 255, 255, 255),
		Vertex_PCU(0.25f,1.0f,		255, 255, 255, 255),

		Vertex_PCU(-1.8f,-1.5f,		255, 255, 255, 255),
		Vertex_PCU(0.25f,-1.0f,		255, 255, 255, 255),
		Vertex_PCU(0.25f,1.0f,		255, 255, 255, 255),

		Vertex_PCU(-1.8f,-1.5f,		255, 255, 255, 255),
		Vertex_PCU(0.25f,1.0f,		255, 255, 255, 255),
		Vertex_PCU(-1.8f,1.5f,		255, 255, 255, 255),

		Vertex_PCU(-1.8f,-2.f,		96, 96, 96, 255),
		Vertex_PCU(-1.8f,-2.5f,		96, 96, 96, 255),
		Vertex_PCU(-0.5f,-2.f,		96, 96, 96, 255),

		Vertex_PCU(-1.8f,2.f,		96, 96, 96, 255),
		Vertex_PCU(-0.5f,2.f,		96, 96, 96, 255),
		Vertex_PCU(-1.8f,2.5f,		96, 96, 96, 255),

	};
	int vertNum = sizeof(shipVertsGlobal) / sizeof(shipVertsGlobal[0]);
	TransformVertexArrayXY3D(vertNum, shipVertsGlobal, 7.0f, m_orientationDegrees, m_position);
	g_theRenderer->DrawVertexArray(vertNum, shipVertsGlobal);
}

void UIElements::RenderAttractModePlayButton() const
{
	Vertex_PCU shipVertsGlobal[3]
	{
		Vertex_PCU(-4.f, 6.f,	m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(5.f, 0.f,	m_color.r, m_color.g, m_color.b, m_color.a),
		Vertex_PCU(-4.f, -6.f,	m_color.r, m_color.g, m_color.b, m_color.a),
	};
	TransformVertexArrayXY3D(3, shipVertsGlobal, 2.f, m_orientationDegrees, m_position);
	g_theRenderer->DrawVertexArray(3, shipVertsGlobal);
	return;
}

void UIElements::RenderText(std::string const& text, Vec2 const& startMins, float cellHeight, const Rgba8& color) const
{
	std::vector<Vertex_PCU> textVerts;
	AddVertsForTextTriangles2D(textVerts, text, startMins, cellHeight, color);
	g_theRenderer->DrawVertexArray( static_cast<int>(textVerts.size()), textVerts.data() );
}

Rgba8 UIElements::GetColor() const
{
	return m_color;
}

Vec2 UIElements::GetPosition() const
{
	return m_position;
}

float UIElements::GetColorAlpha() const
{
	return m_color.a;
}

float UIElements::GetOrientationDegrees() const
{
	return m_orientationDegrees;
}


void UIElements::SetColor(Rgba8& color)
{
	m_color = color;
}

void UIElements::SetPosition(Vec2& position)
{
	m_position = position;
}

void UIElements::SetColorAlpha(unsigned char deltaAlpha)
{
	if (m_color.a >=2550) 
	{

	}
	m_color = Rgba8(m_color.r, m_color.g, m_color.b, m_color.a + deltaAlpha);
}

void UIElements::SetOrientationDegrees(float orientationDegrees)
{
	m_orientationDegrees = orientationDegrees;
}

