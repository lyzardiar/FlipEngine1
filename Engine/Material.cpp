#include "Material.h"
#include "Lexer.h"
#include "sys/sys_public.h"

Material::Material()
{

}

Material::~Material()
{

}

bool Material::LoadBuffer( const char* buffer )
{
	Lexer lexer;
	lexer.LoadMemory(buffer);

	Token tk;
	while(lexer.Lex(tk))
	{
		Sys_Printf(tk._data.c_str());
	}

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



