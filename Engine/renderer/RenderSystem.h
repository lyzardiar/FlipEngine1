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
class Material;


class RenderSystem
{
public:
	virtual ~RenderSystem() {}

	virtual void Init() = 0;

	virtual void FrameUpdate() = 0;

	virtual void DrawString(const char* text) = 0;

	virtual bool AddStaticModel(StaticModel* model) = 0;

	virtual bool AddDrawSur(drawSurf_t* drawSur) = 0;

	virtual bool AddSprite(Sprite* sprite) = 0;

	virtual	Shader* GetShader(int t) = 0;
};

class RenderSystemLocal : public RenderSystem
{
public:
	RenderSystemLocal(glimpParms_t *glimpParms_t);
	~RenderSystemLocal() {}

	void Init();
	void FrameUpdate();
	void DrawString(const char* text);

	void RenderShadowMap(drawSurf_t* drawSur);

	virtual bool AddStaticModel(StaticModel* model);

	virtual bool AddDrawSur(drawSurf_t* drawSur);

	virtual bool AddSprite(Sprite* sprite);

	Shader* GetShader(int t);

private:
	
	void RenderCommon();

	void RenderPasses();

	void RenderBounds();

private:
	renderBuffer_t	_renderBuffer;
	array<drawSurf_t*> _surfaces;
	Sprite*	_defaultSprite;
	shadowMap_t* _shadowMap;
};

extern RenderSystem* renderSys;
#endif
