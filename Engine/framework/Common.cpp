#include "Common.h"
#include "../sys/sys_public.h"
#include "../renderer/RenderSystem.h"
#include "../common/str.h"
#include "../Game.h"
#include "../ResourceSystem.h"

const char* GAME_NAME = "null";

bool bShowFps = true;

void Com_Init() 
{
	glimpParms_t pram;
	pram.width = 800;
	pram.height = 600;
	pram.displayHz = 1/60;
	pram.stereo = 1/60;

	renderSys = new RenderSystemLocal(&pram);
	renderSys->Init();

	game = new GameLocal();
	game->Init();

	Sys_Printf("common_init");
}

void Com_Frame()
{
	game->Frame();
	renderSys->FrameUpdate();

	// report timing information
	if ( bShowFps ) {
		static int	lastTime;
		int		nowTime = Sys_Milliseconds();
		int		com_frameMsec = nowTime - lastTime;
		lastTime = nowTime;
		char buff[255];
		sprintf( buff, "FPS: %d", com_frameMsec );

		renderSys->DrawString(buff);
	}	
}

void Com_Quit()
{
	Sys_Quit();
}
