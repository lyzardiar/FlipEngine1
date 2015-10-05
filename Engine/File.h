#ifndef __FILE_H__
#define __FILE_H__

#include <stdio.h>
#include "common/str.h"
#include "common/vec3.h"
#include "common/quat.h"

// mode parm for Seek
typedef enum {
	FS_SEEK_SET,
	FS_SEEK_CUR,
	FS_SEEK_END,
} fsOrigin_t;

class lfBuffer
{
public:
	
	 lfStr ReadString();
	 int ReadInt();
	 float ReadFloat();
	 int ReadChar();
	 vec3 ReadVec3();
	 quat ReadQuat();

	 unsigned int ReadUnsignedInt();
	 unsigned short ReadUnsignedShort();
	 unsigned char ReadUnsignedChar();

	 virtual int Length();
	 virtual int Read( void *buffer, int len );
	 virtual int Seek( long offset, fsOrigin_t origin );
};

class lfFile : public lfBuffer
{
public:
	lfFile()  {}
	~lfFile();

	bool Open(const char* filepath);

	int Tell();
public:
	virtual int Read( void *buffer, int len );
	virtual int Length();
	virtual int Seek( long offset, fsOrigin_t origin );
private:
	FILE* fp;

	int fileSize;
};

#endif

