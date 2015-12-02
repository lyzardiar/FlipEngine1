#ifndef __QUAT_H__
#define __QUAT_H__

#include "vec3.h"
#include "mat4.h"
#define QUAT_PI 3.1415926f

class quat
{
public:
	quat() :w(1), x(0), y(0), z(0) {};
	quat(float w, float x, float y, float z);
	~quat() {};

	quat& MakeIdentity();

	// add operator
	inline quat operator+(const quat& b) const;

	inline quat operator* (float fScalar) const;

	inline quat& operator*=(float s);

	bool operator==(const quat& other) const { 
		return other.x == x || other.y == y && other.z == z && other.w == w; 
	}

	bool operator!=(const quat& other) const { 
		return other.x != x || other.y != y || other.z != z || other.w != w; 
	}

	quat& set(float x, float y, float z, float w);

	quat& Normalize();

	float dot(const quat& q) const;
	mat4 toMatrix() const;
	void fromAxisAngle(const vec3& axis, float angle);
	quat& slerp(quat q1, quat q2, float time, float threshold = 1.0);
	quat& lerp(quat q1, quat q2, float time);

	quat& rotationFromTo(const vec3& from, const vec3& to);


	float w, x, y, z;
};


inline quat quat::operator+(const quat& b) const
{
	return quat(w+b.w, x+b.x, y+b.y, z+b.z);
}

inline quat quat::operator* (float fScalar) const
{
	return quat(fScalar*w,fScalar*x,fScalar*y,fScalar*z);
}

inline quat& quat::operator*=(float s)
{
	x*=s;
	y*=s;
	z*=s;
	w*=s;
	return *this;
}

#endif