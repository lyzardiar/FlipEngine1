#include "PipelinePTO.h"
#include "Mesh.h"
#include "Texture.h"

PipelinePTO::PipelinePTO( renderBuffer_t* rb ) : Pipeline(rb)
{
	_positionShader = _render->shaders[0];
	_positionTexShader = _render->shaders[1];
}

PipelinePTO::~PipelinePTO()
{
}

void PipelinePTO::Init()
{
}

void PipelinePTO::DrawScene()
{
}

void PipelinePTO::DrawMesh( array<Mesh*>* meshs )
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	mat4* mat = &_render->matWVP;

	if(meshs != NULL)
	{
		for (unsigned int i=0; i < meshs->size(); ++i)
		{
			Mesh* mesh = (*meshs)[i];

			mat4 t;
			t.buildTranslate(mesh->getPosition());
			t = (_render->matWVP * t);


			glEnable(GL_STENCIL_TEST);
			// Render the mesh into the stencil buffer.
			glPolygonMode(GL_FRONT, GL_FILL);
			glStencilFunc(GL_ALWAYS, 1, -1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

			glUseProgram(_positionTexShader->GetProgarm());
			glUniformMatrix4fv(_positionTexShader->GetUniform(eUniform_MVP), 1, GL_FALSE, &t.m[0]);
			glUniform1i(_positionTexShader->GetUniform(eUniform_Samper0), 0);
			glBindTexture( GL_TEXTURE_2D, mesh->GetTexture()->GetName() );
			DrawMeshPT(mesh);

			// Render the thick wireframe version.
			glStencilFunc(GL_NOTEQUAL, 1, -1);
			// glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glLineWidth(3);
			glPolygonMode(GL_FRONT, GL_LINE);
			glUseProgram(_positionShader->GetProgarm());
			glUniformMatrix4fv(_positionShader->GetUniform(eUniform_MVP), 1, GL_FALSE, &t.m[0]);
			glUniform3f(_positionShader->GetUniform(eUniform_Color), 0.0, 1.0, 1.0);
			DrawMeshP(mesh);
		}
	}
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	GL_CheckError("pipelinepT");
}
