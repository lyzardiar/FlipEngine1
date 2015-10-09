#include "aabb3d.h"


aabb3d::aabb3d()
{

}

aabb3d::~aabb3d()
{

}

bool aabb3d::AddPoint(const vec3& v)
{
	bool expanded = false;
	if ( v.x < _min.x) {
		_min.x = v.x;
		expanded = true;
	}
	if ( v.y < _min.y) {
		_min.y = v.y;
		expanded = true;
	}
	if ( v.z < _min.z ) {
		_min.z = v.z;
		expanded = true;
	}
	if ( v.x > _max.x) {
		_max.x = v.x;
		expanded = true;
	}
	if ( v.y > _max.y ) {
		_max.y = v.y;
		expanded = true;
	}
	if ( v.z > _max.z) {
		_max.z = v.z;
		expanded = true;
	}
	return expanded;
}
