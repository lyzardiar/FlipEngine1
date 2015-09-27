#include "Model.h"




StaticModel::StaticModel() 
{

}

StaticModel::~StaticModel()
{

}

modelSurface_t* StaticModel::AllocStaticSurface()
{
	modelSurface_t* surface = new modelSurface_t;
	memset(surface, 0, sizeof(modelSurface_t));
	_surfaces.push_back(surface);
	return surface;
}

array<modelSurface_t*> StaticModel::getSurfaces()
{
	return _surfaces;
}
