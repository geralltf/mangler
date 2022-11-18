#ifndef LEVELPROV_H
#define LEVELPROV_H

#include <vector>
#include <string>

#include <SFML/Graphics.hpp>
#include "Level.h"
#include "TextureManager.h"
#include "WorldSettings.h"
#include "GameConfig.h"
#include "XPath3/XmlParser.h"

using namespace std;
using namespace sf;

class LevelProvider
{
public:
	const Texture* defaultBackground = nullptr;

private:
	Level* world = nullptr;
	TextureManager* textureManager;
	GameConfig* SystemConfig;

public:
	LevelProvider(GameConfig* config, TextureManager* textureManager);
	LevelProvider(GameConfig* config, TextureManager* textureManager, Level* world);
	~LevelProvider();

	// Gets the current Level.
	Level* GetCurrent();

	// Save the state of the specified level to a file.
	void Save(const string filename, Level* state, const WorldSettings settings);

	// Loads the state of a level from a file into the editor.
	Level* Load(const string filename, const WorldSettings settings);

private:

	// Append the scene instances state as xml to the current stream
	void appendSceneInstances(Level* state, ofstream* ofs);

	// Parse all the game object types and create object instances.
	void parseGameObjects(CXPathSequence* types, CXPathSequence* scene, Level* level);

	// Parse a single integer from a string value.
	int parseInt(string stringValue);

	// Parse a single floating point value from a string value.
	float parseFloat(string stringValue);

	// Parse a set of integers from a string value.
	vector<int> parseInts(string stringValue);

	// Parse a set of floats from a string value.
	vector<float> parseFloats(string stringValue);

	// Parse a set of Vec3f from a string value.
	Vector3f parseVec3f(string stringValue);
};

#endif