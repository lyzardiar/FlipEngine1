#ifndef __PIPELINEP_H__
#define __PIPELINEP_H__
#include "glutils.h"
#include "Pipeline.h"

class PipelineP:public Pipeline
{
public:
	PipelineP(renderBuffer_t* rb);
	~PipelineP();

	void init();
	void drawScene();
	void drawMesh(array<Mesh*>* meshs);

	GLuint mProgram;
	GLuint mWorldLocation;
	GLuint mVertexColor;
};


#endif