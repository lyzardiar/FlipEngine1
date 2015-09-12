#pragma once
#include "vec3.h"
class FPSCameraController
{
public:
	FPSCameraController();
	~FPSCameraController();

	void doCommand();

	vec3 mPosition;
	vec3 mLookAt;
};

