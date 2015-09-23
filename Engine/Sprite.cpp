#include "Sprite.h"
#include "renderer/RenderSystem.h"
#include "Texture.h"

Sprite::Sprite() : _width(0),
				_height(0)
{
}

Sprite::~Sprite()
{

}

void Sprite::setTexture( const char* imgPath )
{
	_texture = _resourceSys->AddTexture(imgPath);
	updateVertex();
}

void Sprite::setLabel( const char* label )
{
	_texture = _resourceSys->AddText(label);
	updateVertex();
}

void Sprite::setPosition( int x, int y )
{
	x;
	y;
}

void Sprite::updateVertex()
{
	//1 3
	//0 2
	float w = (float)_texture->_pixelsWide;
	float h = (float)_texture->_pixelsHigh;
	_positions.push_back(vec3(0.f, 0.f, 0.f));
	_positions.push_back(vec3(0.f, h, 0.f));
	_positions.push_back(vec3(w, 0.f, 0.f));
	_positions.push_back(vec3(w, h, 0.f));

	_texCoords.push_back(vec2(0.f, 0.f));
	_texCoords.push_back(vec2(0.f, 1.f));
	_texCoords.push_back(vec2(1.f, 0.f));
	_texCoords.push_back(vec2(1.f, 1.f));

	_indices.push_back(0);
	_indices.push_back(1);
	_indices.push_back(2);
	_indices.push_back(2);
	_indices.push_back(1);
	_indices.push_back(3);
}
