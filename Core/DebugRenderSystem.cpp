#include "DebugRenderSystem.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/Renderer.hpp"

DebugRenderSystem* g_theRenderSystem = nullptr;

class DebugRenderSystem 
{
public:
	DebugRenderSystem();
	~DebugRenderSystem();

public:
	Clock* m_clock;
	bool m_visible = true;
	DebugRenderConfig m_config;
	BitmapFont*	m_font;

	Camera* m_worldCamera = nullptr;
	Camera* m_screenCamera = nullptr;

	std::vector<DebugRenderEntityWorld> m_renderSystemWorldEntities;
	std::vector<DebugRenderEntityScreen> m_renderSystemScreenMsgEntities;
	std::vector<DebugRenderEntityScreen> m_normalScreenTextEntities;

	DebugRenderEntityScreen m_playerPositionMsg;
	DebugRenderEntityScreen m_gameInfoMsg;
};

DebugRenderSystem::DebugRenderSystem()
	:m_clock(new Clock())
{
	m_renderSystemWorldEntities.reserve(100);
	m_renderSystemScreenMsgEntities.reserve(100);
	m_playerPositionMsg = DebugRenderEntityScreen();
	m_gameInfoMsg = DebugRenderEntityScreen();
}

void DebugRenderSystemSetUp(const DebugRenderConfig& config)
{
	g_theRenderSystem = new DebugRenderSystem();
	g_theRenderSystem->m_config = config;
	g_theRenderSystem->m_font = g_theRenderSystem->m_config.m_renderer->CreateOrGetBitmapFont("Data/Fonts/SquirrelFixedFont");

	g_theEventSystem->SubscribeEventCallbackFunction("renderclear", (EventSystemCallbackFunction)DebugRenderClear);
	g_theEventSystem->SubscribeEventCallbackFunction("rendertoggle", (EventSystemCallbackFunction)DebugRenderToggle);
}

void DebugRenderSystemShutDown()
{
}

void DebugRenderSystemSetVisble()
{
	g_theRenderSystem->m_visible = true;
}

void DebugRenderSystemSetHidden()
{
	g_theRenderSystem->m_visible = false;
}

void DebugRenderSystemClear()
{
}

void DebugRenderSystemBeginFrame()
{
	for (int worldEntityIndex = 0; worldEntityIndex < (int)g_theRenderSystem->m_renderSystemWorldEntities.size();++worldEntityIndex)
	{
		if (g_theRenderSystem->m_renderSystemWorldEntities[worldEntityIndex].m_isActive)
		{
			if (g_theRenderSystem->m_renderSystemWorldEntities[worldEntityIndex].m_currentTime >= 0.0f)
			{
				g_theRenderSystem->m_renderSystemWorldEntities[worldEntityIndex].m_currentTime -= Clock::GetSystemClock().GetDeltaSeconds();

				if (g_theRenderSystem->m_renderSystemWorldEntities[worldEntityIndex].m_currentTime < 0.0f &&
					g_theRenderSystem->m_renderSystemWorldEntities[worldEntityIndex].m_currentTime >-0.9999f)
				{
					g_theRenderSystem->m_renderSystemWorldEntities[worldEntityIndex].m_isActive = false;
					continue;
				}
			}
		}
	}

	for (int screenEntityIndex = 0; screenEntityIndex < (int)g_theRenderSystem->m_renderSystemScreenMsgEntities.size(); ++screenEntityIndex)
	{
		if (g_theRenderSystem->m_renderSystemScreenMsgEntities[screenEntityIndex].m_isActive)
		{
			if (g_theRenderSystem->m_renderSystemScreenMsgEntities[screenEntityIndex].m_currentTime >= 0.0f)
			{
				g_theRenderSystem->m_renderSystemScreenMsgEntities[screenEntityIndex].m_currentTime -= Clock::GetSystemClock().GetDeltaSeconds();

				if (g_theRenderSystem->m_renderSystemScreenMsgEntities[screenEntityIndex].m_currentTime < 0.0f &&
					g_theRenderSystem->m_renderSystemScreenMsgEntities[screenEntityIndex].m_currentTime >-0.9999f)
				{
					g_theRenderSystem->m_renderSystemScreenMsgEntities[screenEntityIndex].m_isActive = false;
					continue;
				}
			}
		}
	}


	for (int screenTextIndex = 0; screenTextIndex < (int)g_theRenderSystem->m_normalScreenTextEntities.size(); ++screenTextIndex)
	{
		if (g_theRenderSystem->m_normalScreenTextEntities[screenTextIndex].m_isActive)
		{
			if (g_theRenderSystem->m_normalScreenTextEntities[screenTextIndex].m_currentTime >= 0.0f)
			{
				g_theRenderSystem->m_normalScreenTextEntities[screenTextIndex].m_currentTime -= Clock::GetSystemClock().GetDeltaSeconds();

				if (g_theRenderSystem->m_normalScreenTextEntities[screenTextIndex].m_currentTime < 0.0f &&
					g_theRenderSystem->m_normalScreenTextEntities[screenTextIndex].m_currentTime >-0.9999f)
				{
					g_theRenderSystem->m_normalScreenTextEntities[screenTextIndex].m_isActive = false;
					continue;
				}
			}
		}
	}
}

void DebugRenderWorld(const Camera& camera)
{
	g_theRenderSystem->m_worldCamera = const_cast<Camera*>(&camera);;
	g_theRenderSystem->m_config.m_renderer->BeginCamera(camera);
	
	for (int worldEntityIndex = 0; worldEntityIndex < (int)g_theRenderSystem->m_renderSystemWorldEntities.size(); ++worldEntityIndex)
	{
		DebugRenderEntityWorld& entity = g_theRenderSystem->m_renderSystemWorldEntities[worldEntityIndex];

		Rgba8 drawColor = entity.startColor;

		if (entity.m_isActive && !entity.m_isHidden)
		{		
			if (entity.m_currentTime > 0.0f && entity.m_duration > 0.0f)
			{
				drawColor = drawColor.Interpolate(entity.endColor, entity.startColor, entity.m_currentTime / entity.m_duration);
			}

			switch (entity.m_mode)
			{
			case DebugRenderMode::ALWAYS:
				g_theRenderSystem->m_config.m_renderer->SetDepthMode(DepthMode::DISABLED);
				g_theRenderSystem->m_config.m_renderer->SetBlendMode(entity.m_blendMode);
				g_theRenderSystem->m_config.m_renderer->SetRasterizerState(entity.m_rasterizerMode);
				g_theRenderSystem->m_config.m_renderer->BindTexture(entity.m_texture);
				g_theRenderSystem->m_config.m_renderer->SetModelConstants(entity.m_modelMatrix,drawColor);
				g_theRenderSystem->m_config.m_renderer->DrawVertexArray(
					(int)entity.m_verts.size(),
					entity.m_verts.data());
				break;
			case DebugRenderMode::USE_DEPTH:
				g_theRenderSystem->m_config.m_renderer->SetDepthMode(DepthMode::ENABLED);
				g_theRenderSystem->m_config.m_renderer->SetBlendMode(entity.m_blendMode);
				g_theRenderSystem->m_config.m_renderer->SetRasterizerState(entity.m_rasterizerMode);
				g_theRenderSystem->m_config.m_renderer->BindTexture(entity.m_texture);
				g_theRenderSystem->m_config.m_renderer->SetModelConstants(entity.m_modelMatrix,drawColor);
				g_theRenderSystem->m_config.m_renderer->DrawVertexArray(
					(int)entity.m_verts.size(),
					entity.m_verts.data());
				break;
			case DebugRenderMode::X_RAY:

				Rgba8 lightColor = Rgba8(drawColor.r - 50, drawColor.g - 50, drawColor.b - 50, 200);

				g_theRenderSystem->m_config.m_renderer->SetDepthMode(DepthMode::DISABLED);
				g_theRenderSystem->m_config.m_renderer->SetBlendMode(BlendMode::ALPHA);
				g_theRenderSystem->m_config.m_renderer->SetRasterizerState(entity.m_rasterizerMode);
				g_theRenderSystem->m_config.m_renderer->BindTexture(entity.m_texture);
				g_theRenderSystem->m_config.m_renderer->SetModelConstants(entity.m_modelMatrix,lightColor);
				g_theRenderSystem->m_config.m_renderer->DrawVertexArray(
					(int)entity.m_verts.size(),
					entity.m_verts.data());

				g_theRenderSystem->m_config.m_renderer->SetDepthMode(DepthMode::ENABLED);
				g_theRenderSystem->m_config.m_renderer->SetBlendMode(BlendMode::OPAQUE);
				g_theRenderSystem->m_config.m_renderer->SetRasterizerState(entity.m_rasterizerMode);
				g_theRenderSystem->m_config.m_renderer->BindTexture(entity.m_texture);
				g_theRenderSystem->m_config.m_renderer->SetModelConstants(entity.m_modelMatrix,drawColor);
				g_theRenderSystem->m_config.m_renderer->DrawVertexArray(
					(int)entity.m_verts.size(),
					entity.m_verts.data());
				break;
			}

		}
	}
	g_theRenderSystem->m_config.m_renderer->SetDepthMode(DepthMode::ENABLED);
	g_theRenderSystem->m_config.m_renderer->SetRasterizerState(RasterizerMode::SOLID_CULL_BACK);
	g_theRenderSystem->m_config.m_renderer->EndCamera(camera);
}

void DebugRenderScreen(const Camera& camera)
{
	g_theRenderSystem->m_screenCamera = const_cast<Camera*>(&camera);;
	g_theRenderSystem->m_config.m_renderer->BeginCamera(camera);

	//---------------------------------------------------------
	// Render the player position and other game info
	DebugRenderEntityScreen& playerPosMsg = g_theRenderSystem->m_playerPositionMsg;
	DebugRenderEntityScreen& gameInfoMsg = g_theRenderSystem->m_gameInfoMsg;
	g_theRenderSystem->m_config.m_renderer->BindShader(nullptr);
	g_theRenderSystem->m_config.m_renderer->SetBlendMode(BlendMode::ALPHA);
	g_theRenderSystem->m_config.m_renderer->SetModelConstants();
	g_theRenderSystem->m_config.m_renderer->SetLightConstants();
	if (playerPosMsg.m_isActive && !playerPosMsg.m_isHidden)
	{
		g_theRenderSystem->m_config.m_renderer->BindTexture(playerPosMsg.m_texture);
		g_theRenderSystem->m_config.m_renderer->DrawVertexArray((int)playerPosMsg.m_verts.size(),playerPosMsg.m_verts.data());
	}
	if (gameInfoMsg.m_isActive && !gameInfoMsg.m_isHidden)
	{
		g_theRenderSystem->m_config.m_renderer->BindTexture(gameInfoMsg.m_texture);
		g_theRenderSystem->m_config.m_renderer->DrawVertexArray((int)gameInfoMsg.m_verts.size(), gameInfoMsg.m_verts.data());
	}

	for (int screenTextIndex = 0; screenTextIndex < (int)g_theRenderSystem->m_normalScreenTextEntities.size(); screenTextIndex++)
	{
		DebugRenderEntityScreen& entity = g_theRenderSystem->m_normalScreenTextEntities[screenTextIndex];
		Rgba8 drawColor = entity.m_startColor;

		if (entity.m_isActive && !entity.m_isHidden)
		{
			if (entity.m_currentTime > 0.0f && entity.m_duration > 0.0f)
			{
				drawColor = drawColor.Interpolate(entity.m_endColor, entity.m_startColor, entity.m_currentTime / entity.m_duration);
			}

			Mat44 transformMatrix;
			transformMatrix.AppendTranslation2D(entity.m_screenPosition);

			g_theRenderSystem->m_config.m_renderer->BindShader(nullptr);
			g_theRenderSystem->m_config.m_renderer->SetBlendMode(BlendMode::ALPHA);
			g_theRenderSystem->m_config.m_renderer->BindTexture(entity.m_texture);
			g_theRenderSystem->m_config.m_renderer->SetLightConstants();
			g_theRenderSystem->m_config.m_renderer->SetModelConstants(transformMatrix, entity.m_startColor);
			g_theRenderSystem->m_config.m_renderer->DrawVertexArray((int)entity.m_verts.size(), entity.m_verts.data());
		}

	}
	///----------------------------------------------------------
	//-----------------------------------------------------------
	// Render the all screen message here

	Vec2 startScreenPositionAtTopLeft = Vec2(20.0f, 780.0f);

	for (int i = 0; i < (int)g_theRenderSystem->m_renderSystemScreenMsgEntities.size(); ++i)
	{
		DebugRenderEntityScreen& entity = g_theRenderSystem->m_renderSystemScreenMsgEntities[i];

		Rgba8 drawColor = entity.m_startColor;

		if (entity.m_isActive && !entity.m_isHidden)
		{
			if (entity.m_currentTime > 0.0f && entity.m_duration > 0.0f)
			{
				drawColor = drawColor.Interpolate(entity.m_endColor, entity.m_startColor, entity.m_currentTime / entity.m_duration);
			}

			Mat44 transformMatrix;
			entity.m_screenPosition = startScreenPositionAtTopLeft;
			transformMatrix.AppendTranslation2D(entity.m_screenPosition);

			g_theRenderSystem->m_config.m_renderer->BindShader(nullptr);
			g_theRenderSystem->m_config.m_renderer->SetBlendMode(BlendMode::ALPHA);
			g_theRenderSystem->m_config.m_renderer->BindTexture(entity.m_texture);
			g_theRenderSystem->m_config.m_renderer->SetLightConstants();
			g_theRenderSystem->m_config.m_renderer->SetModelConstants(transformMatrix,entity.m_startColor);
			g_theRenderSystem->m_config.m_renderer->DrawVertexArray((int)entity.m_verts.size(), entity.m_verts.data());

			startScreenPositionAtTopLeft += Vec2(0.0f, -30.0f);
		}
	}
	g_theRenderSystem->m_config.m_renderer->EndCamera(camera);
}

void DebugRenderSystemEndFrame()
{
	if (g_theRenderSystem->m_worldCamera != nullptr)
	{
		for (int worldEntityIndex = 0; worldEntityIndex < (int)g_theRenderSystem->m_renderSystemWorldEntities.size(); ++worldEntityIndex)
		{
			DebugRenderEntityWorld& entity = g_theRenderSystem->m_renderSystemWorldEntities[worldEntityIndex];

			if (entity.m_isActive && entity.m_faceCamera)
			{
				entity.m_modelMatrix.Reset();
				entity.m_modelMatrix.AppendTranslation3D(entity.m_position);
				entity.m_modelMatrix.Append(GetBillboardMatrix(BillboardType::FULL_CAMERA_OPPOSING,g_theRenderSystem->m_worldCamera->GetModelMatrix(),entity.m_position,Vec2::ONE));
			}
		}
	}
}

void DebugAddWorldPoint(const Vec3& pos, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	DebugRenderEntityWorld entity = DebugRenderEntityWorld();

	AddVertsForSphere3D(entity.m_verts, pos, radius, Rgba8::WHITE);
	entity.m_duration = duration;
	entity.m_currentTime = duration;
	entity.startColor = startColor;
	entity.endColor = endColor;
	entity.m_mode = mode;
	entity.m_blendMode = BlendMode::OPAQUE;
	entity.m_rasterizerMode = RasterizerMode::SOLID_CULL_BACK;
	AddDebugRenderWolrdEntity(entity);

}

void DebugAddWorldLine(const Vec3& start, const Vec3& end, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	DebugRenderEntityWorld entity = DebugRenderEntityWorld();

	AddVertsForCylinder3D(entity.m_verts,start,end,radius,Rgba8::WHITE);
	entity.m_duration = duration;
	entity.m_currentTime = duration;
	entity.m_mode = mode;
	entity.startColor = startColor;
	entity.endColor = endColor;
	entity.m_blendMode = BlendMode::OPAQUE;
	entity.m_rasterizerMode = RasterizerMode::SOLID_CULL_BACK;
	AddDebugRenderWolrdEntity(entity);
}

void DebugAddWorldWireCylinder(const Vec3& base, const Vec3& top, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	DebugRenderEntityWorld entity = DebugRenderEntityWorld();

	AddVertsForCylinder3D(entity.m_verts, base, top, radius, Rgba8::WHITE, AABB2::ZERO_TO_ONE, 16);
	entity.m_duration = duration;
	entity.m_currentTime = duration;
	entity.m_mode = mode;
	entity.startColor = startColor;
	entity.endColor = endColor;
	entity.m_blendMode = BlendMode::OPAQUE;
	entity.m_rasterizerMode = RasterizerMode::WIREFRAME_CULL_BACK;
	AddDebugRenderWolrdEntity(entity);
}

void DebugAddWorldWireSphere(const Vec3& center, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	DebugRenderEntityWorld entity = DebugRenderEntityWorld();

	AddVertsForSphere3D(entity.m_verts, center, radius, Rgba8::WHITE);
	entity.m_duration = duration;
	entity.m_currentTime = duration;
	entity.m_mode = mode;
	entity.startColor = startColor;
	entity.endColor = endColor;
	entity.m_blendMode = BlendMode::OPAQUE;
	entity.m_rasterizerMode = RasterizerMode::WIREFRAME_CULL_BACK;
	AddDebugRenderWolrdEntity(entity);
}

void DebugAddWorldArrow(const Vec3& start, const Vec3& end, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
 	DebugRenderEntityWorld entity = DebugRenderEntityWorld();
	AddVertsForArrow3D(entity.m_verts, start, end, radius, 0.2f);
	entity.m_duration = duration;
	entity.m_currentTime = duration;
	entity.m_mode = mode;
	entity.startColor = startColor;
	entity.endColor = endColor;
	entity.m_blendMode = BlendMode::OPAQUE;
	entity.m_rasterizerMode = RasterizerMode::SOLID_CULL_BACK;
	AddDebugRenderWolrdEntity(entity);
}

void DebugAddWorldText(const std::string& text, const Mat44& transform, float textHeight, const Vec2& alignment, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	DebugRenderEntityWorld entity = DebugRenderEntityWorld();
	g_theRenderSystem->m_font->AddVertsForText3DAtOriginXForward(entity.m_verts, textHeight, text, Rgba8::WHITE, 1.0f, alignment);
	TransformVertexArray3D(entity.m_verts,transform);
	entity.m_duration = duration;
	entity.m_currentTime = duration;
	entity.startColor = startColor;
	entity.endColor = endColor;
	entity.m_mode = mode;
	entity.m_texture = const_cast<Texture*>(&g_theRenderSystem->m_font->GetTexture());
	entity.m_blendMode = BlendMode::ALPHA;
	entity.m_rasterizerMode = RasterizerMode::SOLID_CULL_NONE;
	AddDebugRenderWolrdEntity(entity);
}

void DebugAddWorldBillboardText(const std::string& text, const Vec3& origin, float textHeight, const Vec2& alignment, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{

	DebugRenderEntityWorld entity = DebugRenderEntityWorld();
	g_theRenderSystem->m_font->AddVertsForText3DAtOriginXForward(entity.m_verts, textHeight, text, Rgba8::WHITE, 1.0f, alignment);

	entity.m_faceCamera = true;
	entity.m_duration = duration;
	entity.m_currentTime = duration;
	entity.startColor = startColor;
	entity.endColor = endColor;
	entity.m_position = origin;
	entity.m_mode = mode;
	entity.m_texture = const_cast<Texture*>(&g_theRenderSystem->m_font->GetTexture());
	entity.m_blendMode = BlendMode::ALPHA;
	entity.m_rasterizerMode = RasterizerMode::SOLID_CULL_NONE;
	AddDebugRenderWolrdEntity(entity);

}

void DebugAddScreenText(const std::string& text, const Vec2& position, float size, const Vec2& alignment, float duration, const Rgba8& startColor, const Rgba8& endColor)
{
	DebugRenderEntityScreen entity = DebugRenderEntityScreen();

	AABB2 textBox = AABB2();
	textBox.m_mins = Vec2::ZERO;
	textBox.m_maxs = textBox.m_mins + Vec2((int)text.size() * size, size);

	g_theRenderSystem->m_font->AddVertsForTextInBox2D(entity.m_verts, textBox, size, text, Rgba8::WHITE, 1.0f, alignment);
	entity.m_duration = duration;
	entity.m_currentTime = duration;
	entity.m_screenPosition = position;
	entity.m_startColor = startColor;
	entity.m_endColor = endColor;
	entity.m_texture = const_cast<Texture*>(&g_theRenderSystem->m_font->GetTexture());
	
	g_theRenderSystem->m_normalScreenTextEntities.push_back(entity);
}

void DebugAddMessage(const std::string& text, float duration, const Rgba8& startColor, const Rgba8& endColor)
{
	DebugRenderEntityScreen entity = DebugRenderEntityScreen();

	g_theRenderSystem->m_font->AddVertsForText2D(entity.m_verts, Vec2::ZERO, 15.0f, text);
	entity.m_duration = duration;
	entity.m_currentTime = duration;
	entity.m_startColor = startColor;
	entity.m_endColor = endColor;
	entity.m_texture = const_cast<Texture*>(&g_theRenderSystem->m_font->GetTexture());

	AddDebugRenderScreenEntity(entity);

}

void UpdatePlayerPositionMsg(const std::string& text, const Vec2& position, float size, const Vec2& alignment, const Rgba8& startColor /*= Rgba8::WHITE*/)
{

	DebugRenderEntityScreen& playerPosMsg = g_theRenderSystem->m_playerPositionMsg;
	
	playerPosMsg.m_verts.clear();

	playerPosMsg.m_screenPosition = position;

	AABB2 textBox = AABB2();

	textBox.m_mins = position;
	textBox.m_maxs = position + Vec2((int)text.size() * size, size);

	g_theRenderSystem->m_font->AddVertsForTextInBox2D(playerPosMsg.m_verts, textBox, size, text, startColor, 1.0f, alignment);

	playerPosMsg.m_texture = const_cast<Texture*>(&g_theRenderSystem->m_font->GetTexture());
	playerPosMsg.m_currentTime = -1.0f;
}



void UpdateGameInfoMsg(const std::string& text, const Vec2& position, float size, const Vec2& alignment /*= Vec2(0.0f,0.5f)*/, const Rgba8& startColor /*= Rgba8::WHITE*/)
{
	DebugRenderEntityScreen& gameInfo = g_theRenderSystem->m_gameInfoMsg;

	gameInfo.m_verts.clear();

	gameInfo.m_screenPosition = position;

	AABB2 textBox = AABB2();
	textBox.m_mins = position;
	textBox.m_maxs = position + Vec2((int)text.size() * size, size);

	g_theRenderSystem->m_font->AddVertsForTextInBox2D(gameInfo.m_verts, textBox, size, text, startColor, 1.0f, alignment);

	gameInfo.m_texture = const_cast<Texture*>(&g_theRenderSystem->m_font->GetTexture());
	gameInfo.m_currentTime = -1.0f;
}

void AddDebugRenderWolrdEntity(const DebugRenderEntityWorld& entity)
{
	if (g_theRenderSystem==nullptr)
	{
		return;
	}

	g_theRenderSystem->m_renderSystemWorldEntities.push_back(entity);
}

void AddDebugRenderScreenEntity(const DebugRenderEntityScreen& entity)
{
	if (g_theRenderSystem == nullptr)
	{
		return;
	}
	g_theRenderSystem->m_renderSystemScreenMsgEntities.push_back(entity);
}

bool DebugRenderClear(EventArgs& args)
{
	UNUSED(args);
	for (int i = 0; i < (int)g_theRenderSystem->m_renderSystemWorldEntities.size(); i++)
	{
		g_theRenderSystem->m_renderSystemWorldEntities[i].m_isActive = false;
		//if (g_theRenderSystem->m_renderSystemWorldEntities[i].m_duration = -1.0f)
		//{
		//}
	}

	for (int j = 0; j < (int)g_theRenderSystem->m_renderSystemScreenMsgEntities.size(); j++)
	{
		//if (g_theRenderSystem->m_renderSystemScreenEntities[j].m_duration = -1.0f)
		//{
			g_theRenderSystem->m_renderSystemScreenMsgEntities[j].m_isActive = false;
		//}
	}

	g_theRenderSystem->m_playerPositionMsg.m_isActive = false;
	g_theRenderSystem->m_gameInfoMsg.m_isActive = false;

	return false;
}

bool DebugRenderToggle(EventArgs& args)
{
	UNUSED(args);
	for (int i = 0; i < (int)g_theRenderSystem->m_renderSystemWorldEntities.size(); i++)
	{
		//if (g_theRenderSystem->m_renderSystemWorldEntities[i].m_duration = -1.0f)
		//{
			g_theRenderSystem->m_renderSystemWorldEntities[i].m_isHidden = !g_theRenderSystem->m_renderSystemWorldEntities[i].m_isHidden;
		//}
	}

	for (int j = 0; j < (int)g_theRenderSystem->m_renderSystemScreenMsgEntities.size(); j++)
	{
		//if (g_theRenderSystem->m_renderSystemScreenEntities[j].m_duration = -1.0f)
		//{
			g_theRenderSystem->m_renderSystemScreenMsgEntities[j].m_isHidden = !g_theRenderSystem->m_renderSystemWorldEntities[j].m_isHidden;
		//}
	}

	g_theRenderSystem->m_playerPositionMsg.m_isHidden = !g_theRenderSystem->m_playerPositionMsg.m_isHidden;
	g_theRenderSystem->m_gameInfoMsg.m_isHidden = !g_theRenderSystem->m_gameInfoMsg.m_isHidden;

	return false;
}

