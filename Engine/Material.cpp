#include "Material.h"

Material::Material()
{

}

Material::~Material()
{

}

bool Material::LoadBuffer( const char* buffer )
{
	return false;
}

bool Material::HasPosition()
{

	return false;
}

bool Material::HasTexCoord()
{

	return false;
}

bool Material::HasNormal()
{

	return false;
}

bool Material::HasColor()
{

	return false;
}

unsigned int Material::ProgramId()
{

	return 11;
}



