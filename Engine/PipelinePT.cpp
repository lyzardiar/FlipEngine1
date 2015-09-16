#include "PipelinePT.h"

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
	
PipelinePT::PipelinePT()
{
}


PipelinePT::~PipelinePT()
{
}

void PipelinePT::init()
{
	mProgram = GL_CreateProgram(gVertexShader, gFragmentShader);
	
	mWorldLocation = glGetUniformLocation(mProgram, "WVP");
	mSampler	= glGetUniformLocation(mProgram, "texture1");

	glActiveTexture(GL_TEXTURE0);
	glBindAttribLocation(mProgram, 0, "vPosition");
	glBindAttribLocation(mProgram, 1, "vTexCoord");
}

void PipelinePT::drawScene()
{

}
