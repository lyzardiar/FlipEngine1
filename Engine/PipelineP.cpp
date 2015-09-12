#include "PipelineP.h"
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
	

PipelineP::PipelineP() :mProgram(NULL), mVertexColor(NULL)
{
}


PipelineP::~PipelineP()
{
}

void PipelineP::init()
{
	mProgram = uglCreateProgram(gVertexShader, gFragmentShader);
	
	mWorldLocation = glGetUniformLocation(mProgram, "WVP");
	mVertexColor   = glGetUniformLocation(mProgram, "COLOR");
	glBindAttribLocation(mProgram, 0, "vPosition");
}

void PipelineP::drawScene()
{

}