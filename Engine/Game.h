#ifndef __GAME_H__
#define __GAME_H__
#include "sys/sys_public.h"

class Camera;
class Sprite;


class Game
{
public:
	virtual		~Game() {};
	virtual void  Init() = 0;
	virtual void  Frame() = 0;
};


class GameLocal : public Game
{
public:
	GameLocal( void );
	~GameLocal();

	void Init();

	void Frame();

	void ProcessEvent(sysEvent_s* event);

	void SetupCamera();

	void LoadAllModel();
private:
	Camera* _camera;

	Sprite* _defaultSprite;
};

extern Game* game;
#endif


