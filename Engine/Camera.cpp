#include "Camera.h"
#include "common/quat.h"
#include "sys/sys_public.h"

Camera::Camera() {
}

Camera::~Camera() {
}

void Camera::Walk(float displacement) {
	vec3 dir = _at - _pos;
	_pos += dir * displacement;
	_at = _pos + dir;
	_matView.buildLookAt(_pos, _at, vec3(0.f, 1.f, 0.f));
	_VP = _matProj * _matView;	
}

mat4* Camera::GetViewProj() {
	return &_VP;
}

void Camera::Setup3DCamera() {
	_matProj.buildPerspectiveProjection(3.1415926535898f / 3, 800.f / 600, 0.1f, 800.f);
	_at.set(0.f, 0.f, -1.f);
	_matView.buildLookAt(_pos, _at, vec3(0.f, 1.f, 0.f));
	_VP = _matProj * _matView;	
}

vec3 Camera::GetPosition()
{
	return _pos;
}

void Camera::Yaw(float angle)
{
	vec3 dir = _at - _pos;
	quat q;
	q.fromAxisAngle(vec3(_pos.x, 1.0, _pos.z), angle * QUAT_PI / 360.f);
	q.toMatrix().transformVec3(dir.x, dir.y, dir.z);
	_at = _look + dir;

	//Sys_Printf("%f %f %f \n", _dir.x, _dir.y, _dir.z);

	//_at.rotatexzBy(angle, vec3(_pos.x, _pos.y, _pos.z));
	//Sys_Printf("%f %f %f \n", dir.x, dir.y, dir.z);

	_matView.buildLookAt(_pos, _at, vec3(0.f, 1.f, 0.f));
	_VP = _matProj * _matView;
}
