#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

using namespace std;
using namespace sf;

class GameObject
{
public:
	string name;
	Transform transform;
	Texture* texture;
	Shape* shape;
	//Sprite* sprite;

	GameObject* Instantiate()
	{
		GameObject* inst = new GameObject();
		inst->name = name;
		inst->transform = transform;
		inst->texture = texture;
		inst->shape = shape;

		if (CircleShape* circle = dynamic_cast< CircleShape* >(&*shape))
		{
			inst->shape = new CircleShape(circle->getRadius());
		}
		else if (RectangleShape* rect = dynamic_cast< RectangleShape* >(&*shape))
		{
			inst->shape = new RectangleShape(rect->getSize());
		}

		inst->shape->setTexture(inst->texture);

		return inst;
	}
};

class Level
{
public:
	enum TileType
	{
		TT_AREA_0 = 0,
		TT_AREA_1 = 1,
		TT_AREA_2 = 2,
		TT_AREA_3 = 3,
		TT_AREA_4 = 4,
		TT_AREA_5 = 5
	};
	typedef map<string, GameObject*>* GameObjectTypes;
	typedef vector<GameObject*>* GameObjectInstances;
public:
	int rows;
	int columns;
	int cellCount;
	vector<Sprite*>* cells;
	string name;
	Vector3f cameraPosition;
	int numPlayers;

	GameObjectTypes gameObjectTypes = nullptr;
	GameObjectInstances objectInstances = nullptr;

	Level() {}
	~Level() {}
};

#endif // !LEVEL_H