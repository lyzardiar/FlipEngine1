#include "ResourceSystem.h"
#include "Image.h"
#include "ImageLoader.h"
#include "Texture.h"
#include <algorithm>
#include <iostream>
#include <string>
#include "MeshLoaderB3D.h"
#include "Shader.h"
#include "glutils.h"
#include "../sys/sys_public.h"
#include "Model.h"
#include "Material.h"
#include "File.h"

using std::string;

typedef bool(*loadImageFunc)(const char*, Image&);
struct LoaderPlugin{
	const char* name;
	loadImageFunc pFunc;
};

static LoaderPlugin loaderPlugin[] = {
    { "jpg", loadImageJPG},
    { "png", loadImagePNG},
	{ "tga", loadImageTGA},
	{ "bmp", loadImageBMP},
};
static int PluginCount = sizeof(loaderPlugin) / sizeof(LoaderPlugin);
static Texture* defaultTexture;

static sysTextContent_t textContent;

ResourceSystem* resourceSys = NULL;

//ResourceManager* ResourceManager::sm_pSharedInstance = nullptr;
ResourceSystem::ResourceSystem()
{
	defaultTexture = AddTexture("../Media/nskinbl.jpg");
}

ResourceSystem::~ResourceSystem()
{
	
}

//ResourceManager* ResourceManager::getInstance()
//{
//	if (sm_pSharedInstance == NULL)
//	{
//		sm_pSharedInstance = new ResourceManager();
//	}
//	return sm_pSharedInstance;
//}

Texture* ResourceSystem::AddTexture(const char* file)
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
			if( !loaderPlugin[i].pFunc(fullPath.c_str(), image) )
			{
				Sys_Printf( "load image %s failed\n", fullPath.c_str() );
				return defaultTexture;
			}
			else
			{
				texture = new Texture();
				texture->Init(&image);

				_textures.insert(std::make_pair(fullPath, texture));
				return texture;
			}
		}
	}

	Sys_Printf( "load image %s failed\n", fullPath.c_str() );
	return defaultTexture;
};

StaticModel* ResourceSystem::AddMesh(const char* file)
{
	MeshLoaderB3D meshLoader;
	meshLoader.Load(file);

	int size = meshLoader._textures.size();
	for (int i = 0; i<size; ++i)
	{
		Texture* tex = AddTexture( meshLoader._textures[i].TextureName.c_str() );
	}
	return meshLoader._model;
}

Shader* ResourceSystem::AddShader(const char* vfile, const char* ffile)
{
	Shader* shader = new Shader;
	shader->LoadFromBuffer(vfile, ffile);
	return shader;
}

Texture* ResourceSystem::AddText( const char* text )
{
	if( Sys_DrawText(text, &textContent))
	{
		// get the texture pixels, width, height
		Texture* texture = new Texture;
		texture->Init(textContent.w, textContent.h, textContent.pData);	
		return texture;
	}
	else
	{
		Sys_Printf("sys_drawtext error %s\n", text);
		return defaultTexture;
	}
}

Shader* ResourceSystem::AddShaderFromFile( const char* vfile, const char* ffile )
{
	Shader* shader = new Shader;
	shader->LoadFromFile(vfile, ffile);
	return shader;
}

Material* ResourceSystem::AddMaterial( const char* file )
{
	Material* mtr;
	string fullPath = file;
	auto it = _materials.find(fullPath);
	if( it != _materials.end() ) {
		mtr = it->second;
		return mtr;
	}

	mtr = new Material();
	const char* buffer = F_ReadFileData(file); //"../media/Position.mtr");

	if (buffer == NULL)
	{
		Sys_Error("add material failed %s\n", file);
		return NULL;
	}

	mtr->LoadMemory(buffer);
	_materials.insert(std::make_pair(fullPath, mtr));
	return mtr;
}

