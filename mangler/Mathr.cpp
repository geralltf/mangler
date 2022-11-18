#include "Mathr.h"
//#include <cmath>
#include <algorithm>
#include <stdlib.h>

//namespace ath
//{

	float ClampF(float value, float min, float max)
	{
		return std::max(std::min(value, max), min);
	}

	Vector2 Clamp(Vector2 uv)
	{
		Vector2 clamped(uv.x, uv.y);

		clamped.x = std::max(std::min(clamped.x, 1.0f), 0.0f);
		clamped.y = std::max(std::min(clamped.y, 1.0f), 0.0f);

		return clamped;
	}

	Vector2 Wrap(Vector2 uv)
	{
		float sphereRadius = 1.0;
		//TODO: Might want to look into Toroidal mapping for UV coordinates (which should handle 2D wrapping).

		// Map 3D UV Sphere to 2D uv coordinate:
		// u = 0.5 + ATan2(z,x)/twopi
		// v = 0.5 - ASin(y) / pi

		// Inverse function of above is Mercator projection/spherical wrapping:

		// https://en.wikipedia.org/wiki/Mercator_projection
		float longitude = uv.x / sphereRadius;
		float latitude = (2.0 * std::atan(std::exp(uv.y / sphereRadius))) - PiOverTwo;

		Vector3 xyz(
			sphereRadius * std::cos(latitude) * std::cos(longitude),
			sphereRadius * std::cos(latitude) * std::sin(longitude),
			sphereRadius * std::sin(latitude)
		);

		return xyz.xy();
	}

	Vector2 NormaliseTexelCoord(Vector2 uv, TexelMode texelMode)
	{
		Vector2 normalised = uv;
		switch (texelMode)
		{
		case TexelMode::TexelMode_Clamp:
			normalised = Clamp(uv);
			break;
		case TexelMode::TexelMode_Wrap:
			normalised = Wrap(uv);
			break;
		}
		return normalised;
	}

	Vector2i ClampImageSpace(Vector2i texelCoord, Vector2i rectBounds)
	{
		Vector2i clamped(
			(int)std::max(std::min((double)texelCoord.x, rectBounds.x - 1.0), 0.0),
			(int)std::max(std::min((double)texelCoord.y, rectBounds.y - 1.0), 0.0)
		);
		return clamped;
	}


//}