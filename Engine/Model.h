#ifndef __MODEL_H__
#define __MODEL_H__
#include "common/array.h"
#include "common/hashtable.h"
#include "r_public.h"

class Joint;

class Model {
	friend class RenderSystemLocal;
public:
	Model();
	virtual ~Model();

	virtual void Init();

	virtual void SetFile(const char* filename);

	void SetPosition(float x, float y, float z);

	vec3 GetPosition();

	void SetViewProj(mat4* viewProj);
protected:
	drawSurf_t* _drawSurf;
	vec3 _position;
};

struct AniAction
{
	int startframe;
	int endframe;
	lfStr name;
};

class AniModel {
public:
	AniModel();
	virtual ~AniModel();

	void Init();

	void SetFile(const char* filename);	

	void Update(float elsepedtime);

	void UpdateJointPoses(Joint* joint);

	void SetPosition(float x, float y, float z);

	vec3 GetPosition();

	void SetViewProj(mat4* viewProj);

	void PushAnimation(const char* name, int start, int end);

	void Play(const char* name, bool isLoop);
public:
	Joint* _root;
	drawSurf_t* _drawSurf;
	vec3 _position;

	float  _startFrame;
	float  _endFrame;
	float  _currentFrame;
	float  _totalFrame;

	bool _isLoop;

	hashtable _animations;
};

#endif


