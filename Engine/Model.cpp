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

void StaticModel::GenerateNormals()
{
	for (int i=0; i<_surfaces.size(); ++i)
	{
		R_DeriveNormals(_surfaces[i]->geo);
	}
}

void StaticModel::CalcBounds()
{
	for (int i=0; i<_surfaces.size(); ++i)
	{
		R_BoundTriSurf(_surfaces[i]->geo);
		_surfaces[i]->bShowBound = true;
	}
}
