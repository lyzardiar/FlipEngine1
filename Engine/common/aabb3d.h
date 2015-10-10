#ifndef __AABB3D_H__
#define __AABB3D_H__

#include "vec3.h"

class aabb3d
{
public:
	aabb3d();
	~aabb3d();

	bool AddPoint(const vec3& v);

	bool RayIntersection( const vec3 &start, const vec3 &dir, float &scale ) const;
public:
	vec3 _min;
	vec3 _max;
};


#endif



