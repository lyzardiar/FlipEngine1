#include "r_public.h"
#include "common/Plane.h"
/*
==================
R_DeriveTangents

This is called once for static surfaces, and every frame for deforming surfaces

Builds tangents, normals, and face planes
==================
*/
void R_DeriveNormals( srfTriangles_t *tri ) {
	int				i;
	Plane			*planes;

	if ( tri->tangentsCalculated ) {
		return;
	}

	for ( i = 0; i < tri->numVerts; i++ ) {
		tri->verts[i].normal.Zero();
		tri->verts[i].tangents[0].Zero();
		tri->verts[i].tangents[1].Zero();
	}

	for ( i = 0; i < tri->numIndexes; i += 3 ) {
		// make face tangents
		float		d0[3], d1[3];
		DrawVert	*a, *b, *c;
		vec3		temp, normal, tangents[2];

		a = tri->verts + tri->indexes[i + 0];
		b = tri->verts + tri->indexes[i + 1];
		c = tri->verts + tri->indexes[i + 2];

		d0[0] = b->xyz[0] - a->xyz[0];
		d0[1] = b->xyz[1] - a->xyz[1];
		d0[2] = b->xyz[2] - a->xyz[2];

		d1[0] = c->xyz[0] - a->xyz[0];
		d1[1] = c->xyz[1] - a->xyz[1];
		d1[2] = c->xyz[2] - a->xyz[2];

		// normal
		temp[0] = d1[1] * d0[2] - d1[2] * d0[1];
		temp[1] = d1[2] * d0[0] - d1[0] * d0[2];
		temp[2] = d1[0] * d0[1] - d1[1] * d0[0];
		temp.normalize();

		// sum up the tangents and normals for each vertex on this face
		for ( int j = 0 ; j < 3 ; j++ ) {
			DrawVert* vert = &tri->verts[tri->indexes[i+j]];
			vert->normal += temp;
		}
	}

	tri->tangentsCalculated = true;
}