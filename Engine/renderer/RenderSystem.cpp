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
	//_renderBuffer.matPerspective.buildPerspectiveProjection(3.1415926535898f / 3, 800.f / 600, 0.1f, 800.f);
	_renderBuffer.matPerspective.BuildProjectionOrthoRH(800.f, 600.f, 0.1f, 800.f);
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
	AddSprite(_defaultSprite);
	GL_CheckError("frameupdate");
}

void RenderSystemLocal::FrameUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	for (unsigned int i = 0; i < _surfaces.size(); i++)
	{
		if (_surfaces[i]->bShaowmap)
		{
			//RenderShadowMap(_surfaces[i]);
		}
		else
			R_RenderPTPass(_surfaces[i], R_DrawPositonTex);
	}
	GL_CheckError("frameupdate");
	GL_SwapBuffers();
}

void RenderSystemLocal::RenderShadowMap(drawSurf_t* drawSur)
{
	glViewport(0, 0, 1024, 1024);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glBindFramebuffer(GL_FRAMEBUFFER, _shadowMap->fbo);
	glClear(GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_FRONT);
	R_RenderShadowMap(drawSur, R_DrawPositon);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderSystemLocal::DrawString( const char* text )
{
	_defaultSprite->SetLabel(text);
}

bool RenderSystemLocal::AddStaticModel( StaticModel* model )
{
	array<drawSurf_t*> surfaces = model->getSurfaces();
	for (unsigned int i = 0; i < surfaces.size(); i++)
	{
		drawSurf_t* drawSur = surfaces[i];
		drawSur->material = R_AllocMaterail();
		drawSur->material->shader = _renderBuffer.shaders[1];
		drawSur->material->tex = resourceSys->AddTexture(".png");
		
		if (drawSur->view == NULL)
			drawSur->view = &_renderBuffer.matWVP;

		R_GenerateGeometryVbo(drawSur->geo);
		_surfaces.push_back(drawSur);
	}
	return true;
}

bool RenderSystemLocal::AddDrawSur( drawSurf_t* drawSur )
{
	if (drawSur->geo->vbo[0] <= 0)
		R_GenerateGeometryVbo(drawSur->geo);

	if (drawSur->material == NULL)
		drawSur->material = R_AllocMaterail();
	
	if (drawSur->material->shader == NULL)
		drawSur->material->shader = _renderBuffer.shaders[eShader_PositionTex];
	
	if (drawSur->material->tex == NULL)
		drawSur->material->tex = resourceSys->AddTexture(".png");
	
	_surfaces.push_back(drawSur);
	return true;
}

bool RenderSystemLocal::AddSprite( Sprite* sprite )
{
	drawSurf_t* drawSurf = sprite->_drawSurf;
	drawSurf->view = &_renderBuffer.matWVP;
	drawSurf->material->shader = _renderBuffer.shaders[eShader_PositionTex];
	_surfaces.push_back(drawSurf);
	return true;
}

