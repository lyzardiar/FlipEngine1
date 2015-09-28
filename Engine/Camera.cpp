#include "Camera.h"

Camera::Camera() {
}

Camera::~Camera() {
}

void Camera::Walk(float displacement) {
	_pos += _dir * displacement;
	_at = _pos + _dir;
	_matView.buildLookAt(_pos, _at, vec3(0.f, 1.f, 0.f));
	_VP = _matProj * _matView;	
}

mat4* Camera::GetViewProj() {
	return &_VP;
}

void Camera::Setup3DCamera() {
	_matProj.buildPerspectiveProjection(3.1415926535898f / 3, 800.f / 600, 0.1f, 800.f);
	_dir.set(0.f, 0.f, -1.f);
	_at = _pos + _dir;
	_matView.buildLookAt(_pos, _at, vec3(0.f, 1.f, 0.f));
	_VP = _matProj * _matView;	
}

vec3 Camera::getPosition()
{
	return _pos;
}
