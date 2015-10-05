#include "File.h"
#include "sys/sys_public.h"

int lfBuffer::ReadInt()
{
	int value;
	Read(&value, sizeof(int));
	return value;
}

float lfBuffer::ReadFloat()
{
	float value;
	Read(&value, sizeof(float));
	return value;
}


int lfBuffer::ReadChar()
{
	int value;
	Read(&value, sizeof(char));
	return value;
}

unsigned int lfBuffer::ReadUnsignedInt()
{
	unsigned int value;
	Read(&value, sizeof(unsigned int));
	return value;
}

lfStr lfBuffer::ReadString()
{
	lfStr str;
	char c = ReadChar();

	while (c)
	{
		str += c;
		if (c=='/0')
			break;
		c = ReadChar();
	}
	return str;
}

int lfBuffer::Read( void *buffer, int len )
{
	return 0;
}

int lfBuffer::Length()
{
	return 0;
}

vec3 lfBuffer::ReadVec3()
{
	vec3 value;
	Read(&value, sizeof(vec3));
	return value;
}

quat lfBuffer::ReadQuat()
{
	quat value;
	Read(&value, sizeof(quat));
	return value;
}

unsigned short lfBuffer::ReadUnsignedShort()
{
	unsigned short value;
	Read(&value, sizeof(unsigned short));
	return value;
}

unsigned char lfBuffer::ReadUnsignedChar()
{
	unsigned char value;
	Read(&value, sizeof(unsigned char));
	return value;
}

int lfBuffer::Seek( long offset, fsOrigin_t origin )
{
	return 0;
}

bool lfFile::Open( const char* filepath )
{
	fp = fopen(filepath, "rb");
	if (fp == nullptr)
	{
		Sys_Printf("open file failed", filepath);
		return false;
	}

	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return true;
}

int lfFile::Read( void *buffer, int len )
{
	fread(buffer, len, 1, fp);
	return len;
}

int lfFile::Length()
{
	return fileSize;
}

lfFile::~lfFile()
{
	if (fp != nullptr)
		fclose(fp);
}

int lfFile::Tell()
{
	return ftell(fp);
}

int lfFile::Seek( long offset, fsOrigin_t origin )
{
	fseek(fp, offset, origin);
	return 0;
}
