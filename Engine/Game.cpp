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
	mesh->SetPosition(0, 0, 660);
	//Pipeline* pileline1 = renderSys->GetPipeline(1);
	//pileline1->AddMesh(mesh);
	//renderSys->GetPipeline(2)->AddMesh(mesh);
}

void GameLocal::Frame()
{

}

