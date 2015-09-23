#ifndef __RESOURCESYSTEM_H__
#define __RESOURCESYSTEM_H__

#include <unordered_map>
#include <map>

class Texture;
class Mesh;
class Shader;

class ResourceSystem
{
public:
	ResourceSystem();
	~ResourceSystem();

	Texture* AddTexture(const char* file);

	Texture* AddText(const char* text);

	Mesh* AddMesh(const char* file);

	Shader* AddShader(const char* vfile, const char* ffile);
private:

	std::map<std::string, Texture*> _textures;
};

extern ResourceSystem* resourceSys;
#endif // !__RESOURCESYSTEM_H__
