#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "r_public.h"
#include "ResourceSystem.h"

class Box
{
public:
	Box(float sx = 1.f, float sy = 1.f, float sz = 1.f)
	{
		_drawSurf = R_AllocDrawSurf();
		_drawSurf->shaderParms = R_AllocMaterail();
		_drawSurf->geo = R_AllocStaticTriSurf();
		R_GenerateBox( _drawSurf->geo, sx, sy, sz);

		_drawSurf->shaderParms->tex = resourceSys->AddTexture("0.png");
		R_GenerateGeometryVbo(_drawSurf->geo);
		_drawSurf->mtr = resourceSys->AddMaterial("../media/mtr/positiontex.mtr");
	}

	~Box()
	{
		delete _drawSurf;
	}

	void SetPosition(float x, float y, float z)
	{
		_position.set(x, y, z);
		_drawSurf->matModel.buildTranslate(x, y, z);
	}

	vec3 GetPosition()
	{
		return _position;
	}

	void SetViewProj( mat4* viewProj )
	{
		_drawSurf->viewProj = viewProj;
	}

	drawSurf_t* _drawSurf;
private:

	vec3 _position;
};




#endif