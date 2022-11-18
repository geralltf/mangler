#ifndef MATH_HELPERS_H
#define MATH_HELPERS_H

#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>

//#include "Vector3.h"

//namespace ath
//{

	class MathHelpers
	{
	public:
		static sf::Vector3f& ConvertColourToNormalSpace(sf::Vector3f& colour);
		static sf::Vector3f& ConvertNormalToColourSpace(sf::Vector3f& normal);
		static sf::Vector3f& RandomPointInCube();
		static sf::Vector3f& RandomPointInUnitSphere();
		static sf::Vector3f& RandomPointInFrontOfPlane();
		static sf::Vector3f& GetRandomPointInFrontOfPlane();
		static sf::Vector3f& GetRandomPointInFrontOfPlane(float zOffset);
		static sf::Vector3f& GetRandomPointInCube();
		static int Clamp(int value, int min, int max);
		static float Clamp(float value, float min, float max);

		//TODO: kinda belongs in each vector class
		static sf::Vector3f& ClampVector(sf::Vector3f& v, float min, float max);
		static sf::Vector3f& ClampVector(sf::Vector3f& v, float min, sf::Vector3f& max);
		static sf::Vector2f& ClampVector(sf::Vector2f& v, float min, float max);
		static sf::Vector2f& ClampVector(sf::Vector2f& v, float min, sf::Vector2f& max);
		static sf::Vector2i& ClampVector(sf::Vector2i& v, int min, int max);
		static sf::Vector2i& ClampVector(sf::Vector2i& v, int min, sf::Vector2i& max);
		//TODO: kinda belongs in each vector class
		static float Lerp(float left, float right, float ratio);

		static sf::Vector2f& Lerp(sf::Vector2f& left, sf::Vector2f& right, float ratio);
		static sf::Vector3f& Lerp(sf::Vector3f& left, sf::Vector3f& right, float ratio);

		//static Vector4 Lerp(Vector4 left, Vector4 right, float ratio);

		/// <summary>
		/// Normalise value between -1 and 1.
		/// </summary>
		static float NormaliseBetweenNegPve(float value);

		static float RandomFloat();

		static float Range(float min, float max);

		static sf::Vector3f& GetRandomPointInSphere()
		{
			return GetRandomPointInSphere(1.0f);
		}

		static sf::Vector3f& GetRandomPointInSphere(float radius);

		static sf::Vector3f& GetPointInSphere(float radius, float theta, float phiValue);
	};



//}

#endif