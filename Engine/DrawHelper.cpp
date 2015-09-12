#include "DrawHelper.h"
#include "ImageLoaderJPG.h"
#include "MeshLoaderB3D.h"
#include "ImageLoaderPNG.h"

#include "Image.h"

void draw3DCoordinate()
{
	float vertices[] = {0.f, 0.f, 0.f, 
						0.f, 1.f, 0.f,
						0.f, 0.f, 1.f,
						1.f, 0.f, 0.f
	};

	unsigned short indices[] = {0, 1, 0, 2, 0, 3};
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_SHORT, indices);
}


