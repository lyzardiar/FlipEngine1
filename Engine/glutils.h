#pragma once
#include "gl/glew.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#define LOG(...) printf(...);
#endif // WIN32


void uglLog(const char *fmt, ...);

GLuint uglGenTextureRGBA(int w, int h, void* data);

GLuint uglGenTextureRGB(int w, int h, void* data);

void uglCheckError(const char* op);

GLuint uglLoadShader(GLenum shaderType, const char* source);

GLuint uglCreateProgram(const char* pVertexSource, const char* pFragmentSource);

#include "glshaderutils.h"
inline GLuint createShaders(char* vert, char* frag)
{
	GLuint v, f;

    if(! (v = compileGLSLShaderFromFile(GL_VERTEX_SHADER, vert)))
        v = compileGLSLShaderFromFile(GL_VERTEX_SHADER, &vert[3]); //skip the first three chars to deal with path differences

    if(! (f = compileGLSLShaderFromFile(GL_FRAGMENT_SHADER, frag)))
        f = compileGLSLShaderFromFile(GL_FRAGMENT_SHADER, &frag[3]); //skip the first three chars to deal with path differences

	return LinkGLSLProgram(v, f);
}

