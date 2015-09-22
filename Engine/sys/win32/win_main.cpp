#include "../sys_public.h"

#include "../../framework/Common.h"
#include "../../framework/KeyInput.h"

#include "win_local.h"

Win32Vars_t	win32;

int Win32Vars_t::win_xpos = 100;
int Win32Vars_t::win_ypos = 100;
bool Win32Vars_t::win_outputDebugString = true;
bool Win32Vars_t::win_outputEditString = true;


static sysMemoryStats_t exeLaunchMemoryStats;


/*
==================
WinMain
==================
*/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

	const HCURSOR hcurSave = ::SetCursor( LoadCursor( 0, IDC_WAIT ) );

	//Sys_SetPhysicalWorkMemory( 192 << 20, 1024 << 20 );
	Sys_GetCurrentMemoryStatus( exeLaunchMemoryStats );

	win32.hInstance = hInstance;
//
//	// done before Com/Sys_Init since we need this for error output
	Sys_CreateConsole();
	Sys_ShowConsole( 1, true );

	win32.defaultFont = CreateFont(20, // nHeight 
        0, // nWidth 
        0, // nEscapement 
        0, // nOrientation 
        FW_THIN, // nWeight 
        FALSE, // bItalic ÇãÐ±
        FALSE, // bUnderline 
        0, // cStrikeOut 
        ANSI_CHARSET, // nCharSet 
        OUT_DEFAULT_PRECIS, // nOutPrecision 
        CLIP_DEFAULT_PRECIS, // nClipPrecision 
        DEFAULT_QUALITY, // nQuality 
        DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
        ("Arial")); // lpszFac

	Com_Init();

//	::SetFocus( win32.hWnd );

    // main game loop
	while( 1 ) {
		Sys_PumpEvents();

		Com_Frame();
	}

	return 0;
}
