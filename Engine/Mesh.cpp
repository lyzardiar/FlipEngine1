#include "Mesh.h"

const void* Mesh::getPositions() const
{
	return _positions.const_pointer();
}

const void* Mesh::getTexCoords() const
{
	return _texCoords.const_pointer();
}

const void* Mesh::getIndices() const
{
	return _indices.const_pointer();
}

void Mesh::calculateEdge()
{
	for (unsigned int i = 0; i < _indices.size()/3; ++i)
	{
		for (unsigned int e = 0; e < 3; e++)
		{
			const vec3& v0 = _positions[_indices[i * 3 + e]];
			const vec3& v1 = _positions[_indices[i * 3 + (e + 1)%3]];
		
			for (int ii = 0; ii < _edgeList.size(); ++ii)
			{
				Edge& edge = _edgeList[ii];
			//	if (edge)
			//	{
			//	}
			}
		}
	}
}