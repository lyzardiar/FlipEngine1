#ifndef __SHADER_H__
#define __SHADER_H__

#include "glutils.h"

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

class Shader
{
public:
	Shader() {}
	~Shader() {}

	void BindAttribLocation(attribType_t type);

	void GetUniformLocation(unformType_t type);

	GLuint GetProgarm();

	GLuint GetUniform(unformType_t type);

	bool LoadFromBuffer(const char* vfile, const char* ffile);

	bool LoadFromFile(const char* vfile, const char* ffile);
private:
	GLuint _program;
	GLuint _uniforms[eUniform_Count];
};

#endif


