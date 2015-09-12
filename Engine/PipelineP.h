#pragma once
#include "glUtils.h"
#include "Pipeline.h"
class PipelineP:public Pipeline
{
public:
	PipelineP();
	~PipelineP();

	void init();
	void drawScene();

	GLuint mProgram;
	GLuint mWorldLocation;
	GLuint mVertexColor;

};

