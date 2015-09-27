#ifndef __MODEL_H__
#define __MODEL_H__
#include "r_public.h"
#include "common/array.h"

class StaticModel
{
public:
	StaticModel();
	~StaticModel();

	modelSurface_t* AllocStaticSurface();

	array<modelSurface_t*> getSurfaces();
private:

	array<modelSurface_t*> _surfaces;

};
#endif


