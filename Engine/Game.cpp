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
	StaticModel* model = resourceSys->AddMesh("ninja.b3d");
	renderSys->AddStaticModel(model);
}

void GameLocal::Frame()
{

}

