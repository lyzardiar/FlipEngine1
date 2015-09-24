#ifndef __SPRITE_H__
#define __SPRITE_H__
#include "Mesh.h"
#include "vec2.h"
#include "RenderBuffer.h"

class Texture;


class Sprite : public Mesh
{
public:
	Sprite();
	~Sprite();


	void SetTexture(const char* imgPath);

	void SetLabel(const char* label);

	void SetupVBO();
private:
	void UpdateVertex();

private:
public:
	drawSurf_t* _drawSurf;
	int  _width;
	int  _height;
	vec2 _position;
};


#endif
