#include "Sprite.h"
#include "renderer/RenderSystem.h"
#include "Texture.h"
#include "DrawVert.h"
#include "glutils.h"

Sprite::Sprite() : _width(0),
				_height(0)
{
	_drawSurf = new drawSurf_t;

	float w = 1;
	float h = 1;

	_drawSurf->geo = R_AllocStaticTriSurf();
	R_GenerateQuad(_drawSurf->geo);
	_drawSurf->geo->verts[0].xyz = vec3(0.f, 0.f, 0.f);
	_drawSurf->geo->verts[1].xyz = vec3(0.f, h, 0.f);
	_drawSurf->geo->verts[2].xyz = vec3(w, 0.f, 0.f);
	_drawSurf->geo->verts[3].xyz = vec3(w, h, 0.f);

	_drawSurf->material = R_AllocMaterail();

	SetupVBO();
}

Sprite::~Sprite()
{

}

void Sprite::SetTexture( const char* imgPath )
{
	_drawSurf->material->tex = resourceSys->AddTexture(imgPath);
	UpdateVertex();
}

void Sprite::SetLabel( const char* label )
{
	if (_drawSurf->material->tex != NULL )
	{
		GLuint tex = _drawSurf->material->tex->GetName();
		glDeleteTextures(1, &tex );
	}
	_drawSurf->material->tex = resourceSys->AddText(label);
	UpdateVertex();
}

void Sprite::UpdateVertex()
{
	//1 3
	//0 2
	Texture* texture = _drawSurf->material->tex;
	float w = (float)texture->_pixelsWide;
	float h = (float)texture->_pixelsHigh;

	srfTriangles_t* tri = _drawSurf->geo;
	tri->verts[1].xyz.y = h;
	tri->verts[2].xyz.x = w;
	tri->verts[3].xyz = vec3(w, h, 0.f);

	SetupVBO();
}


void Sprite::SetupVBO()
{
	R_GenerateGeometryVbo(_drawSurf->geo);
}

void Sprite::SetPosition(float x, float y, float z)
{
	_drawSurf->matModel.buildTranslate(x, y, z);
}