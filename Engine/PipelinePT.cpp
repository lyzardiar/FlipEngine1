#include "PipelinePT.h"
#include "Mesh.h"
#include "Texture.h"

PipelinePT::PipelinePT(renderBuffer_t* rb)
{
	_render = rb;	
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
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, mesh->GetPositions());
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, mesh->GetTexCoords());
			glDrawElements(GL_TRIANGLES, 904*3, GL_UNSIGNED_SHORT, mesh->_indices.pointer());
		}
	}
	//glUniform3f(gPipelineP.mVertexColor, 1.0, 1.0, 1.0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	GL_CheckError("pipelinepT");
}

//void drawMesh(Mesh* mesh)
//{
//	glClearStencil(0);   //outline
//    glClear(GL_STENCIL_BUFFER_BIT);
//
//	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//	mat4 mat = perspectiveMatrix* viewMatrix * worldMatrix;
//
//	glEnableVertexAttribArray(0);
//	glEnableVertexAttribArray(1);
//	glEnable(GL_STENCIL_TEST);
//
//    // Render the mesh into the stencil buffer.
//	glPolygonMode(GL_FRONT, GL_FILL);
//	// (ref & mask ) compare ( stencil & mask)
//    glStencilFunc(GL_ALWAYS, 1, -1);
//
//    // 深度测试不通过 保持原来值 0 不通过gl_replace （即使用模板值1）
//	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//	
//	glUseProgram(gPipelinePT.mProgram);
//    glUniformMatrix4fv(gPipelinePT.mWorldLocation, 1, GL_FALSE, &mat.m[0]);
//	glUniform1f(gPipelinePT.mSampler, 0);
//	glBindTexture(GL_TEXTURE_2D, gTexId);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, gMesh->getPositions());
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, gMesh->getTexCoords());
//	glDrawElements(GL_TRIANGLES, 904*3, GL_UNSIGNED_SHORT, gMesh->_indices.pointer());
//    
//	// Render the thick wireframe version.
//	glStencilFunc(GL_NOTEQUAL, 1, -1);
//   // glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//
//    glLineWidth(3);
//    glPolygonMode(GL_FRONT, GL_LINE);
//	glUseProgram(gPipelineP.mProgram);
//	//glUniform3f(gPipelineP.mVertexColor, 1.0, 1.0, 1.0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, gMesh->_positions.pointer());
//	glDrawElements(GL_TRIANGLES, 904*3, GL_UNSIGNED_SHORT, gMesh->_indices.pointer());
//}
