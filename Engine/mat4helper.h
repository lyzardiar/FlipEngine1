#ifndef __MA4HELPER_H__
#define __MA4HELPER_H__

#include <math.h>
#include "mat4.h"
#include "vec3.h"

inline mat4 matTrans(mat4& mat, float x, float y, float z)
{
	mat4 r;
	r.buildTranslate(vec3(x, y, z));
	return mat*r;
}

inline mat4 build2DCamera(int width, int height, mat4& perspectiveMatrix, mat4& lookatMatrix)
{
	perspectiveMatrix.buildPerspectiveProjection(QUAT_PI / 3, (float)width / height, 1, 100);
	lookatMatrix.buildLookAt(vec3(0, 0, (float)width/tanf(QUAT_PI/6)), vec3(0, 0, 0), vec3(0, 1.0, 0));
}

#endif