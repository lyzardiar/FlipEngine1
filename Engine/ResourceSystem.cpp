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

#include "../ShaderSource.h"
using std::string;

static LoaderPlugin loaderPlugin[] = {
    { "jpg", loadImageJPG},
    { "png", loadImagePNG},
	{ "tga", loadImageTGA},
	{ "bmp", loadImageBMP},
};
static int TexPluginCount = sizeof(loaderPlugin) / sizeof(LoaderPlugin);
static Texture* defaultTexture;
//--------------------------------------------------------------------------------------------
static Shader* LoadPostionShader()
{
	Shader* shader = new Shader;
	shader->LoadFromBuffer(position_vert, position_frag);
	shader->SetName("position");
	shader->BindAttribLocation(eAttrib_Position);
	shader->GetUniformLocation(eUniform_MVP);
	shader->GetUniformLocation(eUniform_Color);
	return shader;
}

static Shader* LoadPositionTexShader()
{
	Shader* shader = new Shader;
	shader->LoadFromBuffer(positiontex_vert, positiontex_frag);
	shader->SetName("positionTex");
	shader->BindAttribLocation(eAttrib_Position);
	shader->BindAttribLocation(eAttrib_TexCoord);
	shader->GetUniformLocation(eUniform_MVP);
	shader->GetUniformLocation(eUniform_Samper0);
	return shader;
}

static Shader* LoadPhongShader()
{
	Shader* shader = new Shader;
	shader->LoadFromFile("../media/shader/phong.vert", "../media/shader/phong.frag");
	shader->SetName("phong");
	shader->BindAttribLocation(eAttrib_Position);
	shader->BindAttribLocation(eAttrib_TexCoord);
	shader->BindAttribLocation(eAttrib_Normal);

	shader->GetUniformLocation(eUniform_MVP);
	shader->GetUniformLocation(eUniform_EyePos);
	shader->GetUniformLocation(eUniform_LightPos);
	shader->GetUniformLocation(eUniform_ModelView);
	shader->GetUniformLocation(eUniform_InvModelView);
	shader->GetUniformLocation(eUniform_Samper0);
	GL_CheckError("load phong shader");
	return shader;
}

static Shader* LoadBumpShader()
{
	Shader* shader = resourceSys->AddShaderFromFile("../media/shader/bump.vert",
		"../media/shader/bump.frag");
	shader->SetName("bump");
	shader->BindAttribLocation(eAttrib_Position);
	shader->BindAttribLocation(eAttrib_TexCoord);
	shader->BindAttribLocation(eAttrib_Normal);
	shader->BindAttribLocation(eAttrib_Tangent);
	shader->BindAttribLocation(eAttrib_Binormal);

	shader->GetUniformLocation(eUniform_MVP);
	shader->GetUniformLocation(eUniform_EyePos);
	shader->GetUniformLocation(eUniform_LightPos);
	shader->GetUniformLocation(eUniform_ModelView);
	shader->GetUniformLocation(eUniform_InvModelView);
	shader->GetUniformLocation(eUniform_Samper0);
	shader->GetUniformLocation(eUniform_BumpMap);
	return shader;
}

static Shader* LoadBlurShader()
{
	Shader* shader = new Shader;
	shader->LoadFromFile("../media/blur.vs", "../media/blur.fs");
	shader->SetName("blur");
	shader->BindAttribLocation(eAttrib_Position);
	shader->BindAttribLocation(eAttrib_TexCoord);
	shader->GetUniformLocation(eUniform_MVP);
	shader->GetUniformLocation(eUniform_Samper0);
	return shader;
}


static ShaderPlugin shaderplugin[] = {
	{ eShader_Position, LoadPostionShader },
	{ eShader_PositionTex, LoadPositionTexShader },
	//{ eShader_Phong, LoadPhongShader },
	//{ eShader_Blur, LoadBlurShader  },
	//{ eShader_Bump, LoadBumpShader },
};
static int ShaderPluginCount = sizeof(shaderplugin) / sizeof(ShaderPlugin);

//--------------------------------------------------------------------------------------------

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
    
	for (int i = 0; i < TexPluginCount; ++i)
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

bool ResourceSystem::LoadAllShader()
{
	memset(_shaders, 0, 32);
	for (int i =0; i<ShaderPluginCount; i++)
	{
		_shaders[shaderplugin[i].name] = shaderplugin[i].func();
	}
	return true;
}

Shader* ResourceSystem::FindShader( int shaderId )
{
	if (shaderId >= MAX_SHADER_COUNT && shaderId < 0)
	{
		Sys_Error("find shader out of bounds");
		return NULL;
	}	

	return _shaders[shaderId];
}

