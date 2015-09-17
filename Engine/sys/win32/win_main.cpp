#include "../sys_public.h"
#include "../../common/Str.h"

#include "../../framework/Common.h"
#include "../../framework/KeyInput.h"

#include "win_local.h"

Win32Vars_t	win32;

int Win32Vars_t::win_xpos = 100;
int Win32Vars_t::win_ypos = 100;
bool Win32Vars_t::win_outputDebugString = true;
bool Win32Vars_t::win_outputEditString = true;


static sysMemoryStats_t exeLaunchMemoryStats;

/* ============== Sys_Quit ============== */
void Sys_Quit( void ) {
	//timeEndPeriod( 1 );
	//Sys_ShutdownInput();
	Sys_DestroyConsole();
	ExitProcess( 0 );
}


#define MAXPRINTMSG 4096
void Sys_Printf( const char *fmt, ... ) {
	char		msg[MAXPRINTMSG];

	va_list argptr;
	va_start(argptr, fmt);
	idStr::vsnPrintf( msg, MAXPRINTMSG-1, fmt, argptr );
	va_end(argptr);
	msg[sizeof(msg)-1] = '\0';

	if ( win32.win_outputDebugString ) {
		OutputDebugString( msg );
	}
	if ( win32.win_outputEditString ) {
		Conbuf_AppendText( msg );
	}
}

#define MAXPRINTMSG 4096
void Sys_DebugPrintf( const char *fmt, ... ) {
	char msg[MAXPRINTMSG];

	va_list argptr;
	va_start( argptr, fmt );
	idStr::vsnPrintf( msg, MAXPRINTMSG-1, fmt, argptr );
	msg[ sizeof(msg)-1 ] = '\0';
	va_end( argptr );

	OutputDebugString( msg );
}


void Sys_ShowWindow( bool show ) {
	::ShowWindow( win32.hWnd, show ? SW_SHOW : SW_HIDE );
}

void Sys_PumpEvents( void ) {
    MSG msg;

	// pump the message loop
	while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) {
		if ( !GetMessage( &msg, NULL, 0, 0 ) ) {
			Common_Quit();
		}

		// save the msg time, because wndprocs don't have access to the timestamp
		if ( win32.sysMsgTime && win32.sysMsgTime > (int)msg.time ) {
			// don't ever let the event times run backwards	
//			common->Printf( "Sys_PumpEvents: win32.sysMsgTime (%i) > msg.time (%i)\n", win32.sysMsgTime, msg.time );
		} else {
			win32.sysMsgTime = msg.time;
		}

		TranslateMessage (&msg);
      	DispatchMessage (&msg);
	}
}

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

	Common_Init();

//	::SetFocus( win32.hWnd );

    // main game loop
	while( 1 ) {
		Sys_PumpEvents();

		// run the game
		Common_Frame();
	}
//
//	// never gets here
	return 0;
}

///*
//====================
//clrstk
//
//I tried to get the run time to call this at every function entry, but
//====================
//*/
//static int	parmBytes;
//__declspec( naked ) void clrstk( void ) {
//	// eax = bytes to add to stack
//	__asm {
//		mov		[parmBytes],eax
//        neg     eax                     ; compute new stack pointer in eax
//        add     eax,esp
//        add     eax,4
//        xchg    eax,esp
//        mov     eax,dword ptr [eax]		; copy the return address
//        push    eax
//        
//        ; clear to zero
//        push	edi
//        push	ecx
//        mov		edi,esp
//        add		edi,12
//        mov		ecx,[parmBytes]
//		shr		ecx,2
//        xor		eax,eax
//		cld
//        rep	stosd
//        pop		ecx
//        pop		edi
//        
//        ret
//	}
//}
//
///*
//==================
//idSysLocal::OpenURL
//==================
//*/
//void idSysLocal::OpenURL( const char *url, bool doexit ) {
//	static bool doexit_spamguard = false;
//	HWND wnd;
//
//	if (doexit_spamguard) {
//		common->DPrintf( "OpenURL: already in an exit sequence, ignoring %s\n", url );
//		return;
//	}
//
//	common->Printf("Open URL: %s\n", url);
//
//	if ( !ShellExecute( NULL, "open", url, NULL, NULL, SW_RESTORE ) ) {
//		common->Error( "Could not open url: '%s' ", url );
//		return;
//	}
//
//	wnd = GetForegroundWindow();
//	if ( wnd ) {
//		ShowWindow( wnd, SW_MAXIMIZE );
//	}
//
//	if ( doexit ) {
//		doexit_spamguard = true;
//		cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "quit\n" );
//	}
//}
//
///*
//==================
//idSysLocal::StartProcess
//==================
//*/
//void idSysLocal::StartProcess( const char *exePath, bool doexit ) {
//	TCHAR				szPathOrig[_MAX_PATH];
//	STARTUPINFO			si;
//	PROCESS_INFORMATION	pi;
//
//	ZeroMemory( &si, sizeof(si) );
//	si.cb = sizeof(si);
//
//	strncpy( szPathOrig, exePath, _MAX_PATH );
//
//	if( !CreateProcess( NULL, szPathOrig, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi ) ) {
//        common->Error( "Could not start process: '%s' ", szPathOrig );
//	    return;
//	}
//
//	if ( doexit ) {
//		cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "quit\n" );
//	}
//}
//
///*
//==================
//Sys_SetFatalError
//==================
//*/
//void Sys_SetFatalError( const char *error ) {
//}
//
///*
//==================
//Sys_DoPreferences
//==================
//*/
//void Sys_DoPreferences( void ) {
//}
