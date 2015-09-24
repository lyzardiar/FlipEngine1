#include "Mesh.h"
#include "DrawVert.h"

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

vec3& Mesh::getPosition()
{
	return _position;
}

void Mesh::SetPosition( float x, float y, float z )
{
	_position.x = x;
	_position.y = y;
	_position.z = z;
}

void Mesh::setupVBO()
{
	_indexCount = _indices.size();
	// This functions copies the vertex and index buffers into their respective VBO's
	glGenBuffers(1, &_vertexBuffer);
	glGenBuffers(1, &_indexBuffer);

	// Stick the data for the vertices into its VBO
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * _vertexCount, _positions.const_pointer(), GL_STATIC_DRAW);

	// Stick the data for the indices into its VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * _indexCount, _indices.const_pointer(), GL_STATIC_DRAW);

	// Clear the VBO state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
