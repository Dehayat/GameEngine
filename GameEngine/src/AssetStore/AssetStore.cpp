#include "AssetStore.h"
#include "../Logger/Logger.h"
#include "SDL_image.h"

AssetStore::AssetStore()
{
	Logger::Log("Asset Store Started!");
}

AssetStore::~AssetStore()
{
	ClearAssets();
	Logger::Log("Asset Store Destoryed!");
}

void AssetStore::ClearAssets()
{
	for (auto texture : textures) {
		SDL_DestroyTexture(texture.second);
	}
	textures.clear();
}

void AssetStore::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string filePath)
{
	SDL_Surface* surface = IMG_Load(filePath.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	textures[assetId] = texture;
}

SDL_Texture* AssetStore::GetTexture(const std::string& assetId)
{
	return textures[assetId];
}
