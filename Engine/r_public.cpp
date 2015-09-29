#include "r_public.h"
#include "DrawVert.h"

srfTriangles_t * R_AllocStaticTriSurf( void )
{
	srfTriangles_t *tris = new srfTriangles_t;
	memset( tris, 0, sizeof( srfTriangles_t ) );
	return tris;
}

void R_AllocStaticTriSurfVerts( srfTriangles_t *tri, int numVerts ) {
	tri->verts = new DrawVert[ numVerts ];
}

material_t* R_AllocMaterail()
{
	material_t* material = new material_t;
	memset(material, 0, sizeof(material_t));
	return material;
}

void R_GenerateGeometryVbo( srfTriangles_t *tri )
{
	if (tri->vbo[0] != 0)
		glDeleteBuffers(1, &tri->vbo[0]);

	if (tri->vbo[1] != 0)
		glDeleteBuffers(1, &tri->vbo[0]);

	glGenBuffers(1, &tri->vbo[0]);
	glGenBuffers(1, &tri->vbo[1]);

	// Stick the data for the vertices into its VBO
	glBindBuffer(GL_ARRAY_BUFFER, tri->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(DrawVert) * tri->numVerts, tri->verts, GL_STATIC_DRAW);

	// Stick the data for the indices into its VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri->vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glIndex_t) * tri->numIndexes, tri->indexes, GL_STATIC_DRAW);

	// Clear the VBO state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

drawSurf_t* R_AllocDrawSurf()
{
	drawSurf_t* drawSurf = new drawSurf_t;
	memset(drawSurf, 0, sizeof(drawSurf_t));
	drawSurf->matModel.makeIdentity();
	return drawSurf;
}
