#include <string>
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Math/Matrix44.hpp"
#include "Engine/Core/EngineCommon.hpp"

enum class DebugRenderMode
{
	ALWAYS,
	USE_DEPTH,
	X_RAY,
};

class Renderer;
class Texture;
enum class BlendMode;
enum class RasterizerMode;

struct DebugRenderConfig 
{
	Renderer* m_renderer = nullptr;
	std::string m_fontName = "SquirrelFixedFont";
};

struct DebugRenderEntityWorld
{
	bool m_isActive = true;
	bool m_isHidden = false;
	bool m_faceCamera = false;
	Vec3 m_position;
	Texture* m_texture = nullptr;
	BlendMode m_blendMode;
	RasterizerMode m_rasterizerMode;
	float m_currentTime;
	float m_duration;
	Mat44 m_modelMatrix;
	std::vector<Vertex_PCU> m_verts;
	Rgba8 startColor;
	Rgba8 endColor;
	DebugRenderMode m_mode;
};

struct DebugRenderEntityScreen 
{
	bool m_isActive = true;
	bool m_isHidden = false;
	Texture* m_texture = nullptr;
	float m_currentTime;
	Rgba8 m_startColor;
	Rgba8 m_endColor;
	float m_duration;
	std::vector<Vertex_PCU> m_verts;
	Vec2 m_screenPosition;
};

// Set up
void DebugRenderSystemSetUp(const DebugRenderConfig& config);
void DebugRenderSystemShutDown();

// Control
void DebugRenderSystemSetVisble();
void DebugRenderSystemSetHidden();
void DebugRenderSystemClear();

// Output
void DebugRenderSystemBeginFrame();
void DebugRenderWorld(const Camera& camera);
void DebugRenderScreen(const Camera& camera);
void DebugRenderSystemEndFrame();

// Geometry
void DebugAddWorldPoint(const Vec3& pos, float radius, float duration, const Rgba8& startColor = Rgba8::WHITE, const Rgba8& endColor = Rgba8::WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldLine(const Vec3& start, const Vec3& end, float radius, float duration, const Rgba8& startColor = Rgba8::WHITE, const Rgba8& endColor = Rgba8::WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldWireCylinder(const Vec3& base, const Vec3& top, float radius, float duration, const Rgba8& startColor = Rgba8::WHITE, const Rgba8& endColor = Rgba8::WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldWireSphere(const Vec3& center, float radius, float duration, const Rgba8& startColor = Rgba8::WHITE, const Rgba8& endColor = Rgba8::WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldArrow(const Vec3& start, const Vec3& end, float radius, float duration, const Rgba8& startColor = Rgba8::WHITE, const Rgba8& endColor = Rgba8::WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldText(const std::string& text, const Mat44& transform, float textHeight, const Vec2& alignment, float duration, const Rgba8& startColor = Rgba8::WHITE, const Rgba8& endColor = Rgba8::WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldBillboardText(const std::string& text, const Vec3& origin, float textHeight, const Vec2& alignment, float duration, const Rgba8& startColor = Rgba8::WHITE, const Rgba8& endColor = Rgba8::WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddScreenText(const std::string& text, const Vec2& position, float size, const Vec2& alignment, float duration, const Rgba8& startColor = Rgba8::WHITE, const Rgba8& endColor = Rgba8::WHITE);
void DebugAddMessage(const std::string& text, float duration, const Rgba8& startColor = Rgba8::WHITE, const Rgba8& endColor = Rgba8::WHITE);

void UpdatePlayerPositionMsg(const std::string& text, const Vec2& position, float size, const Vec2& alignment = Vec2(0.0f,0.5f), const Rgba8& startColor = Rgba8::WHITE);
void UpdateGameInfoMsg(const std::string& text, const Vec2& position, float size, const Vec2& alignment = Vec2(0.0f,0.5f), const Rgba8& startColor = Rgba8::WHITE);

void AddDebugRenderWolrdEntity(const DebugRenderEntityWorld& entity);
void AddDebugRenderScreenEntity(const DebugRenderEntityScreen& entity);

// Console command
bool DebugRenderClear(EventArgs& args);
bool DebugRenderToggle(EventArgs& args);