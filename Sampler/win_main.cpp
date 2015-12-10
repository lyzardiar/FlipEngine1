#include "sys/win32/win_local.h"
#include "sys/sys_public.h"
#include "framework/KeyInput.h"


#include "Material.h"
#include "ShaderSource.h"
#include "File.h"
#include "sampler.h"

Win32Vars_t win32;
/*
==================
WinMain
==================
*/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) 
{
	//const char* buffer = "width = 5";
	const HCURSOR hcurSave = ::SetCursor( LoadCursor( 0, IDC_WAIT ) );

	//Sys_SetPhysicalWorkMemory( 192 << 20, 1024 << 20 );
	//Sys_GetCurrentMemoryStatus( exeLaunchMemoryStats );
//
//	// done before Com/Sys_Init since we need this for error output
	Sys_CreateConsole();
	Sys_ShowConsole( 1, true );

	Sys_Init();

	glimpParms_t pram;
	pram.width = 800;
	pram.height = 600;
	pram.displayHz = 1/60;
	pram.stereo = 1/60;

	GL_CreateDevice(&pram);
	ShadowSampler game;
	game.Init();

//	::SetFocus( win32.hWnd );

    // main game loop
	while( 1 ) 
	{
		Sys_PumpEvents();
		game.Frame();
	}

	return 0;
}
