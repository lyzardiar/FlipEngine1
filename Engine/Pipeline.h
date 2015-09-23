#ifndef __PIPELINE_H__
#define __PIPELINE_H__
#include "array.h"
#include "RenderBuffer.h"
#include "Shader.h"

class Mesh;

class Pipeline
{
public:
	Pipeline(renderBuffer_t* rb);

	virtual ~Pipeline();

	virtual void Init() = 0;

	virtual void DrawScene() = 0;
	
	virtual void DrawMesh(array<Mesh*>* meshs) = 0;

	void AddMesh(Mesh* mesh)
	{
		_meshs.push_back(mesh);
	}

	void DrawMeshP(Mesh* mesh);

	void DrawMeshPT(Mesh* mesh);

	void DrawMesh() 
	{ 
		if (_meshs.empty())
			return;

		DrawMesh(&_meshs); 
	}
protected:
	renderBuffer_t* _render;

	array<Mesh*>    _meshs;
};
#endif