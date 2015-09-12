#pragma once
#include "gl/glew.h"

class  Image;

class Texture
{
public:
	Texture() {}
	virtual ~Texture() {}
	
	bool init(Image* i);

	bool init(int w, int h, void* data);

	void begin();

	void end();

	GLuint getName();


    int _pixelsWide;

    int _pixelsHigh;

private:
    GLuint _name;

    /** texture max S */
    GLfloat _maxS;
    
    GLfloat _maxT;

    bool _hasPremultipliedAlpha;

    bool _hasMipmaps;

    bool _antialiasEnabled;

};