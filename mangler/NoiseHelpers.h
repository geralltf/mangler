#ifndef MATHHELPERS_H
#define MATHHELPERS_H

#include <SFML/Graphics.hpp>
#include <vector>
//#include "VectorLib.h"

//namespace ath
//{

	//using namespace sf;
	using namespace std;

	// Noise settings
#define NOISE_W 256
#define NOISE_H 256
#define SAMPLE_ATTENUATION (1 / 0.7)

// 3D
#define SAMPLE_FREQ 0.07f
#define SAMPLE_AMP 1.0f
#define SAMPLE_PERS 0.1f
#define SAMPLE_OCTAVES 10

// 2D
//#define SAMPLE_FREQ 0.2f
//#define SAMPLE_AMP 1.0f
//#define SAMPLE_PERS 0.25f
//#define SAMPLE_OCTAVES 8

//#define SAMPLE_USE_EXP

// Math constants
//float PI = atan(1) * 4.0f; 
//#define PI 3.14159265358979323846f // 180 degrees
//#define PI2 = (2.0f * PI) // 360 degrees 
//#define PI3OverTwo = ((3.0f * PI) / 2.0f) // 270 degrees
//#define PIOver2 = (PI / 2.0f) // 90 degrees
//#define PIOver4 = (PI / 4.0f) // 45 degrees
//#define MAX_FLOAT 3.402823466e+38F // max floating point value
//#define MIN_FLOAT 1.175494351e-38F // min floating point value

// MISC MATH FUNCTIONS
	sf::Vector2i TransformToGridSpace(sf::Vector2f gridPosition, sf::Vector2f cellOffset, sf::Vector2f mousePosition);
	int GetMatrixIndex(int x, int y, int columns);
	float Clamp(float min, float max, float value);

	// DRAWING ALGORITHMS

	/*
	@Description: Computes a list of points of a line segment between two vectors.
	*/
	vector<sf::Vector2i> Line(sf::Vector2i& begin, sf::Vector2i& end);

	// BOUNDS CHECKING
	bool WithinBounds(sf::Vector2f p, sf::Rect<float> bounds);
	bool WithinBounds(sf::Vector2u p, sf::Rect<float> bounds);

	// NOISE FUNCTIONS
	void Init_Perlin_Noisy(); // perlin noise initilisation function
	float PerlinNoise2D(float x, float y);
	float PerlinNoise2D(float x, float y, float frequency, float amplitude, float persistance, int octaves);
	float PerlinNoise3D(float x, float y, float z);
	float PerlinNoise3D(float x, float y, float z, float frequency, float amplitude, float persistance, int octaves);
	sf::Uint8* GetNoisePixelData();
	bool HasNoiseChanged();
	float Noise(float x, float y, float z);
	float Noise(sf::Vector3f p);

	// INTERPOLATORS
	float Lerp(float from, float to, float ratio); // Linear Interpolator
	float Coserp(float from, float to, float ratio); // Cosine Interpolator
	float BilinearInterpolation(float x, float y, float** sample);
	float TrilinearInterpolation(sf::Vector3f p, float q0, float q1, float q2, float q3, float q4, float q5, float q6, float q7);


//}
#endif