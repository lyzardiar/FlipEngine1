#include "PipelinePT.h"
#include "Mesh.h"
#include "Texture.h"

PipelinePT::PipelinePT(renderBuffer_t* rb) : Pipeline(rb)
{
	_shader = _render->shaders[1];
}


PipelinePT::~PipelinePT()
{
}

void PipelinePT::Init()
{
}

void PipelinePT::DrawScene()
{


}

void PipelinePT::DrawMesh(array<Mesh*>* meshs)
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glUseProgram(_shader->GetProgarm());
	mat4* mat = &_render->matWVP;
    glUniformMatrix4fv(_shader->GetUniform(eUniform_MVP), 1, GL_FALSE, &mat->m[0]);

	//glBindTexture(GL_TEXTURE_2D, gTexId);

	if(meshs != NULL)
	{
		for (int i=0; i < meshs->size(); ++i)
		{
			Mesh* mesh = (*meshs)[i];

			glBindTexture( GL_TEXTURE_2D, mesh->GetTexture()->GetName() );
			DrawMeshPT(mesh);	
		}
	}
	//glUniform3f(gPipelineP.mVertexColor, 1.0, 1.0, 1.0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	GL_CheckError("pipelinepT");
}

