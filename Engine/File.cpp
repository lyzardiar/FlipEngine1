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
	current = 0;
	return true;
}

int lfFile::Read( void *buffer, int len )
{
	current += len;
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

int lfFile::Skip( int len )
{
	Seek(len, FS_SEEK_CUR);
	return 0;
}

char* F_ReadFileData( const char* filename )
{
	FILE* fp;
	char* buf;
	long len;

	//must read files as binary to prevent problems from newline translation
	fp = fopen( filename, "rb");

	if ( fp == NULL)
		return 0;

	fseek( fp, 0, SEEK_END);
	len = ftell(fp);
	fseek( fp, 0, SEEK_SET);
	buf = new char[len+1];
	fread( buf, len, 1, fp);
	fclose( fp);

	buf[len] = '\0';
	return buf;
}