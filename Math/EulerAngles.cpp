#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/Matrix44.hpp"


EulerAngles::EulerAngles(float yawDegrees, float pitchDegrees, float rollDegrees)
{
	m_yawDegrees = yawDegrees;
	m_pitchDegrees = pitchDegrees;
	m_rollDegrees = rollDegrees;
}

void EulerAngles::GetVectors_XFwd_YLeft_ZUp(Vec3& out_forwardIBasis, Vec3& out_leftJBasis, Vec3& out_upKBasis) const
{
	Mat44 matrix = GetMatrix_XFwd_YLeft_ZUp();

	out_forwardIBasis = matrix.GetIBasis3D();
	out_leftJBasis = matrix.GetJBasis3D();
	out_upKBasis = matrix.GetKBasis3D();
}

Mat44 EulerAngles::GetMatrix_XFwd_YLeft_ZUp() const
{
	Mat44 rotationMatrix = Mat44();

	rotationMatrix.AppendZRotation(m_yawDegrees);
	rotationMatrix.AppendYRotation(m_pitchDegrees);
	rotationMatrix.AppendXRotation(m_rollDegrees);

	return rotationMatrix;
}

Vec3 EulerAngles::GetForwardIBasis() const
{
	Mat44 matrix = GetMatrix_XFwd_YLeft_ZUp();
	return matrix.GetIBasis3D();
}

Vec3 EulerAngles::GetLeftJBasis() const
{
	Mat44 matrix = GetMatrix_XFwd_YLeft_ZUp();
	return matrix.GetJBasis3D();
}

Vec3 EulerAngles::GetUpKBasis() const
{
	Mat44 matrix = GetMatrix_XFwd_YLeft_ZUp();
	return matrix.GetKBasis3D();
}
