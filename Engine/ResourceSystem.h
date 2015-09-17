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

	Texture* addTexture(const char* file);

	Mesh* addMesh(const char* file);

private:

	std::map<std::string, Texture*> _textures;
};

#endif // !__RESOURCESYSTEM_H__
