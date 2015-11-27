#ifndef __RENDERSYSTEM_H__
#define	__RENDERSYSTEM_H__ 
#include "../glutils.h"
#include "../common/mat4.h"
#include "../common/array.h"
#include "../r_public.h"

class Pipeline;
class Model;
class Sprite;
class Material;
class Camera;


class RenderSystem
{
public:
	virtual ~RenderSystem() {}

	virtual void Init() = 0;

	virtual void FrameUpdate() = 0;

	virtual void DrawString(const char* text) = 0;

	virtual bool AddDrawSur(drawSurf_t* drawSur) = 0;

	virtual bool AddSprite(Sprite* sprite) = 0;

	virtual bool AddModel(Model* model) = 0;

	virtual bool AddUISurf(drawSurf_t* drawSurf) = 0;

	virtual int GetNumSurf() = 0;
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

	virtual bool AddDrawSur(drawSurf_t* drawSur);

	virtual bool AddSprite(Sprite* sprite);

	virtual bool AddModel(Model* model);

	virtual bool AddUISurf(drawSurf_t* drawSurf);

	virtual int GetNumSurf(){ return _surfaces.size(); }
private:
	
	void RenderCommon();

	void RenderPasses();

	void RenderBounds();

private:
	Camera* _camera;
	array<drawSurf_t*> _surfaces;
	Sprite*	_defaultSprite;
	shadowMap_t* _shadowMap;

	int _winWidth;
	int _winHeight;
};

#endif
