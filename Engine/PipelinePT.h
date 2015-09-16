#pragma once
#include "Pipeline.h"
#include "glutils.h"
class PipelinePT:public Pipeline
{
public:
	PipelinePT();
	~PipelinePT();

	void init();
	void drawScene();

	GLuint mProgram;
	GLuint mWorldLocation;
	GLuint mSampler;
};

