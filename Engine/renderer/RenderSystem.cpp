#include "RenderSystem.h"
#include "../glutils.h"
#include "Material.h"
#include "../r_public.h"
#include "../framework/Common.h"
#include "../ResourceSystem.h"
#include <stdarg.h>

#include "../ShaderSource.h"
#include "../Shader.h"
#include "../sys/sys_public.h"

#include "../Sprite.h"
#include "../common/Timer.h"
#include "draw_common.h"
#include "../Model.h"

RenderSystem* renderSys;

RenderSystemLocal::RenderSystemLocal(glimpParms_t *glimpParms_t)
{
	GL_CreateDevice(glimpParms_t);
}

void RenderSystemLocal::Init()
{
	// 文本需要
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	resourceSys = new ResourceSystem;

	// _renderbuffer init
	_renderBuffer.matPerspective.buildPerspectiveProjection(3.1415926535898f / 3, 800.f / 600, 0.1f, 800.f);
	//_renderBuffer.matView.buildLookAt(vec3(0, 0,  300.f / tanf(3.1415936f/6.f)), vec3(0, 0, 0), vec3(0, 1, 0));
	_renderBuffer.matView.m[12] = -400;
	_renderBuffer.matView.m[13] = -300;
	_renderBuffer.matView.m[14] = -300.f / tanf(3.1415936f/6.f);
	float zfar = -300.f / tanf(3.1415936f/6.f);
	Sys_Printf("\nzfar: %f\n", zfar);
	_renderBuffer.matWVP = _renderBuffer.matPerspective * _renderBuffer.matView * _renderBuffer.matWorld;

	// shader init
	Shader* shader1 = resourceSys->AddShader(position_vert, position_frag);
	shader1->BindAttribLocation(eAttrib_Position);
	shader1->GetUniformLocation(eUniform_MVP);
	shader1->GetUniformLocation(eUniform_Color);

	Shader* shader2 = resourceSys->AddShader(positiontex_vert, positiontex_frag);
	shader2->BindAttribLocation(eAttrib_Position);
	shader2->BindAttribLocation(eAttrib_TexCoord);
	shader2->GetUniformLocation(eUniform_MVP);
	shader2->GetUniformLocation(eUniform_Samper0);

	_renderBuffer.shaders[eShader_Position]		= shader1;
	_renderBuffer.shaders[eShader_PositionTex]	= shader2;

	// fps  init
	_defaultSprite = new Sprite;
	_defaultSprite->SetLabel("...");
	_defaultSprite->SetPosition(0.f, 0.f, 0.f);
///	pipe1->AddMesh(_defaultSprite);

	GL_CheckError("frameupdate");
}

void RenderSystemLocal::FrameUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	drawSurf_t* drawSurf = _defaultSprite->_drawSurf;
	drawSurf->material->shader = _renderBuffer.shaders[eShader_PositionTex];
	R_RenderPTPass(drawSurf, &_renderBuffer.matWVP, R_DrawPositonTex);

	for (int i = 0; i < _surfaces.size(); i++)
	{
		mat4 t;
		t.buildTranslate(vec3(400.f, 300.f, 515.f));
		t = _renderBuffer.matWVP * t;
		R_RenderPTPass(_surfaces[i], &t, R_DrawPositonTex);
	}
	GL_CheckError("frameupdate");
	GL_SwapBuffers();
}

void RenderSystemLocal::DrawString( const char* text )
{
	_defaultSprite->SetLabel(text);
}

bool RenderSystemLocal::AddStaticModel( StaticModel* model )
{
	array<modelSurface_t*> surfaces = model->getSurfaces();
	for (int i = 0; i < surfaces.size(); i++)
	{
		drawSurf_t* drawSur = new drawSurf_t;
		memset(drawSur, 0, sizeof(drawSurf_t));
		drawSur->geo = surfaces[i]->geometry;
		drawSur->material = R_AllocMaterail();
		drawSur->material->shader = _renderBuffer.shaders[1];
		drawSur->material->tex = resourceSys->AddTexture(".png");
		R_GenerateGeometryVbo(drawSur->geo);
		_surfaces.push_back(drawSur);
	}
	return true;
}

bool RenderSystemLocal::AddDrawSur( drawSurf_t* drawSur )
{
	drawSur->material = R_AllocMaterail();
	drawSur->material->shader = _renderBuffer.shaders[1];
	drawSur->material->tex = resourceSys->AddTexture(".png");
	_surfaces.push_back(drawSur);
	return true;
}
