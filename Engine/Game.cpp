#include "Game.h"
#include "Mesh.h"
#include "ResourceSystem.h"
#include "renderer/RenderSystem.h"
#include "Pipeline.h"
#include "../media/KnightModel.h"
#include "r_public.h"
#include "sys/sys_public.h"

Game* game = NULL;

GameLocal::GameLocal( void )
{
}

GameLocal::~GameLocal()
{

}

void GameLocal::Init()
{
	//StaticModel* model = resourceSys->AddMesh("ninja.b3d");
	//renderSys->AddStaticModel(model);

		// _renderbuffer init
	mat4 matPerspective, matView;
	matPerspective.buildPerspectiveProjection(3.1415926535898f / 3, 800.f / 600, 0.1f, 800.f);
	matView.buildLookAt(vec3(0, 0,  300.f / tanf(3.1415936f/6.f)), vec3(0, 0, 0), vec3(0, 1, 0));

	drawSurf_t* drawSur = R_AllocDrawSurf();
	drawSur->geo = R_AllocStaticTriSurf();
	srfTriangles_t* tri = drawSur->geo;
	tri->numVerts = KnightModel::numVertices;
	tri->numIndexes = KnightModel::numIndices;
	R_AllocStaticTriSurfVerts(tri, tri->numVerts);

	for (int i = 0; i < KnightModel::numVertices; i++)
	{
		memcpy(&(tri->verts[i].xyz), KnightModel::vertices[i].position, sizeof(float) * 3);
		memcpy(&(tri->verts[i].uv), KnightModel::vertices[i].uv, sizeof(float) * 3);
		memcpy(&(tri->verts[i].normal), KnightModel::vertices[i].normal, sizeof(float) * 3);
	}
	tri->indexes = new glIndex_t[KnightModel::numIndices];
	memcpy(tri->indexes, KnightModel::indices, tri->numIndexes*sizeof(glIndex_t));

	R_GenerateGeometryVbo(tri);
	renderSys->AddDrawSur(drawSur);
}

void GameLocal::Frame()
{
	sysEvent_t ev = Sys_GetEvent();
	while (ev.evType != SE_NONE )
	{

		ev = Sys_GetEvent();
	}

}

