#include <iostream>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "LevelProvider.h"

using namespace std;
using namespace sf;

LevelProvider::LevelProvider(GameConfig* config, TextureManager* textureManager)
{
	this->SystemConfig = config;
	this->textureManager = textureManager;
	this->world = nullptr;
}

LevelProvider::LevelProvider(GameConfig* config, TextureManager* textureManager, Level* world)
{
	this->SystemConfig = config;
	this->textureManager = textureManager;
	this->world = world;
}

// Gets the current Level.
Level* LevelProvider::GetCurrent()
{
	return this->world;
}

// Save the state of the specified level to a file.
void LevelProvider::Save(const string filename, Level* state, const WorldSettings settings)
{
	std::stringstream ss;
	ss << SystemConfig->assetFolder << SystemConfig->PATH_SEPARATOR << filename;
	ofstream ofs(ss.str());
	const Texture* tex;

	// SAVE level in XML Format.
	if (ofs.is_open())
	{
		Texture* grass = textureManager->GetTextureByName(TEX_GRASS);
		Texture* fire = textureManager->GetTextureByName(TEX_FIRE);
		Texture* water = textureManager->GetTextureByName(TEX_WATER);
		Texture* mountain = textureManager->GetTextureByName(TEX_MOUNTAIN);
		Texture* white = textureManager->GetTextureByName(TEX_WHITE);
		Texture* black = textureManager->GetTextureByName(TEX_BLACK);

		int size = state->cells->size();
		int i;

		ofs << "<?xml version=“1.0” encoding=“utf-8”?>\n";
		ofs << "<level name=\"test\">\n";
		ofs << "		<worldSettings cameraPosition=\"" << state->cameraPosition.x << " " << state->cameraPosition.y << " " << state->cameraPosition.z << "\" />\n";
		ofs << "		<gameSettings players=\"1\" />\n\n";
		ofs << "		<grid w=\"" << world->columns << "\" h=\"" << world->rows << "\">\n";

		for (i = 0; i < size; i++)
		{
			Sprite cell = *(*state->cells)[i];

			tex = cell.getTexture();

			if (tex != nullptr)
			{
				if (tex == grass)
				{
					ofs << Level::TileType::TT_AREA_0;
				}
				else if (tex == fire)
				{
					ofs << Level::TileType::TT_AREA_1;
				}
				else if (tex == defaultBackground)
				{
					ofs << Level::TileType::TT_AREA_2;
				}
				else if (tex == mountain)
				{
					ofs << Level::TileType::TT_AREA_3;
				}
				else if (tex == white)
				{
					ofs << Level::TileType::TT_AREA_4;
				}
				else if (tex == black)
				{
					ofs << Level::TileType::TT_AREA_5;
				}
			}
		}
		ofs << "\n		</grid>\n\n";

		// Append the game objects.
		appendSceneInstances(state, &ofs);


		ofs << "\n		<types>"; // hard coded types will do for now
		ofs << "\n			<gameObject name=\"Chair\"><texture url=\"chair.png\" /><circleShape radius=\"50\" /></gameObject>";
		ofs << "\n			<gameObject name=\"Pistol\"><texture url=\"waterpistol.png\" /><rectShape size=\"50 50 0\" /></gameObject>";
		ofs << "\n			<gameObject name=\"Hat\"><texture url=\"hat.png\" /><rectShape size=\"50 50 0\" /></gameObject>\n\n";

		ofs << "\n			<gameObject name=\"Tp0\"><texture url=\"terrain-piece0.png\" /><rectShape size=\"272 180 0\" /></gameObject>";
		ofs << "\n			<gameObject name=\"Tp1\"><texture url=\"terrain-piece1.png\" /><rectShape size=\"380 204 0\" /></gameObject>";
		ofs << "\n			<gameObject name=\"Tp2\"><texture url=\"terrain-piece2.png\" /><rectShape size=\"243 61 0\" /></gameObject>";
		ofs << "\n		</types>\n";
		ofs << "</level>";

		ofs.close();
	}
}

// Loads the state of a level from a file into the editor.
Level* LevelProvider::Load(string filename, const WorldSettings settings)
{
	Level* level = nullptr;
	XmlParser xmlParser;
	string result;
	int rows, cols;
	Level::TileType v;
	vector<Sprite*>* newGrid;
	Sprite* cell;
	int i, c, r;

	// LOAD level from XML Format.

	stringstream ss;
	ss << SystemConfig->assetFolder << SystemConfig->PATH_SEPARATOR << filename;

	// PARSING
	xmlParser.Parse(ss.str());

	level = new Level();
	newGrid = new vector<Sprite*>();

	// XPATH QUERIES
	CXPathSequence resultGrid = *xmlParser.Expr("//grid[@w @h]");
	CXPathSequence resultLevel = *xmlParser.Expr("//level");
	CXPathSequence resultWorldSettings = *xmlParser.Expr("//worldSettings[@cameraPosition]");
	CXPathSequence resultGameSettings = *xmlParser.Expr("//gameSettings[@players]");
	CXPathNode* itemLevel = (CXPathNode*)resultLevel.GetItemAt(0);
	CXPathNode* itemGrid = (CXPathNode*)resultGrid.GetItemAt(0);
	CXPathSequence* resultTypes = xmlParser.Expr("//types/gameObject");
	CXPathSequence* resultScene = xmlParser.Expr("//scene/gameObject");

	// PARSE level metadata
	level->cameraPosition = parseVec3f((((CXPathNode*)resultWorldSettings.GetItemAt(0))->GetAttributeAt(0))->GetValue());
	level->numPlayers = parseInt((((CXPathNode*)resultGameSettings.GetItemAt(0))->GetAttributeAt(0))->GetValue());
	level->name = itemLevel->GetAttributeAt(0)->GetValue();

	// PARSE game objects
	parseGameObjects(resultTypes, resultScene, level);

	// PARSE the grid data
	cols = stoi(itemGrid->GetAttributeByName("w")->GetValue());
	rows = stoi(itemGrid->GetAttributeByName("h")->GetValue());
	string body = itemGrid->GetValue();
	body.erase(std::remove_if(body.begin(), body.end(), ::isspace), body.end()); // remove whitespace, TODO: slow

																				 // BUILD the grid
	Texture* grass = textureManager->GetTextureByName(TEX_GRASS);
	Texture* fire = textureManager->GetTextureByName(TEX_FIRE);
	Texture* water = textureManager->GetTextureByName(TEX_WATER);
	Texture* mountain = textureManager->GetTextureByName(TEX_MOUNTAIN);
	Texture* white = textureManager->GetTextureByName(TEX_WHITE);
	Texture* black = textureManager->GetTextureByName(TEX_BLACK);

	i = 0;
	c = 0;
	r = 0;

	for (char& gridValue : body)
	{
		v = (Level::TileType)(gridValue - '0');

		cell = new Sprite();

		Vector2f pos;
		pos.x = c * settings.cellOffset.x;
		pos.y = r * settings.cellOffset.y;

		cell->setScale(settings.cellScale);


		switch (v)
		{
		case Level::TileType::TT_AREA_0:
			cell->setTexture(*grass);
			break;
		case Level::TileType::TT_AREA_1:
			cell->setTexture(*fire);
			break;
		case Level::TileType::TT_AREA_2:
			cell->setTexture(*defaultBackground);
			break;
		case Level::TileType::TT_AREA_3:
			cell->setTexture(*mountain);
			break;
		case Level::TileType::TT_AREA_4:
			cell->setTexture(*white);
			break;
		case Level::TileType::TT_AREA_5:
			cell->setTexture(*black);
			break;
		}

		cell->setPosition(pos + settings.gridPosition);

		newGrid->push_back(cell);

		//cout << v;
		//if (c == cols - 1)
		//{
		//	cout << "\n";
		//}

		i++;
		c++;

		if (c >= cols) {
			c = 0;

			r++;

			if (r >= rows) {
				r = 0;
			}
		}
	}

	level->rows = rows;
	level->columns = cols;
	level->cellCount = rows * cols;
	level->cells = newGrid;

	if (world != nullptr)
	{
		delete world;
	}

	world = level;

	return level;
}

// Append the scene instances state as xml to the current stream
void LevelProvider::appendSceneInstances(Level* state, ofstream* ofs)
{
	int i;
	GameObject* inst;
	Vector2f pos;

	(*ofs) << "		<scene>\n";

	for (i = 0; i < state->objectInstances->size(); i++)
	{
		inst = state->objectInstances->at(i);
		(*ofs) << "			";

		pos = inst->shape->getPosition();

		(*ofs) << "<gameObject position =\"" << pos.x << " " << pos.y << " 0\" ";
		(*ofs) << "rotation=\"" << inst->shape->getRotation() << "\" ref=\"" << inst->name << "\" />\n";
	}

	(*ofs) << "		</scene>\n\n";

}

// Parse all the game object types and create object instances.
void LevelProvider::parseGameObjects(CXPathSequence* types, CXPathSequence* scene, Level* level)
{
	// A level needs to have 
	//  -> gameObjectTypes - a collection of the types of game objects that could be in the world
	//  -> objectInstances - a collection of object instances that are a game object type
	CXPathItem* item;
	CXPathNode *node, *child;
	CXPathSequence* children;
	string qName;
	string objName;
	string url;
	float radius;
	Vector3f size;
	int i, j;
	GameObject* objPrefab;
	string test;

	// SETUP type system

	level->gameObjectTypes = new map<string, GameObject*>();

	for (i = 0; i < types->GetSize(); i++)
	{
		item = types->GetItemAt(i);
		node = (CXPathNode*)item;
		objName = node->GetAttributeAt(0)->GetValue();
		children = node->GetChildren();

		// Parse all prefabs.
		objPrefab = new GameObject();

		for (j = 0; j < children->GetSize(); j++)
		{
			child = (CXPathNode*)children->GetItemAt(j);
			qName = string(child->GetQName());

			if (qName == "texture")
			{
				url = child->GetAttributeAt(0)->GetValue();

				textureManager->LoadTextureByName(url);

				objPrefab->texture = textureManager->GetTextureByName(url);
				//objPrefab->sprite = new Sprite(*objPrefab->texture);
			}
			else if (qName == "circleShape")
			{
				radius = parseFloat(child->GetAttributeAt(0)->GetValue());

				objPrefab->shape = new CircleShape(radius);
				//objPrefab->shape->setFillColor(sf::Color(100, 250, 50)); // for testing
				//objPrefab->shape->setOutlineThickness(10);
				//objPrefab->shape->setOutlineColor(sf::Color(250, 150, 100));
				//objPrefab->shape->setTextureRect(sf::IntRect(10, 10, 100, 100));
			}
			else if (qName == "rectShape")
			{
				test = child->GetAttributeAt(0)->GetValue();

				size = parseVec3f(test);

				objPrefab->shape = new RectangleShape(Vector2f(size.x, size.y));
				//objPrefab->shape->setFillColor(sf::Color(100, 250, 50));  // for testing
				//objPrefab->shape->setOutlineThickness(10);
				//objPrefab->shape->setOutlineColor(sf::Color(250, 150, 100));
				//objPrefab->shape->setTextureRect(sf::IntRect(10, 10, 100, 100));
			}
		}

		objPrefab->shape->setTexture(objPrefab->texture);
		objPrefab->name = objName;

		// Append game object type to type system.
		(*level->gameObjectTypes)[objName] = objPrefab;
	}

	// SETUP object instances
	// create instances from the prefabs defined above

	level->objectInstances = new vector<GameObject*>();

	string ref; // the name of the prefab that is referenced
	Vector3f position, rotation;
	GameObject* inst;

	for (i = 0; i < scene->GetSize(); i++)
	{
		// Parse object instance configuration.
		item = scene->GetItemAt(i);
		node = (CXPathNode*)item;
		objName = node->GetAttributeAt(0)->GetValue();
		ref = node->GetAttributeByName("ref")->GetValue();
		position = parseVec3f(node->GetAttributeByName("position")->GetValue());
		rotation = parseVec3f(node->GetAttributeByName("rotation")->GetValue());

		// Get prefab defined by type system.
		objPrefab = (*level->gameObjectTypes)[ref];

		// Create instance of prefab.
		inst = objPrefab->Instantiate(); // clone prefab
		inst->name = ref;

		// Configure new instance with runtime properties.
		inst->transform = Transform::Identity;
		inst->transform.translate(Vector2f(position.x, position.y));
		inst->transform.rotate(rotation.x);

		inst->shape->setPosition(Vector2f(position.x, position.y));
		inst->shape->setRotation(rotation.x);

		// Add to scene.
		level->objectInstances->push_back(inst);
	}

}

// Parse a single integer from a string value.
int LevelProvider::parseInt(string stringValue)
{
	return stoi(stringValue);
}

// Parse a single floating point value from a string value.
float LevelProvider::parseFloat(string stringValue)
{
	return stof(stringValue);
}

// Parse a set of integers from a string value.
vector<int> LevelProvider::parseInts(string stringValue)
{
	vector<int> result;

	std::istringstream iss(stringValue);

	std::copy(std::istream_iterator<int>(iss), std::istream_iterator<int>(), std::back_inserter(result));

	return result;
}

// Parse a set of floats from a string value.
vector<float> LevelProvider::parseFloats(string stringValue)
{
	vector<float> result;

	std::istringstream iss(stringValue);

	std::copy(std::istream_iterator<float>(iss), std::istream_iterator<float>(), std::back_inserter(result));

	return result;
}

// Parse a set of Vec3f from a string value.
Vector3f LevelProvider::parseVec3f(string stringValue)
{
	vector<float> result;

	std::istringstream iss(stringValue);

	std::copy(std::istream_iterator<float>(iss), std::istream_iterator<float>(), std::back_inserter(result));


	if (result.size() != 3)
	{
		cout << "could not parse Vec3f" << endl;
		return Vector3f(0, 0, 0);
	}

	return Vector3f(result[0], result[1], result[2]);
}

LevelProvider::~LevelProvider()
{
	delete this->defaultBackground;
	delete this->world;
}

