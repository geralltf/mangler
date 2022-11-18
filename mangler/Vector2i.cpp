#include "Vector2i.h"


//namespace ath
//{
	// Define and Initialise static members of class Vector2i.
Vector2i& Vector2i::Zero = Vector2i(0, 0);
Vector2i& Vector2i::One = Vector2i(1, 1);
Vector2i& Vector2i::UnitX = Vector2i(1, 0);
Vector2i& Vector2i::UnitY = Vector2i(0, 1);

Vector2i::Vector2i()
{
	x = 0; y = 0;
}

Vector2i::Vector2i(int X, int Y)
{
	x = X;
	y = Y;
}

Vector2i::Vector2i(float X, float Y)
{
	x = static_cast<int>(X);
	y = static_cast<int>(Y);
}

const Vector2& Vector2i::xy() const
{
	return Vector2(x, y);
}

void Vector2i::xy(const Vector2& xy)
{
	x = static_cast<int>(xy.x);
	y = static_cast<int>(xy.y);
}

const Vector3& Vector2i::xyz() const
{
	return Vector3(x, y, 0);
}

void Vector2i::xyz(const Vector3& xyz)
{
	x = static_cast<int>(xyz.x);
	y = static_cast<int>(xyz.y);
}

const Vector4& Vector2i::xyzw() const
{
	return Vector4(x, y, 0, 0);
}

const Vector3& Vector2i::xyw()const
{
	Vector3 result(x, y, 0);
	return result;
}

void Vector2i::xyw(const Vector3& xyw)
{
	x = static_cast<int>(xyw.x);
	y = static_cast<int>(xyw.y);
}
const Vector2i& Vector2i::Translate(const Vector2i& translation) const
{
	Vector2i result((x + translation.x), (y + translation.y));
	return result;
}

const Vector2i& Vector2i::Scale(const Vector2i& s) const
{
	Vector2i result(x * s.x, y * s.y);
	return result;
}

const Vector2i& Vector2i::Scale(float s) const
{
	return Scale(*this, s);
}

const Vector2i& Vector2i::RotateXY(float xAngleDegrees, float yAngleDegrees) const
{
	Vector2i result(
		static_cast<int>(((x * std::cos(xAngleDegrees * Deg2Rad)) - (x * std::sin(xAngleDegrees * Deg2Rad)))),
		static_cast<int>(((y * std::sin(yAngleDegrees * Deg2Rad)) + (y * std::cos(yAngleDegrees * Deg2Rad))))
	);
	return result;
}

const float Vector2i::MagnitudeSquared() const
{
	return std::sqrt(Dot(*this, *this));
}

const float Vector2i::Magnitude() const
{
	return Dot(*this, *this);
}

const float Vector2i::GetMagnitudeSquared() const
{
	return std::sqrt(Dot(*this, *this));
}

const float Vector2i::GetMagnitude() const
{
	return (x * x) + (y * y);
}

// Static functions.
const float Vector2i::Dot(const Vector2i& left, const Vector2i& right)
{
	return (left.x * right.x) + (left.y * right.y);
}

const float Vector2i::Cross(const Vector2i& left, const Vector2i& right)
{
	return (left.x * right.y) - (right.x * left.y);
}

const float Vector2i::Lerp(const float from, const float to, const float ratio)
{
	return from + ((to - from) * ratio);
}

const Vector2i& Vector2i::Lerp(const Vector2i& from, const Vector2i& to, const float ratio)
{
	return from + ((to - from) * ratio);
}

const Vector2i& Vector2i::TriangleLerp(const Vector2i& a, const Vector2i& b, const Vector2i& c, const float ratioX, const float ratioY)
{
	return a + ((b - a) * ratioX) + ((c - a) * ratioY);
}

const Vector2i& Vector2i::BarycetricInterpolator(const Vector2i& a, const Vector2i& b, const Vector2i& c, const float ratioX, const float ratioY)
{
	return TriangleLerp(a, b, c, ratioX, ratioY);
}

const Vector2i& Vector2i::Add(const Vector2i& left, const Vector2i& right)
{
	Vector2i result(left.x + right.x, left.y + right.y);
	return result;
}

const Vector2i& Vector2i::Add(const Vector2i& left, const int scalar)
{
	Vector2i result(left.x + scalar, left.y + scalar);
	return result;
}

const Vector2i& Vector2i::Subtract(const Vector2i& left, const Vector2i& right)
{
	Vector2i result(left.x - right.x, left.y - right.y);
	return result;
}

const Vector2i& Vector2i::Subtract(const Vector2i& left, const int scalar)
{
	Vector2i result(left.x - scalar, left.y - scalar);
	return result;
}

const Vector2i& Vector2i::Subtract(const float scalar, const Vector2i& right)
{
	Vector2i result(scalar - right.x, scalar - right.y);
	return result;
}

const Vector2i& Vector2i::Multiply(const Vector2i& left, const Vector2i& right)
{
	Vector2i result(left.x * right.x, left.y * right.y);
	return result;
}

const Vector2i& Vector2i::Multiply(const Vector2i& left, const float right)
{
	Vector2i result(left.x * right, left.y * right);
	return result;
}

const Vector2i& Vector2i::Scale(const Vector2i& left, const float scalar)
{
	Vector2i result(static_cast<int>(left.x * scalar), static_cast<int>(left.y * scalar));
	return result;
}

const Vector2i& Vector2i::Scale(const Vector2i& left, const int scalar)
{
	Vector2i result(left.x * scalar, left.y * scalar);
	return result;
}

const Vector2i& Vector2i::Divide(const Vector2i& left, const Vector2i& right)
{
	Vector2i result(left.x / right.x, left.y / right.y);
	return result;
}

const Vector2i& Vector2i::Divide(const Vector2i& left, const int scalar)
{
	Vector2i result(left.x / scalar, left.y / scalar);
	return result;
}

//}