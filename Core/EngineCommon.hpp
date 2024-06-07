#pragma once
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/LineSegment2.hpp"
#include "Engine/Math/FloatRange.hpp"
#define UNUSED(x) (void)(x)

class NamedStrings;
class DevConsole;
class EventSystem;
class DebugRenderSystem;

typedef bool(*EventSystemCallbackFunction)(NamedStrings const&);
typedef NamedStrings EventArgs;
typedef std::vector<EventSystemCallbackFunction> SubscriptionList;

extern NamedStrings		g_gameConfigBlackboard;				// declared in EngineCommon.hpp, defined in EngineCommon.cpp
extern DevConsole*		g_theConsole;						// declared in EngineCommon.hpp, defined in DevConsole.cpp
extern EventSystem*		g_theEventSystem;					// declared in EngineCommon.hpp, defined in EventSystem.cpp
extern DebugRenderSystem* g_theRenderSystem;				// defined in DebugRenderSystem.cpp
