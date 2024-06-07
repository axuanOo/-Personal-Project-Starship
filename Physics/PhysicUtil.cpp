#include "Engine/Physics/PhysicUtil.hpp"
#include "Engine/Math/MathUtils.hpp"


void DiscBounceOffFixedCapsule(Vec2& discCenter, float discRadius, Vec2& discVelocity, float discElasticity, Capsule2 const& capsule, float capsuleElasticity)
{
	Vec2 nearestPoint = GetNearestPointOnCapsule2D(discCenter, capsule);

	if (PushDiscOutOfFixedPoint2D(discCenter, discRadius, nearestPoint))
	{
		Vec2 hitNormal = (discCenter - nearestPoint).GetNormalized();

		Vec2 velocityAlongNormal = DotProduct2D(discVelocity, hitNormal) * hitNormal;
		Vec2 velocityPerpendicular = discVelocity - velocityAlongNormal;
		velocityAlongNormal *= -1.0f * discElasticity * capsuleElasticity;

		discVelocity = velocityAlongNormal + velocityPerpendicular;
	}

}

void DiscBounceOffFixedOBB(Vec2& discCenter, float discRadius, Vec2& discVelocity, float discElasticity, OBB2 const& obb, float obbElasticity)
{
	Vec2 nearestPoint = GetNearestPointOnOBB2D(discCenter, obb);

	if (PushDiscOutOfFixedPoint2D(discCenter, discRadius, nearestPoint))
	{
		Vec2 hitNormal = (discCenter - nearestPoint).GetNormalized();

		Vec2 velocityAlongNormal = DotProduct2D(discVelocity, hitNormal) * hitNormal;
		Vec2 velocityPerpendicular = discVelocity - velocityAlongNormal;
		velocityAlongNormal *= -1.0f * discElasticity * obbElasticity;

		discVelocity = velocityAlongNormal + velocityPerpendicular;
	}
}

void DiscBounceOffFixedLineSegment(Vec2& discCenter, float discRadius, Vec2& discVelocity, float discElasticity, LineSegment2 const& lineSegment, float lineElasticity)
{
	Vec2 nearestPoint = Vec2::ZERO;
	nearestPoint = GetNearestPointLineSegment2D(discCenter, lineSegment);
	if (PushDiscOutOfFixedPoint2D(discCenter, discRadius, nearestPoint))
	{
		Vec2 hitNormal = (discCenter - nearestPoint).GetNormalized();

		Vec2 velocityAlongNormal = DotProduct2D(discVelocity, hitNormal) * hitNormal;
		Vec2 velocityPerpendicular = discVelocity - velocityAlongNormal;
		velocityAlongNormal *= -1.0f * discElasticity * lineElasticity;

		discVelocity = velocityAlongNormal + velocityPerpendicular;
	}
}

void DiscBounceOffFixedDisc(Vec2& discCenter, float discRadius, Vec2& discVelocity, float discElasticity, Vec2 const& fixedDiscCenter, float fixedDiscRadius, float fixedDiscElasticity)
{
	if (PushDiscOutOfFixedDisc2D(discCenter, discRadius, fixedDiscCenter, fixedDiscRadius))
	{
		Vec2 hitNormal = (discCenter - fixedDiscCenter).GetNormalized();

		Vec2 velocityAlongNormal = DotProduct2D(discVelocity, hitNormal) * hitNormal;
		Vec2 velocityPerpendicular = discVelocity - velocityAlongNormal;

		velocityAlongNormal *= -1.0f * discElasticity * fixedDiscElasticity;

		discVelocity = velocityAlongNormal + velocityPerpendicular;
	}

}

void DiscBounceOffEachOther(Vec2& discCenter1, float discRadius1, Vec2& discVelocity1, float discElasticity1, Vec2& discCenter2, float discRadius2, Vec2& discVelocity2, float discElasticity2)
{
	if (PushDiscOutOfEachOther2D(discCenter1, discRadius1, discCenter2, discRadius2))
	{
		Vec2 hitNormal = (discCenter2 - discCenter1).GetNormalized();

		float velocity1AlongNormalMagnitude = DotProduct2D(discVelocity1, hitNormal);
		Vec2 velocity1AlongNormal = velocity1AlongNormalMagnitude * hitNormal;
		Vec2 velocity1Perpendicular = discVelocity1 - velocity1AlongNormal;
		float velocity2AlongNormalMagnitude = DotProduct2D(discVelocity2, hitNormal);
		Vec2 velocity2AlongNormal = velocity2AlongNormalMagnitude * hitNormal;
		Vec2 velocity2Perpendicular = discVelocity2 - velocity2AlongNormal;

		if (velocity2AlongNormalMagnitude - velocity1AlongNormalMagnitude < 0.0f)
		{
			Vec2 tmp = velocity1AlongNormal;
			velocity1AlongNormal = velocity2AlongNormal;
			velocity2AlongNormal = tmp;
			velocity1AlongNormal *= discElasticity1;
			velocity1AlongNormal *= discElasticity2;

			velocity2AlongNormal *= discElasticity1;
			velocity2AlongNormal *= discElasticity2;

			discVelocity1 = velocity1AlongNormal + velocity1Perpendicular;
			discVelocity2 = velocity2AlongNormal + velocity2Perpendicular;
		}


	}

}
void DiscBounceOffWall(Vec2& discVelocity1, float discElasticity1, Vec2 const& wallNormal, float wallElasticity)
{
	Vec2 velocityAlongNormal = GetProjectedOnto2D(discVelocity1, wallNormal);
	Vec2 velocityPerpendicular = discVelocity1 - velocityAlongNormal;
	velocityAlongNormal *= -1.0f * discElasticity1 * wallElasticity;
	discVelocity1 = velocityAlongNormal + velocityPerpendicular;
}
