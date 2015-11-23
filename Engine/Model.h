#ifndef __MODEL_H__
#define __MODEL_H__
#include "r_public.h"

class Model
{
public:
	Model();
	~Model();

	void SetFile(const char* filename);

	void SetPosition(float x, float y, float z);

	vec3 GetPosition();
private:
	drawSurf_t* _drawSurf;
	vec3 _position;
};

#endif


