#include "TextureManager.h"
#include <sstream>

TextureManager::TextureManager(GameConfig* config)
{
	this->SystemConfig = config;
	this->_textures = new std::map<string, sf::Texture*>();
}

// Loads a texture given the name with a matching file in the Assets folder.
void TextureManager::LoadTextureByName(AssetTextureName tex)
{
	std::string name = SystemConfig->GetAssetFilePath(tex);

	LoadTextureByName(name);
}

// Loads a texture given the name with a matching file in the Assets folder.
void TextureManager::LoadTextureByName(string name)
{
	sf::Texture* texture = new sf::Texture();

	std::stringstream ss;
	ss << SystemConfig->assetFolder << SystemConfig->PATH_SEPARATOR << name;
	texture->loadFromFile(ss.str());

	(*_textures)[name] = texture;
}

// Gets a pointer to a texture given the specified asset name.
sf::Texture* TextureManager::GetTextureByName(AssetTextureName tex)
{
	std::string name = SystemConfig->GetAssetFilePath(tex);

	return (*_textures)[name];
}

// Gets a pointer to a texture given the specified asset name.
sf::Texture* TextureManager::GetTextureByName(string name)
{
	return (*_textures)[name];
}

// Removes the asset from memory given the specified asset name.
void TextureManager::RemoveAsset(string name)
{
	this->_textures->erase(name);
}

// Loads all texture assets in the Assets folder.
void TextureManager::LoadAllAssets()
{
	LoadTextureByName(TEX_CAMERA);
	LoadTextureByName(TEX_CURSOR);
	LoadTextureByName(TEX_CURSOR2);
	LoadTextureByName(TEX_WATER);
	LoadTextureByName(TEX_FIRE);
	LoadTextureByName(TEX_GRASS);
	LoadTextureByName(TEX_MOUNTAIN);
	LoadTextureByName(TEX_WHITE);
	LoadTextureByName(TEX_BLACK);
	LoadTextureByName(TEX_LINET);
	LoadTextureByName(TEX_PAINTT);
	LoadTextureByName(TEX_PLAYER_R);
	LoadTextureByName(TEX_PLAYER_L);
}

TextureManager::~TextureManager()
{
	delete this->_textures;
}