#include "Vector4.h"

//namespace ath
//{
	// Define and Initialise static members of class Vector4.
Vector4& Vector4::Zero = Vector4(0, 0, 0, 0);
Vector4& Vector4::One = Vector4(1, 1, 1, 1);
Vector4& Vector4::UnitX = Vector4(1, 0, 0, 0);
Vector4& Vector4::UnitY = Vector4(0, 1, 0, 0);
Vector4& Vector4::UnitZ = Vector4(0, 0, 1, 0);
Vector4& Vector4::UnitW = Vector4(0, 0, 0, 1);

Vector4::Vector4()
{
	x = 0; y = 0; z = 0; w = 0;
}

Vector4::Vector4(const Vector3& v3)
{
	x = v3.x;
	y = v3.y;
	z = v3.z;
	w = 0.0f;
}

Vector4::Vector4(const Vector3& v3, const float W)
{
	x = v3.x;
	y = v3.y;
	z = v3.z;
	w = W;
}

Vector4::Vector4(const float X, const float Y, const float Z, const float W)
{
	x = X;
	y = Y;
	z = Z;
	w = W;
}

const Vector2& Vector4::xy()const
{
	Vector2 result(x, y);
	return result;
}
void Vector4::xy(const Vector2& xy)
{
	x = xy.x;
	y = xy.y;
}
const Vector3& Vector4::xyz()const
{
	Vector3 result(x, y, z);
	return result;
}
void Vector4::xyz(const Vector3& xyz)
{
	x = xyz.x;
	y = xyz.y;
	z = xyz.z;
}

const Vector3& Vector4::xyw()const
{
	Vector3 result(x, y, w);
	return result;
}

void Vector4::xyw(const Vector3& xyw)
{
	x = xyw.x;
	y = xyw.y;
	w = xyw.z;
}

const Vector4& Vector4::xyzw() const
{
	Vector4 result(x, y, z, w);
	return result;
}

void Vector4::xyzw(const Vector4& xyzw)
{
	x = xyzw.x;
	y = xyzw.y;
	z = xyzw.z;
	w = xyzw.w;
}

const Vector4& Vector4::Translate(const Vector4& translation) const
{
	Vector4 result(x + translation.x, y + translation.y, z + translation.z, w + translation.w);
	return result;
}

const Vector4& Vector4::Scale(const Vector4& s) const
{
	Vector4 result(x * s.x, y * s.y, z * s.z, w * s.w);
	return result;
}

const Vector4& Vector4::Scale(const float s) const
{
	return Scale(*this, s);
}

const Vector4& Vector4::RotateXY(const float xAngleDegrees, const float yAngleDegrees) const
{
	Vector4 result(
		(x * cos(xAngleDegrees * Deg2Rad)) - (x * std::sin(xAngleDegrees * Deg2Rad)),
		(y * sin(yAngleDegrees * Deg2Rad)) + (y * std::cos(yAngleDegrees * Deg2Rad)),
		z,
		w
	);
	return result;
}

const Vector4& Vector4::Normalised() const
{
	return Normalise();
}

float Vector4::MagnitudeSquared() const
{
	return std::sqrt(Dot(*this, *this));
}

float Vector4::Magnitude() const
{
	return Dot(*this, *this);
}

const Vector4& Vector4::PerspectiveDivide() const
{
	Vector4 projected(x / w, y / w, z / w, w);
	return projected;
}

const Vector4& Vector4::Normalise() const
{
	float magnitude = MagnitudeSquared();
	Vector4 n(x / magnitude, y / magnitude, z / magnitude, w / magnitude);
	return n;
}

float Vector4::GetMagnitudeSquared() const
{
	return sqrt(Dot(*this, *this));
}

float Vector4::GetMagnitude() const
{
	return Dot(*this, *this);
}

float Vector4::Combine() const
{
	return x * y * z * w;
}

//const Vector4& Vector4::TransformVector(const Matrix4& transformationMatrix)
//{
//	const Vector4 vector = *this;
//	const Vector4 result = (transformationMatrix * vector);
//}

//Vector4 Vector4::TransformVector(Vector4 vector, Matrix4 matrix)
//{
//	Vector4 result(
//		Dot(vector, matrix.GetColumn0()),
//		Dot(vector, matrix.GetColumn1()),
//		Dot(vector, matrix.GetColumn2()),
//		Dot(vector, matrix.GetColumn3())
//	);
//	return result;
//}

float Vector4::Dot(const Vector4& left, const Vector4& right)
{
	return (left.x * right.x)
		+ (left.y * right.y)
		+ (left.z * right.z)
		+ (left.w * right.w);
}

const Vector4& Vector4::Reflect(const Vector4& vertexPosition, const Vector4& normal)
{
	return (normal - vertexPosition) * (2.0 * Dot(vertexPosition, normal));
}

const Vector4& Vector4::Lerp(const Vector4& from, const Vector4& to, const float ratio)
{
	return from + ((to - from) * ratio);
}

const Vector4& Vector4::TriangleLerp(const Vector4& a, const Vector4& b, const Vector4& c, const float ratioX, const float ratioY)
{
	return a + ((b - a) * ratioX) + ((c - a) * ratioY);
}

const Vector4& Vector4::BarycetricInterpolator(const Vector4& a, const Vector4& b, const Vector4& c, const float ratioX, const float ratioY)
{
	return TriangleLerp(a, b, c, ratioX, ratioY);
}

const Vector4& Vector4::Add(const Vector4& left, const Vector4& right)
{
	Vector4 result(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
	return result;
}

const Vector4& Vector4::Add(const Vector4& left, const float scalar)
{
	Vector4 result(left.x + scalar, left.y + scalar, left.z + scalar, left.w + scalar);
	return result;
}

const Vector4& Vector4::Subtract(const Vector4& left, const Vector4& right)
{
	Vector4 result(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
	return result;
}

const Vector4& Vector4::Subtract(const Vector4& left, const float scalar)
{
	Vector4 result(left.x - scalar, left.y - scalar, left.z - scalar, left.w - scalar);
	return result;
}

const Vector4& Vector4::Subtract(const float scalar, const Vector4& right)
{
	Vector4 result(scalar - right.x, scalar - right.y, scalar - right.z, scalar - right.w);
	return result;
}

const Vector4& Vector4::Multiply(const Vector4& left, const Vector4& right)
{
	Vector4 result(left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w);
	return result;
}

const Vector4& Vector4::Multiply(const Vector4& left, const float right)
{
	Vector4 result(left.x * right, left.y * right, left.z * right, left.w * right);
	return result;
}

const Vector4& Vector4::Scale(const Vector4& left, const float scalar)
{
	Vector4 result(left.x * scalar, left.y * scalar, left.z * scalar, left.w * scalar);
	return result;
}
const Vector4& Vector4::Divide(const Vector4& left, const Vector4& right)
{
	Vector4 result(left.x / right.x, left.y / right.y, left.z / right.z, left.w / right.w);
	return result;
}

const Vector4& Vector4::Divide(const Vector4& left, const float scalar)
{
	Vector4 result(left.x / scalar, left.y / scalar, left.z / scalar, left.w / scalar);
	return result;
}

//}