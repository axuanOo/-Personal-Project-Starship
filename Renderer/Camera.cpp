#include "Engine/Renderer/Camera.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Matrix44.hpp"
#define  WIN32_LEAN_AND_MEAN		// Always define this before #including <windows.h>
#include <windows.h>

void Camera::SetOrthographicView(Vec2 const& bottomLeft, Vec2 const& topRight, float cnear, float cfar)
{
	m_mode = eMode_Orthographic;
	m_orthographicBottomLeft = bottomLeft;
	m_orthographicTopRight = topRight;
	m_orthographicNear = cnear;
	m_orthographicFar = cfar;
	m_dimension = topRight - bottomLeft;
}

void Camera::SetPerspectiveView(float aspect, float fov, float cnear, float cfar)
{
	m_mode = eMode_Perspective;
	m_perspectiveAspect = aspect;
	m_perspectiveFOV = fov;
	m_perspectiveNear = cnear;
	m_perspetiveFar = cfar;

}

Mat44 Camera::GetOrthographicMatrix() const
{
	return Mat44::CreateOrthoProjection(m_orthographicBottomLeft.x, m_orthographicTopRight.x, m_orthographicBottomLeft.y, m_orthographicTopRight.y, m_orthographicNear, m_orthographicFar);
}

Mat44 Camera::GetPerspectiveMatrix() const
{
	return Mat44::CreatePerspectiveProjection(m_perspectiveFOV,m_perspectiveAspect,m_perspectiveNear,m_perspetiveFar);
}

Mat44 Camera::GetProjectionMatrix() const
{
	Mat44 projectMatrix;
	if (m_mode==eMode_Orthographic)
	{
		projectMatrix=Camera::GetOrthographicMatrix();
	}
	else 
	{
		projectMatrix=Camera::GetPerspectiveMatrix();
		projectMatrix.Append(GetRenderMatrix());
	}

	return projectMatrix;
}

Camera::Camera()
{
}

Camera::~Camera()
{

}

void Camera::SetCameraPosition(Vec2 const& position)
{
	Vec2 cameraCenter = GetOrthoCenter();
	Vec2 displacement = position - cameraCenter;
	Translate2D(displacement);
}

void Camera::FollowPlayerInBounds(Vec2 const& playerPosition, Vec2 const& worlds_min, Vec2 const& worlds_max)
{
	Vec2 centerPosition = playerPosition;

	float halfDimensionX = GetHalfDimension().x;
	float halfDimensionY = GetHalfDimension().y;

	centerPosition.x = GetClamped(centerPosition.x, worlds_min.x + halfDimensionX, worlds_max.x - halfDimensionX);
	centerPosition.y = GetClamped(centerPosition.y, worlds_min.y + halfDimensionY, worlds_max.y - halfDimensionY);

	m_orthographicBottomLeft = centerPosition - GetHalfDimension();
	m_orthographicTopRight = centerPosition + GetHalfDimension();
}

Vec2 Camera::GetOrthoBottomLeft() const
{
	return m_orthographicBottomLeft;
}

Vec2 Camera::GetOrthoTopRight() const
{
	return m_orthographicTopRight;
}

Vec2 Camera::GetOrthoCenter() const
{
	Vec2 center = m_orthographicBottomLeft;
	center += m_orthographicTopRight;
	center *= 0.5f;
	return center;
}

Vec2 Camera::GetHalfDimension() const
{
	return Vec2(m_dimension.x * 0.5f, m_dimension.y * 0.5f);
}

Vec3 Camera::GetPerspectivePosition() const
{
	return m_position;
}

EulerAngles Camera::GetPerspectiveOrientation() const
{
	return m_orientation;
}

void Camera::SetCameraRenderMode(Mode cameraMode)
{
	m_mode = cameraMode;
}

void Camera::SetRenderBasis(Vec3 const& iBasis /*= Vec3(1.0f, 0.0f, 0.0f)*/, Vec3 const& jBasis /*= Vec3(0.0f, 1.0f, 0.0f)*/, Vec3 const& kBasis /*= Vec3(0.0f, 0.0f, 1.0f)*/)
{
	m_renderIBasis = iBasis;
	m_renderJBasis = jBasis;
	m_renderKBasis = kBasis;
}

void Camera::SetTransform(Vec3 const& cameraPosition, EulerAngles const& orientation)
{
	m_position = cameraPosition;
	m_orientation = orientation;
}

Mat44 Camera::GetRenderMatrix() const
{
	Mat44 renderMatrix = Mat44();

	renderMatrix.SetIJK3D(m_renderIBasis, m_renderJBasis, m_renderKBasis);

	return renderMatrix;
}

Mat44 Camera::GetViewMatrix() const
{
	Mat44 viewMatrix;

	Vec3 iBasisView;
	Vec3 jBasisView;
	Vec3 kBasisView;

	m_orientation.GetVectors_XFwd_YLeft_ZUp(iBasisView, jBasisView, kBasisView);
	viewMatrix.SetIJKT3D(iBasisView, jBasisView, kBasisView, m_position);
	return viewMatrix.GetOrthonormalInverse();
}

Mat44 Camera::GetModelMatrix() const
{
	Mat44 modelMatrix;

	Vec3 iBasisView;
	Vec3 jBasisView;
	Vec3 kBasisView;

	m_orientation.GetVectors_XFwd_YLeft_ZUp(iBasisView, jBasisView, kBasisView);
	modelMatrix.SetIJKT3D(iBasisView, jBasisView, kBasisView, m_position);
	return modelMatrix;
}

void Camera::Translate2D(const Vec2& translation2D)
{
	Vec2 bottomLeft = GetOrthoBottomLeft() + translation2D;
	Vec2 topRight = GetOrthoTopRight() + translation2D;
	SetOrthographicView(bottomLeft, topRight);
}

void Camera::SetDimensions(const Vec2& dimensions)
{
	m_dimension = dimensions;
}
