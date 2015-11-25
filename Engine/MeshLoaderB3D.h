/*
http://www.blitzbasic.com/sdkspecs/sdkspecs/b3dfile_specs.txt
*/

#ifndef __MESHLOADB3D_H__
#define __MESHLOADB3D_H__

#include "common/str.h"
#include "common/quat.h"
#include "common/vec3.h"
#include "common/vec2.h"
#include "common/array.h"

#include "r_public.h"

class Mesh;
class lfFile;
class Joint;

class MeshLoaderB3D
{
public:
	MeshLoaderB3D();
	~MeshLoaderB3D();

	bool Load(const char* file);

	struct SB3dChunkHeader
	{
		char name[4];
		int size;
	};

	struct SB3dTexture
	{
		lfStr TextureName;
		int Flags;
		int Blend;
		float Xpos;
		float Ypos;
		float Xscale;
		float Yscale;
		float Angle;
	};

	struct SB3dMaterial
	{
		SB3dMaterial() : red(1.0f), green(1.0f),
			blue(1.0f), alpha(1.0f), shininess(0.0f), blend(1),
			fx(0)
		{
		//	for (unsigned int i=0; i<3; ++i)
				//Textures[i]=0;
		}
		//video::SMaterial Material;
		float red, green, blue, alpha;
		float shininess;
		int blend,fx;
		//SB3dTexture *Textures[video::MATERIAL_MAX_TEXTURES];
	};

	void ReadTEXS();
	bool ReadVRTS();
	void ReadNODE();
	void ReadBRUS();
	void ReadBONE();
	void ReadMesh();
	void ReadTRIS();
	void ReadANIM();
	void ReadKEY();

	lfStr ReadChunk();
	bool CheckSize();
	void ExitChunk();

	void printTree(const char *psz, ...);
	long _curNodePos;
	int _layer;
	array<SB3dTexture> _textures;
	array<unsigned int> _stack;

	array<unsigned short> _indices;
	Mesh*    _mesh;

	int             _meshCount;
	Joint*			_rootJoint;
	Joint*			_readJoint;
	float			_totalFrame;

	lfFile*         _file;
};

#endif
