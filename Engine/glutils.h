#ifndef __GLUTILS_H__
#define __GLUTILS_H__
#include "gl/glew.h"

#include <stdio.h>
#include <stdlib.h>

class Texture;
typedef unsigned short glIndex_t;

typedef struct {
	int			width;
	int			height;
	bool			fullScreen;
	bool			stereo;
	int			displayHz;
	int			multiSamples;
} glimpParms_t;

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
