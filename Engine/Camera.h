#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "common/mat4.h"
#include "common/vec3.h"

class Camera  
{
public:
	Camera();
	virtual ~Camera();
	void Setup3DCamera(int width, int height);
	void Setup2DCamera(float width, float height);
	
	mat4* GetProj();
	mat4* GetView();
	mat4* GetViewProj();
	vec3 GetPosition();
	
	void Walk(float displacement);
	void Yaw(float angle);
	void Rise(float displacement);
	void SetPosition(float x, float y, float z);
	void RotateByAxis(vec3 axis, float angle);

private:
	mat4 _matView;
	mat4 _matProj;
	vec3 _at;					
	vec3 _pos;								
	mat4 _matViewProj;
	mat4 _matBillboard;
};

#endif // !defined(AFX_CAMERA_H__48B7BC27_B223_47B1_ABD4_C17EFD0A7401__INCLUDED_)
