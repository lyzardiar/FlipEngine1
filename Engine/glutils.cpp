#include "glutils.h"


static GLuint uglLoadShader(GLenum shaderType, const char* source)
{
	GLuint shader = glCreateShader(shaderType);
	if (shader)
	{
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);
		GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
					GL_LOG("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint GL_GenTextureRGBA(int w, int h, void* data)
{
	GLuint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, w, h, 0, GL_BGRA , GL_UNSIGNED_BYTE, data);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return texId;
}

GLuint GL_GenTextureRGB(int w, int h, void* data)
{
	GLuint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return texId;
}



GLuint GL_CreateProgram(const char* pVertexSource, const char* pFragmentSource) {
	GLuint vertexShader = uglLoadShader(GL_VERTEX_SHADER, pVertexSource);
	if (!vertexShader) {
		return 0;
	}

	GLuint pixelShader = uglLoadShader(GL_FRAGMENT_SHADER, pFragmentSource);
	if (!pixelShader) {
		return 0;
	}

	GLuint program = glCreateProgram();
	if (program) {
		glAttachShader(program, vertexShader);
		GL_CheckError("glAttachShader");
		glAttachShader(program, pixelShader);
		GL_CheckError("glAttachShader");
		glLinkProgram(program);
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE) {
			GLint bufLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			if (bufLength) {
				char* buf = (char*)malloc(bufLength);
				if (buf) {
					glGetProgramInfoLog(program, bufLength, NULL, buf);
					GL_LOG("Could not link program:\n%s\n", buf);
					free(buf);
				}
			}
			glDeleteProgram(program);
			program = 0;
		}
	}
	return program;
}

GLuint GL_CreateProgramFromFile(char* vert, char* frag)
{
	GLuint v, f;

       if(! (v = compileGLSLShaderFromFile(GL_VERTEX_SHADER, vert)))
        v = compileGLSLShaderFromFile(GL_VERTEX_SHADER, &vert[3]); //skip the first three chars to deal with path differences

    if(! (f = compileGLSLShaderFromFile(GL_FRAGMENT_SHADER, frag)))
        f = compileGLSLShaderFromFile(GL_FRAGMENT_SHADER, &frag[3]); //skip the first three chars to deal with path differences

	return LinkGLSLProgram(v, f);
}

void RB_SetGL2D( void ) 
{
	// set 2D virtual screen size
	// glViewport( 0, 0, 800, 600);

	glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	glOrtho( 0, 800, 0, 600, 0, 1 );		// always assume 640x480 virtual coordinates
	glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}


void GL_CheckError(const char* op)
{
    for (GLint error = glGetError(); error; error
            = glGetError()) {
		GL_LOG( "after %s() glError (0x%x)\n", op, error);
    }
}