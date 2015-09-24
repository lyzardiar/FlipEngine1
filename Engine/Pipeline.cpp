#include "Pipeline.h"
#include "Mesh.h"


Pipeline::Pipeline(renderBuffer_t* rb) : _render(rb)
{
}

Pipeline::~Pipeline()
{
}

void Pipeline::DrawMeshP( Mesh* mesh )
{
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, mesh->GetPositions());
	//glDrawElements(GL_TRIANGLES, 904*3, GL_UNSIGNED_SHORT, mesh->_indices.pointer());
	glDrawElements(GL_TRIANGLES, mesh->_indices.size(), GL_UNSIGNED_SHORT, mesh->_indices.pointer());
}

void Pipeline::DrawMeshPT( Mesh* mesh )
{
	 // Bind the VBO for the vertex data
    glBindBuffer(GL_ARRAY_BUFFER, mesh->_vertexBuffer);
	//3021
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, mesh->GetPositions());
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, mesh->GetTexCoords());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->_indexBuffer);
	glDrawElements(GL_TRIANGLES, mesh->_indices.size(), GL_UNSIGNED_SHORT, 0);
}
