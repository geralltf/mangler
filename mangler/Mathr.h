#ifndef MATH_H
#define MATH_H

//namespace ath
//{

	const float PI = 3.14159274F;
	const float Deg2Rad = 0.0174532924F;
	const float Rad2Deg = 57.29578F;
	const float TwoPi = 2 * PI;
	const float PI2 = 2 * PI;
	const float PI3OverTwo = ((3.0f * PI) / 2.0f); // 270 degrees
	const float PiOverTwo = 1.57079637;
	const float PIOver2 = (PI / 2.0f); // 90 degrees
	const float PIOver4 = (PI / 4.0f); // 45 degrees
	const float MAX_FLOAT = 3.402823466e+38F; // max floating point value
	const float MIN_FLOAT = 1.175494351e-38F; // min floating point value


	class Vector2;
	class Vector2i;

#include "Vector2.h"
#include "Vector2i.h"

	enum TexelMode
	{
		TexelMode_Clamp,
		TexelMode_Wrap
	};

	float ClampF(float value, float min, float max);

	Vector2 Clamp(Vector2 uv);

	Vector2 Wrap(Vector2 uv);

	Vector2 NormaliseTexelCoord(Vector2 uv, TexelMode texelMode);

	Vector2i ClampImageSpace(Vector2i texelCoord, Vector2i rectBounds);


//}
#endif