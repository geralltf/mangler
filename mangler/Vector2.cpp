#include "Vector2.h"

//namespace ath
//{

	// Define and Initialise static members of class Vector2.
Vector2& Vector2::Zero = Vector2(0, 0);
Vector2& Vector2::One = Vector2(1, 1);
Vector2& Vector2::UnitX = Vector2(1, 0);
Vector2& Vector2::UnitY = Vector2(0, 1);

Vector2::Vector2()
{
	x = 0; y = 0;
}

Vector2::Vector2(const float X, const float Y)
{
	x = X;
	y = Y;
}

const Vector2i& Vector2::xyi() const
{
	Vector2i result(x, y);
	return result;
}

void Vector2::xyi(const Vector2i& xyi)
{
	x = xyi.x;
	y = xyi.y;
}

const Vector2& Vector2::xy() const
{
	Vector2 result(x, y);
	return result;
}

void Vector2::xy(const Vector2& xy)
{
	x = xy.x;
	y = xy.y;
}

const Vector3& Vector2::xyz() const
{
	Vector3 result(x, y, 0.0);
	return result;
}

void Vector2::xyz(const Vector3& xyz)
{
	x = xyz.x;
	y = xyz.y;
}

const Vector4& Vector2::xyzw() const
{
	Vector4 result(x, y, 0.0, 0.0);
	return result;
}

void Vector2::xyzw(const Vector3& xyzw)
{
	x = xyzw.x;
	y = xyzw.y;
}

const Vector2& Vector2::Normalised() const
{
	return Normalise();
}

const Vector2& Vector2::Translate(const Vector2& translation) const
{
	Vector2 result(x + translation.x, y + translation.y);
	return result;
}

const Vector2& Vector2::Scale(const Vector2& s) const
{
	Vector2 result(x * s.x, y * s.y);
	return result;
}

const Vector2& Vector2::Scale(const float s) const
{
	return Scale(*this, s);
}

const Vector2& Vector2::RotateXY(const float xAngleDegrees, const float yAngleDegrees) const
{
	Vector2 result(
		(x * std::cos(xAngleDegrees * Deg2Rad)) - (x * std::sin(xAngleDegrees * Deg2Rad)),
		(y * std::sin(yAngleDegrees * Deg2Rad)) + (y * std::cos(yAngleDegrees * Deg2Rad))
	);

	return result;
}

const float Vector2::MagnitudeSquared() const
{
	return std::sqrt(Dot(*this, *this));
}

const float Vector2::Magnitude() const
{
	return Dot(*this, *this);
}

const float Vector2::GetMagnitudeSquared() const
{
	return std::sqrt(Dot(*this, *this));
}

const float Vector2::GetMagnitude() const
{
	return Dot(*this, *this);
}

const Vector2& Vector2::Normalise() const
{
	float magnitude = MagnitudeSquared();
	Vector2 result(x / magnitude, y / magnitude);
	return result;
}


const float Vector2::Dot(const Vector2& left, const Vector2& right)
{
	return (left.x * right.x) + (left.y * right.y);
}

const float Vector2::Cross(const Vector2& left, const Vector2& right)
{
	return (left.x * right.y) - (right.x * left.y);
}

const float Vector2::Lerp(const float from, const float to, const float ratio)
{
	return from + ((to - from) * ratio);
}

const Vector2& Vector2::Lerp(const Vector2& from, const Vector2& to, const float ratio)
{
	return from + ((to - from) * ratio);
}

const Vector2& Vector2::TriangleLerp(const Vector2& a, const Vector2& b, const Vector2& c, const float ratioX, const float ratioY)
{
	return a + ((b - a) * ratioX) + ((c - a) * ratioY);
}

const Vector2& Vector2::BarycetricInterpolator(const Vector2& a, const Vector2& b, const Vector2& c, const float ratioX, const float ratioY)
{
	return TriangleLerp(a, b, c, ratioX, ratioY);
}

const Vector2& Vector2::Add(const Vector2& left, const Vector2& right)
{
	Vector2 result(left.x + right.x, left.y + right.y);
	return result;
}

const Vector2& Vector2::Add(const Vector2& left, const float scalar)
{
	Vector2 result(left.x + scalar, left.y + scalar);
	return result;
}

const Vector2& Vector2::Subtract(const Vector2& left, const Vector2& right)
{
	Vector2 result(left.x - right.x, left.y - right.y);
	return result;
}

const Vector2& Vector2::Subtract(const Vector2& left, const float scalar)
{
	Vector2 result(left.x - scalar, left.y - scalar);
	return result;
}

const Vector2& Vector2::Multiply(const Vector2& left, const Vector2& right)
{
	Vector2 result(left.x * right.x, left.y * right.y);
	return result;
}

const Vector2& Vector2::Multiply(const Vector2& left, const float right)
{
	Vector2 result(left.x * right, left.y * right);
	return result;
}

const Vector2& Vector2::Scale(const Vector2& left, const float scalar)
{
	Vector2 result(left.x * scalar, left.y * scalar);
	return result;
}
const Vector2& Vector2::Divide(const Vector2& left, const Vector2& right)
{
	Vector2 result(left.x / right.x, left.y / right.y);
	return result;
}

const Vector2& Vector2::Divide(const Vector2& left, const float scalar)
{
	Vector2 result(left.x / scalar, left.y / scalar);
	return result;
}

//}