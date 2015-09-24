#ifndef __GLUTILS_H__
#define __GLUTILS_H__
#include "gl/glew.h"

#include <stdio.h>
#include <stdlib.h>
#include "glshaderutils.h"


#include "mat4.h"


class Shader;
class DrawVert;
class Texture;
typedef unsigned int glIndex_t;

// our only drawing geometry type
typedef struct srfTriangles_s {
	int							numVerts;				// number of vertices
	DrawVert *					verts;					// vertices, allocated with special allocator

	int							numIndexes;				// for shadows, this has both front and rear end caps and silhouette planes
	glIndex_t *					indexes;

	GLuint vbo[2];
}srfTriangles_t;

typedef struct
{
	Shader* shader;
	Texture* tex;
}material_t;

typedef struct{
	int							id;
	material_t*					shader;
	srfTriangles_t *			geometry;
} modelSurface_t;

typedef struct{
	int							id;
	Shader*						shader;
	Texture*					tex;
	srfTriangles_t *			geo;
	mat4						matModel;
} drawSurf_t;

typedef struct
{
	mat4 matWorld;
	mat4 matView;
	mat4 matPerspective;
	mat4 matWVP;

	Shader* shaders[32];
}renderBuffer_t;


typedef struct {
	int			width;
	int			height;
	bool			fullScreen;
	bool			stereo;
	int			displayHz;
	int			multiSamples;
} glimpParms_t;



typedef enum
{
	eUniform_MVP,
	eUniform_Color,
	eUniform_Samper0,

	eUniform_Count,
}unformType_t;

typedef enum
{
	eAttrib_Position,
	eAttrib_TexCoord,

	eAttrib_Count,
}attribType_t;

typedef enum
{
	eShader_Position,
	eShader_PositionTex,

	eShader_Count,
}shaderType_t;

GLuint GL_GenTextureRGBA(int w, int h, void* data);
GLuint GL_GenTextureRGB(int w, int h, void* data);

GLuint GL_CreateProgram(const char* pVertexSource, const char* pFragmentSource);
GLuint GL_CreateProgramFromFile(const char* vert, const char* frag);

void GL_CheckError(const char* op);

void RB_SetGL2D( void );


/**
/* @interface
 */
bool GL_CreateDevice(glimpParms_t *parm);

void GL_SwapBuffers( void );

void Test_2DDraw();

#endif
