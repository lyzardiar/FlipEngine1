#include "Material.h"
#include "Lexer.h"
#include "sys/sys_public.h"

Material::Material()
{

}

Material::~Material()
{

}

	//"vPosition",
	//"vTexCoord",
	//"vNormal",
	//"vTangent",
	//"vBinormal"
bool Material::LoadMemory( const char* buffer )
{
	_numAttri = 0;

	Lexer lexer;
	lexer.LoadMemory(buffer);

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

bool Material::ParseVertProgram( Lexer& lexer )
{
	Token tk;
	while(lexer.Lex(tk))
	{
		Sys_Printf("%s %s\n", tk.Name(), tk._data.c_str());
		if (tk._data.Icmp("vPosition"))
			_attriArr[_numAttri++] = 0;
		else if(tk._data.Icmp("vTexCoord"))
			_attriArr[_numAttri++] = 1;
		else if(tk._data.Icmp("vNormal"))
			_attriArr[_numAttri++] = 2;
		else if (tk._data.Icmp("vTangent"))
			_attriArr[_numAttri++] = 3;
		else if (tk._data.Icmp("vBinormal"))
			_attriArr[_numAttri++] = 4;
	}
	return false;
}

bool Material::ParseFragProgram( Lexer& lexer )
{
	return false;
}

