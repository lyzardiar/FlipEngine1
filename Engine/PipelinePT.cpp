#include "PipelinePT.h"
#include "Mesh.h"
#include "Texture.h"

	static const char gVertexShader[] =
		"attribute vec2 vTexCoord;\n"
		"attribute vec4 vPosition;\n"
		"uniform mat4 WVP;\n"
		"varying vec2 v_texCoord;\n"
		"void main() {\n"
		"  gl_Position = WVP*vPosition;\n"
		"  v_texCoord = vTexCoord;\n"
    "}\n";

	static const char gFragmentShader[] =
		"precision mediump float;\n"
		"uniform sampler2D texture1;"
		"varying vec2 v_texCoord;\n"
		"void main() {\n"
		"   gl_FragColor = texture2D(texture1, v_texCoord);\n"
		//"   gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
		"}\n";
	
PipelinePT::PipelinePT(renderBuffer_t* rb) : mProgram(0)
{
	_render = rb;	
	Init();
}


PipelinePT::~PipelinePT()
{
}

void PipelinePT::Init()
{
	mProgram = GL_CreateProgram(gVertexShader, gFragmentShader);
	
	mWorldLocation = glGetUniformLocation(mProgram, "WVP");
	mSampler	= glGetUniformLocation(mProgram, "texture1");

	glActiveTexture(GL_TEXTURE0);
	glBindAttribLocation(mProgram, 0, "vPosition");
	glBindAttribLocation(mProgram, 1, "vTexCoord");
}

void PipelinePT::DrawScene()
{


}

void PipelinePT::DrawMesh(array<Mesh*>* meshs)
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glUseProgram(mProgram);
	mat4* mat = &_render->matWVP;
    glUniformMatrix4fv(mWorldLocation, 1, GL_FALSE, &mat->m[0]);

	//glBindTexture(GL_TEXTURE_2D, gTexId);

	if(meshs != NULL)
	{
		for (int i=0; i < meshs->size(); ++i)
		{
			Mesh* mesh = (*meshs)[i];

			if ( mesh->GetTexture() )
				glBindTexture(GL_TEXTURE_2D, mesh->GetTexture()->GetName() );

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
