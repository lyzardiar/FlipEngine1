#pragma once
#include "Pipeline.h"
#include "glUtils.h"
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

