#ifndef VECTOR3_H
#define VECTOR3_H

#include "Mathr.h"
#include "Vector4.h"
#include "Vector2i.h"
#include "Vector2.h"
#include "Matrix4.h"
#include <SFML/System/Vector3.hpp>

//#ifndef VEC3_FWD_DECLARE_H
//#define VEC3_FWD_DECLARE_H
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
#ifndef VEC3_FWD_DECLARE_H
#define VEC3_FWD_DECLARE_H
	class Vector4;
	class Vector3;
	class Vector2;
	class Vector2i;
#endif

	using namespace std;

	class Vector3
	{
	public:
		float x;
		float y;
		float z;

		static Vector3& Zero;
		static Vector3& One;
		static Vector3& Forward;
		static Vector3& Back;
		static Vector3& Left;
		static Vector3& Right;
		static Vector3& Up;
		static Vector3& Down;
		static Vector3& UnitX;
		static Vector3& UnitY;
		static Vector3& UnitZ;

		Vector3();
		Vector3(const float X, const float Y, const float Z);
		Vector3(const Vector4& v);
		Vector3(const Vector2& v);

		const Vector3& Normalised() const;
		float MagnitudeSquared() const;
		float Magnitude() const;

		const Vector2& xz() const;
		void xz(const Vector2& xz);
		const Vector2& yz() const;
		void yz(const Vector2& xy);
		const Vector2& xy() const;
		void xy(const Vector2& xy);
		const Vector4& xyzw() const;
		void xyzw(const Vector4& xyzw);

		//Vector3 TransformVector(Matrix4 transformationMatrix);
		//Vector3 TransformNormal(Matrix4 transformationMatrix);
		const Vector3& Add(const Vector3& v);
		const Vector3& Multiply(const Vector3& v);
		const Vector3& Translate(const Vector3& translation);
		const Vector3& Scale(const Vector3& s);
		const Vector3& Scale(const float s);
		const Vector3& RotateXY(const float xAngleDegrees, const float yAngleDegrees);
		const Vector3& Normalise() const;
		float GetMagnitudeSquared() const;
		float GetMagnitude() const;


		static float Distance(const Vector3& source, const Vector3& target);
		static float DistanceSquared(const Vector3& source, const Vector3& target);
		static const Vector3& Reflect(const Vector3& vertexPosition, const Vector3& normal);
		static float Dot(const Vector3& left, const Vector3& right);
		static const Vector3& Cross(const Vector3& left, const Vector3& right);
		static float Crossf(const Vector3& left, const Vector3& right);
		static const Vector3& Lerp(const Vector3& from, const Vector3& to, const float ratio);
		static const Vector3& TriangleLerp(const Vector3& a, const Vector3& b, const Vector3& c, const float u, const float v);
		static const Vector3& BarycetricInterpolator(const Vector3& a, const Vector3& b, const Vector3& c, const float ratioX, const float ratioY);
		static const Vector3& Add(const Vector3& left, const Vector3& right);
		static const Vector3& Add(const Vector3& left, const float scalar);
		static const Vector3& Subtract(const Vector3& left, const Vector3& right);
		static const Vector3& Subtract(const float scalar, const Vector3& right);
		static const Vector3& Subtract(const Vector3& left, const float scalar);
		static const Vector3& Multiply(const Vector3& left, const Vector3& right);
		static const Vector3& Multiply(const Vector3& left, const float right);
		static const Vector3& Scale(const Vector3& left, const float scalar);
		static const Vector3& Divide(const Vector3& left, const Vector3& right);
		static const Vector3& Divide(const Vector3& left, const float scalar);


		Vector3 operator +(Vector3 right) const { return Add(*this, right); }
		Vector3 operator +(int right)const { return Add(*this, right); }
		Vector3 operator -(Vector3 right)const { return Subtract(*this, right); }
		Vector3 operator -(float right)const { return Subtract(*this, right); }
		Vector3 operator *(float right)const { return Multiply(*this, right); }
		Vector3 operator *(Vector3 right)const { return Multiply(*this, right); }
		Vector3 operator /(float right)const { return Divide(*this, right); }
		Vector3 operator /(Vector3 right)const { return Divide(*this, right); }

		//Vector3 operator +(Vector3 left, Vector3 right) const { return Add(left, right); }
		//Vector3 operator +(Vector3 left, float right) const { return Add(left, right); }
		//Vector3 operator -(Vector3 left, Vector3 right)const { return Subtract(left, right); }
		//Vector3 operator -(Vector3 left, float right) const { return Subtract(left, right); }
		//Vector3 operator -(float left, Vector3 right)const { return Subtract(left, right); }
		//Vector3 operator *(Vector3 left, Vector3 right)const { return Multiply(left, right); }
		//Vector3 operator *(Vector3 left, float right)const { return Scale(left, right); }
		//Vector3 operator /(Vector3 left, Vector3 right)const { return Divide(left, right); }
		//Vector3 operator /(Vector3 left, float right)const { return Divide(left, right); }

		//operator Vector4()const
		//{
		//	Vector4 result(x, y, z, 0.0);
		//	return result;
		//}

		//operator Vector2()const
		//{
		//	Vector2 result(x, y);
		//	return result;
		//}

		//operator Vector2i()const
		//{
		//	int _x = x;
		//	int _y = y;
		//	Vector2i result(_x, _y);
		//	return result;
		//}
		/// Get a vector component given the specified index.
		float operator [] (int i) const
		{
			return (&x)[i];
		}

		operator sf::Vector3f()const
		{
			sf::Vector3f result(x, y, z);
			return result;
		}
	};

	// Operators

	inline Vector3 operator -(const Vector3& left)
	{
		return Vector3(-left.x, -left.y, -left.z);
	}

	inline bool operator ==(const Vector3& left, const Vector3 right)
	{
		return (std::fabs(left.x - right.x) <= FLT_EPSILON) 
			&& (std::fabs(left.y - right.y) <= FLT_EPSILON) 
			&& (std::fabs(left.z - right.z) <= FLT_EPSILON);
		//return (left.x == right.x) && (left.y == right.y) && (left.z == right.z);
	}

	inline bool operator !=(const Vector3& left, Vector3 right)
	{
		return !((std::fabs(left.x - right.x) <= FLT_EPSILON)
			&& (std::fabs(left.y - right.y) <= FLT_EPSILON)
			&& (std::fabs(left.z - right.z) <= FLT_EPSILON));
		//return (left.x != right.x) || (left.y != right.y) || (left.z != right.z);
	}

	inline Vector3& operator +=(Vector3& left, const Vector3& right)
	{
		left.x += right.x;
		left.y += right.y;
		left.z += right.z;
		return left;
	}

	inline Vector3& operator *=(Vector3& left, const Vector3& right)
	{
		left.x *= right.x;
		left.y *= right.y;
		left.z *= right.z;
		return left;
	}

	inline Vector3& operator -=(Vector3& left, const Vector3& right)
	{
		left.x -= right.x;
		left.y -= right.y;
		left.z -= right.z;
		return left;
	}

	inline Vector3& operator /=(Vector3& left, const Vector3& right)
	{
		left.x /= right.x;
		left.y /= right.y;
		left.z /= right.z;
		return left;
	}

	inline bool operator < (Vector3 const& left, Vector3 const& right) 
	{ 
		return (left.x < right.x) || (left.y < right.y) || (left.z < right.z); 
	}

	inline bool operator > (Vector3 const& left, Vector3 const& right) 
	{ 
		return (left.x > right.x) || (left.y > right.y) || (left.z > right.z); 
	}

//}
#endif