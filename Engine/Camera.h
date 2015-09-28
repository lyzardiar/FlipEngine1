#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "common/mat4.h"
#include "common/vec3.h"

class Camera  
{
public:
	Camera();
	virtual ~Camera();
	void Setup3DCamera();
	mat4* GetViewProj();

	vec3 getPosition();
	void Walk(float displacement);

private:
	mat4 _matView;
	mat4 _matProj;
	vec3 _at;					
	vec3 _pos;								
	vec3 _look;
	vec3 _dir;
	mat4 _VP;
};

#endif // !defined(AFX_CAMERA_H__48B7BC27_B223_47B1_ABD4_C17EFD0A7401__INCLUDED_)
