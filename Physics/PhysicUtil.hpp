#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/Capsule2.hpp"
#include "Engine/Math/OBB2.hpp"
#include "Engine/Math/LineSegment2.hpp"

void DiscBounceOffFixedCapsule(Vec2& discCenter, float discRadius, Vec2& discVelocity, float discElasticity, Capsule2 const& capsule, float capsuleElasticity);
void DiscBounceOffFixedOBB(Vec2& discCenter, float discRadius, Vec2& discVelocity, float discElasticity, OBB2 const& obb, float obbElasticity);
void DiscBounceOffFixedLineSegment(Vec2& discCenter, float discRadius, Vec2& discVelocity, float discElasticity, LineSegment2 const& lineSegment, float lineElasticity);
void DiscBounceOffFixedDisc(Vec2& discCenter, float discRadius, Vec2& discVelocity, float discElasticity, Vec2 const& fixedDiscCenter, float fixedDiscRadius, float fixedDiscElasticity);
void DiscBounceOffEachOther(Vec2& discCenter1, float discRadius1, Vec2& discVelocity1, float discElasticity1, Vec2& discCenter2, float discRadius2, Vec2& discVelocity2, float discElasticity2);
void DiscBounceOffWall(Vec2& discVelocity1, float discElasticity1, Vec2 const& wallNormal, float wallElasticity);
