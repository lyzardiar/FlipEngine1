// draw common °æ±¾2

#include "draw_common.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../DrawVert.h"
#include "../sys/sys_public.h"
#include "../Material.h"

static void R_BindArrayBuffer(int i) {
	switch (i) {
	case 0:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DrawVert), 0);
		break;
	case 1:
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid *)12);
		break;
	case 2:
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid *)20);
		break;
	case 3:
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid *)32);
		break;
	case 4:
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid *)44);
		break;
	default:
		Sys_Error("%s %s", __FILE__, __LINE__);
		break;
	}
}

void R_RenderCommon(drawSurf_t* drawSurf){
	Material* matr = drawSurf->matr;
	srfTriangles_t* tri = drawSurf->geo;
	unsigned short* attri = matr->_attriArr;
	unsigned short numAttri = matr->_numAttri;

	for (int i = 0; i < numAttri; i++)
		glEnableVertexAttribArray(attri[i]);

	R_DrawCommon(tri, attri, numAttri);

	for (int i = 0; i < numAttri; i++)
		glEnableVertexAttribArray(attri[i]);
}

void R_DrawCommon( srfTriangles_t* tri, unsigned short *attri, unsigned short numAttri ) {
	glBindBuffer(GL_ARRAY_BUFFER, tri->vbo[0]);
	for (int i=0; i<numAttri; i++){
		R_BindArrayBuffer(attri[i]);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri->vbo[1]);
	glDrawElements(GL_TRIANGLES, tri->numIndexes, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

