#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/AABB2.hpp"

struct Mat44;

class Camera
{
public:

	enum Mode
	{
		eMode_Orthographic,
		eMode_Perspective,
		eMode_Count
	};

	void SetOrthographicView(Vec2 const& bottomLeft, Vec2 const& topRight, float cnear = 0.0f, float cfar = 1.0f);
	void SetPerspectiveView(float aspect, float fov, float cnear, float cfar);
	void SetRenderBasis(Vec3 const& iBasis = Vec3(1.0f, 0.0f, 0.0f), Vec3 const& jBasis = Vec3(0.0f, 1.0f, 0.0f), Vec3 const& kBasis = Vec3(0.0f, 0.0f, 1.0f));
	void SetTransform(Vec3 const& cameraPosition, EulerAngles const& orientation);

	Mat44 GetOrthographicMatrix() const;
	Mat44 GetPerspectiveMatrix() const;
	Mat44 GetProjectionMatrix() const;
	Mat44 GetRenderMatrix() const;
	Mat44 GetViewMatrix() const;
	Mat44 GetModelMatrix() const;

public:

	Camera();
	~Camera();

	Vec2 GetOrthoBottomLeft() const;
	Vec2 GetOrthoTopRight() const;
	Vec2 GetOrthoCenter() const;
	Vec2 GetHalfDimension() const;

	Vec3 GetPerspectivePosition() const;
	EulerAngles	GetPerspectiveOrientation() const;
	void SetCameraRenderMode(Mode cameraMode);

	void Translate2D(const Vec2& translation2D);
	void SetDimensions(const Vec2& dimensions);
	void SetCameraPosition(Vec2 const& position);
	void FollowPlayerInBounds(Vec2 const& playerPosition, Vec2 const& worlds_min, Vec2 const& worlds_max);

	AABB2 m_cameraBox = AABB2::ZERO_TO_ONE;
private:
	Mode m_mode = eMode_Orthographic;

	Vec2 m_orthographicBottomLeft;
	Vec2 m_orthographicTopRight;
	float m_orthographicNear;
	float m_orthographicFar;
	Vec2 m_dimension;

	float m_perspectiveAspect;
	float m_perspectiveFOV;
	float m_perspectiveNear;
	float m_perspetiveFar;

	Vec3 m_renderIBasis = Vec3(0.0f, 0.0f, 1.0f);
	Vec3 m_renderJBasis = Vec3(-1.0f, 0.0f, 0.0f);
	Vec3 m_renderKBasis = Vec3(0.0f, 1.0f, 0.0f);

	Vec3 m_position;
	EulerAngles m_orientation;

};
