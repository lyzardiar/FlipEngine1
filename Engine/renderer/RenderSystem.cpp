#include "RenderSystem.h"
#include "../glutils.h"
#include "Material.h"
#include "../RenderBuffer.h"
#include "../PipelineP.h"
#include "../PipelinePT.h"
#include "../PipelinePTO.h"
#include "../framework/Common.h"
#include "../ResourceSystem.h"
#include <stdarg.h>

#include "../ShaderSource.h"
#include "../Shader.h"
#include "../sys/sys_public.h"

#include "../Sprite.h"
#include "../common/Timer.h"

RenderSystem* renderSys = NULL;


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
	_renderBuffer.matPerspective.buildPerspectiveProjection(3.1415926535898 / 3, 800.f / 600, 0.1, 800);
	//_renderBuffer.matView.buildLookAt(vec3(0, 0,  300.f / tanf(3.1415936f/6.f)), vec3(0, 0, 0), vec3(0, 1, 0));
	_renderBuffer.matView.m[12] = -400;
	_renderBuffer.matView.m[13] = -300;
	_renderBuffer.matView.m[14] = -300.f / tanf(3.1415936f/6.f);
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

	Pipeline* pipe = new PipelineP(&_renderBuffer);
	_pipelines.push_back(pipe);

	Pipeline* pipe1 = new PipelinePT(&_renderBuffer);
	_pipelines.push_back(pipe1);

	Pipeline* pipe2 = new PipelinePTO(&_renderBuffer);
	_pipelines.push_back(pipe2);

	// fps  init
	_defaultSprite = new Sprite;
	_defaultSprite->SetLabel(" ");
	_defaultSprite->SetPosition(0.f, 0.f, 0.f);
	pipe1->AddMesh(_defaultSprite);
	
	
}

void RenderSystemLocal::FrameUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	for (int i = 0; i < _pipelines.size(); i++)
	{
		//_pipelines[i]->DrawMesh(&_meshs);
		_pipelines[i]->DrawMesh();
	}
	
	GL_SwapBuffers();
}

void RenderSystemLocal::DrawString( const char* text )
{
	_defaultSprite->SetLabel(text);
}

Pipeline* RenderSystemLocal::GetPipeline( int idx )
{
	//assert(idx >= 0 && idx <_pipelines.size() );
	return _pipelines[idx];
}
