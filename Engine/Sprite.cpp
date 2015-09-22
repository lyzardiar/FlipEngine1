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

}

void Sprite::setLabel( const char* label )
{
	//1 3
	//0 2
	_texture = _resourceSys->AddText(label);
	
	int w = _texture->_pixelsWide;
	int h = _texture->_pixelsHigh;
	_positions.push_back(vec3(0, 0, 0));
	_positions.push_back(vec3(0, h, 0));
	_positions.push_back(vec3(w, 0, 0));
	_positions.push_back(vec3(w, h, 0));

	_texCoords.push_back(vec2(0, 0));
	_texCoords.push_back(vec2(0, 1));
	_texCoords.push_back(vec2(1, 0));
	_texCoords.push_back(vec2(1, 1));

	_indices.push_back(0);
	_indices.push_back(1);
	_indices.push_back(2);
	_indices.push_back(2);
	_indices.push_back(1);
	_indices.push_back(3);
}

void Sprite::setPosition( int x, int y )
{

}
