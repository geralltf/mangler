#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics.hpp>
#include "Box2D/Box2D.h"

using namespace sf;

class Collidable
{
public:
	// Data:
	Vector2f position;
	Vector2f size;

	b2Body* wallBody;
	b2BodyDef* wallBodyDef;
	b2PolygonShape* wallBox;
};

#endif // !WALL_H
