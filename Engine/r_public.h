#ifndef __RENDERBUFFER_H__
#define __RENDERBUFFER_H__
#include "glutils.h"

#include "common/mat4.h"

class DrawVert;
class Shader;

// our only drawing geometry type
typedef struct srfTriangles_s {
	int				numVerts;				
	DrawVert*			verts;				

	int				numIndexes;			
	glIndex_t*		indexes;

	GLuint			vbo[2];
}srfTriangles_t;

typedef struct
{
	Shader* shader;
	Texture* tex;
}material_t;

typedef struct{
	int						id;
	material_t*				material;
	srfTriangles_t *		geometry;
} modelSurface_t;

typedef struct{
	int						id;
	material_t*             material;
	srfTriangles_t *		geo;
	mat4					matModel;
} drawSurf_t;

typedef struct
{
	mat4 matWorld;
	mat4 matView;
	mat4 matPerspective;
	mat4 matWVP;

	Shader* shaders[32];
}renderBuffer_t;


srfTriangles_t *R_AllocStaticTriSurf( void );

void	 R_AllocStaticTriSurfVerts( srfTriangles_t *tri, int numVerts );

material_t* R_AllocMaterail();
#endif
