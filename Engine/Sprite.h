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


	void setTexture(const char* imgPath);

	void setLabel(const char* label);

	void setPosition(int x, int y);

private:
	int  _width;
	int  _height;
	vec2 _position;
};


#endif
