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
static const int view_width = 1366;
static const int view_height = 768;
static const int win_width = 1366;
static const int win_height = 768;


static Shader* LoadPostionShader()
{
	Shader* shader = resourceSys->AddShader(position_vert, position_frag);
	shader->SetName("position");
	shader->BindAttribLocation(eAttrib_Position);
	shader->GetUniformLocation(eUniform_MVP);
	shader->GetUniformLocation(eUniform_Color);
	return shader;
}

static Shader* LoadPositionTexShader()
{
	Shader* shader = resourceSys->AddShader(positiontex_vert, positiontex_frag);
	shader->SetName("positionTex");
	shader->BindAttribLocation(eAttrib_Position);
	shader->BindAttribLocation(eAttrib_TexCoord);
	shader->GetUniformLocation(eUniform_MVP);
	shader->GetUniformLocation(eUniform_Samper0);
	return shader;
}

static Shader* LoadPhongShader()
{
	Shader* shader = resourceSys->AddShaderFromFile("../media/shader/phong.vert",
		"../media/shader/phong.frag");
	shader->SetName("phong");
	shader->BindAttribLocation(eAttrib_Position);
	shader->BindAttribLocation(eAttrib_TexCoord);
	shader->BindAttribLocation(eAttrib_Normal);

	shader->GetUniformLocation(eUniform_MVP);
	shader->GetUniformLocation(eUniform_EyePos);
	shader->GetUniformLocation(eUniform_LightPos);
	shader->GetUniformLocation(eUniform_ModelView);
	shader->GetUniformLocation(eUniform_InvModelView);
	shader->GetUniformLocation(eUniform_Samper0);
	GL_CheckError("load phong shader");
	return shader;
}

static Shader* LoadBumpShader()
{
	Shader* shader = resourceSys->AddShaderFromFile("../media/shader/bump.vert",
		"../media/shader/bump.frag");
	shader->SetName("bump");
	shader->BindAttribLocation(eAttrib_Position);
	shader->BindAttribLocation(eAttrib_TexCoord);
	shader->BindAttribLocation(eAttrib_Normal);
	shader->BindAttribLocation(eAttrib_Tangent);
	shader->BindAttribLocation(eAttrib_Binormal);

	shader->GetUniformLocation(eUniform_MVP);
	shader->GetUniformLocation(eUniform_EyePos);
	shader->GetUniformLocation(eUniform_LightPos);
	shader->GetUniformLocation(eUniform_ModelView);
	shader->GetUniformLocation(eUniform_InvModelView);
	shader->GetUniformLocation(eUniform_Samper0);
	shader->GetUniformLocation(eUniform_BumpMap);
	return shader;
}

static Shader* LoadBlurShader()
{
	Shader* shader = resourceSys->AddShaderFromFile("../media/blur.vs", "../media/blur.fs");
	shader->SetName("blur");
	shader->BindAttribLocation(eAttrib_Position);
	shader->BindAttribLocation(eAttrib_TexCoord);
	shader->GetUniformLocation(eUniform_MVP);
	shader->GetUniformLocation(eUniform_Samper0);
	return shader;
}

typedef Shader* (*LoadShaderFunc)();
struct ShaderPlugin
{
	int name;
	LoadShaderFunc func;
};

static ShaderPlugin shaderplugin[] = {
	{ eShader_Position, LoadPostionShader },
	{ eShader_PositionTex, LoadPositionTexShader },
	{ eShader_Phong, LoadPhongShader },
	{ eShader_Blur, LoadBlurShader  },
	{ eShader_Bump, LoadBumpShader },
};
static int PluginCount = sizeof(shaderplugin) / sizeof(ShaderPlugin);

RenderSystemLocal::RenderSystemLocal(glimpParms_t *glimpParms_t)
{
	GL_CreateDevice(glimpParms_t);
}

void RenderSystemLocal::Init()
{	
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);			// init value

	glClearDepth(1.0f);									
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);								
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	glViewport(0, 0, win_width, win_height);

	// 文本需要
	glEnable(GL_BLEND);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);// The initial value is GL_CCW.
	glCullFace(GL_BACK);// The initial value is GL_BACK.

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	//glActiveTexture(GL_TEXTURE1);

	resourceSys = new ResourceSystem;

	// _renderbuffer init
	//_renderBuffer.matPerspective.buildPerspectiveProjection(3.1415926535898f / 3, 800.f / 600, 0.1f, 800.f);
	_renderBuffer.matPerspective.BuildProjectionOrthoRH((float)view_width, (float)view_height, 0.1f, 800.f);
	//_renderBuffer.matView.buildLookAt(vec3(0, 0,  300.f / tanf(3.1415936f/6.f)), vec3(0, 0, 0), vec3(0, 1, 0));
	_renderBuffer.matView.m[12] = -view_width/2;
	_renderBuffer.matView.m[13] = -view_height/2;
	_renderBuffer.matView.m[14] = -((float)view_height/2.f) / tanf(3.1415936f/6.f);
	float zfar = -300.f / tanf(3.1415936f/6.f);
	Sys_Printf("\nzfar: %f\n", zfar);
	_renderBuffer.matWVP = _renderBuffer.matPerspective * _renderBuffer.matView * _renderBuffer.matWorld;

	memset(_renderBuffer.shaders, 0, 32);
	for (int i =0; i<PluginCount; i++)
	{
		_renderBuffer.shaders[shaderplugin[i].name] = shaderplugin[i].func();
	}

	// fps  init
	_defaultSprite = new Sprite;
	_defaultSprite->SetLabel("...");
	AddSprite(_defaultSprite);
	GL_CheckError("frameupdate");
}

void draw3DCoordinate()
{
	float vertices[] = {0.f, 0.f, 0.f, 
						0.f, 1.f, 0.f,
						0.f, 0.f, 1.f,
						1.f, 0.f, 0.f
	};

	unsigned short indices[] = {0, 1, 0, 2, 0, 3};
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_SHORT, indices);
}


void RenderSystemLocal::FrameUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	for (unsigned int i = 0; i < _surfaces.size(); i++)
	{
		//if (_surfaces[i]->bShaowmap)
		{
			//R_RenderPTPass(_surfaces[i], R_DrawPositonTex);
			//RenderShadowMap(_surfaces[i]);
		}
		if (_surfaces[i]->material->bumpMap != nullptr)
		{
			R_RenderBumpPass(_surfaces[i], R_DrawPositonTangent);
		}
		else if(_surfaces[i]->geo->tangentsCalculated)
		{
			R_RenderPhongPass(_surfaces[i], R_DrawPositionTexNorm);
		}
		else
			R_RenderPTPass(_surfaces[i], R_DrawPositonTex);
	}
	
	glEnableVertexAttribArray(0);
	glUseProgram(_renderBuffer.shaders[eShader_Position]->GetProgarm());

	for (int i = 0; i < _surfaces.size(); i++)
	{
		if(!_surfaces[i]->bShowBound)
			continue;

		if (_surfaces[i]->bHit)
			glUniform3f(_renderBuffer.shaders[0]->GetUniform(eUniform_Color), 1.0, 0.0, 0.0);
		else
			glUniform3f(_renderBuffer.shaders[0]->GetUniform(eUniform_Color), 0.0, 1.0, 0.0);

		mat4 t = (*_surfaces[i]->viewProj) * _surfaces[i]->matModel;
		glUniformMatrix4fv( _renderBuffer.shaders[0]->GetUniform(eUniform_MVP), 1, GL_FALSE, &t.m[0] );
		RB_DrawBounds(&_surfaces[i]->geo->aabb);
	}
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
		drawSur->material->shader = _renderBuffer.shaders[eShader_Phong];
		drawSur->material->tex = resourceSys->AddTexture(".png");
		
		if (drawSur->view == NULL)
			drawSur->view = &_renderBuffer.matWVP;

		R_GenerateGeometryVbo(drawSur->geo);
		AddDrawSur(drawSur);
	}
	return true;
}

bool RenderSystemLocal::AddDrawSur( drawSurf_t* drawSur )
{
	if (drawSur->geo->vbo[0] <= 0)
		R_GenerateGeometryVbo(drawSur->geo);

	if (drawSur->material == NULL || drawSur->material->shader == NULL)
	{
		Sys_Error("draw surface material is not");
		return false;
	}
	
	_surfaces.push_back(drawSur);
	Sys_Printf("draw surfce size %d\n", _surfaces.size());
	return true;
}

bool RenderSystemLocal::AddSprite( Sprite* sprite )
{
	drawSurf_t* drawSurf = sprite->_drawSurf;
	drawSurf->view = &_renderBuffer.matWVP;
	drawSurf->material->shader = _renderBuffer.shaders[eShader_PositionTex];
	if (drawSurf->material->tex == NULL)
		drawSurf->material->tex = resourceSys->AddTexture(".png");
	return AddDrawSur(drawSurf);
}

Shader* RenderSystemLocal::GetShader( int t )
{
	return _renderBuffer.shaders[t];
}

