#ifndef __AABB3D_H__
#define __AABB3D_H__

#include "vec3.h"

class aabb3d
{
public:
	aabb3d();
	~aabb3d();

	bool AddPoint(const vec3& v);

public:
	vec3 _min;
	vec3 _max;
};


#endif



