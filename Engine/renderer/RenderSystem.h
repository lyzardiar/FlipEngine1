#ifndef __RENDERSYSTEM_H__
#define	__RENDERSYSTEM_H__ 
#include "../glutils.h"
#include "../common/mat4.h"
#include "../common/array.h"
#include "../r_public.h"

#include "../ResourceSystem.h"

class Pipeline;
class StaticModel;
class Sprite;


class RenderSystem
{
public:
	virtual ~RenderSystem() {}

	virtual void Init() = 0;

	virtual void FrameUpdate() = 0;

	virtual void DrawString(const char* text) = 0;

	virtual bool AddStaticModel(StaticModel* model) = 0;
};

class RenderSystemLocal : public RenderSystem
{
public:
	RenderSystemLocal(glimpParms_t *glimpParms_t);
	~RenderSystemLocal() {}

	void Init();
	void FrameUpdate();
	void DrawString(const char* text);
	virtual bool AddStaticModel(StaticModel* model);


private:

	renderBuffer_t	_renderBuffer;
	array<drawSurf_t*> _surfaces;
	Sprite*			_defaultSprite;
};

extern RenderSystem* renderSys;
#endif
