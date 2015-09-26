#ifndef __MAT4_H__
#define __MAT4_H__
#include <memory.h>
#include <math.h>
#include "vec3.h"
/*
		|0  4  8 12|
  m =   |1  5  9 13|
        |2  6 10 14|
		|3  7 11 15|
*/

class mat4
{
public:
	mat4() { makeIdentity(); }
	~mat4() {}

	inline mat4 operator*(const mat4 mat);

	void makeIdentity();
	void buildPerspectiveProjection(float fieldOfViewRadians, float aspectRatio, float zNear, float zFar);
	void transformVec3(float& x, float& y, float& z);
	void setScale(float x, float y, float z);
	void multiplyWith1x4Matrix(float *mat);

	mat4 inverse();
	inline void buildTranslate(const vec3& v);
	inline mat4 buildLookAt(vec3 position, vec3 target, vec3 up);
	inline void buildScale(float x, float y, float z);
public:
	float m[16];
};

inline void mat4::buildTranslate(const vec3& v)
{
	m[12] = v.x;
	m[13] = v.y;
	m[14] = v.z;
}

inline void mat4::buildScale(float x, float y, float z)
{
	makeIdentity();
	m[0] = x; m[5] = y; m[10] = z;
}

inline mat4 mat4::operator*(const mat4 mat)
{
	mat4 r;
	const float* m1 = mat.m;
	r.m[0] = m[0] * m1[0] + m[4] * m1[1] + m[8] * m1[2] + m[12] * m1[3];
	r.m[1] = m[1] * m1[0] + m[5] * m1[1] + m[9] * m1[2] + m[13] * m1[3];
	r.m[2] = m[2] * m1[0] + m[6] * m1[1] + m[10] * m1[2] + m[14] * m1[3];
	r.m[3] = m[3] * m1[0] + m[7] * m1[1] + m[11] * m1[2] + m[15] * m1[3];
	r.m[4] = m[0] * m1[4] + m[4] * m1[5] + m[8] * m1[6] + m[12] * m1[7];
	r.m[5] = m[1] * m1[4] + m[5] * m1[5] + m[9] * m1[6] + m[13] * m1[7];
	r.m[6] = m[2] * m1[4] + m[6] * m1[5] + m[10] * m1[6] + m[14] * m1[7];
	r.m[7] = m[3] * m1[4] + m[7] * m1[5] + m[11] * m1[6] + m[15] * m1[7];
	r.m[8] = m[0] * m1[8] + m[4] * m1[9] + m[8] * m1[10] + m[12] * m1[11];
	r.m[9] = m[1] * m1[8] + m[5] * m1[9] + m[9] * m1[10] + m[13] * m1[11];
	r.m[10] = m[2] * m1[8] + m[6] * m1[9] + m[10] * m1[10] + m[14] * m1[11];
	r.m[11] = m[3] * m1[8] + m[7] * m1[9] + m[11] * m1[10] + m[15] * m1[11];
	r.m[12] = m[0] * m1[12] + m[4] * m1[13] + m[8] * m1[14] + m[12] * m1[15];
	r.m[13] = m[1] * m1[12] + m[5] * m1[13] + m[9] * m1[14] + m[13] * m1[15];
	r.m[14] = m[2] * m1[12] + m[6] * m1[13] + m[10] * m1[14] + m[14] * m1[15];
	r.m[15] = m[3] * m1[12] + m[7] * m1[13] + m[11] * m1[14] + m[15] * m1[15];

	return r;
}

inline mat4 mat4::buildLookAt(vec3 position, vec3 target, vec3 up)
{
	// right hand  opengl
	vec3 zaxis = position - target;
	// left hand
	// vec3 zaxis = target - position;
	zaxis.normalize();

	vec3 xaxis = up.cross(zaxis);
	xaxis.normalize();

	vec3 yaxis = zaxis.cross(xaxis);

	m[0] = xaxis.x;
	m[1] = yaxis.x;
	m[2] = zaxis.x;
	m[3] = 0;

	m[4] = xaxis.y;
	m[5] = yaxis.y;
	m[6] = zaxis.y;
	m[7] = 0;

	m[8] = xaxis.z;
	m[9] = yaxis.z;
	m[10] = zaxis.z;
	m[11] = 0;

	m[12] = -xaxis.dot(position);
	m[13] = -yaxis.dot(position);
	m[14] = -zaxis.dot(position);
	m[15] = 1;

	return *this;
}

#endif
