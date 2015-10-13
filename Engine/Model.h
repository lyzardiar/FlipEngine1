#ifndef __MODEL_H__
#define __MODEL_H__
#include "r_public.h"
#include "common/array.h"
#include "common/str.h"

class StaticModel
{
public:
	StaticModel();
	~StaticModel();

	drawSurf_t* AllocStaticSurface();

	array<drawSurf_t*> getSurfaces();

	void GenerateNormals();

	void CalcBounds();

	bool ConvertLWOToModelSurfaces( const struct st_lwObject *lwo );

private:

	array<drawSurf_t*> _surfaces;

	lfStr name;
};
#endif


