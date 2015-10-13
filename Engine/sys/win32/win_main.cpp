#include "../sys_public.h"
#include "../../framework/Common.h"
#include "../../framework/KeyInput.h"

#include "win_local.h"


#include "Model_lwo.h"

Win32Vars_t	win32;


int Win32Vars_t::win_xpos = 0;
int Win32Vars_t::win_ypos = 0;
bool Win32Vars_t::win_outputDebugString = true;
bool Win32Vars_t::win_outputEditString = true;


static sysMemoryStats_t exeLaunchMemoryStats;


/*
==================
WinMain
==================
*/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {


	lwGetObject("../media/planet2.lwo");

	const HCURSOR hcurSave = ::SetCursor( LoadCursor( 0, IDC_WAIT ) );

	//Sys_SetPhysicalWorkMemory( 192 << 20, 1024 << 20 );
	Sys_GetCurrentMemoryStatus( exeLaunchMemoryStats );

	win32.hInstance = hInstance;
//
//	// done before Com/Sys_Init since we need this for error output
	Sys_CreateConsole();
	Sys_ShowConsole( 1, true );

	Sys_Init();
	Com_Init();

//	::SetFocus( win32.hWnd );

    // main game loop
	while( 1 ) 
	{
		Sys_PumpEvents();
		Com_Frame();
	}

	return 0;
}
