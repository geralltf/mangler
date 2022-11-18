#ifndef VECTOR2I_H
#define VECTOR2I_H

#include "Mathr.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Vector2.h"

//#ifndef VEC2i_FWD_DECLARE_H
//#define VEC2i_FWD_DECLARE_H
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
#ifndef VEC2i_FWD_DECLARE_H
#define VEC2i_FWD_DECLARE_H

class Vector4;
class Vector3;
class Vector2;
class Vector2i;

#endif

using namespace std;

class Vector2i
{
public:
	int x;
	int y;

	static Vector2i& Zero;
	static Vector2i& One;
	static Vector2i& UnitX;
	static Vector2i& UnitY;

	Vector2i();
	Vector2i(const int X, const int Y);
	Vector2i(const float X, const float Y);

	const Vector2& xy() const;
	const Vector3& xyz() const;
	const Vector4& xyzw() const;

	void xy(const Vector2& xy);
	void xyz(const Vector3& xyz);
	const Vector3& xyw()const;
	void xyw(const Vector3& xyw);

	const Vector2i& Translate(const Vector2i& translation) const;
	const Vector2i& Scale(const Vector2i& s) const;
	const Vector2i& Scale(const float s) const;
	const Vector2i& RotateXY(const float xAngleDegrees, const float yAngleDegrees) const;

	const float MagnitudeSquared() const;
	const float Magnitude() const;
	const float GetMagnitudeSquared() const;
	const float GetMagnitude() const;

	// Static functions.
	static const float Dot(const Vector2i& left, const Vector2i& right);
	static const float Cross(const Vector2i& left, const Vector2i& right);
	static const float Lerp(const float from, const float to, const float ratio);
	static const Vector2i& Lerp(const Vector2i& from, const Vector2i& to, const float ratio);
	static const Vector2i& TriangleLerp(const Vector2i& a, const Vector2i& b, const Vector2i& c, const float ratioX, const float ratioY);
	static const Vector2i& BarycetricInterpolator(const Vector2i& a, const Vector2i& b, const Vector2i& c, const float ratioX, const float ratioY);
	static const Vector2i& Add(const Vector2i& left, const Vector2i& right);
	static const Vector2i& Add(const Vector2i& left, const int scalar);
	static const Vector2i& Subtract(const Vector2i& left, const Vector2i& right);
	static const Vector2i& Subtract(const Vector2i& left, const int scalar);
	static const Vector2i& Subtract(const float scalar, const Vector2i& right);
	static const Vector2i& Multiply(const Vector2i& left, const Vector2i& right);
	static const Vector2i& Multiply(const Vector2i& left, const float right);
	static const Vector2i& Scale(const Vector2i& left, const float scalar);
	static const Vector2i& Scale(const Vector2i& left, const int scalar);
	static const Vector2i& Divide(const Vector2i& left, const Vector2i& right);
	static const Vector2i& Divide(const Vector2i& left, const int scalar);

	// Operators

	/// Get a vector component given the specified index.
	int operator [] (int i) const
	{
		return (&x)[i];
	}

	Vector2i operator -() const
	{
		return Vector2i(-x, -y);
	}

	bool operator ==(Vector2i right) const
	{
		return (x == right.x) && (y == right.y);
	}

	bool operator !=(Vector2i right) const
	{
		return (x != right.x) || (y != right.y);
	}

	Vector2i& operator +=(const Vector2i& right)
	{
		x += right.x;
		y += right.y;
		return *this;
	}

	Vector2i& operator *=(const Vector2i& right)
	{
		x *= right.x;
		y *= right.y;
		return *this;
	}

	Vector2i& operator *=(const float& right)
	{
		x *= right;
		y *= right;
		return *this;
	}

	Vector2i& operator -=(const Vector2i& right)
	{
		x -= right.x;
		y -= right.y;
		return *this;
	}

	Vector2i& operator /=(const Vector2i& right)
	{
		x /= right.x;
		y /= right.y;
		return *this;
	}

	friend bool operator < (Vector2i const& left, Vector2i const& right)
	{
		return (left.x < right.x) || (left.y < right.y);
	}
	friend bool operator > (Vector2i const& left, Vector2i const& right)
	{
		return (left.x > right.x) || (left.y > right.y);
	}

	Vector2i operator +(Vector2i right) const { return Add(*this, right); }
	Vector2i operator +(int right)const { return Add(*this, right); }
	Vector2i operator -(Vector2i right)const { return Subtract(*this, right); }
	Vector2i operator -(float right) const { return Subtract(*this, right); }
	Vector2i operator *(float right)const { return Multiply(*this, right); }
	Vector2i operator *(Vector2i right)const { return Multiply(*this, right); }
	Vector2i operator /(float right)const { return Divide(*this, right); }
	Vector2i operator /(Vector2i right)const { return Divide(*this, right); }

	//Vector2i operator +(Vector2i left, Vector2i right) { return Add(left, right); }
	//Vector2i operator +(Vector2i left, int right) { return Add(left, right); }
	//Vector2i operator -(Vector2i left, Vector2i right) { return Subtract(left, right); }
	//Vector2i operator -(float left, Vector2i right) { return Subtract(left, right); }
	//Vector2i operator -(Vector2i left, int right) { return Subtract(left, right); }
	//Vector2i operator *(Vector2i left, Vector2i right) { return Multiply(left, right); }
	//Vector2i operator *(Vector2i left, float right) { return Scale(left, right); }
	//Vector2i operator *(Vector2i left, int right) { return Scale(left, right); }
	//Vector2i operator /(Vector2i left, Vector2i right) { return Divide(left, right); }
	//Vector2i operator /(Vector2i left, int right) { return Divide(left, right); }


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

	//operator Vector2()const
	//{
	//	Vector2 result((int)x, (int)y);
	//	return result;
	//}
};
//}


#endif	