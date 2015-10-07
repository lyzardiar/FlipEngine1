#ifndef __RENDERBUFFER_H__
#define __RENDERBUFFER_H__
#include "glutils.h"

#include "common/mat4.h"
#include "DrawVert.h"

class DrawVert;
class Shader;

// our only drawing geometry type
typedef struct srfTriangles_s {
	int	numVerts;				
	DrawVert* verts;				

	int	numIndexes;			
	glIndex_t* indexes;

	GLuint vbo[2];

	bool tangentsCalculated;
}srfTriangles_t;

typedef struct
{
	Shader* shader;
	Texture* tex;
}material_t;


typedef struct{
	int	id;
	material_t* material;
	srfTriangles_t*	geo;
	mat4* view;
	mat4* proj;
	mat4* viewProj;
	mat4 matModel;

	vec3 eyePos;
	vec3 lightPos;

	bool bShaowmap;
} drawSurf_t;

typedef struct
{
	mat4 matWorld;
	mat4 matView;
	mat4 matPerspective;
	mat4 matWVP;

	Shader* shaders[32];
}renderBuffer_t;

typedef struct
{
	GLuint fbo;
	GLuint rbo;
	GLuint texId;
	unsigned int width;
	unsigned int height;
}shadowMap_t;

drawSurf_t* R_AllocDrawSurf();

srfTriangles_t *R_AllocStaticTriSurf( void );

void	 R_AllocStaticTriSurfVerts( srfTriangles_t *tri, int numVerts );

material_t* R_AllocMaterail();

drawSurf_t* R_GenerateFloor(float w, float h);

void R_GenerateGeometryVbo( srfTriangles_t *tri);

void R_GenerateQuad(srfTriangles_t* geo);

shadowMap_t* R_GenerateShadowMap();

mat4& R_BillboardModelView(mat4& model, mat4& view);

void R_DeriveNormals( srfTriangles_t *tri );

#endif
