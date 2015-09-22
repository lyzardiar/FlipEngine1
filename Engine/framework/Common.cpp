#include "Common.h"
#include "../sys/sys_public.h"
#include "../renderer/RenderSystem.h"
#include "../common/str.h"

const char* GAME_NAME = "null";

typedef struct CommonLocal
{
	RenderSystem *renderSystem;

}commonLocal_t;

commonLocal_t common;

void Com_Init() 
{
	glimpParms_t pram;
	pram.width = 800;
	pram.height = 600;
	pram.displayHz = 1/60;
	pram.stereo = 1/60;

	common.renderSystem = new RenderSystemLocal(&pram);
	common.renderSystem->Init();


	Sys_Printf("common_init");
}

void Com_Frame()
{
	common.renderSystem->FrameUpdate();
}

void Com_Quit()
{
	Sys_Quit();
}
