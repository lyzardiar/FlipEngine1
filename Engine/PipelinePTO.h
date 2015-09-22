#ifndef __PIPELINEPTO_H__
#define __PIPELINEPTO_H__

#include "Pipeline.h"
#include "glutils.h"

class PipelinePTO:public Pipeline
{
public:
	PipelinePTO(renderBuffer_t* rb);
	~PipelinePTO();

	void Init();
	void DrawScene();
	void DrawMesh(array<Mesh*>* meshs);

	Shader* _positionShader;
	Shader* _positionTexShader;
};

#endif