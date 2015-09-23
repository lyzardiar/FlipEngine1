#ifndef __SPRITE_H__
#define __SPRITE_H__
#include "Mesh.h"
#include "vec2.h"

class Texture;

class Sprite : public Mesh
{
public:
	Sprite();
	~Sprite();


	void SetTexture(const char* imgPath);

	void SetLabel(const char* label);


private:
	void UpdateVertex();

private:
	int  _width;
	int  _height;
	vec2 _position;
};


#endif
