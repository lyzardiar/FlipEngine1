#ifndef __RESOURCESYSTEM_H__
#define __RESOURCESYSTEM_H__

#include <unordered_map>
#include <map>

class Texture;
class Mesh;

class ResourceSystem
{
public:
	ResourceSystem();
	~ResourceSystem();

	Texture* AddTexture(const char* file);

	Mesh* AddMesh(const char* file);

private:

	std::map<std::string, Texture*> _textures;
};

#endif // !__RESOURCESYSTEM_H__
