#include "Texture.h"
#include "Image.h"


bool Texture::Init(Image* i)
{
	if (i== nullptr)
		return false;

	glGenTextures(1, &_name);
	glBindTexture(GL_TEXTURE_2D, _name);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	_pixelsWide = i->_width;
	_pixelsHigh = i->_height;
	int w = _pixelsWide;
	int h = _pixelsHigh;
	for (int l=0; l<i->GetMipLevels(); ++l)
	{
		if (i->IsCompressed())
		{
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, l, i->_internalFormat, (GLsizei)w, (GLsizei)h, 0, 
				i->_format, i->_type, i->GetLevel(l));
			w >>= 1;
			h >>= 1;
			w = w ? w : 1;
			h = h ? h : 1;
		}
	}
	return true;
}

bool Texture::Init(int w, int h, void* data)
{
	glGenTextures(1, &_name);
	glBindTexture(GL_TEXTURE_2D, _name);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	_pixelsWide = w;
	_pixelsHigh = h;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	return true;
}

void Texture::Begin()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _name);
}

void Texture::End()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::GetName()
{
	return _name;
}


