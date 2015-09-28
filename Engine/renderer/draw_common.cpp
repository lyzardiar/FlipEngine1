#include "draw_common.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../DrawVert.h"
#include "../sys/sys_public.h"

#define offsetof(s,m)   (size_t)&reinterpret_cast<const volatile char&>((((s *)0)->m))


void R_DrawPositonTex( srfTriangles_t* tri )
{
	glBindBuffer(GL_ARRAY_BUFFER, tri->vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DrawVert), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid *)12);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri->vbo[1]);
	glDrawElements(GL_TRIANGLES, tri->numIndexes, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void R_RenderPTPass( drawSurf_t* drawSur, DrawFunc drawFunc )
{
	srfTriangles_t* tri = drawSur->geo;
	material_t* material = drawSur->material;
	mat4 t = (*drawSur->view) * drawSur->matModel;
	Shader* shader = material->shader;

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glUseProgram( shader->GetProgarm() );
	glUniformMatrix4fv( shader->GetUniform(eUniform_MVP), 1, GL_FALSE, &t.m[0] );
	glUniform1i( shader->GetUniform(eUniform_Samper0), 0 );
	glBindTexture( GL_TEXTURE_2D, material->tex->GetName() );

	drawFunc(tri);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
