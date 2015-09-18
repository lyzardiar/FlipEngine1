#ifndef __PIPELINE_H__
#define __PIPELINE_H__
#include "array.h"
#include "RenderBuffer.h"

class Mesh;

class Pipeline
{
public:
	Pipeline() {}
	Pipeline(renderBuffer_t* rb) {_render = rb;}

	virtual ~Pipeline() {};

	virtual void Init() = 0;

	virtual void DrawScene() = 0;
	
	virtual void DrawMesh(array<Mesh*>* meshs) {};

protected:
	renderBuffer_t* _render;
};
#endif