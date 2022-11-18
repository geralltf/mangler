#ifndef VECTOR2_H
#define VECTOR2_H

#include <algorithm>
#include "Mathr.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2i.h"

//#ifndef VEC2_FWD_DECLARE_H
//#define VEC2_FWD_DECLARE_H
//namespace ath
//{
//	class Vector4;
//	class Vector3;
//	class Vector2;
//	class Vector2i;
//}
//#endif

//namespace ath
//{
#ifndef VEC2_FWD_DECLARE_H
#define VEC2_FWD_DECLARE_H
class Vector4;
class Vector3;
class Vector2;
class Vector2i;
#endif

using namespace std;

class Vector2
{
public:
	float x;
	float y;

	static Vector2& Zero;
	static Vector2& One;
	static Vector2& UnitX;
	static Vector2& UnitY;

	Vector2();
	Vector2(const float X, const float Y);

	const Vector2i& xyi() const;
	void xyi(const Vector2i& xyi);
	const Vector2& xy() const;
	void xy(const Vector2& xy);
	const Vector3& xyz() const;
	void xyz(const Vector3& xyz);
	const Vector4& xyzw() const;
	void xyzw(const Vector3& xyzw);

	const Vector2& Normalised() const;

	const Vector2& Translate(const Vector2& translation) const;

	const Vector2& Scale(const Vector2& s) const;

	const Vector2& Scale(const float s) const;

	const Vector2& RotateXY(const float xAngleDegrees, const float yAngleDegrees) const;

	const float MagnitudeSquared() const;

	const float Magnitude() const;

	const float GetMagnitudeSquared() const;

	const float GetMagnitude() const;

	const Vector2& Normalise() const;

	// Static functions.

	static const float Dot(const Vector2& left, const Vector2& right);

	static const float Cross(const Vector2& left, const Vector2& right);

	static const float Lerp(const float from, const float to, const float ratio);

	static const Vector2& Lerp(const Vector2& from, const Vector2& to, const float ratio);

	static const Vector2& TriangleLerp(const Vector2& a, const Vector2& b, const Vector2& c, const float ratioX, const float ratioY);

	static const Vector2& BarycetricInterpolator(const Vector2& a, const Vector2& b, const Vector2& c, const float ratioX, const float ratioY);

	static const Vector2& Add(const Vector2& left, const Vector2& right);

	static const Vector2& Add(const Vector2& left, const float scalar);

	static const Vector2& Subtract(const Vector2& left, const Vector2& right);

	static const Vector2& Subtract(const Vector2& left, const float scalar);

	static const Vector2& Multiply(const Vector2& left, const Vector2& right);

	static const Vector2& Multiply(const Vector2& left, const float right);

	static const Vector2& Scale(const Vector2& left, const float scalar);

	static const Vector2& Divide(const Vector2& left, const Vector2& right);

	static const Vector2& Divide(const Vector2& left, const float scalar);



	// Operators

	/// Get a vector component given the specified index.
	float operator [] (int i) const
	{
		return (&x)[i];
	}

	const Vector2& operator -()const
	{
		return Vector2(-x, -y);
	}

	bool operator ==(const Vector2& right) const
	{
		return (std::fabs(x - right.x) <= FLT_EPSILON)
			&& (std::fabs(y - right.y) <= FLT_EPSILON);
		//return (x == right.x) && (y == right.y);
	}

	bool operator != (Vector2 right) const
	{
		return !((std::fabs(x - right.x) <= FLT_EPSILON)
			&& (std::fabs(y - right.y) <= FLT_EPSILON));
		//return (x != right.x) || (y != right.y);
	}

	const Vector2& operator +=(const Vector2& right)
	{
		x += right.x;
		y += right.y;
		return *this;
	}

	const Vector2& operator *=(const Vector2& right)
	{
		x *= right.x;
		y *= right.y;
		return *this;
	}

	const Vector2& operator *=(const float& right)
	{
		x *= right;
		y *= right;
		return *this;
	}

	const Vector2& operator -=(const Vector2& right)
	{
		x -= right.x;
		y -= right.y;
		return *this;
	}

	const Vector2& operator /=(const Vector2& right)
	{
		x /= right.x;
		y /= right.y;
		return *this;
	}

	friend bool operator < (Vector2 const& left, Vector2 const& right)
	{
		return (left.x < right.x) || (left.y < right.y);
	}
	friend bool operator > (Vector2 const& left, Vector2 const& right)
	{
		return (left.x > right.x) || (left.y > right.y);
	}

	Vector2 operator +(Vector2 right)const { return Add(*this, right); }
	Vector2 operator +(int right) const { return Add(*this, right); }
	Vector2 operator -(Vector2 right)const { return Subtract(*this, right); }
	Vector2 operator -(float right) const { return Subtract(*this, right); }
	Vector2 operator *(float right)const { return Multiply(*this, right); }
	Vector2 operator *(Vector2 right)const { return Multiply(*this, right); }
	Vector2 operator /(float right) const { return Divide(*this, right); }
	Vector2 operator /(Vector2 right)const { return Divide(*this, right); }

	//Vector2 operator +(Vector2 left, Vector2 right) { return Add(left, right); }
	//Vector2 operator +(Vector2 left, float right) { return Add(left, right); }
	//Vector2 operator -(Vector2 left, Vector2 right) { return Subtract(left, right); }
	//Vector2 operator -(Vector2 left, float right) { return Subtract(left, right); }
	//Vector2 operator *(Vector2 left, Vector2 right) { return Multiply(left, right); }
	//Vector2 operator *(Vector2 left, float right) { return Scale(left, right); }
	//Vector2 operator /(Vector2 left, Vector2 right) { return Divide(left, right); }
	//Vector2 operator /(Vector2 left, float right) { return Divide(left, right); }

	//operator Vector4()const
	//{
	//	Vector4 result(x, y, 0.0, 0.0);
	//	return result;
	//}

	//operator Vector3()const
	//{
	//	Vector3 result(x, y, 0);
	//	return result;
	//}

	//operator Vector2i()const
	//{
	//	Vector2i result((int)x, (int)y);
	//	return result;
	//}
};


//}
#endif