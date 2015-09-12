#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

class FPSCounter
{
public:
	FPSCounter()	:	fps(0.0f), lastTime(0.0f), frames(0), time(0.0f)
	{}
	~FPSCounter()	{}

	void update(void);
	float getFps(void)	{	return fps;	}

protected:
	float fps;

	float lastTime;
	int frames;
	float time;
};

#endif	//FPS_COUNTER_H