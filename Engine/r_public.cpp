#include "r_public.h"
#include "DrawVert.h"

static const int SHADOWMAP_DEPTH_SIZE = 1024;

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

drawSurf_t* R_GenerateFloor(float w, float h)
{
	float halfw = w/2;
	float halfh = h/2;
	drawSurf_t* surf = R_AllocDrawSurf();
	surf->geo = R_AllocStaticTriSurf();
	
	R_GenerateQuad(surf->geo);
	surf->geo->verts[0].xyz = vec3(-halfw, 0.f, -halfh);
	surf->geo->verts[1].xyz = vec3(-halfw, 0.f, halfh);
	surf->geo->verts[2].xyz = vec3(halfw, 0.f, -halfh);
	surf->geo->verts[3].xyz = vec3(halfw, 0.f, halfh);

	surf->geo->verts[0].uv = vec2(0.f, 0.f);
	surf->geo->verts[1].uv = vec2(0.f, 1.f);
	surf->geo->verts[2].uv = vec2(1.f, 0.f);
	surf->geo->verts[3].uv = vec2(1.f, 1.f);

	return surf;
}

void R_GenerateQuad( srfTriangles_t* geo )
{
	geo->vbo[0] = 0;
	geo->vbo[1] = 0;

	geo->numVerts = 4;
	 R_AllocStaticTriSurfVerts(geo, 4);

	geo->verts[0].uv = vec2(0.f, 1.f);
	geo->verts[1].uv = vec2(0.f, 0.f);
	geo->verts[2].uv = vec2(1.f, 1.f);
	geo->verts[3].uv = vec2(1.f, 0.f);

	geo->numIndexes = 6;
	geo->indexes = new glIndex_t[6];
	geo->indexes[0] = 0;
	geo->indexes[1] = 1;
	geo->indexes[2] = 2;
	geo->indexes[3] = 2;
	geo->indexes[4] = 1;
	geo->indexes[5] = 3;
}

shadowMap_t* R_GenerateShadowMap()
{
	shadowMap_t* shadowMap = new shadowMap_t;

	// Create texid
	glGenTextures(1, &shadowMap->texId);
	glBindTexture(GL_TEXTURE_2D, shadowMap->texId);
	glTexStorage2D( GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, SHADOWMAP_DEPTH_SIZE, SHADOWMAP_DEPTH_SIZE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glGenFramebuffers(1, &shadowMap->fbo);    
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMap->fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap->texId, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return shadowMap;
}

mat4& R_BillboardModelView( mat4& model, mat4& view )
{
	mat4 mat;
	mat = view * model;

	for(int i=0; i<3; i++ ) 
		for(int j=0; j<3; j++ ) 
		{
			if ( i==j )
				mat.m[i*4+j] = 1.0;
			else
				mat.m[i*4+j] = 0.0;
		}
	
	return mat;
}
