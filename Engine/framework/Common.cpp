#include "Common.h"
#include "../sys/sys_public.h"
#include "../renderer/RenderSystem.h"
#include "../common/Str.h"

const char* GAME_NAME = "null";

typedef struct CommonLocal
{
	RenderSystem *renderSystem;

}commonLocal_t;

commonLocal_t common;

void Common_Init() 
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

void Common_Frame()
{
	common.renderSystem->FrameUpdate();
}

#define MAX_STRING_SIZE 4096
void Common_Printf(const char* fmt, ...)
{
	char msg[MAX_STRING_SIZE];
	va_list argptr;
	va_start( argptr, fmt );
	idStr::vsnPrintf(msg, MAX_STRING_SIZE-1, fmt, argptr);
	va_end( argptr );

	Sys_Printf( msg );
}

void Common_Error(const char* fmt, ...)
{

}

void Common_Quit()
{
	Sys_Quit();
}
