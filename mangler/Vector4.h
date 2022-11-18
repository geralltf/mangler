#ifndef VECTOR4_H
#define VECTOR4_H

#include "Mathr.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Vector2i.h"

//#ifndef VEC4_FWD_DECLARE_H
//#define VEC4_FWD_DECLARE_H
//namespace ath
//{
//	class Vector4;
//	class Vector3;
//	class Vector2;
//	class Vector2i;
//	//class Matrix4;
//}
//#endif

//namespace ath
//{
#ifndef VEC4_FWD_DECLARE_H
#define VEC4_FWD_DECLARE_H
class Vector4;
class Vector3;
class Vector2;
class Vector2i;
//class Matrix4;
#endif

using namespace std;

class Vector4
{
public:
	float x;
	float y;
	float z;
	float w;

	static Vector4& Zero;
	static Vector4& One;
	static Vector4& UnitX;
	static Vector4& UnitY;
	static Vector4& UnitZ;
	static Vector4& UnitW;

	Vector4();
	Vector4(const Vector3& v3);
	Vector4(const Vector3& v3, const float W);
	Vector4(const float X, const float Y, const float Z, const float W);

	const Vector2& xy()const;
	void xy(const Vector2& xy);
	const Vector3& xyz()const;
	void xyz(const Vector3& xyz);
	const Vector3& xyw()const;
	void xyw(const Vector3& xyw);
	const Vector4& xyzw() const;
	void xyzw(const Vector4& xyzw);

	const Vector4& Translate(const Vector4& translation)const;

	const Vector4& Scale(const Vector4& s)const;

	const Vector4& Scale(float s)const;

	const Vector4& RotateXY(float xAngleDegrees, float yAngleDegrees)const;

	const Vector4& Normalised()const;

	float MagnitudeSquared()const;

	float Magnitude()const;

	const Vector4& PerspectiveDivide()const;

	const Vector4& Normalise()const;

	float GetMagnitudeSquared()const;

	float GetMagnitude()const;

	float Combine() const;

	//const Vector4& TransformVector(const Matrix4& transformationMatrix);

	//static const Vector4& TransformVector(const Vector4& vector, const Matrix4& matrix);

	static float Dot(const Vector4& left, const Vector4& right);

	static const Vector4& Reflect(const Vector4& vertexPosition, const Vector4& normal);

	static const Vector4& Lerp(const Vector4& from, const Vector4& to, float ratio);

	static const Vector4& TriangleLerp(const Vector4& a, const Vector4& b, const Vector4& c, float ratioX, float ratioY);

	static const Vector4& BarycetricInterpolator(const Vector4& a, const Vector4& b, const Vector4& c, float ratioX, float ratioY);

	static const Vector4& Add(const Vector4& left, const Vector4& right);

	static const Vector4& Add(const Vector4& left, float scalar);

	static const Vector4& Subtract(const Vector4& left, const Vector4& right);

	static const Vector4& Subtract(const Vector4& left, float scalar);

	static const Vector4& Subtract(float scalar, const Vector4& right);

	static const Vector4& Multiply(const Vector4& left, const Vector4& right);

	static const Vector4& Multiply(const Vector4& left, float right);

	static const Vector4& Scale(const Vector4& left, float scalar);

	static const Vector4& Divide(const Vector4& left, const Vector4& right);

	static const Vector4& Divide(const Vector4& left, float scalar);

	// Operators.

	/// Get a vector component given the specified index.
	float operator [] (int i) const
	{
		return (&x)[i];
	}
	operator const Vector4()const
	{
		const Vector4 result(x, y, z, w);
		return result;
	}
	operator Vector4()const
	{
		Vector4 result(x,y,z,w);
		return result;
	}
};

// Operators



inline Vector4& operator -(const Vector4& left)
{
	Vector4 result(-left.x, -left.y, -left.z, -left.w);
	return result;
}

inline bool operator ==(const Vector4& left, const Vector4 right)
{
	// Provide a tolerance for floating point error.
	return (std::fabs(left.x - right.x) <= FLT_EPSILON)
		&& (std::fabs(left.y - right.y) <= FLT_EPSILON)
		&& (std::fabs(left.z - right.z) <= FLT_EPSILON)
		&& (std::fabs(left.w - right.w) <= FLT_EPSILON);
	//return (left.x == right.x) && (left.y == right.y) && (left.z == right.z) && (left.w == right.w);
}
inline bool operator !=(const Vector4& left, const Vector4 right)
{
	// Provide a tolerance for floating point error.
	return !((std::fabs(left.x - right.x) <= FLT_EPSILON)
		&& (std::fabs(left.y - right.y) <= FLT_EPSILON)
		&& (std::fabs(left.z - right.z) <= FLT_EPSILON)
		&& (std::fabs(left.w - right.w) <= FLT_EPSILON));
	//return (left.x != right.x) || (left.y != right.y) || (left.z != right.z) || (left.w == right.w);
}

inline bool operator < (const Vector4& left, const Vector4& right)
{
	return (left.x < right.x) || (left.y < right.y) || (left.z < right.z) || (left.w < right.w);
}
inline bool operator > (const Vector4& left, const Vector4& right)
{
	return (left.x > right.x) || (left.y > right.y) || (left.z > right.z) || (left.w > right.w);
}

//Vector4 operator +(Vector4 right)const { return Add(*this, right); }
//Vector4 operator +(int right) const { return Add(*this, right); }
//Vector4 operator -(Vector4 right)const { return Subtract(*this, right); }
//Vector4 operator -(float right)const { return Subtract(*this, right); }
//Vector4 operator *(float right)const { return Multiply(*this, right); }
//Vector4 operator *(Vector4 right)const { return Multiply(*this, right); }
//Vector4 operator /(float right)const { return Divide(*this, right); }
//Vector4 operator /(Vector4 right) const { return Divide(*this, right); }

inline Vector4 operator +(const Vector4 left, const Vector4 right) { return Vector4::Add(left, right); }
inline Vector4 operator +(const Vector4 left, const float right) { return Vector4::Add(left, right); }
inline Vector4 operator -(const Vector4 left, const Vector4 right) { return Vector4::Subtract(left, right); }
inline Vector4 operator -(const Vector4 left, const float right) { return Vector4::Subtract(left, right); }
inline Vector4 operator -(const float left, const Vector4 right) { return Vector4::Subtract(left, right); }
inline Vector4 operator *(const Vector4 left, const Vector4 right) { return Vector4::Multiply(left, right); }
inline Vector4 operator *(const Vector4 left, const float right) { return Vector4::Scale(left, right); }
inline Vector4 operator /(const Vector4 left, const Vector4 right) { return Vector4::Divide(left, right); }
inline Vector4 operator /(const Vector4 left, const float right) { return Vector4::Divide(left, right); }

inline Vector4& operator +=(Vector4& left, const Vector4& right)
{
	left.x += right.x;
	left.y += right.y;
	left.z += right.z;
	left.w += right.w;
	return left;
}

inline Vector4& operator *=(Vector4& left, const Vector4& right)
{
	left.x *= right.x;
	left.y *= right.y;
	left.z *= right.z;
	left.w *= right.w;
	return left;
}

inline Vector4& operator *=(Vector4& left, const float& right)
{
	left.x *= right;
	left.y *= right;
	left.z *= right;
	left.w *= right;
	return left;
}

inline Vector4& operator /=(Vector4& left, const Vector4& right)
{
	left.x /= right.x;
	left.y /= right.y;
	left.z /= right.z;
	left.w /= right.w;
	return left;
}

inline Vector4& operator -=(Vector4& left, const Vector4& right)
{
	left.x -= right.x;
	left.y -= right.y;
	left.z -= right.z;
	left.w -= right.w;
	return left;
}

//operator Vector3()const
//{
//	Vector3 result(x, y, 0);
//	return result;
//}

//operator Vector2()const
//{
//	Vector2 result(x, y);
//	return result;
//}

//operator Vector2i()const
//{
//	Vector2i result((int)x, (int)y);
//	return result;
//}
//}
#endif