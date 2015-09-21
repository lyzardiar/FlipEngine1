#ifndef __RENDERSYSTEM_H__
#define	__RENDERSYSTEM_H__ 
#include "../glutils.h"
#include "../mat4.h"
#include "../array.h"
#include "../RenderBuffer.h"

class Material;
class Pipeline;
class Mesh;
class ResourceSystem;

class RenderSystem
{
public:
	virtual ~RenderSystem() {}

	virtual void Init() = 0;

	virtual void FrameUpdate() = 0;
};


class RenderSystemLocal : public RenderSystem
{
public:
	RenderSystemLocal(glimpParms_t *glimpParms_t);
	~RenderSystemLocal() {}

	void Init();

	void FrameUpdate();

private:

	renderBuffer_t _renderBuffer;

	array<Pipeline*> _pipelines;

	array<Mesh*> _meshs;


	ResourceSystem* _resourceSys;
};
#endif
