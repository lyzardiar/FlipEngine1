#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#define MAX_ATTRI 9

class Lexer;

class Material
{
public:
	Material();
	~Material();

	bool LoadMemory(const char* buffer);

	bool HasPosition();

	bool HasTexCoord();

	bool HasNormal();

	bool HasColor();

	bool ParseVertProgram(Lexer& lexer);

	bool ParseFragProgram(Lexer& lexer);

	unsigned int ProgramId();

private:
	bool _hasPosition;
	bool _hasTexCoord;
	bool _hasNormal;
	bool _hasTangent;
	bool _hasBinormal;

public:
	unsigned short _attriArr[MAX_ATTRI];
	unsigned short _numAttri;
};

#endif


