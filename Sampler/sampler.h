#ifndef __SAMPLER_H__
#define __SAMPLER_H__

#include "Game.h"
#include "common/vec3.h"

class ShadowSampler : public Game
{
public:
	ShadowSampler( void );
	~ShadowSampler();

	void Init();

	void Frame();

	void ProcessEvent(sysEvent_s* event);

	void SetupCamera();

	void AddStaticModel(Mesh* model);

	void CreateLight();

	void CastShadowMap();

	bool HitTest(int x, int y);

private:
	Camera* _camera;

	Sprite* _defaultSprite;

	Sprite* _spLight;

	Camera* _light;



};

#endif