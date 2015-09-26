#ifndef __FILE_H__
#define __FILE_H__

#include <stdio.h>
#include "common/str.h"
#include "common/vec3.h"
#include "common/quat.h"

class lfBuffer
{
public:
	
	 lfStr			ReadString();
	 int				ReadInt();
	 unsigned int		ReadUInt();
	 float			ReadFloat();
	 int				ReadChar();
	 vec3			ReadVec3();
	 quat			ReadQuat();

	 virtual int		Length();
	 virtual int		Read( void *buffer, int len );
};

class lfFile : public lfBuffer
{
public:
	lfFile()  {}
	~lfFile();

	bool Open(const char* filepath);

	int Tell();
public:
	virtual int		Read( void *buffer, int len );
	virtual int		Length();
private:
	FILE* fp;

	int fileSize;
};

#endif

