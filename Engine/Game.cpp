#include "Game.h"
#include "Mesh.h"
#include "ResourceSystem.h"
#include "renderer/RenderSystem.h"
#include "Pipeline.h"
#include "../media/KnightModel.h"
#include "r_public.h"

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

}

