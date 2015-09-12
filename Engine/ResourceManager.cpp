#include "ResourceManager.h"
#include "Image.h"
#include "ImageLoader.h"
#include "Texture.h"
#include <algorithm>
#include <iostream>
#include <string>
#include "Mesh.h"
#include "MeshLoaderB3D.h"

using std::string;

typedef bool(*loadImageFunc)(const char*, Image&);
struct LoaderPlugin{
	const char* name;
	loadImageFunc pFunc;
};

static LoaderPlugin loaderPlugin[] = {
    { "jpg", loadImageJPG},
    { "png", loadImagePNG},
   // { "tga", loadImageTGA},
	{ "bmp", loadImageBMP}
};
static int PluginCount = sizeof(loaderPlugin) / sizeof(LoaderPlugin);

ResourceManager* ResourceManager::sm_pSharedInstance = nullptr;
ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
	
}

ResourceManager* ResourceManager::getInstance()
{
	if (sm_pSharedInstance == NULL)
	{
		sm_pSharedInstance = new ResourceManager();
	}
	return sm_pSharedInstance;
}

Texture* ResourceManager::addTexture(const char* file)
{
	Texture* texture = NULL;

	string fullPath = file;
	auto it = _textures.find(fullPath);
    if( it != _textures.end() ) {
		texture = it->second;
		return texture;
    }

	Image image;

	std::string basename(file);
    std::transform(basename.begin(), basename.end(), basename.begin(), ::tolower);
    
	for (int i = 0; i < PluginCount; ++i)
	{
		if (basename.find(loaderPlugin[i].name) != std::string::npos)
		{
			loaderPlugin[i].pFunc(fullPath.c_str(), image);
			break;
		}
	}

	texture = new Texture();
	texture->init(&image);

	_textures.insert(std::make_pair(fullPath, texture));

	return texture;
};

Mesh* ResourceManager::addMesh(const char* file)
{
	MeshLoaderB3D meshLoader;
	meshLoader.loadMesh(file);

	Mesh* mesh = new Mesh;
	mesh = meshLoader._meshVec[0];

	return mesh;
}