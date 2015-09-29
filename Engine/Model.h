#ifndef __MODEL_H__
#define __MODEL_H__
#include "r_public.h"
#include "common/array.h"

class StaticModel
{
public:
	StaticModel();
	~StaticModel();

	drawSurf_t* AllocStaticSurface();

	array<drawSurf_t*> getSurfaces();
private:

	array<drawSurf_t*> _surfaces;

};
#endif


