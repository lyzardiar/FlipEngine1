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

	Sys_Printf("Initializing Game\n");
	game->Init();

}

void Com_Frame()
{
	game->Frame();
	renderSys->FrameUpdate();

	// report timing information
	if ( bShowFps ) {
		static int	lastTime = 0;
		static int	frames = 0;
		
		++frames;
		int nowTime = Sys_Milliseconds();

		//If a second has passed
		if(nowTime - lastTime >= 1000)
		{
			float fps		= frames * 1000.f/(nowTime - lastTime);	//update the number of frames per second
			lastTime	= nowTime;						//set time for the start of the next count
			frames		=0;								//reset fps for this second
			
			char buff[255];
			sprintf( buff, "FPS: %.02f, run %d", fps, nowTime );
			renderSys->DrawString(buff);
		}
	}	
}

void Com_Quit()
{
	Sys_Quit();
}
