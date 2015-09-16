#include "PipelinePosTexLig.h"

static const char gVertexShader[] =
	"attribute vec3 vPosition;\n"
	"attribute vec2 vTexCoord;\n"
	"uniform mat4 WVP;\n"
	"varying vec2 v_texCoord;\n"
	"void main() {\n"
	"  gl_Position = WVP*vec4(vPosition, 1.0);\n"
	"  v_texCoord = vTexCoord;\n"
"}\n";

static const char gFragmentShader[] =
	"struct DirectionalLight\n"
	"{\n"
	"vec3 Color;\n"
	"float AmbientIntensity;\n"
	"};\n"
	"uniform DirectionalLight gDirectionalLight;\n"
	"precision mediump float;\n"
	"uniform sampler2D texture1;"
	"varying vec2 v_texCoord;\n"
	"void main() {\n"
	"   gl_FragColor = texture2D(texture1, v_texCoord)*vec4(gDirectionalLight.Color, 1.0f) *\n"
    "   gDirectionalLight.AmbientIntensity;\n"
	//"   gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
	"}\n";

PipelinePosTexLig::PipelinePosTexLig()
{
}


PipelinePosTexLig::~PipelinePosTexLig()
{
}

void PipelinePosTexLig::init()
{
	mProgram = GL_CreateProgram(gVertexShader, gFragmentShader);
	mWorldLocation = glGetUniformLocation(mProgram, "WVP");
	mSampler	= glGetUniformLocation(mProgram, "texture1");
	mDirLightColor = glGetUniformLocation(mProgram, "gDirectionalLight.Color");
	mDirLightAmbientIntensity = glGetUniformLocation(mProgram, "gDirectionalLight.AmbientIntensity");

	glActiveTexture(GL_TEXTURE0);
	glBindAttribLocation(mProgram, 0, "vPosition");
	glBindAttribLocation(mProgram, 1, "vTexCoord");
}

void PipelinePosTexLig::drawScene()
{

}