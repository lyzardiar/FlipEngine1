#include "Model.h"




StaticModel::StaticModel() 
{

}

StaticModel::~StaticModel()
{

}

drawSurf_t* StaticModel::AllocStaticSurface()
{
	drawSurf_t* surface = R_AllocDrawSurf();
	_surfaces.push_back(surface);
	return surface;
}

array<drawSurf_t*> StaticModel::getSurfaces()
{
	return _surfaces;
}
