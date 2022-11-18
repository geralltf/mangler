#include "MathHelpers.h"

#include "Vector3.h"

//namespace ath
//{

	sf::Vector3f& MathHelpers::ConvertColourToNormalSpace(sf::Vector3f& colour)
	{
		// Convert from 0 and 1 to -1, 1.
		sf::Vector3f normalCoord((2 * colour.x) - 1, (2 * colour.y) - 1, (2 * colour.z) - 1);

		return normalCoord;
	}
	sf::Vector3f& MathHelpers::ConvertNormalToColourSpace(sf::Vector3f& normal)
	{
		//if (normal.x < 0) normal.x *= -1;  // normals must be in 0 - 1 range not (-1 to 1)
		//if (normal.y < 0) normal.y *= -1;
		
		Vector3 N = Vector3(normal.x, normal.y, normal.z);
		N += Vector3::One;
		N = N.Normalised();
		return static_cast<sf::Vector3f>(N);
	}

	sf::Vector3f& MathHelpers::RandomPointInCube()
	{
		return GetRandomPointInCube();
	}

	sf::Vector3f& MathHelpers::RandomPointInUnitSphere()
	{
		return GetRandomPointInSphere();
	}

	sf::Vector3f& MathHelpers::RandomPointInFrontOfPlane()
	{
		return GetRandomPointInFrontOfPlane();
	}

	sf::Vector3f& MathHelpers::GetRandomPointInFrontOfPlane()
	{
		return GetRandomPointInFrontOfPlane(0.00001f);
	}

	sf::Vector3f& MathHelpers::GetRandomPointInFrontOfPlane(float zOffset)
	{
		sf::Vector3f& point = sf::Vector3f(0,0,0);
		point.x = (2 * RandomFloat() - 0.5f);
		point.y = (2 * RandomFloat() - 0.5f);
		point.z = (RandomFloat() + zOffset);
		return point;
	}

	sf::Vector3f& MathHelpers::GetRandomPointInCube()
	{
		// return RandomPointInUnitSphere;

		return sf::Vector3f(NormaliseBetweenNegPve(RandomFloat()),
			NormaliseBetweenNegPve(RandomFloat()),
			NormaliseBetweenNegPve(RandomFloat()));
	}

	int MathHelpers::Clamp(int value, int min, int max)
	{
		int v = value;
		if (value < min) v = min;
		if (value > max) v = max;
		return v;
	}

	float MathHelpers::Clamp(float value, float min, float max)
	{
		float v = value;
		if (value < min) v = min;
		if (value > max) v = max;
		return v;
	}

	//TODO: kinda belongs in each vector class
	sf::Vector3f& MathHelpers::ClampVector(sf::Vector3f& v, float min, float max)
	{
		return sf::Vector3f(Clamp(v.x, min, max), Clamp(v.y, min, max), Clamp(v.z, min, max));
	}
	sf::Vector3f& MathHelpers::ClampVector(sf::Vector3f& v, float min, sf::Vector3f& max)
	{
		return sf::Vector3f(Clamp(v.x, min, max.x), Clamp(v.y, min, max.y), Clamp(v.z, min, max.z));
	}
	sf::Vector2f& MathHelpers::ClampVector(sf::Vector2f& v, float min, float max)
	{
		return sf::Vector2f(Clamp(v.x, min, max), Clamp(v.y, min, max));
	}
	sf::Vector2f& MathHelpers::ClampVector(sf::Vector2f& v, float min, sf::Vector2f& max)
	{
		return sf::Vector2f(Clamp(v.x, min, max.x), Clamp(v.y, min, max.y));
	}
	sf::Vector2i& MathHelpers::ClampVector(sf::Vector2i& v, int min, int max)
	{
		return sf::Vector2i(Clamp(v.x, min, max), Clamp(v.y, min, max));
	}
	sf::Vector2i& MathHelpers::ClampVector(sf::Vector2i& v, int min, sf::Vector2i& max)
	{
		return sf::Vector2i(Clamp(v.x, min, max.x), Clamp(v.y, min, max.y));
	}
	//TODO: kinda belongs in each vector class
	float MathHelpers::Lerp(float left, float right, float ratio)
	{
		float result;
		result = left + ((right - left) * ratio);
		return result;
	}

	sf::Vector2f& MathHelpers::Lerp(sf::Vector2f& left, sf::Vector2f& right, float ratio)
	{
		sf::Vector2f result;
		result.x = left.x + ((right.x - left.x) * ratio);
		result.y = left.y + ((right.y - left.y) * ratio);
		return result;
	}

	sf::Vector3f& MathHelpers::Lerp(sf::Vector3f& left, sf::Vector3f& right, float ratio)
	{
		sf::Vector3f result;
		result.x = left.x + ((right.x - left.x) * ratio);
		result.y = left.y + ((right.y - left.y) * ratio);
		result.z = left.z + ((right.z - left.z) * ratio);
		return result;
	}

	//Vector4 MathHelpers::Lerp(Vector4 left, Vector4 right, float ratio)
	//{
	//	Vector4 result;
	//	result.x = left.x + ((right.x - left.x) * ratio);
	//	result.y = left.y + ((right.y - left.y) * ratio);
	//	result.z = left.z + ((right.z - left.z) * ratio);
	//	result.w = left.w + ((right.w - left.w) * ratio);
	//	return result;
	//}

	/// <summary>
	/// Normalise value between -1 and 1.
	/// </summary>
	float MathHelpers::NormaliseBetweenNegPve(float value)
	{
		return (value - 0.5f) * 2;
	}

	float MathHelpers::RandomFloat()
	{
		return ((float)rand()) / (float)RAND_MAX;
	}

	float MathHelpers::Range(float min, float max) {
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = max - min;
		float r = random * diff;
		return min + r;
	}

	sf::Vector3f& MathHelpers::GetRandomPointInSphere(float radius)
	{
		float v = RandomFloat();

		float theta = RandomFloat() * TwoPi;
		float phi = acos(2 * v - 1);

		float x = (radius * cos(theta) * sin(phi));
		float y = (radius * sin(theta) * sin(phi));
		float z = (radius * cos(phi));

		return sf::Vector3f(x, y, z);
	}

	sf::Vector3f& MathHelpers::GetPointInSphere(float radius, float theta, float phiValue)
	{
		float phi = acos(2 * phiValue - 1);

		float x = (radius * cos(theta) * sin(phi));
		float y = (radius * sin(theta) * sin(phi));
		float z = (radius * cos(phi));

		return sf::Vector3f(x, y, z);

	}

//}