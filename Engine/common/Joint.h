#ifndef __JOINT_H__
#define __JOINT_H__

#include "precompiled.h"
#include "vec3.h"
#include "mat4.h"
#include "quat.h"
#include "str.h"
#include "array.h"


struct PositionKey
{
	vec3 position;
	float frame;
};

struct ScaleKey
{
	vec3 position;
	float frame;
};

struct RotationKey
{
	quat rotation;
	float frame;
};

class Joint
{
public:
	Joint();
	~Joint();

	lfStr name;
	vec3 position;
	vec3 scale;
	quat rotation;
	array<Joint*> children;
	Joint* parent;

	array<PositionKey> positionKeys;
	array<RotationKey> rotationKeys;
	array<ScaleKey>    scaleKeys;
	array<int>         vertexIndices;
	array<float>       vertexWeights;

	mat4 globalAnimatedMatrix;
	mat4 globalInvMatrix;

	mat4 globalPosition;

	void GetPositionKeyFramesAtFrame(float frame, PositionKey* preKey, PositionKey* nextKey);
	void GetScaleKeyFramesAtFrame(float frame, ScaleKey* preKey, ScaleKey* nextKey);
	void GetRotationKeyFramesAtFrame(float frame, RotationKey* preKey, RotationKey* nextKey);
	void GetFrame(float frame, vec3& position, quat& rotation);
};

#endif
