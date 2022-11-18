#ifndef QUATERNION_H
#define QUATERNION_H

//namespace ath
//{
#ifndef QUAT_FWD_DECLARE_H
#define QUAT_FWD_DECLARE_H
	class Vector4;
	class Vector3;
	class Vector2;
	class Vector2i;
	class Quaternion;
	class Matrix4;

#include "Matrix4.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"

#endif

	class Quaternion
	{
	public:

		float x;
		float y;
		float z;
		float w;

		static Quaternion& Identity;

		const Vector2& xy() const;

		void xy(const Vector2& value);

		const Vector3& xyz() const;

		void xyz(const Vector3& value);

		const Vector4& xyzw() const;

		void xyzw(const Vector4& value);

		const Matrix4& RotationMatrix() const;

		static Quaternion& GetIdentity();

		Quaternion();

		Quaternion(const float X, const float Y, const float Z, const float W);

		Quaternion(const Vector4& vector);

		Quaternion(const Vector3& vector, const float w);

		Quaternion(const Matrix4& matrix);

		/// <summary>
		/// Deconstruct quaternion into axis and angle
		/// </summary>
		/// <param name="axis"></param>
		/// <param name="angle"></param>
		void DeconstructQuaternion(Vector3& axis, float& angle) const;

		/// <summary>
		/// Deconstruct quaternion into Forward and Up vectors
		/// </summary>
		/// <param name="qOrientation"></param>
		/// <param name="newForwardVector"></param>
		/// <param name="newUpVector"></param>
		void DeconstructQuaternion(Vector3& newForwardVector, Vector3& newUpVector) const;

		/// <summary>
		/// Deconstruct quaternion into Forward and Up vectors
		/// </summary>
		/// <param name="qOrientation"></param>
		/// <param name="newForwardVector"></param>
		/// <param name="newUpVector"></param>
		static void DeconstructQuaternion(const Quaternion& qOrientation, Vector3& newForwardVector, Vector3& newUpVector);

		static const Quaternion& LookRotation(const Vector3& lookAt, const Vector3& up);

		static const Quaternion& Euler(const float x, const float y, const float z);

		void ToAxisAngle(Vector3& axis, float& angle) const;

		const Vector4& ToAxisAngle() const;

		const Quaternion& Conjugate() const;

		const Quaternion& Normalised() const;

		void Normalise();

		const float Magnitude() const;

		const float MagnitudeSquared() const;

		static const Quaternion& Add(const Quaternion& left, const Quaternion& right);

		static const Quaternion& Add(const Quaternion& left, const float right);

		static const Quaternion& Subtract(const Quaternion& left, const Quaternion& right);

		static const Quaternion& Subtract(const Quaternion& left, const float right);

		static const Vector4& Multiply(const Quaternion& left, const Vector4& right);

		static const Vector3& Multiply(const Quaternion& left, const Vector3& right);

		static const Quaternion& Multiply(const Quaternion& left, const Quaternion& right);

		static const Quaternion& Scale(const Quaternion& quaternion, const float scale);

		static const Quaternion& Divide(const Quaternion& quaternion, const float divider);

		//Quaternion operator +(Quaternion left, Quaternion right) { return Add(left, right); }
		//Quaternion operator +(Quaternion left, float right) { return Add(left, right); }
		//Quaternion operator -(Quaternion left, Quaternion right) { return Subtract(left, right); }
		//Quaternion operator -(Quaternion left, float right) { return Subtract(left, right); }
		//Quaternion operator *(Quaternion left, Quaternion right) { return Multiply(left, right); }
		//Vector4 operator *(Quaternion left, Vector4 right) { return Multiply(left, right); }
		//Vector3 operator *(Quaternion left, Vector3 right) { return Multiply(left, right); }
		//Quaternion operator *(Quaternion left, float right) { return Scale(left, right); }
		////Quaternion operator /(Quaternion left, Quaternion right) { return Divide(left, right); } // Is actually possible to divide two quaternions but not sure how.
		//Quaternion operator /(Quaternion left, float right) { return Divide(left, right); }
	};

	inline const Quaternion& operator +(const Quaternion& left, const Quaternion& right) { return Quaternion::Add(left, right); }
	inline const Quaternion& operator +(const Quaternion& left, const float right) { return Quaternion::Add(left, right); }
	inline const Quaternion& operator -(const Quaternion& left, const Quaternion& right) { return Quaternion::Subtract(left, right); }
	inline const Quaternion& operator -(const Quaternion& left, const float right) { return Quaternion::Subtract(left, right); }
	inline const Quaternion& operator *(const Quaternion& left, const Quaternion& right) { return Quaternion::Multiply(left, right); }
	inline const Vector4& operator *(const Quaternion& left, const Vector4& right) { return Quaternion::Multiply(left, right); }
	inline const Vector3& operator *(const Quaternion& left, const Vector3& right) { return Quaternion::Multiply(left, right); }
	inline const Quaternion& operator *(const Quaternion& left, const float right) { return Quaternion::Scale(left, right); }
	inline const Quaternion& operator /(const Quaternion& left, const float right) { return Quaternion::Divide(left, right); }


//}
#endif