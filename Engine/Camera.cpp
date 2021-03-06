#include "Camera.h"
#include "common/quat.h"

#include "sys/sys_public.h"

Camera::Camera() {
}

Camera::~Camera() {
}

void Camera::Forward(float displacement) {
	vec3 dir = _at - _pos;
	dir.normalize();
	_pos += dir * displacement;
	_matView.buildLookAt(_pos, _at, vec3(0.f, 1.f, 0.f));
	_matViewProj = _matProj * _matView;	
}

mat4* Camera::GetViewProj() {
	return &_matViewProj;
}

void Camera::Setup3DCamera(int width, int height) {
	_matProj.buildPerspectiveProjection(3.1415926535898f / 3, float(width)/(height), 0.1f, 800.f);
	_at.set(0.f, 0.f, -1.f);
	_matView.buildLookAt(_pos, _at, vec3(0.f, 1.f, 0.f));
	_matViewProj = _matProj * _matView;	
}

void Camera::Setup2DCamera(float width, float height) {
	_matProj.BuildProjectionOrthoRH(width, height, 0.1f, 800.f);
	_matView.m[12] = -width/2;
	_matView.m[13] = -height/2;
	_matView.m[14] = -((float)height/2.f) / tanf(3.1415936f/6.f);
	_matViewProj = _matProj * _matView;	
	//float zfar = -300.f / tanf(3.1415936f/6.f);
}

vec3 Camera::GetPosition() {
	return _pos;
}

void Camera::Rise( float displacement ) {
	vec3 dir = _at - _pos;
	_pos.y += displacement;
	_at = _pos + dir;
	_matView.buildLookAt(_pos, _at, vec3(0.f, 1.f, 0.f));
	_matViewProj = _matProj * _matView;	
}

void Camera::SetPosition( float x, float y, float z ) {
	_pos.set(x, y, z);
	_matView.buildLookAt(_pos, _at, vec3(0.f, 1.f, 0.f));
	_matViewProj = _matProj * _matView;	
}

mat4* Camera::GetProj() {
	return &_matProj;
}

mat4* Camera::GetView() {
	return &_matView;
}

void Camera::RotateByAxis( vec3 axis, float angle ) {
	quat q;
	q.fromAxisAngle(axis, angle * QUAT_PI / 360.f);
	q.toMatrix().transformVec3(_pos.x, _pos.y, _pos.z);
	_matView.buildLookAt(_pos, _at, vec3(0.f, 1.f, 0.f));
	_matViewProj = _matProj * _matView;	
}

void Camera::Yaw(float angle) {
	vec3 dir = _at - _pos;
	quat q;
	q.fromAxisAngle(vec3(0.f, 1.0f, 0.f), angle * QUAT_PI / 360.f);
	q.toMatrix().transformVec3(dir.x, dir.y, dir.z);
	_at = _pos + dir;

	//Sys_Printf("%f %f %f \n", _dir.x, _dir.y, _dir.z);

	//_at.rotatexzBy(angle, vec3(_pos.x, _pos.y, _pos.z));
	//Sys_Printf("%f %f %f \n", dir.x, dir.y, dir.z);

	_matView.buildLookAt(_pos, _at, vec3(0.f, 1.f, 0.f));
	_matViewProj = _matProj * _matView;
}

void Camera::Right( float displacement )
{
	vec3 dir = _at - _pos;
	vec3 right = vec3(dir.x, dir.y, dir.z);
	right.normalize();
	right.rotatexzBy(-90, vec3(0, 0, 0));
	_pos += right * displacement;
	_at = _pos + dir;

	_matView.buildLookAt(_pos, _at, vec3(0.f, 1.f, 0.f));
	_matViewProj = _matProj * _matView;
}
