#pragma once
#include "array.h"
#include "vec3.h"
#include "color4.h"
#include "Joint.h"
#include "vec2.h"


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
	const void* getPositions() const;
	const void* getTexCoords() const;
	const void* getIndices() const;
	const void* getNormals() const;
	const void* getColors() const;

	int getPositionCount() const;
    int getNormalCount() const;
    int getTexCoordCount() const;
    int getTangentCount() const;
    int getColorCount() const;

    int getIndexCount() const;

	bool hasNormals();
	bool hasTexCoords();

	void calculateEdge();

	array<vec3>				_positions;
	array<vec3>				_normals;
	array<vec2>				_texCoords;
	array<unsigned short>	_indices;

	array<Edge>             _edgeList;
};
