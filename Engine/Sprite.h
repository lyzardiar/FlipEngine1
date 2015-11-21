#ifndef __SPRITE_H__
#define __SPRITE_H__
#include "Mesh.h"
#include "common/vec2.h"
#include "r_public.h"

class Texture;


class Sprite
{
public:
	Sprite();
	~Sprite();

	void Init();

	void SetTexture(const char* imgPath);

	void SetLabel(const char* label);

	void SetupVBO();

	void SetPosition(float x, float y, float z);

	vec3 GetPosition();

	// billboard
	void LookAtView(mat4* view);

	vec2 ToScreenCoord(mat4& viewProj);

private:
	void UpdateVertex();

private:
public:
	drawSurf_t* _drawSurf;
	int  _width;
	int  _height;
	vec3 _position;
};


#endif
