#include "Quaternion.h"
#include <algorithm>
#include <cmath>

//namespace ath
//{

	// Define and Initialise static members of class Quaternion.
	Quaternion& Quaternion::Identity = Quaternion::GetIdentity();


	const Vector2& Quaternion::xy() const
	{
		Vector2 result(x, y);
		return result;
	}

	void Quaternion::xy(const Vector2& value)
	{
		x = value.x;
		y = value.y;
	}

	const Vector3& Quaternion::xyz() const
	{
		return Vector3(x, y, z);
	}

	void Quaternion::xyz(const Vector3& value)
	{
		x = value.x;
		y = value.y;
		z = value.z;
	}

	const Vector4& Quaternion::xyzw()const
	{
		return Vector4(x, y, z, w);
	}

	void Quaternion::xyzw(const Vector4& value)
	{
		x = value.x;
		y = value.y;
		z = value.z;
		w = value.w;
	}

	const Matrix4& Quaternion::RotationMatrix() const
	{
		return Matrix4::CreateRotatationMatrix(*this);
	}

	Quaternion& Quaternion::GetIdentity()
	{
		return Quaternion(0, 0, 0, 1);
	}

	Quaternion::Quaternion() { }

	Quaternion::Quaternion(const float X, const float Y, const float Z, const float W)
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}

	Quaternion::Quaternion(const Vector4& vector)
	{
		xyzw(vector);
	}

	Quaternion::Quaternion(const Vector3& vector, const float w)
	{
		xyzw(Vector4(vector, w));
	}

	Quaternion::Quaternion(const Matrix4& matrix)
	{
		float scale = pow(matrix.GetDeterminant(), 1.0f / 3.0f);

		w = (std::sqrt(std::max(0.0f, scale + matrix.m00 + matrix.m11 + matrix.m22)) / 2.0f);
		x = (std::sqrt(std::max(0.0f, scale + matrix.m00 - matrix.m11 - matrix.m22)) / 2.0f);
		y = (std::sqrt(std::max(0.0f, scale - matrix.m00 + matrix.m11 - matrix.m22)) / 2.0f);
		z = (std::sqrt(std::max(0.0f, scale - matrix.m00 - matrix.m11 + matrix.m22)) / 2.0f);

		if (matrix.m12 - matrix.m12 < 0) x = -x;
		if (matrix.m02 - matrix.m20 < 0) y = -y;
		if (matrix.m10 - matrix.m01 < 0) z = -z;
	}

	/// <summary>
	/// Deconstruct quaternion into axis and angle
	/// </summary>
	/// <param name="axis"></param>
	/// <param name="angle"></param>
	void Quaternion::DeconstructQuaternion(Vector3& axis, float& angle) const
	{
		Vector4 result = ToAxisAngle();
		axis = result.xyz();
		angle = result.w;
	}

	/// <summary>
	/// Deconstruct quaternion into Forward and Up vectors
	/// </summary>
	/// <param name="qOrientation"></param>
	/// <param name="newForwardVector"></param>
	/// <param name="newUpVector"></param>
	void Quaternion::DeconstructQuaternion(Vector3& newForwardVector, Vector3& newUpVector) const
	{
		DeconstructQuaternion(*this, newForwardVector, newUpVector);
	}

	/// <summary>
	/// Deconstruct quaternion into Forward and Up vectors
	/// </summary>
	/// <param name="qOrientation"></param>
	/// <param name="newForwardVector"></param>
	/// <param name="newUpVector"></param>
	void Quaternion::DeconstructQuaternion(const Quaternion& qOrientation, Vector3& newForwardVector, Vector3& newUpVector)
	{
		// How to update local Forward and Up vectors given current Quaternion rotation:
		Vector3 worldUp = Vector3::Up;
		Vector3 worldBack = Vector3::Back;

		newForwardVector = qOrientation * worldBack; // Negated direction since we are facing camera
		newUpVector = qOrientation * worldUp;
	}

	const Quaternion& Quaternion::LookRotation(const Vector3& lookAt, const Vector3& up)
	{
		Vector3 forward = lookAt.Normalised();
		Vector3 right = Vector3::Cross(up, forward);

		Quaternion ret;
		ret.w = sqrt(1.0f + right.x + up.y + forward.z) * 0.5f;
		float w4_recip = 1.0f / (4.0f * ret.w);
		ret.x = (forward.y - up.z) * w4_recip;
		ret.y = (right.z - forward.x) * w4_recip;
		ret.z = (up.x - right.y) * w4_recip;

		return ret;
	}

	const Quaternion& Quaternion::Euler(const float x, const float y, const float z)
	{
		// Assuming the angles are in radians.
		float c1 = cos(x / 2);
		float s1 = sin(x / 2);
		float c2 = cos(y / 2);
		float s2 = sin(y / 2);
		float c3 = cos(z / 2);
		float s3 = sin(z / 2);
		float c1c2 = c1 * c2;
		float s1s2 = s1 * s2;
		Quaternion q;
		q.w = c1c2 * c3 - s1s2 * s3;
		q.x = c1c2 * s3 + s1s2 * c3;
		q.y = s1 * c2 * c3 + c1 * s2 * s3;
		q.z = c1 * s2 * c3 - s1 * c2 * s3;
		return q;
	}

	void Quaternion::ToAxisAngle(Vector3& axis, float& angle) const
	{
		Vector4 result = ToAxisAngle();
		axis = result.xyz();
		angle = result.w;
	}

	const Vector4& Quaternion::ToAxisAngle() const
	{
		Quaternion q = *this;

		if (abs(q.w) > 1.0f)
		{
			q.Normalise();
		}


		Vector4 result;

		result.w = 2.0f * acos(q.w);

		float scale = sqrt(1.0f - q.w * q.w);

		if (scale > 0.0001f)
		{
			result.xyz(q.xyz() / scale);
		}
		else
		{
			// This occurs when the angle is zero. 
			// Not a problem: just set an arbitrary normalized axis.
			result.xyz(Vector3::UnitX);
		}

		return result;
	}

	const Quaternion& Quaternion::Conjugate() const
	{
		Quaternion result(-xyz(), w);
		return result;
	}

	const Quaternion& Quaternion::Normalised() const
	{
		Quaternion norm(xyzw());
		float scale = 1.0f / norm.Magnitude();
		Vector4 xyzw = norm.xyzw();
		xyzw *= scale;
		norm.xyzw(xyzw);
		return norm;
	}

	void Quaternion::Normalise()
	{
		float scale = 1.0f / Magnitude();
		Vector4 q = xyzw();
		q *= scale;
		xyzw(q);
	}

	const float Quaternion::Magnitude() const
	{
		return xyzw().Magnitude();
	}

	const float Quaternion::MagnitudeSquared() const
	{
		return xyzw().MagnitudeSquared();
	}

	const Quaternion& Quaternion::Add(const Quaternion& left, const Quaternion& right)
	{
		Quaternion result(left.xyzw() + right.xyzw());
		return result;
	}

	const Quaternion& Quaternion::Add(const Quaternion& left, const float right)
	{
		Vector4 rhs = Vector4::One * right;
		Quaternion result(Multiply(left, rhs));
		return result;
	}

	const Quaternion& Quaternion::Subtract(const Quaternion& left, const Quaternion& right)
	{
		Quaternion result(left.xyzw() - right.xyzw());
		return result;
	}

	const Quaternion& Quaternion::Subtract(const Quaternion& left, const float right)
	{
		Quaternion result(left.xyzw() - right);
		return result;
	}

	const Vector3& Quaternion::Multiply(const Quaternion& left, const Vector3& right)
	{
		Vector4 rhs(right, 0);
		Vector3 result = Multiply(left, rhs);
		return result;
	}

	const Vector4& Quaternion::Multiply(const Quaternion& left, const Vector4& right)
	{
		// Only works if input is a unit quaternion.
		Quaternion norm = left.Normalised();
		//right.w = 1.0f;
		Quaternion p = Quaternion(right);
		Quaternion c = norm.Conjugate();
		Vector4 result = (norm * p * c).xyzw();
		return result;
	}

	const Quaternion& Quaternion::Multiply(const Quaternion& left, const Quaternion& right)
	{
		Quaternion result(
			(left.xyz() * right.w) +
			(right.xyz() * left.w) +
			Vector3::Cross(left.xyz(), right.xyz()),
			(left.w * right.w) - Vector3::Dot(left.xyz(), right.xyz())
		);
		return result;
	}

	const Quaternion& Quaternion::Scale(const Quaternion& quaternion, const float scale)
	{
		Quaternion result(
			quaternion.x * scale,
			quaternion.y * scale,
			quaternion.z * scale,
			quaternion.w * scale
		);
		return result;
	}

	const Quaternion& Quaternion::Divide(const Quaternion& quaternion, const float divider)
	{
		Quaternion result(
			quaternion.x / divider,
			quaternion.y / divider,
			quaternion.z / divider,
			quaternion.w / divider
		);
		return result;
	}

//}