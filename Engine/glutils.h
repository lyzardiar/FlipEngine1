#ifndef __GLUTILS_H__
#define __GLUTILS_H__
#include "gl/glew.h"

#include <stdio.h>
#include <stdlib.h>
#include "glshaderutils.h"

#ifdef _DEBUG
#define GL_LOG(fmt, ...);
#define  GL_CheckError(op) {\
    for (GLint error = glGetError(); error; error\
            = glGetError()) {\
		GL_LOG( "after %s() glError (0x%x)\n", op, error);\
    }\
}
#else
#define GL_LOG(...)
#define GL_CheckError(...)
#endif // WIN32

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
GLuint GL_CreateProgramFromFile(char* vert, char* frag);

/**
/* @interface
 */
bool GL_CreateDevice(glimpParms_t *parm);

void GL_SwapBuffers( void );

#endif
