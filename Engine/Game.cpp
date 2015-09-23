#include "Game.h"
#include "Mesh.h"
#include "ResourceSystem.h"
#include "renderer/RenderSystem.h"
#include "Pipeline.h"

Game* game = NULL;

GameLocal::GameLocal( void )
{
}

GameLocal::~GameLocal()
{

}

void GameLocal::Init()
{
	Mesh* mesh = resourceSys->AddMesh("ninja.b3d");
	Pipeline* pileline0 = renderSys->GetPipeline(0);

	Pipeline* pileline1 = renderSys->GetPipeline(1);
	pileline1->AddMesh(mesh);
	//_meshs.push_back(mesh);
}

void GameLocal::Frame()
{

}

