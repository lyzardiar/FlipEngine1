#pragma once
#include "array.h"
#include "vec3.h"
#include "color4.h"
#include "Joint.h"
#include "vec2.h"
#include "glutils.h"

class Texture;
class DrawVert;



class Edge {
public:
	Edge(void) {}
	Edge(unsigned short v0, unsigned short v1,
		unsigned short plane0, unsigned short plane1)
	{
		v[0] = v0;
		v[1] = v1;
		this->plane[0] = plane0;
		this->plane[1] = plane1;
	}

	unsigned short v[2];
	unsigned short plane[2];
};


class Mesh
{
public:
	Mesh();
	~Mesh();

	const void* GetPositions() const;
	const void* GetTexCoords() const;
	const void* GetIndices() const;
	const void* GetNormals() const;
	const void* GetColors() const;
	Texture* GetTexture();
	void SetTexture(Texture* tex);

	int GetPositionCount() const;
    int GetNormalCount() const;
    int GetTexCoordCount() const;
    int GetTangentCount() const;
    int GetColorCount() const;

    int GetIndexCount() const;

	bool HasNormals();
	bool HasTexCoords();

	void CalculateEdge();

	void SetPosition(float x, float y, float z);
	vec3& getPosition();

	void setupVBO();

	array<DrawVert*>         _vertex;

	array<vec3>				_positions;
	array<vec3>				_normals;
	array<vec2>				_texCoords;
	array<unsigned short>		_indices;

	array<Edge>				_edgeList;

	Texture*					_texture;

	vec3                     _position;

	unsigned int				_indexCount;
	unsigned int             _vertexCount;

	GLuint                  _vertexBuffer;
	GLuint                  _indexBuffer;
};
