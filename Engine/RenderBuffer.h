#ifndef __RENDERBUFFER_H__
#define __RENDERBUFFER_H__

#include "mat4.h"

typedef struct
{
	mat4 matWorld;
	mat4 matView;
	mat4 matPerspective;
	mat4 matWVP;

}renderBuffer_t;


#endif
