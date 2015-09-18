#include "Mesh.h"

Mesh::Mesh():_texture(NULL), _positions(NULL)
{

}

Mesh::~Mesh()
{

}

const void* Mesh::GetPositions() const
{
	return _positions.const_pointer();
}

const void* Mesh::GetTexCoords() const
{
	return _texCoords.const_pointer();
}

const void* Mesh::GetIndices() const
{
	return _indices.const_pointer();
}

void Mesh::CalculateEdge()
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

Texture* Mesh::GetTexture()
{
	return _texture;
}

void Mesh::SetTexture(Texture* tex)
{
	_texture = tex;
}