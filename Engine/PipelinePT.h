#pragma once
#include "Pipeline.h"
#include "glutils.h"
class PipelinePT:public Pipeline
{
public:
	PipelinePT(renderBuffer_t* rb);
	~PipelinePT();

	void Init();
	void DrawScene();
	void DrawMesh(array<Mesh*>* meshs);

	GLuint mProgram;
	GLuint mWorldLocation;
	GLuint mSampler;
};

