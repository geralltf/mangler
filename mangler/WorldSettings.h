#ifndef WORLD_SETTINGS_H
#define WORLD_SETTINGS_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class WorldSettings
{
public:

	Vector2f cellOffset;
	Vector2f cellScale;
	Vector2f gridPosition;

};

#endif // !WORLD_SETTINGS_H