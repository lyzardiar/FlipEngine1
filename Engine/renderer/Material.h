#ifndef __Material_H__
#define	__Material_H__ 
#include "../glutils.h"

class Material
{
public:
	Material();
	virtual ~Material();

	void LoadShader(const char* vbuffer, const char* fbuffer);

	bool BindUniform(int type);

private:
	GLuint _program;
};

#endif
