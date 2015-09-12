#pragma once
#include <unordered_map>
#include <map>
class Texture;
class Mesh;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

    static ResourceManager* getInstance();

	Texture* addTexture(const char* file);

	Mesh* addMesh(const char* file);

private:

    static ResourceManager* sm_pSharedInstance;


	std::map<std::string, Texture*> _textures;
};
