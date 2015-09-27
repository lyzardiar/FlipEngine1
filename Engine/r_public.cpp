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
