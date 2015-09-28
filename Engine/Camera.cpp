#include "Camera.h"

Camera::Camera() {
}

Camera::~Camera() {
}

void Camera::Walk(float fUnits) {
	D3DXVec3Normalize(&_dir, &_dir);   
	_pos += _dir * fUnits;
	_at.x = _look.x + _pos.x;
	_at.z = _look.z + _pos.z;
}

void Camera::MoveOnX(float fUnits) {
	D3DXVec3Normalize(&_look,&_look);   
	_pos+=vec3(_look.z,0.f,-_look.x)*fUnits;//fUnits为正数 往右移动 为负数往左移动
}


void Camera::GetCameraLo(vec3 *look) {
	*look=_look;
}

void Camera::GetCameraPos(vec3* Pos) {
	*Pos=_pos;
}

void Camera::GetCameraAt(vec3 *at) {
	*at=_at;
}

void Camera::GetCameraDir(vec3 *dir) {
	*dir=_dir;
}

void Camera::SetPos(vec3 vPos) {
	D3DXVec3Normalize(&_look,&_look);
    _pos=vPos+vec3(0.f,36,0)-255.0f*_look;
	_at=_pos+_look;
}

void Camera::SetCameraRotationX(float fAngle)
{
    D3DXVec3Normalize(&_look,&_look);
    if ((_look.y<0.12&&_look.y>-0.8)||(_look.y>0.12&&fAngle>0)||(_look.y<-0.8&&fAngle<0))
    {
		vec3 vOldLook=_look;
		float fRad=0.f,fNewRad;
		fRad=sqrt(_look.x*_look.x+_look.z*_look.z);
		fNewRad=fRad*cos(fAngle)+vOldLook.y*sin(fAngle);
		_look.y=vOldLook.y*cos(fAngle)-fRad*sin(fAngle);
        _look.x=vOldLook.x*fNewRad/fRad;
		_look.z=vOldLook.z*fNewRad/fRad;
		_pos+=-255.f*(_look-vOldLook);
		_at=_look+_pos;
		_dir=vec3(_look.x,0.f,_look.z);

// 		D3DXVec3Normalize(&_look,&_look);
// 		D3DXVec3Normalize(&vOldLook,&vOldLook);
// 		_pos.y+=-255.f*(_look.y-vOldLook.y);
// 		_pos.z+=-255.f*(_look.z-vOldLook.z);
// 		_at=_look+_pos;
// 		_dir=D3DXVECTOR3(_look.x,0.f,_look.z);
    }



}
void Camera::SetCameraRotationY(float fAngle)
{
	vec3 vOldLook=_look;
	float fRad=0.f;
	fRad=sqrt(_look.x*_look.x+_look.z*_look.z);
    _look.x=vOldLook.x*cos(fAngle)+vOldLook.z*sin(fAngle);
	_look.z=vOldLook.z*cos(fAngle)-vOldLook.x*sin(fAngle);
	D3DXVec3Normalize(&_look,&_look);
	D3DXVec3Normalize(&vOldLook,&vOldLook);
	_pos+=-255.f*(_look-vOldLook);
//     _pos.x+=-250*(_look.x-vOldLook.x)/fRad;
// 	_pos.z+=-250*(_look.z-vOldLook.z)/fRad;
	_at=_look+_pos;
	_dir=vec3(_look.x,0.f,_look.z);
}

mat4* Camera::GetViewProj()
{
	return &_VP;
}
