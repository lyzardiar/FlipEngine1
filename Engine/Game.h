#ifndef __GAME_H__
#define __GAME_H__
#include "sys/sys_public.h"

class Camera;
class Sprite;
class StaticModel;

class RenderSystem;
class ScriptSystem;

class Game
{
public:
	virtual		~Game() {};
	virtual void  Init() = 0;
	virtual void  Frame() = 0;

	void SetRenderSys(RenderSystem* renderSys)
	{
		_renderSys = renderSys;
	}

protected:
	RenderSystem* _renderSys;
	ScriptSystem* _scriptSys;

};


extern Game* game;
#endif


