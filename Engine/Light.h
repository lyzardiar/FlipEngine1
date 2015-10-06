#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "color4.h"
#include "common/vec3.h"

enum E_LIGHT_TYPE
{
	ELT_POINT,
	ELT_SPOT,
	ELT_DIRECTIONAL,
	ELT_COUNT
};

class Light
{
public:
	Light();
	~Light();

	void GetView();

private:
	vec3 _positon;
	vec3 _dir;

	vec3 _matView;
};

#endif
