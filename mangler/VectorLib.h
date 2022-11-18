#ifndef VECTORLIB_H
#define VECTORLIB_H

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

float GetMagnitude(Vector2f v);
void Normalise(Vector2f v);
void Subtract(Vector2f v);
float GetDistance(Vector2f a, Vector2f b);
float GetDistanceSquared(Vector2f a, Vector2f b);

#endif