#include "draw_common.h"
#include "../Shader.h"
#include "../Texture.h"


void R_DrawPositonTex( drawSurf_t* drawSur, mat4* t )
{
	// Bind the VBO for the vertex data
	srfTriangles_t* tri = drawSur->geo;
	Shader* shader = drawSur->shader;

	glUniformMatrix4fv(shader->GetUniform(eUniform_MVP), 1, GL_FALSE, &t->m[0]);

	glBindTexture( GL_TEXTURE_2D, drawSur->tex->GetName() );

	glBindBuffer(GL_ARRAY_BUFFER, tri->vbo[0]);
	//3021
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri->vbo[2]);
	glDrawElements(GL_TRIANGLES, tri->numIndexes, GL_UNSIGNED_SHORT, 0);
}
