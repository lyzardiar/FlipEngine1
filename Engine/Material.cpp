#include "Material.h"
#include "Lexer.h"
#include "sys/sys_public.h"

Material::Material() {

}

Material::~Material() {

}

	//"vPosition",
	//"vTexCoord",
	//"vNormal",
	//"vTangent",
	//"vBinormal"

	//"WVP",
	//"COLOR",
	//"texture1",
	//"modelView",
	//"invModelView",
	//"fvEyePosition",
	//"fvLightPosition",
	//"bumpMap"
bool Material::LoadMemory( const char* buffer ) {
	_numAttri = 0;

	Lexer lexer;
	lexer.LoadMemory(buffer);

	Token tk;
	while(lexer.Lex(tk)) {
		if (tk._data == "vert")
		{
			ParseVertProgram(lexer);
		}
		else if (tk._data == "frag")
		{
			ParseFragProgram(lexer);
		}
		else
		{
			Sys_Error("error %s", tk.Name(), tk._data.c_str());
		}
	}

	_program = GL_CreateProgram(_vert, _frag);
	return false;
}

bool Material::HasPosition() {
	return false;
}

bool Material::HasTexCoord() {
	return false;
}

bool Material::HasNormal() {
	return false;
}

bool Material::HasColor() {
	return false;
}

unsigned int Material::ProgramId() {
	return 11;
}

bool Material::ParseVertProgram( Lexer& lexer ) {
	int openParen = 0;
	int start = lexer.CurrentPos();
	Token tk;
	while (lexer.Lex(tk))
	{
		if (tk._data == "vPosition")
			_attriArr[_numAttri++] = 0;
		else if(tk._data == "vTexCoord")
			_attriArr[_numAttri++] = 1;
		else if(tk._data == "vNormal")
			_attriArr[_numAttri++] = 2;
		else if (tk._data == "vTangent")
			_attriArr[_numAttri++] = 3;
		else if (tk._data == "vBinormal")
			_attriArr[_numAttri++] = 4;
		else if (tk._data == "WVP")
			_hasWorldViewPorj = true;
		else if (tk._data == "COLOR")
			_hasColor = true;
		else if (tk._data == "texture1")
			_hasTexture = true;
		else if (tk._data == "modelView")
			_hasModelView = true;
		else if (tk._data == "invModelView")
			_hasInvModelView = true;
		else if (tk._type == '{')
		{
			openParen ++;
		}
		else if (tk._type == '}')
		{
			openParen--;
			if (openParen < 0)
			{
				_vert = lexer.SubStr(start, lexer.CurrentPos()-1);
				return true;
			}
		}
	}
	return false;
}

bool Material::ParseFragProgram( Lexer& lexer ) {
		int openParen = 0;
	int start = lexer.CurrentPos();
	Token tk;
	while (lexer.Lex(tk))
	{
		if (tk._data == "vPosition")
			_attriArr[_numAttri++] = 0;
		else if(tk._data == "vTexCoord")
			_attriArr[_numAttri++] = 1;
		else if(tk._data == "vNormal")
			_attriArr[_numAttri++] = 2;
		else if (tk._data == "vTangent")
			_attriArr[_numAttri++] = 3;
		else if (tk._data == "vBinormal")
			_attriArr[_numAttri++] = 4;
		else if (tk._data == "WVP")
			_hasWorldViewPorj = true;
		else if (tk._data == "COLOR")
			_hasColor = true;
		else if (tk._data == "texture1")
			_hasTexture = true;
		else if (tk._data == "modelView")
			_hasModelView = true;
		else if (tk._data == "invModelView")
			_hasInvModelView = true;
		else if (tk._type == '{')
		{
			openParen ++;
		}
		else if (tk._type == '}')
		{
			openParen--;
			if (openParen < 0)
			{
				_frag = lexer.SubStr(start, lexer.CurrentPos()-1);
				return true;
			}
		}
	}
	return false;
}

