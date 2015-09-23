#include "PipelineP.h"
#include "Mesh.h"

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


PipelineP::PipelineP(renderBuffer_t* rb) : Pipeline(rb)
{
	_shader = rb->shaders[0];
}


PipelineP::~PipelineP()
{
}


void PipelineP::DrawScene()
{

}

void PipelineP::DrawMesh(array<Mesh*>* meshs)
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glUseProgram(_shader->GetProgarm());
	glUniform3f(_shader->GetUniform(eUniform_Color), 0.0, 1.0, 0.0);
	
	mat4 t;
	t.buildTranslate(vec3(-10, 0, 0));

	t = (_render->matWVP * t);
    glUniformMatrix4fv(_shader->GetUniform(eUniform_MVP), 1, GL_FALSE, &t.m[0]);

	if(meshs != NULL)
	{
		for (int i=0; i < meshs->size(); ++i)
		{
			Mesh* mesh = (*meshs)[i];
			DrawMeshP(mesh);
		}
	}

	GL_CheckError("pipelinep");
}