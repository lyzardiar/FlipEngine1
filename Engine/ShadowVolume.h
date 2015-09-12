#pragma once
#include "vec3.h"
#include "array.h"

class ShadowVolume
{
public:
	ShadowVolume(vec3* vertices, unsigned short* indices, unsigned int faceCount);
	~ShadowVolume();
	
	void createVolumeShadow();
	int  createEdgesAndCaps(vec3 lightPosition, array<vec3>* svp);
	void createSideFace();
	void calculateAdjacency();

	vec3 mCapsVertices;
	vec3 mEdgeVertices;


	array<vec3> mShadowVolume; 
	array<unsigned short> mEdges;
	bool*			mIsFrontFace;
	int*			mAdjacency;
	vec3*			mVertices;
	unsigned short* mIndices;
	unsigned int	mFaceCount;
};

