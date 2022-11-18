#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "GameConfig.h"

using namespace std;

class TextureManager
{
private:
	std::map<string, sf::Texture*>* _textures;
	GameConfig* SystemConfig;

public:
	TextureManager(GameConfig* config);
	~TextureManager();

	// Loads a texture given the name with a matching file in the Assets folder.
	void LoadTextureByName(AssetTextureName tex);

	// Loads a texture given the name with a matching file in the Assets folder.
	void LoadTextureByName(string name);

	// Gets a pointer to a texture given the specified asset name.
	sf::Texture* GetTextureByName(string name);

	// Gets a pointer to a texture given the specified asset name.
	sf::Texture* GetTextureByName(AssetTextureName tex);

	// Removes the asset from memory given the specified asset name.
	void RemoveAsset(string name);

	// Loads all texture assets in the Assets folder.
	void LoadAllAssets();
};

#endif