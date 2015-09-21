#ifndef __RENDERBUFFER_H__
#define __RENDERBUFFER_H__

#include "mat4.h"
class Shader;

typedef struct
{
	mat4 matWorld;
	mat4 matView;
	mat4 matPerspective;
	mat4 matWVP;

	Shader* shaders[32];
}renderBuffer_t;


#endif
