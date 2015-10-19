#ifndef __MATERIAL_H__
#define __MATERIAL_H__

class Material
{
public:
	Material();
	~Material();

	bool LoadBuffer(const char* buffer);

	bool HasPosition();

	bool HasTexCoord();

	bool HasNormal();

	bool HasColor();

	bool ParseVertProgram();

	bool ParseFlagProgram();

	unsigned int ProgramId();

private:
	unsigned int _AttriBits;
};

#endif


