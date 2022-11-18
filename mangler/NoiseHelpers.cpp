#include "NoiseHelpers.h"

#include <random>
#include "MathHelpers.h"
#include "Mathr.h"

//namespace ath
//{

	float _noise[NOISE_W][NOISE_H];
	sf::Vector2i _sampleSize(NOISE_W, NOISE_H);
	bool _rngLoaded = false;
	std::mt19937_64 _rng;
	std::uniform_real_distribution<float> dist01(0, 1); // Create a distribution in range [0 1].
	std::uniform_int_distribution<int> dist0255(0, 255); // Create a distribution in range [0 255]
	bool _noiseSeedChanged = false;
	int _noise2[3 * NOISE_W];

	sf::Vector2i TransformToGridSpace(sf::Vector2f gridPosition, sf::Vector2f cellOffset, sf::Vector2f mousePosition)
	{
		sf::Vector2i cellPos;

		cellPos.x = ((mousePosition.x - gridPosition.x) / cellOffset.x) + 1;

		cellPos.y = ((mousePosition.y - gridPosition.y) / cellOffset.y) + 1;

		return cellPos;
	}

	int GetMatrixIndex(int x, int y, int columns)
	{
		return x + ((columns) * (y - 1)) - 1;
	}

	/*
	@Description: Computes a list of points of a line segment between two vectors.
	*/
	vector<sf::Vector2i> Line(sf::Vector2i& begin, sf::Vector2i& end)
	{
		// Bresenham Line Algorithm
		vector<sf::Vector2i> lst;
		int x, y, deltaX, deltaY, numerator;
		int longest;
		int shortest;
		int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
		int i;
	
		x = begin.x;
		y = begin.y;
		deltaX = (end.x - begin.x);
		deltaY = (end.y - begin.y);
	
		if (deltaX < 0)
		{
			dx1 = -1;
		}
		else if (deltaX > 0)
		{
			dx1 = 1;
		}
	
		if (deltaY < 0)
		{
			dy1 = -1;
		}
		else if (deltaY > 0)
		{
			dy1 = 1;
		}
	
		if (deltaX < 0)
		{
			dx2 = -1;
		}
		else if (deltaX > 0)
		{
			dx2 = 1;
		}
	
	
		longest = abs(deltaX);
		shortest = abs(deltaY);
	
		if (longest <= shortest)
		{
			longest = abs(deltaY);
			shortest = abs(deltaX);
	
			if (deltaY < 0)
			{
				dy2 = -1;
			}
			else if (deltaY > 0)
			{
				dy2 = 1;
			}
			dx2 = 0;
		}
	
		numerator = longest >> 1;
	
		for (i = 0; i <= longest; i++)
		{
			lst.push_back(sf::Vector2i(x, y));
	
			numerator += shortest;
	
			if (!(numerator < longest))
			{
				numerator -= longest;
	
				x += dx1;
				y += dy1;
			}
			else
			{
				x += dx2;
				y += dy2;
			}
		}
	
		return lst;
	}

	bool WithinBounds(sf::Vector2f p, sf::Rect<float> bounds)
	{
		return (((p.x >= bounds.left - bounds.width) && (p.x <= bounds.left + bounds.width)) && ((p.y >= bounds.top - bounds.height) && p.y <= (bounds.top + bounds.height)));
	}

	bool WithinBounds(sf::Vector2u p, sf::Rect<float> bounds)
	{
		return (((p.x >= bounds.left - bounds.width) && p.x <= bounds.left + bounds.width) && ((p.y >= bounds.top - bounds.height) && (p.y <= bounds.top + bounds.height)));
	}

	float Clamp(float min, float max, float value)
	{
		value = value < min ? min : value;
		return value > max ? max : value;
	}

	/*
	Description: Generate 2D Perlin noise.
	*/
	float PerlinNoise2D(float x, float y)
	{
		return PerlinNoise2D(x, y, SAMPLE_FREQ, SAMPLE_AMP, SAMPLE_PERS, SAMPLE_OCTAVES);
	}

	float PerlinNoise3D(float x, float y, float z)
	{
		return PerlinNoise3D(x, y, z, SAMPLE_FREQ, SAMPLE_AMP, SAMPLE_PERS, SAMPLE_OCTAVES);
	}

	/*
	Description: Generates sample noise using a normalised random distribution. Call this function once on initilisation.
	*/
	void Init_Perlin_Noisy()
	{
		// Create a RNG with a random seed.

		if (!_rngLoaded)
		{
			_rng.seed(std::random_device()());

			_rngLoaded = true;
		}

		// Fill the noise sample.
		for (int x = 0; x < _sampleSize.x; x++)
		{
			for (int y = 0; y < _sampleSize.y; y++)
			{
				// Compute normalised noise value.
				_noise[x][y] = dist01(_rng);
			}
		}

		// Initilise 3D Noise Permutations
		for (int i = 0; i < 3 * NOISE_W; i++)
		{
			_noise2[i] = dist0255(_rng);
		}

		_noiseSeedChanged = true;
	}

	sf::Uint8* GetNoisePixelData()
	{
		sf::Uint8* _noisePixels = new sf::Uint8[NOISE_W * NOISE_H * 4]; // RGBA
		int i, x, y;
		float n;
		sf::Uint8 grey;

		// Store noise value in pixel buffer to use as a texture for debugging

		for (x = 0; x < NOISE_W; x++)
		{
			for (y = 0; y < NOISE_H; y++)
			{
				//n = PerlinNoise2D(x, y);
				n = PerlinNoise3D(x, y, 0); //HACK

				i = ((NOISE_H * y) + x);

				grey = n * 255;
				_noisePixels[(i * 4) + 0] = grey;
				_noisePixels[(i * 4) + 1] = grey;
				_noisePixels[(i * 4) + 2] = grey;
				_noisePixels[(i * 4) + 3] = 255;
			}
		}

		_noiseSeedChanged = false;

		return _noisePixels;
	}

	bool HasNoiseChanged()
	{
		return _noiseSeedChanged;
	}

	/*
	Description: Generate 2D Perlin noise.
	Informal: Computes a 2D noise value given the specified 2D coordinate and settings.
	Formal: Computes a noise value using the specified settings: octaves, frequency, amplitude, and persistance.
	Returns: Returns a floating point value between -1 and 1.
	*/
	float PerlinNoise2D(float x, float y, float frequency, float amplitude, float persistance, int octaves)
	{
		float FinalValue = 0.0f;
		int i;
		float amp = amplitude;
		float freq = frequency;

		//HACK: slow
		float *copy[NOISE_W];
		for (i = 0; i < NOISE_W; i++)
		{
			copy[i] = _noise[i];
		}

		// PERLIN NOISE
		for (i = 0; i < octaves; i++)
		{
			// Smoothing makes more refined perlin noise.
			FinalValue += BilinearInterpolation(x * freq, y * freq, copy) * amp;

			freq *= 2.0f;

			amp *= persistance;
		}

		FinalValue = FinalValue / ((octaves * persistance) * amplitude);

		// Normalise between 0 and 1
		FinalValue = Clamp(0.0f, 1.0f, FinalValue); // normalise between 0 and 1

#ifdef SAMPLE_USE_EXP
												// Apply exponent to reduce artifacts/features in noise.
		FinalValue = std::pow(FinalValue, SAMPLE_ATTENUATION); // feature reduction through attenuation.
#endif

		return FinalValue;
	}

	float PerlinNoise3D(float x, float y, float z, float frequency, float amplitude, float persistance, int octaves)
	{
		float FinalValue = 0.0f;
		int i;
		float amp = amplitude;
		float freq = frequency;

		// PERLIN NOISE
		for (i = 0; i < octaves; i++)
		{
			// Smoothing makes more refined perlin noise.
			FinalValue += Noise(x * freq, y * freq, z * freq) * amp;

			freq *= 2.0f;

			amp *= persistance;
		}

		FinalValue = FinalValue / ((octaves * persistance) * amplitude);

		// Normalise between 0 and 1
		FinalValue = Clamp(0.0f, 1.0f, FinalValue); // normalise between 0 and 1

#ifdef SAMPLE_USE_EXP
												// Apply exponent to reduce artifacts/features in noise.
		FinalValue = std::pow(FinalValue, SAMPLE_ATTENUATION); // feature reduction through attenuation.
#endif

		return FinalValue;
	}

	/*
	Description: Bilinear filtering function.
	Informal: Applies a Bilinear Filter to a 2 Dimensional Array.
	Formal: Specify a coordinate in the specified sample
	and the function will return the smoothed value using 4 adjacent values.
	Could be used in a convolution kernel.
	*/
	float BilinearInterpolation(float x, float y, float** sample)
	{
		float smooth = 0.0f;

		int width = _sampleSize.x;
		int height = _sampleSize.y;

		float fractionX = x - (int)x;
		float fractionY = y - (int)y;

		int X1 = ((int)x + width) % width;
		int Y1 = ((int)y + height) % height;
		int X2 = ((int)x + width - 1) % width;
		int Y2 = ((int)y + height - 1) % height;


		smooth += fractionX * fractionY * sample[X1][Y1];
		smooth += fractionX * (1 - fractionY) * sample[X1][Y2];

		smooth += (1 - fractionX) * fractionY * sample[X2][Y1];
		smooth += (1 - fractionX) * (1 - fractionY) * sample[X2][Y2];

		return smooth;
	}
	float NoiseWeight(float t)
	{
		float t3 = t*t*t;
		float t4 = t3 * t;
		return (6.0f* t4 * t) - (15.0f * t4) + (10.0f * t3);
	}

	float Grad(int x, int y, int z, float dx, float dy, float dz)
	{
		// test permutation table
		int h = _noise2[_noise2[_noise2[x] + y] + z];
		h &= 15;
		float u = (h < 8 || h == 12 || h == 13) ? dx : dy;
		float v = (h < 4 || h == 12 || h == 13) ? dy : dz;
		return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
	}

	int FloorI(float val) { return (int)floorf(val); }

	float Noise(float x, float y, float z) { return Noise(sf::Vector3f(x, y, z)); }

	float Noise(sf::Vector3f p)
	{
		// Compute noise cell coordinates and offsets
		int ix = FloorI(p.x), iy = FloorI(p.y), iz = FloorI(p.z);
		float dx = p.x - ix, dy = p.y - iy, dz = p.z - iz;

		// Compute gradient weights for each corner of 3D cube (8 verticies)
		ix &= (256 - 1);
		iy &= (256 - 1);
		iz &= (256 - 1);

		float q0 = Grad(ix, iy, iz, dx, dy, dz);
		float q4 = Grad(ix + 1, iy, iz, dx - 1, dy, dz);
		float q2 = Grad(ix, iy + 1, iz, dx, dy - 1, dz);
		float q6 = Grad(ix + 1, iy + 1, iz, dx - 1, dy - 1, dz);
		float q1 = Grad(ix, iy, iz + 1, dx, dy, dz - 1);
		float q5 = Grad(ix + 1, iy, iz + 1, dx - 1, dy, dz - 1);
		float q3 = Grad(ix, iy + 1, iz + 1, dx, dy - 1, dz - 1);
		float q7 = Grad(ix + 1, iy + 1, iz + 1, dx - 1, dy - 1, dz - 1);

		// Smooth 
		return TrilinearInterpolation(p, q0, q1, q2, q3, q4, q5, q6, q7);
	}

	float TrilinearInterpolation(sf::Vector3f p, float q0, float q1, float q2, float q3, float q4, float q5, float q6, float q7)
	{
		int ix = FloorI(p.x), iy = FloorI(p.y), iz = FloorI(p.z);
		float dx = p.x - ix, dy = p.y - iy, dz = p.z - iz;

		// Compute gradient weights for each corner of 3D cube (8 verticies)
		ix &= (256 - 1);
		iy &= (256 - 1);
		iz &= (256 - 1);

		// Compute trilinear interpolation of weights
		float wx = NoiseWeight(dx), wy = NoiseWeight(dy), wz = NoiseWeight(dz);
		float x00 = Coserp(q0, q4, wx);
		float x10 = Coserp(q2, q6, wx);
		float x01 = Coserp(q1, q5, wx);
		float x11 = Coserp(q2, q7, wx);

		float y0 = Coserp(x00, x10, wy);
		float y1 = Coserp(x01, x11, wy);

		return Coserp(y0, y1, wz);
	}

	float Lerp(float from, float to, float ratio)
	{
		return((from * (1 - ratio)) + (to * ratio));
		// return (from + (to - from) * ratio);
	}

	float Coserp(float from, float to, float ratio)
	{
		// Translate first to ratio in a circle  
		float circularRatio = (1 - cos(ratio * PI)) / 2.0f;

		// Interpolate linearly based on circular function
		return Lerp(from, to, circularRatio);
	}

//}