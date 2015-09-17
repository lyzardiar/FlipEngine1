#include "Material.h"

Material::Material()
{

}

Material::~Material()
{


}

void Material::LoadShader(const char* vbuffer, const char* fbuffer)
{
	_program = GL_CreateProgram(vbuffer, fbuffer);
}
