#pragma once
#include "Pipeline.h"
#include "glUtils.h"

class PipelinePosTexLig :
	public Pipeline
{
public:
	PipelinePosTexLig();
	~PipelinePosTexLig();
	void init();
	void drawScene();

	GLuint mProgram;
	GLuint mWorldLocation;
	GLuint mSampler;
	GLuint mDirLightColor;
	GLuint mDirLightAmbientIntensity;
};

