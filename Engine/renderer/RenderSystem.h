#ifndef __RENDERSYSTEM_H__
#define	__RENDERSYSTEM_H__ 
#include "../glutils.h"
#include "../mat4.h"
#include "../array.h"
#include "../RenderBuffer.h"

#include "../ResourceSystem.h"

class Pipeline;
class Mesh;
class Sprite;


class RenderSystem
{
public:
	virtual ~RenderSystem() {}

	virtual void Init() = 0;

	virtual void FrameUpdate() = 0;

	virtual void DrawString(const char* text) = 0;

	virtual Pipeline* GetPipeline(int idx) = 0;
};


class RenderSystemLocal : public RenderSystem
{
public:
	RenderSystemLocal(glimpParms_t *glimpParms_t);
	~RenderSystemLocal() {}

	void Init();
	void FrameUpdate();
	void DrawString(const char* text);

	Pipeline* GetPipeline(int idx);
private:

	renderBuffer_t	_renderBuffer;
	array<Pipeline*>	_pipelines;
	array<Mesh*>		_meshs;
	Sprite*			_defaultSprite;
};

extern RenderSystem* renderSys;
#endif
