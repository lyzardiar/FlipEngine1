#include "MeshLoaderB3D.h"
#include "common/Joint.h"
#include "File.h"
#include "sys/sys_public.h"
#include "Model.h"
#include "DrawVert.h"

MeshLoaderB3D::MeshLoaderB3D() :_meshCount(0), _rootJoint(nullptr), _totalFrame(0), _readJoint(NULL) 
{
}

MeshLoaderB3D::~MeshLoaderB3D() 
{
}

bool MeshLoaderB3D::Load(const char* file) 
{
	_file = new lfFile;
	if( !_file->Open(file) )
		return false;

	_model = new StaticModel;

	lfStr head = ReadChunk();
	int nB3DVersion = _file->ReadInt();

	Sys_Printf("load b3d file %s, version %s %d", file, head.c_str(), nB3DVersion);

	while( CheckSize() )
	{
		lfStr t = ReadChunk();
		if (t == "TEXS"){
			ReadTEXS();
		}
		else if (t == "BRUS"){
			ReadBRUS();
		}
		else if (t == "NODE"){
			ReadNODE();
		}
		ExitChunk();
	}

	delete _file;
	_file = NULL;

	drawSurf_t* surf = _model->getSurfaces().getLast();
	srfTriangles_t* tri = surf->geo;
	tri->numIndexes = _indices.size();
	tri->indexes = new glIndex_t[tri->numIndexes];
	for (unsigned int i=0; i<_indices.size(); ++i)
	{
		tri->indexes[i] = _indices[i];
	}
	return true;
}


bool MeshLoaderB3D::ReadVRTS() 
{
	const int max_tex_coords = 3;
	int flags, tex_coord_sets, tex_coord_set_size;

	flags = _file->ReadInt();
	tex_coord_sets = _file->ReadInt();
	tex_coord_set_size = _file->ReadInt();

	if (tex_coord_sets >= max_tex_coords || tex_coord_set_size >= 4) // Something is wrong 
	{
		Sys_Printf("tex_coord_sets or tex_coord_set_size too big");
		return false;
	}

	//------ Allocate Memory, for speed -----------//

	int sizeOfVertex = 3;
	bool hasNormal = false;
	bool hasVertexColors = false;
	if (flags & 1)
	{
		hasNormal = true;
		sizeOfVertex += 3;
	}
	if (flags & 2)
	{
		sizeOfVertex += 4;
		hasVertexColors=true;
	}

	sizeOfVertex += tex_coord_sets*tex_coord_set_size;
	unsigned int size = _stack[_stack.size() - 1] - _file->Tell();
	//106407 16800
	unsigned int numVertex = size / sizeof(float) ;
	numVertex /= sizeOfVertex;

	drawSurf_t* surface = _model->AllocStaticSurface();
	surface->geo = R_AllocStaticTriSurf();
	srfTriangles_t* tri = surface->geo;
	tri->numVerts = numVertex;
	R_AllocStaticTriSurfVerts(tri, numVertex);

	int idx = 0;
	while( CheckSize())
	{
		float color[4]={1.0f, 1.0f, 1.0f, 1.0f};

		tri->verts[idx].xyz = _file->ReadVec3();

		if (flags & 1)
		{
			tri->verts[idx].normal = _file->ReadVec3();
		}
		if (flags & 2)
		{
			color[0] = _file->ReadFloat();
			color[1] = _file->ReadFloat();
			color[2] = _file->ReadFloat();
			color[3] = _file->ReadFloat();
		}
		float u, v;
		for (int i = 0; i < tex_coord_sets; ++i)
		{
			//for (int j = 0; j < tex_coord_set_size; ++j)
			{
				u = _file->ReadFloat();
				v = 1.0f - _file->ReadFloat();
			}
		}

		tri->verts[idx].uv = vec2(u, v);
		idx++;
	}

	_meshCount++;
	return true;
}

lfStr MeshLoaderB3D::ReadChunk()
{
	lfStr s;
	for (int i = 0; i < 4;++i)
	{
		s += char(_file->ReadChar());
	}
	
	unsigned int size = _file->ReadInt();
	unsigned int pos = _file->Tell();
	_stack.push_back(pos + size);
	return s;
}

bool MeshLoaderB3D::CheckSize()
{
	unsigned int pos = _file->Tell();
	unsigned int size = _stack[_stack.size() - 1];
	return size > pos;
}

void MeshLoaderB3D::ExitChunk()
{
	_curNodePos = _stack.getLast();
	_stack.erase(_stack.size() - 1);
}

void MeshLoaderB3D::ReadTEXS()
{
	while (CheckSize())
	{
		printTree("read texs \n");
		SB3dTexture tex;
		tex.TextureName = _file->ReadString();
		tex.Flags = _file->ReadInt();
		tex.Blend = _file->ReadInt();
		tex.Xpos = _file->ReadFloat();
		tex.Ypos = _file->ReadFloat();
		tex.Xscale = _file->ReadFloat();
		tex.Yscale = _file->ReadFloat();
		tex.Angle = _file->ReadFloat();
		_textures.push_back(tex);
	}
}

void MeshLoaderB3D::ReadNODE()
{
	if (_readJoint == NULL)
	{
		_rootJoint = new Joint;
		_readJoint = _rootJoint;
	}
	else
	{
		Joint* newJoint = new Joint;
		_readJoint->children.push_back(newJoint);
		newJoint->parent = _readJoint;
		_readJoint = newJoint;
	}

	lfStr str = _file->ReadString();
	printTree(str.c_str());

	vec3 t = _file->ReadVec3();
	vec3 s = _file->ReadVec3();
	quat r = _file->ReadQuat();

	_readJoint->name = str;
	_readJoint->position = t;
	_readJoint->scale = s;
	_readJoint->rotation = r;

	while( CheckSize() ){
		lfStr t = ReadChunk();
		if( t=="MESH" ){
			ReadMesh();
		}else if( t=="BONE" ){
			ReadBONE();
		}else if( t=="ANIM" ){
			ReadANIM();
		}else if( t=="KEYS" ){
			ReadKEY();
		}else if( t=="NODE" ){
			ReadNODE();
		}
		ExitChunk();
	}

	_readJoint = _readJoint->parent;
}

void MeshLoaderB3D::ReadBRUS()
{
	int n_texs = _file->ReadInt();
	if( n_texs<0 || n_texs>8 ){
		printf( "Bad texture count" );
	}
	while( CheckSize() ){
		lfStr name = _file->ReadString();
		printTree(name.c_str());
		vec3 color = _file->ReadVec3();
		float alpha = _file->ReadFloat();
		float shiny = _file->ReadFloat();
		/*int blend=**/_file->ReadInt();
		int fx = _file->ReadInt();

		//Textures
		for( int i=0;i<n_texs;++i ){
			int texid= _file->ReadInt();
		}
	}
}

void MeshLoaderB3D::ReadBONE() {
	int i = 0;
	while( CheckSize() ){
		int vertex = _file->ReadInt();
		float weight = _file->ReadFloat();
		_readJoint->vertexIndices.push_back(vertex);
		_readJoint->vertexWeights.push_back(weight);
		i++;
	}
	printTree("vertex count: %d", i);
}

void MeshLoaderB3D::ReadMesh() {
	/*int matid=*/_file->ReadInt();

	//printTree("mesh");
	while( CheckSize() ){
		lfStr t = ReadChunk();
		if( t=="VRTS" ){
			ReadVRTS();
		}else if( t=="TRIS" ){
			ReadTRIS();
		}
		ExitChunk();
	}
}

void MeshLoaderB3D::printTree(const char *psz, ...) {
	//char sBuf[128];
	//va_list ap;
	//va_start(ap, psz);
	//vsnprintf_s(sBuf, 128, 128, psz, ap);
	//va_end(ap);

	//for (int i = 0; i < _stack.size();i++)
	//	Sys_Printf("-");

	//Sys_Printf(sBuf);
	//Sys_Printf("\n");
}

void MeshLoaderB3D::ReadTRIS(){
	int matid = _file->ReadInt();
	if( matid==-1 ){
		matid=0;
	}
	int size = _stack.getLast() - _file->Tell();
	int n_tris=size/12;

	for( int i=0;i<n_tris;++i ){
		int i0 = _file->ReadUInt();
		int i1 = _file->ReadUInt();
		int i2 = _file->ReadUInt();

		_indices.push_back(i0);
		_indices.push_back(i1);
		_indices.push_back(i2);
	}
}



void MeshLoaderB3D::ReadANIM(){
	/*int flags=*/_file->ReadInt();
	int frames = _file->ReadInt();
	/*float fps = */_file->ReadFloat();
	_totalFrame = (float)frames;
}

void MeshLoaderB3D::ReadKEY()
{
	int flags = _file->ReadInt();
	while( CheckSize() ){
		int frame = _file->ReadInt();
		if (flags & 1){
			vec3 v = _file->ReadVec3();
			PositionKey k = { v, frame };
			_readJoint->positionKeys.push_back(k);
		}
		if( flags & 2 ){
			vec3 v = _file->ReadVec3();
			ScaleKey k = { v, frame };
			_readJoint->scaleKeys.push_back(k);
		}
		if( flags & 4 ){
			quat r = _file->ReadQuat();
			RotationKey k = { r, frame };
			_readJoint->rotationKeys.push_back(k);
		}
	}
}