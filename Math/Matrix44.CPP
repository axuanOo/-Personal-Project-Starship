#include "Engine/Math/Matrix44.hpp"
#include "Engine/Math/MathUtils.hpp"

Mat44::Mat44()
{
	m_value[Ix] = 1.0f;
	m_value[Iy] = 0.0f;
	m_value[Iz] = 0.0f;
	m_value[Iw] = 0.0f;

	m_value[Jx] = 0.0f;
	m_value[Jy] = 1.0f;
	m_value[Jz] = 0.0f;
	m_value[Jw] = 0.0f;

	m_value[Kx] = 0.0f;
	m_value[Ky] = 0.0f;
	m_value[Kz] = 1.0f;
	m_value[Kw] = 0.0f;

	m_value[Tx] = 0.0f;
	m_value[Ty] = 0.0f;
	m_value[Tz] = 0.0f;
	m_value[Tw] = 1.0f;
}

Mat44::Mat44(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 const& translation2D)
{
	m_value[Ix] = iBasis2D.x;
	m_value[Iy] = iBasis2D.y;
	m_value[Iz] = 0.0f;
	m_value[Iw] = 0.0f;

	m_value[Jx] = jBasis2D.x;
	m_value[Jy] = jBasis2D.y;
	m_value[Jz] = 0.0f;
	m_value[Jw] = 0.0f;

	m_value[Kx] = 0.0f;
	m_value[Ky] = 0.0f;
	m_value[Kz] = 1.0f;
	m_value[Kw] = 0.0f;

	m_value[Tx] = translation2D.x;
	m_value[Ty] = translation2D.y;
	m_value[Tz] = 0.0f;
	m_value[Tw] = 1.0f;
}

Mat44::Mat44(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translation3D)
{
	m_value[Ix] = iBasis3D.x;
	m_value[Iy] = iBasis3D.y;
	m_value[Iz] = iBasis3D.z;
	m_value[Iw] = 0.0f;

	m_value[Jx] = jBasis3D.x;
	m_value[Jy] = jBasis3D.y;
	m_value[Jz] = jBasis3D.z;
	m_value[Jw] = 0.0f;

	m_value[Kx] = kBasis3D.x;
	m_value[Ky] = kBasis3D.y;
	m_value[Kz] = kBasis3D.z;
	m_value[Kw] = 0.0f;

	m_value[Tx] = translation3D.x;
	m_value[Ty] = translation3D.y;
	m_value[Tz] = translation3D.z;
	m_value[Tw] = 1.0f;
}

Mat44::Mat44(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translation4D)
{
	m_value[Ix] = iBasis4D.x;
	m_value[Iy] = iBasis4D.y;
	m_value[Iz] = iBasis4D.z;
	m_value[Iw] = iBasis4D.w;

	m_value[Jx] = jBasis4D.x;
	m_value[Jy] = jBasis4D.y;
	m_value[Jz] = jBasis4D.z;
	m_value[Jw] = jBasis4D.w;

	m_value[Kx] = kBasis4D.x;
	m_value[Ky] = kBasis4D.y;
	m_value[Kz] = kBasis4D.z;
	m_value[Kw] = kBasis4D.w;

	m_value[Tx] = translation4D.x;
	m_value[Ty] = translation4D.y;
	m_value[Tz] = translation4D.z;
	m_value[Tw] = translation4D.w;
}

Mat44::Mat44(float const* sixteenValuesBasisMajor)
{
	for (int i = 0; i < 16; i++)
	{
		m_value[i] = sixteenValuesBasisMajor[i];
	}
}

void Mat44::Reset()
{
	m_value[Ix] = 1.0f;
	m_value[Iy] = 0.0f;
	m_value[Iz] = 0.0f;
	m_value[Iw] = 0.0f;

	m_value[Jx] = 0.0f;
	m_value[Jy] = 1.0f;
	m_value[Jz] = 0.0f;
	m_value[Jw] = 0.0f;

	m_value[Kx] = 0.0f;
	m_value[Ky] = 0.0f;
	m_value[Kz] = 1.0f;
	m_value[Kw] = 0.0f;

	m_value[Tx] = 0.0f;
	m_value[Ty] = 0.0f;
	m_value[Tz] = 0.0f;
	m_value[Tw] = 1.0f;

}

Mat44 const Mat44::CreateTranslation2D(Vec2 const& translationXY)
{
	Mat44 matrix = Mat44();

	matrix.m_value[Tx] = translationXY.x;
	matrix.m_value[Ty] = translationXY.y;

	return matrix;
}

Mat44 const Mat44::CreateTranslation3D(Vec3 const& translationXYZ)
{
	Mat44 matrix = Mat44();

	matrix.m_value[Tx] = translationXYZ.x;
	matrix.m_value[Ty] = translationXYZ.y;
	matrix.m_value[Tz] = translationXYZ.z;

	return matrix;
}

Mat44 const Mat44::CreateUniformScale2D(float uniformScaleXY)
{
	Mat44 matrix = Mat44();

	matrix.m_value[Ix] = uniformScaleXY;
	matrix.m_value[Jy] = uniformScaleXY;

	return matrix;
}

Mat44 const Mat44::CreateUniformScale3D(float uniformScaleXYZ)
{
	Mat44 matrix = Mat44();

	matrix.m_value[Ix] = uniformScaleXYZ;
	matrix.m_value[Jy] = uniformScaleXYZ;
	matrix.m_value[Kz] = uniformScaleXYZ;

	return matrix;
}

Mat44 const Mat44::CreateNonUniformScale2D(Vec2 const& nonUniformScaleXY)
{
	Mat44 matrix = Mat44();

	matrix.m_value[Ix] = nonUniformScaleXY.x;
	matrix.m_value[Jy] = nonUniformScaleXY.y;

	return matrix;
}

Mat44 const Mat44::CreateNonUniformScale3D(Vec3 const& nonUniformScaleXYZ)
{
	Mat44 matrix = Mat44();

	matrix.m_value[Ix] = nonUniformScaleXYZ.x;
	matrix.m_value[Jy] = nonUniformScaleXYZ.y;
	matrix.m_value[Kz] = nonUniformScaleXYZ.z;

	return matrix;
}

Mat44 const Mat44::CreateZRotationDegrees(float rotationDegreesAboutZ)
{
	Mat44 matrix = Mat44();

	float cosineZRotation = CosDegrees(rotationDegreesAboutZ);
	float sineZRotation = SinDegrees(rotationDegreesAboutZ);

	matrix.m_value[Ix] = cosineZRotation;
	matrix.m_value[Iy] = sineZRotation;
	matrix.m_value[Jx] = -sineZRotation;
	matrix.m_value[Jy] = cosineZRotation;


	return matrix;
}

Mat44 const Mat44::CreateXRotationDegrees(float rotationDegreesAboutX)
{
	Mat44 matrix = Mat44();

	float cosineZRotation = CosDegrees(rotationDegreesAboutX);
	float sineZRotation = SinDegrees(rotationDegreesAboutX);

	matrix.m_value[Jy] = cosineZRotation;
	matrix.m_value[Jz] = sineZRotation;
	matrix.m_value[Ky] = -sineZRotation;
	matrix.m_value[Kz] = cosineZRotation;

	return matrix;
}

Mat44 const Mat44::CreateYRotationDegrees(float rotationDegreesAboutY)
{
	Mat44 matrix = Mat44();

	float cosineZRotation = CosDegrees(rotationDegreesAboutY);
	float sineZRotation = SinDegrees(rotationDegreesAboutY);

	matrix.m_value[Ix] = cosineZRotation;
	matrix.m_value[Iz] = -sineZRotation;
	matrix.m_value[Kx] = sineZRotation;
	matrix.m_value[Kz] = cosineZRotation;

	return matrix;
}

Mat44 const Mat44::CreateOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Mat44 ortho;

	ortho.m_value[Ix] = 2.0f / (right - left);
	ortho.m_value[Jy] = 2.0f / (top - bottom);
	ortho.m_value[Kz] = 1.0f / (zFar - zNear);

	ortho.m_value[Tx] = (left + right) / (left - right);
	ortho.m_value[Ty] = (bottom + top) / (bottom - top);
	ortho.m_value[Tz] = (-zNear) / (zFar - zNear);

	return ortho;
}

Mat44 const Mat44::CreatePerspectiveProjection(float fovYDegrees, float aspect, float zNear, float zFar)
{
	Mat44 perspective;

	float scaleY = CosDegrees(fovYDegrees * 0.5f) / SinDegrees(fovYDegrees * 0.5f);
	float scaleX = scaleY / aspect;
	float scaleZ = zFar / (zFar - zNear);
	perspective.m_value[Ix] = scaleX; // equals scaleY if screen is square (aspect=1.0); equals 1 if square screen and FOV 90
	perspective.m_value[Jy] = scaleY;  // equals 1 if vertical Field of View is 90
	perspective.m_value[Kz] = scaleZ;
	perspective.m_value[Kw] = 1.0f; // this puts Z into the W component (in preparation for the hardware w-divide)
	perspective.m_value[Tz] = (zNear * zFar) / (zNear - zFar);
	perspective.m_value[Tw] = 0.0f;  // Otherwise we would be putting Z+1 (instead of Z) into the W component

	return perspective;
}

Vec2 const Mat44::TransformVectorQuantity2D(Vec2 const& vectorQuantityXY) const
{
	float newX = vectorQuantityXY.x * m_value[Ix] + vectorQuantityXY.y * m_value[Jx];
	float newY = vectorQuantityXY.x * m_value[Iy] + vectorQuantityXY.y * m_value[Jy];

	return Vec2(newX, newY);
}

Vec3 const Mat44::TransformVectorQuantity3D(Vec3 const& vectorQuantityXYZ) const
{
	float newX = vectorQuantityXYZ.x * m_value[Ix] + vectorQuantityXYZ.y * m_value[Jx] + vectorQuantityXYZ.z * m_value[Kx];
	float newY = vectorQuantityXYZ.x * m_value[Iy] + vectorQuantityXYZ.y * m_value[Jy] + vectorQuantityXYZ.z * m_value[Ky];
	float newZ = vectorQuantityXYZ.x * m_value[Iz] + vectorQuantityXYZ.y * m_value[Jz] + vectorQuantityXYZ.z * m_value[Kz];
	
	return Vec3(newX, newY, newZ);
}

Vec2 const Mat44::TransformPosition2D(Vec2 const& positionXY) const
{
	float newX = m_value[Ix] * positionXY.x + m_value[Jx] * positionXY.y + m_value[Tx];
	float newY = m_value[Iy] * positionXY.x + m_value[Jy] * positionXY.y + m_value[Ty];

	return Vec2(newX, newY);
}

Vec3 const Mat44::TransformPosition3D(Vec3 const& position3D) const
{
	float newX = m_value[Ix] * position3D.x + m_value[Jx] * position3D.y + m_value[Kx] * position3D.z + m_value[Tx];
	float newY = m_value[Iy] * position3D.x + m_value[Jy] * position3D.y + m_value[Ky] * position3D.z + m_value[Ty];
	float newZ = m_value[Iz] * position3D.x + m_value[Jz] * position3D.y + m_value[Kz] * position3D.z + m_value[Tz];

	return Vec3(newX, newY, newZ);
}

Vec4 const Mat44::TransformHomogeneous3D(Vec4 const& homogeneousPoint3D) const
{
	float newX = homogeneousPoint3D.x * m_value[Ix] + homogeneousPoint3D.y * m_value[Jx] + homogeneousPoint3D.z * m_value[Kx] + homogeneousPoint3D.w * m_value[Tx];
	float newY = homogeneousPoint3D.x * m_value[Iy] + homogeneousPoint3D.y * m_value[Jy] + homogeneousPoint3D.z * m_value[Ky] + homogeneousPoint3D.w * m_value[Ty];
	float newZ = homogeneousPoint3D.x * m_value[Iz] + homogeneousPoint3D.y * m_value[Jz] + homogeneousPoint3D.z * m_value[Kz] + homogeneousPoint3D.w * m_value[Tz];
	float newW = homogeneousPoint3D.x * m_value[Iw] + homogeneousPoint3D.y * m_value[Jw] + homogeneousPoint3D.z * m_value[Kw] + homogeneousPoint3D.w * m_value[Tw];
	
	return Vec4(newX, newY, newZ, newW);
}

float* Mat44::GetAsFloatArray()
{
	return m_value;
}

float const* Mat44::GetAsFloatArray() const
{
	return m_value;
}

Vec2 const Mat44::GetIBasis2D() const
{
	return Vec2(m_value[Ix], m_value[Iy]);
}

Vec2 const Mat44::GetJBasis2D() const
{
	return Vec2(m_value[Jx], m_value[Jy]);
}

Vec2 const Mat44::GetTranslation2D() const
{
	return Vec2(m_value[Tx], m_value[Ty]);
}

Vec3 const Mat44::GetIBasis3D() const
{
	return Vec3(m_value[Ix], m_value[Iy], m_value[Iz]);
}

Vec3 const Mat44::GetJBasis3D() const
{
	return Vec3(m_value[Jx], m_value[Jy], m_value[Jz]);
}

Vec3 const Mat44::GetKBasis3D() const
{
	return Vec3(m_value[Kx], m_value[Ky], m_value[Kz]);
}

Vec3 const Mat44::GetTranslation3D() const
{
	return Vec3(m_value[Tx], m_value[Ty], m_value[Tz]);
}

Vec4 const Mat44::GetIBasis4D() const
{
	return Vec4(m_value[Ix], m_value[Iy], m_value[Iz], m_value[Iw]);
}

Vec4 const Mat44::GetJBasis4D() const
{
	return Vec4(m_value[Jx], m_value[Jy], m_value[Jz], m_value[Jw]);
}

Vec4 const Mat44::GetKBasis4D() const
{
	return Vec4(m_value[Kx], m_value[Ky], m_value[Kz], m_value[Kw]);
}

Vec4 const Mat44::GetTranslation4D() const
{
	return Vec4(m_value[Tx], m_value[Ty], m_value[Tz], m_value[Tw]);
}

Mat44 const Mat44::GetOrthonormalInverse() const
{
	Mat44 rotationOnly = Mat44(GetIBasis4D(), GetJBasis4D(), GetKBasis4D(), Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	Mat44 translationOnly = Mat44(Vec4(1.0f, 0.0f, 0.0f, 0.0f), Vec4(0.0f, 1.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 1.0f, 0.0f), Vec4(-m_value[Tx],-m_value[Ty],-m_value[Tz],1.0f));
	rotationOnly.Transpose();
	rotationOnly.Append(translationOnly);
	return rotationOnly;
}

void Mat44::SetTranslation2D(Vec2 const& translationXY)
{
	m_value[Tx] = translationXY.x;
	m_value[Ty] = translationXY.y;
	m_value[Tz] = 0.0f;
	m_value[Tw] = 1.0f;
}

void Mat44::SetTranslation3D(Vec3 const& translationXYZ)
{
	m_value[Tx] = translationXYZ.x;
	m_value[Ty] = translationXYZ.y;
	m_value[Tz] = translationXYZ.z;
	m_value[Tw] = 1.0f;

}

void Mat44::SetIJ2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D)
{
	m_value[Ix] = iBasis2D.x;
	m_value[Iy] = iBasis2D.y;
	m_value[Iz] = 0.0f;
	m_value[Iw] = 0.0f;

	m_value[Jx] = jBasis2D.x;
	m_value[Jy] = jBasis2D.y;
	m_value[Jz] = 0.0f;
	m_value[Jw] = 0.0f;
}

void Mat44::SetIJT2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 const& translationXY)
{
	m_value[Ix] = iBasis2D.x;
	m_value[Iy] = iBasis2D.y;
	m_value[Iz] = 0.0f;
	m_value[Iw] = 0.0f;

	m_value[Jx] = jBasis2D.x;
	m_value[Jy] = jBasis2D.y;
	m_value[Jz] = 0.0f;
	m_value[Jw] = 0.0f;

	m_value[Tx] = translationXY.x;
	m_value[Ty] = translationXY.y;
	m_value[Tz] = 0.0f;
	m_value[Tw] = 1.0f;
}

void Mat44::SetIJK3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D)
{
	m_value[Ix] = iBasis3D.x;
	m_value[Iy] = iBasis3D.y;
	m_value[Iz] = iBasis3D.z;
	m_value[Iw] = 0.0f;

	m_value[Jx] = jBasis3D.x;
	m_value[Jy] = jBasis3D.y;
	m_value[Jz] = jBasis3D.z;
	m_value[Jw] = 0.0f;

	m_value[Kx] = kBasis3D.x;
	m_value[Ky] = kBasis3D.y;
	m_value[Kz] = kBasis3D.z;
	m_value[Kw] = 0.0f;
}

void Mat44::SetIJKT3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translationXYZ)
{
	m_value[Ix] = iBasis3D.x;
	m_value[Iy] = iBasis3D.y;
	m_value[Iz] = iBasis3D.z;
	m_value[Iw] = 0.0f;

	m_value[Jx] = jBasis3D.x;
	m_value[Jy] = jBasis3D.y;
	m_value[Jz] = jBasis3D.z;
	m_value[Jw] = 0.0f;

	m_value[Kx] = kBasis3D.x;
	m_value[Ky] = kBasis3D.y;
	m_value[Kz] = kBasis3D.z;
	m_value[Kw] = 0.0f;

	m_value[Tx] = translationXYZ.x;
	m_value[Ty] = translationXYZ.y;
	m_value[Tz] = translationXYZ.z;
	m_value[Tw] = 1.0f;

}

void Mat44::SetIJKT4D(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translation4D)
{
	m_value[Ix] = iBasis4D.x;
	m_value[Iy] = iBasis4D.y;
	m_value[Iz] = iBasis4D.z;
	m_value[Iw] = iBasis4D.w;

	m_value[Jx] = jBasis4D.x;
	m_value[Jy] = jBasis4D.y;
	m_value[Jz] = jBasis4D.z;
	m_value[Jw] = jBasis4D.w;

	m_value[Kx] = kBasis4D.x;
	m_value[Ky] = kBasis4D.y;
	m_value[Kz] = kBasis4D.z;
	m_value[Kw] = kBasis4D.w;

	m_value[Tx] = translation4D.x;
	m_value[Ty] = translation4D.y;
	m_value[Tz] = translation4D.z;
	m_value[Tw] = translation4D.w;
}

void Mat44::Transpose()
{
	Mat44 clone = Mat44(GetIBasis4D(), GetJBasis4D(), GetKBasis4D(), GetTranslation4D());

	Vec4 newIBasis = Vec4(clone.GetAsFloatArray()[Ix], clone.GetAsFloatArray()[Jx], clone.GetAsFloatArray()[Kx], clone.GetAsFloatArray()[Tx]);
	Vec4 newJBasis = Vec4(clone.GetAsFloatArray()[Iy], clone.GetAsFloatArray()[Jy], clone.GetAsFloatArray()[Ky], clone.GetAsFloatArray()[Ty]);
	Vec4 newKBasis = Vec4(clone.GetAsFloatArray()[Iz], clone.GetAsFloatArray()[Jz], clone.GetAsFloatArray()[Kz], clone.GetAsFloatArray()[Tz]);
	Vec4 newTBasis = Vec4(clone.GetAsFloatArray()[Iw], clone.GetAsFloatArray()[Jw], clone.GetAsFloatArray()[Kw], clone.GetAsFloatArray()[Tw]);

	SetIJKT4D(newIBasis, newJBasis, newKBasis, newTBasis);
}

void Mat44::Orthonormalize_IFwd_JLeft_KUp()
{
	Vec3 IBasis = GetIBasis3D().GetNormalized();
	Vec3 KBasis = GetKBasis3D();

	Vec3 badKBasisOnIBasis = GetProjectedOnto3D(KBasis, IBasis);
	KBasis = (KBasis - badKBasisOnIBasis).GetNormalized();

	Vec3 JBasis = CrossProduct3D(IBasis, KBasis);

	SetIJK3D(IBasis, JBasis, KBasis);
}

void Mat44::Append(Mat44 const& appendThis)
{
	float newIx = m_value[Ix] * appendThis.m_value[Ix] + m_value[Jx] * appendThis.m_value[Iy] + m_value[Kx] * appendThis.m_value[Iz] + m_value[Tx] * appendThis.m_value[Iw];
	float newIy = m_value[Iy] * appendThis.m_value[Ix] + m_value[Jy] * appendThis.m_value[Iy] + m_value[Ky] * appendThis.m_value[Iz] + m_value[Ty] * appendThis.m_value[Iw];
	float newIz = m_value[Iz] * appendThis.m_value[Ix] + m_value[Jz] * appendThis.m_value[Iy] + m_value[Kz] * appendThis.m_value[Iz] + m_value[Tz] * appendThis.m_value[Iw];
	float newIw = m_value[Iw] * appendThis.m_value[Ix] + m_value[Jw] * appendThis.m_value[Iy] + m_value[Kw] * appendThis.m_value[Iz] + m_value[Tw] * appendThis.m_value[Iw];

	float newJx = m_value[Ix] * appendThis.m_value[Jx] + m_value[Jx] * appendThis.m_value[Jy] + m_value[Kx] * appendThis.m_value[Jz] + m_value[Tx] * appendThis.m_value[Jw];
	float newJy = m_value[Iy] * appendThis.m_value[Jx] + m_value[Jy] * appendThis.m_value[Jy] + m_value[Ky] * appendThis.m_value[Jz] + m_value[Ty] * appendThis.m_value[Jw];
	float newJz = m_value[Iz] * appendThis.m_value[Jx] + m_value[Jz] * appendThis.m_value[Jy] + m_value[Kz] * appendThis.m_value[Jz] + m_value[Tz] * appendThis.m_value[Jw];
	float newJw = m_value[Iw] * appendThis.m_value[Jx] + m_value[Jw] * appendThis.m_value[Jy] + m_value[Kw] * appendThis.m_value[Jz] + m_value[Tw] * appendThis.m_value[Jw];

	float newKx = m_value[Ix] * appendThis.m_value[Kx] + m_value[Jx] * appendThis.m_value[Ky] + m_value[Kx] * appendThis.m_value[Kz] + m_value[Tx] * appendThis.m_value[Kw];
	float newKy = m_value[Iy] * appendThis.m_value[Kx] + m_value[Jy] * appendThis.m_value[Ky] + m_value[Ky] * appendThis.m_value[Kz] + m_value[Ty] * appendThis.m_value[Kw];
	float newKz = m_value[Iz] * appendThis.m_value[Kx] + m_value[Jz] * appendThis.m_value[Ky] + m_value[Kz] * appendThis.m_value[Kz] + m_value[Tz] * appendThis.m_value[Kw];
	float newKw = m_value[Iw] * appendThis.m_value[Kx] + m_value[Jw] * appendThis.m_value[Ky] + m_value[Kw] * appendThis.m_value[Kz] + m_value[Tw] * appendThis.m_value[Kw];

	float newTx = m_value[Ix] * appendThis.m_value[Tx] + m_value[Jx] * appendThis.m_value[Ty] + m_value[Kx] * appendThis.m_value[Tz] + m_value[Tx] * appendThis.m_value[Tw];
	float newTy = m_value[Iy] * appendThis.m_value[Tx] + m_value[Jy] * appendThis.m_value[Ty] + m_value[Ky] * appendThis.m_value[Tz] + m_value[Ty] * appendThis.m_value[Tw];
	float newTz = m_value[Iz] * appendThis.m_value[Tx] + m_value[Jz] * appendThis.m_value[Ty] + m_value[Kz] * appendThis.m_value[Tz] + m_value[Tz] * appendThis.m_value[Tw];
	float newTw = m_value[Iw] * appendThis.m_value[Tx] + m_value[Jw] * appendThis.m_value[Ty] + m_value[Kw] * appendThis.m_value[Tz] + m_value[Tw] * appendThis.m_value[Tw];

	m_value[Ix] = newIx;
	m_value[Iy] = newIy;
	m_value[Iz] = newIz;
	m_value[Iw] = newIw;

	m_value[Jx] = newJx;
	m_value[Jy] = newJy;
	m_value[Jz] = newJz;
	m_value[Jw] = newJw;

	m_value[Kx] = newKx;
	m_value[Ky] = newKy;
	m_value[Kz] = newKz;
	m_value[Kw] = newKw;

	m_value[Tx] = newTx;
	m_value[Ty] = newTy;
	m_value[Tz] = newTz;
	m_value[Tw] = newTw;
}

void Mat44::AppendZRotation(float degreesRotationAboutZ)
{
	Mat44 matrix = Mat44::CreateZRotationDegrees(degreesRotationAboutZ);

	Append(matrix);
}

void Mat44::AppendYRotation(float degreesRotationAboutY)
{
	Mat44 matrix = Mat44::CreateYRotationDegrees(degreesRotationAboutY);

	Append(matrix);
}

void Mat44::AppendXRotation(float degreesRotationAboutX)
{
	Mat44 matrix = Mat44::CreateXRotationDegrees(degreesRotationAboutX);

	Append(matrix);
}

void Mat44::AppendTranslation2D(Vec2 const& tranlationXY)
{
	Mat44 matrix = Mat44::CreateTranslation2D(tranlationXY);

	Append(matrix);
}

void Mat44::AppendTranslation3D(Vec3 const& tranlationXYZ)
{
	Mat44 matrix = Mat44::CreateTranslation3D(tranlationXYZ);

	Append(matrix);
}

void Mat44::AppendScaleUniform2D(float uniformScaleXY)
{
	Mat44 matrix = Mat44::CreateUniformScale2D(uniformScaleXY);

	Append(matrix);
}

void Mat44::AppendScaleUniform3D(float uniformScaleXYZ)
{
	Mat44 matrix = Mat44::CreateUniformScale3D(uniformScaleXYZ);

	Append(matrix);

}

void Mat44::AppendScaleNonUniform2D(Vec2 const& nonUniformScaleXY)
{
	Mat44 matrix = Mat44::CreateNonUniformScale2D(nonUniformScaleXY);
	Append(matrix);

}

void Mat44::AppendScaleNonUniform3D(Vec3 const& nonUniformScaleXYZ)
{
	Mat44 matrix = Mat44::CreateNonUniformScale3D(nonUniformScaleXYZ);
	Append(matrix);

}


