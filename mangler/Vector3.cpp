#include "Vector3.h"


//namespace ath
//{

	// Define and Initialise static members of class Vector3.
	Vector3& Vector3::Zero = Vector3(0, 0, 0);
	Vector3& Vector3::One = Vector3(1, 1, 1);
	Vector3& Vector3::UnitX = Vector3(1, 0, 0);
	Vector3& Vector3::UnitY = Vector3(0, 1, 0);
	Vector3& Vector3::UnitZ = Vector3(0, 0, 1);
	Vector3& Vector3::Forward = Vector3(0, 0, 1);
	Vector3& Vector3::Back = Vector3(0, 0, -1);
	Vector3& Vector3::Left = Vector3(-1, 0, 0);
	Vector3& Vector3::Right = Vector3(1, 0, 0);
	Vector3& Vector3::Up = Vector3(0, 1, 0);
	Vector3& Vector3::Down = Vector3(0, -1, 0);

	Vector3::Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	Vector3::Vector3(const float X, const float Y, const float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}
	Vector3::Vector3(const Vector4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	Vector3::Vector3(const Vector2& v)
	{
		x = v.x;
		y = v.y;
		z = 0;
	}

	const Vector3& Vector3::Normalised() const
	{
		return Normalise();
	}

	float Vector3::MagnitudeSquared() const
	{
		return sqrt(Dot(*this, *this));
	}

	float Vector3::Magnitude() const
	{
		return Dot(*this, *this);
	}

	const Vector2& Vector3::xz() const
	{
		Vector2 result(x, z);
		return result;
	}
	void Vector3::xz(const Vector2& xz)
	{
		x = xz.x;
		z = xz.y;
	}

	const Vector2& Vector3::yz() const
	{
		Vector2 result(y, z);
		return result;
	}
	void Vector3::yz(const Vector2& yz)
	{
		x = yz.x;
		y = yz.y;
	}
	void Vector3::xy(const Vector2& xy)
	{
		x = xy.x;
		y = xy.y;
	}
	const Vector2& Vector3::xy() const
	{
		Vector2 result(x, y);
		return result;
	}
	const Vector4& Vector3::xyzw() const
	{
		Vector4 result(x, y, z, 0.0);
		return result;
	}

	void Vector3::xyzw(const Vector4& xyzw)
	{
		x = xyzw.x;
		y = xyzw.y;
		z = xyzw.z;
	}


	//Vector3 Vector3::TransformVector(Matrix4 transformationMatrix)
	//{
	//	Vector4 v(*this, 1.0f);

	//	return (transformationMatrix * v).xyz();
	//}

	//Vector3 Vector3::TransformNormal(Matrix4 transformationMatrix)
	//{
	//	Vector3 result;
	//	Matrix4 m = transformationMatrix.Inverse();

	//	Vector3 r0(m.GetRow0());
	//	Vector3 r1(m.GetRow1());
	//	Vector3 r2(m.GetRow2());

	//	Vector3 tmp
	//	(
	//		Dot(*this, r0),
	//		Dot(*this, r1),
	//		Dot(*this, r2)
	//	);

	//	result = tmp;

	//	return result;
	//}

	//Vector3 Vector3::TransformVector(Matrix4 transformationMatrix, bool ignoreTraslation = false, bool isNormal = false)
	//{
	//	Vector3 result;
	//	Matrix4 m = transformationMatrix;
	//
	//	if (isNormal)
	//	{
	//		m = transformationMatrix.Inverse();
	//
	//		Vector3 r0(m.GetRow0());
	//		Vector3 r1(m.GetRow1());
	//		Vector3 r2(m.GetRow2());
	//
	//		Vector3 tmp
	//		(
	//			Dot(*this, r0),
	//			Dot(*this, r1),
	//			Dot(*this, r2)
	//		);
	//
	//		result = tmp;
	//	}
	//	else
	//	{
	//		Vector3 c0(m.GetColumn0());
	//		Vector3 c1(m.GetColumn0());
	//		Vector3 c2(m.GetColumn0());
	//
	//		Vector3 tmp
	//		(
	//			Dot(*this, c0) + (ignoreTraslation ? 0 : m.GetRow3().x),
	//			Dot(*this, c1) + (ignoreTraslation ? 0 : m.GetRow3().y),
	//			Dot(*this, c2) + (ignoreTraslation ? 0 : m.GetRow3().z)
	//		);
	//
	//		return tmp;
	//	}
	//
	//	return result;
	//}

	const Vector3& Vector3::Add(const Vector3& left, const Vector3& right)
	{
		Vector3 result(left.x + right.x, left.y + right.y, left.z + right.z);
		return result;
	}

	const Vector3& Vector3::Add(const Vector3& left, const float scalar)
	{
		Vector3 result(left.x + scalar, left.y + scalar, left.z + scalar);
		return result;
	}

	const Vector3& Vector3::Subtract(const Vector3& left, const Vector3& right)
	{
		Vector3 result(left.x - right.x, left.y - right.y, left.z - right.z);
		return result;
	}

	const Vector3& Vector3::Subtract(const float scalar, const Vector3& right)
	{
		Vector3 result(scalar - right.x, scalar - right.y, scalar - right.z);
		return result;
	}

	const Vector3& Vector3::Subtract(const Vector3& left, const float scalar)
	{
		Vector3 result(left.x - scalar, left.y - scalar, left.z - scalar);
		return result;
	}

	const Vector3& Vector3::Multiply(const Vector3& left, const Vector3& right)
	{
		Vector3 result(left.x * right.x, left.y * right.y, left.z * right.z);
		return result;
	}

	const Vector3& Vector3::Multiply(const Vector3& left, const float right)
	{
		Vector3 result(left.x * right, left.y * right, left.z * right);
		return result;
	}

	const Vector3& Vector3::Scale(const Vector3& left, const float scalar)
	{
		Vector3 result(left.x * scalar, left.y * scalar, left.z * scalar);
		return result;
	}
	const Vector3& Vector3::Divide(const Vector3& left, const Vector3& right)
	{
		Vector3 result(left.x / right.x, left.y / right.y, left.z / right.z);
		return result;
	}

	const Vector3& Vector3::Divide(const Vector3& left, const float scalar)
	{
		Vector3 result(left.x / scalar, left.y / scalar, left.z / scalar);
		return result;
	}

	const Vector3& Vector3::Add(const Vector3& v)
	{
		return Add(*this, v);
	}

	const Vector3& Vector3::Multiply(const Vector3& v)
	{
		return Multiply(*this, v);
	}

	const Vector3& Vector3::Translate(const Vector3& translation)
	{
		Vector3 result(x + translation.x, y + translation.y, z + translation.z);
		return result;
	}

	const Vector3& Vector3::Scale(const Vector3& s)
	{
		Vector3 result(x * s.x, y * s.y, z * s.z);
		return result;
	}
	const Vector3& Vector3::Scale(const float s)
	{
		return Scale(*this, s);
	}

	const Vector3& Vector3::RotateXY(const float xAngleDegrees, const float yAngleDegrees)
	{
		Vector3 result(
			(x * cos(xAngleDegrees * Deg2Rad)) - (x * sin(xAngleDegrees * Deg2Rad)),
			(y * sin(yAngleDegrees * Deg2Rad)) + (y * cos(yAngleDegrees * Deg2Rad)),
			z
		);

		return result;
	}

	const Vector3& Vector3::Normalise() const
	{
		float magnitude = MagnitudeSquared();

		Vector3 result(x / magnitude, y / magnitude, z / magnitude);

		return result;
	}

	float Vector3::GetMagnitudeSquared() const
	{
		return sqrt(Dot(*this, *this));
	}

	float Vector3::GetMagnitude() const
	{
		return Dot(*this, *this);
	}

	float Vector3::Distance(const Vector3& source, const Vector3& target)
	{
		return (source.x * target.x + source.y * target.y + source.z * target.z);
	}

	float Vector3::DistanceSquared(const Vector3& source, const Vector3& target)
	{
		return sqrt(source.x * target.x + source.y * target.y + source.z * target.z);
	}

	const Vector3& Vector3::Reflect(const Vector3& vertexPosition, const Vector3& normal)
	{
		return (normal - vertexPosition) * (2.0 * Dot(vertexPosition, normal));
	}

	float Vector3::Dot(const Vector3& left, const Vector3& right)
	{
		return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
	}

	const Vector3& Vector3::Cross(const Vector3& left, const Vector3& right)
	{
		Vector3 result(
			(left.y * right.z) - (left.z * right.y),
			(left.z * right.x) - (left.x * right.z),
			(left.x * right.y) - (left.y * right.x)
		);
		return result;
	}

	float Vector3::Crossf(const Vector3& left, const Vector3& right)
	{

		return (left.y * right.z) - (left.z * right.y) -
			(left.z * right.x) - (left.x * right.z) -
			(left.x * right.y) - (left.y * right.x);

		//return (left.x * right.y) - (right.x * left.y);
	}

	const Vector3& Vector3::Lerp(const Vector3& from, const Vector3& to, const float ratio)
	{
		return from + ((to - from) * ratio);
	}

	const Vector3& Vector3::TriangleLerp(const Vector3& a, const Vector3& b, const Vector3& c, const float u, const float v)
	{
		float w = (1 - u - v);

		return (a + ((b - a) * u) + ((c - a) * v));
	}

	const Vector3& Vector3::BarycetricInterpolator(const Vector3& a, const Vector3& b, const Vector3& c, const float ratioX, const float ratioY)
	{
		return TriangleLerp(a, b, c, ratioX, ratioY); // u v
	}

//}