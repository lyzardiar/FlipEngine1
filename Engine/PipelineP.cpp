#include "PipelineP.h"
#include "Mesh.h"

static const char gVertexShader[] =
"attribute vec4 vPosition;\n"
"uniform mat4 WVP;\n"
"void main() {\n"
"  gl_Position = WVP*vPosition;\n"
"}\n";

static const char gFragmentShader[] =
	"precision mediump float;\n"
	"uniform vec3 COLOR;\n"
	"void main() {\n"
	"   gl_FragColor = vec4(COLOR, 1.0);\n"
	"}\n";
	

static void draw3DCoordinate()
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


PipelineP::PipelineP(renderBuffer_t* rb) : mProgram(0), mVertexColor(0)
{
	_render = rb;	
	Init();
}


PipelineP::~PipelineP()
{
}

void PipelineP::Init()
{
	mProgram = GL_CreateProgram(gVertexShader, gFragmentShader);
	
	mWorldLocation = glGetUniformLocation(mProgram, "WVP");
	mVertexColor   = glGetUniformLocation(mProgram, "COLOR");
	glBindAttribLocation(mProgram, 0, "vPosition");
}

void PipelineP::DrawScene()
{

}

void PipelineP::DrawMesh(array<Mesh*>* meshs)
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glUseProgram(mProgram);
	glUniform3f(mVertexColor, 0.0, 1.0, 0.0);
	
	
	mat4 t;
	t.buildTranslate(vec3(-10, 0, 0));

	t = (_render->matWVP * t);
    glUniformMatrix4fv(mWorldLocation, 1, GL_FALSE, &t.m[0]);

	draw3DCoordinate();

	if(meshs != NULL)
	{
		for (int i=0; i < meshs->size(); ++i)
		{
			Mesh* mesh = (*meshs)[i];
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, mesh->_positions.pointer());
			glDrawElements(GL_TRIANGLES, 904*3, GL_UNSIGNED_SHORT, mesh->_indices.pointer());
		}
	}
	//glUniform3f(gPipelineP.mVertexColor, 1.0, 1.0, 1.0);

	GL_CheckError("pipelinep");
}