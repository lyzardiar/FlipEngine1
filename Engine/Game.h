#ifndef __GAME_H__
#define __GAME_H__

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
private:

};

extern Game* game;
#endif


