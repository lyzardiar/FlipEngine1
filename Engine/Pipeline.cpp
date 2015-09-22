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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, mesh->GetPositions());
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, mesh->GetTexCoords());
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, mesh->GetTexCoords());
	glDrawElements(GL_TRIANGLES, mesh->_indices.size(), GL_UNSIGNED_SHORT, mesh->_indices.pointer());
	//glDrawElements(GL_TRIANGLES, 904*3, GL_UNSIGNED_SHORT, mesh->_indices.pointer());
}
