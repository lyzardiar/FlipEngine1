#include "Sprite.h"
#include "renderer/RenderSystem.h"
#include "Texture.h"
#include "DrawVert.h"

Sprite::Sprite() : _width(0),
				_height(0)
{
	_drawSurf = new drawSurf_t;

	float w = 1;
	float h = 1;

	srfTriangles_t* tri = _drawSurf->geo;
	tri = new srfTriangles_t;
	tri->verts = new DrawVert[4];
	tri->verts[0].xyz = vec3(0.f, 0.f, 0.f);
	tri->verts[1].xyz = vec3(0.f, h, 0.f);
	tri->verts[2].xyz = vec3(w, 0.f, 0.f);
	tri->verts[3].xyz = vec3(w, h, 0.f);

	tri->verts[0].uv = vec2(0.f, 0.f);
	tri->verts[1].uv = vec2(0.f, 1.f);
	tri->verts[2].uv = vec2(1.f, 0.f);
	tri->verts[3].uv = vec2(1.f, 1.f);

	tri->numIndexes = 6;
	tri->indexes = new glIndex_t[6];
	tri->indexes[0] = 0;
	tri->indexes[1] = 1;
	tri->indexes[2] = 2;
	tri->indexes[3] = 2;
	tri->indexes[4] = 1;
	tri->indexes[5] = 3;

	SetupVBO();
}

Sprite::~Sprite()
{

}

void Sprite::SetTexture( const char* imgPath )
{
	_drawSurf->tex = resourceSys->AddTexture(imgPath);
	UpdateVertex();
}

void Sprite::SetLabel( const char* label )
{
	_drawSurf->tex = resourceSys->AddText(label);
	UpdateVertex();
}



void Sprite::UpdateVertex()
{
	//1 3
	//0 2
	float w = (float)_texture->_pixelsWide;
	float h = (float)_texture->_pixelsHigh;

	srfTriangles_t* tri = _drawSurf->geo;
	tri->verts[1].xyz.y = h;
	tri->verts[2].xyz.x = w;
	tri->verts[3].xyz = vec3(w, h, 0.f);
}


void Sprite::SetupVBO()
{
	srfTriangles_t* tri = _drawSurf->geo;
	// This functions copies the vertex and index buffers into their respective VBO's
	glGenBuffers(1, &tri->vbo[0]);
	glGenBuffers(1, &tri->vbo[1]);

	// Stick the data for the vertices into its VBO
	glBindBuffer(GL_ARRAY_BUFFER, tri->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(DrawVert) * tri->numVerts, tri->verts, GL_STATIC_DRAW);

	// Stick the data for the indices into its VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri->vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * tri->numIndexes, tri->indexes, GL_STATIC_DRAW);

	// Clear the VBO state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}