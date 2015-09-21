#include "RenderSystem.h"
#include "../glutils.h"
#include "Material.h"
#include "../RenderBuffer.h"
#include "../PipelineP.h"
#include "../PipelinePT.h"
#include "../framework/Common.h"
#include "../ResourceSystem.h"
#include <stdarg.h>

#include "../ShaderSource.h"
#include "../Shader.h"


void GL_LOG(const char* fmt, ...)
{
	va_list argptr;
	va_start( argptr, fmt );
	Common_Printf( fmt, argptr );
	va_end( argptr );
}

static void Test_2DDraw()
{
	RB_SetGL2D();

	glColor3f (1.0, 0.0, 0.0);
    glPointSize(5);
	glBegin(GL_POINTS);
       for (int i = 0; i < 7; i++)
          glVertex2f (i*100, 100);
	glEnd();
}

RenderSystemLocal::RenderSystemLocal(glimpParms_t *glimpParms_t)
{
	GL_CreateDevice(glimpParms_t);
}

void RenderSystemLocal::Init()
{
	// _renderbuffer init
	_renderBuffer.matPerspective.buildPerspectiveProjection(3.1415926535898 / 3, 1440.0 / 900, 0.1, 800);
	_renderBuffer.matView.m[14] = -30;
	_renderBuffer.matWVP = _renderBuffer.matPerspective * _renderBuffer.matView * _renderBuffer.matWorld;

	// resourceSystem
	_resourceSys = new ResourceSystem;
	
	// shader init
	Shader* shader1 = _resourceSys->AddShader(position_vert, position_frag);
	shader1->BindAttribLocation(eAttrib_Position);
	shader1->GetUniformLocation(eUniform_MVP);
	shader1->GetUniformLocation(eUniform_Color);

	Shader* shader2 = _resourceSys->AddShader(positiontex_vert, positiontex_frag);
	shader2->BindAttribLocation(eAttrib_Position);
	shader2->BindAttribLocation(eAttrib_TexCoord);
	shader2->GetUniformLocation(eUniform_MVP);
	shader2->GetUniformLocation(eUniform_Samper0);

	_renderBuffer.shaders[0] = shader1;
	_renderBuffer.shaders[1] = shader2;

	Pipeline* pipe = new PipelineP(&_renderBuffer);
	_pipelines.push_back(pipe);

	Pipeline* pipe1 = new PipelinePT(&_renderBuffer);
	_pipelines.push_back(pipe1);

	Mesh* mesh = _resourceSys->AddMesh("ninja.b3d");
	_meshs.push_back(mesh);

	GL_CheckError("oo");
}

void RenderSystemLocal::FrameUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for (int i = 0; i < _pipelines.size(); i++)
	{
		_pipelines[i]->DrawMesh(&_meshs);
	}
	
	Test_2DDraw();

	GL_SwapBuffers();
}