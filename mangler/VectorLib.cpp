#include "VectorLib.h"

float GetMagnitude(Vector2f v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

void Normalise(Vector2f v)
{
	float length = sqrt(v.x * v.x + v.y * v.y);

	v.x = v.x / length;
	v.y = v.y / length;
}

void Subtract(Vector2f v)
{
	v.x = v.x - v.x;
	v.y = v.y - v.y;
}

float GetDistance(Vector2f a, Vector2f b)
{
	float d = (a.x - b.x) + (a.y - b.y);

	if (d < 0)
	{
		return sqrt(abs(d));
	}

	return sqrt(d);
}

float GetDistanceSquared(Vector2f a, Vector2f b)
{
	float d = ((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y));

	if (d < 0)
	{
		return sqrt(abs(d));
	}

	return sqrt(d);
}