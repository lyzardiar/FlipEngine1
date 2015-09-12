#include "win_local.h"
#include "../../framework/Common.h"

static void		GLW_InitExtensions( void );

/*
====================
FakeWndProc

Only used to get wglExtensions
====================
*/
LONG WINAPI FakeWndProc (
    HWND    hWnd,
    UINT    uMsg,
    WPARAM  wParam,
    LPARAM  lParam) {

	if ( uMsg == WM_DESTROY ) {
        PostQuitMessage(0);
	}

	if ( uMsg != WM_CREATE ) {
	    return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	const static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		8, 0,
		0, 0, 0, 0,
		24, 8,
		0,
		PFD_MAIN_PLANE,
		0,
		0,
		0,
		0,
	};
	int		pixelFormat;
	HDC hDC;
	HGLRC hGLRC;

    hDC = GetDC(hWnd);

    // Set up OpenGL
    pixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pixelFormat, &pfd);
    hGLRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hGLRC);

	// free things
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hGLRC);
    ReleaseDC(hWnd, hDC);

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



/*
====================
GLW_InitDriver

Set the pixelformat for the window before it is
shown, and create the rendering context
====================
*/
static bool GLW_InitDriver( ) {
    PIXELFORMATDESCRIPTOR src = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),	// size of this pfd
		1,								// version number
		PFD_DRAW_TO_WINDOW |			// support window
		PFD_SUPPORT_OPENGL |			// support OpenGL
		PFD_DOUBLEBUFFER,				// double buffered
		PFD_TYPE_RGBA,					// RGBA type
		32,								// 32-bit color depth
		0, 0, 0, 0, 0, 0,				// color bits ignored
		8,								// 8 bit destination alpha
		0,								// shift bit ignored
		0,								// no accumulation buffer
		0, 0, 0, 0, 					// accum bits ignored
		24,								// 24-bit z-buffer	
		8,								// 8-bit stencil buffer
		0,								// no auxiliary buffer
		PFD_MAIN_PLANE,					// main layer
		0,								// reserved
		0, 0, 0							// layer masks ignored
    };

	common->Printf( "Initializing OpenGL driver\n" );

	//
	// get a DC for our window if we don't already have one allocated
	//
	if ( win32.hDC == NULL ) {
		common->Printf( "...getting DC: " );

		if ( ( win32.hDC = GetDC( win32.hWnd ) ) == NULL ) {
			common->Printf( "^3failed^0\n" );
			return false;
		}
		common->Printf( "succeeded\n" );
	}

	if ( ( win32.pixelformat = ChoosePixelFormat( win32.hDC, &src ) ) == 0 ) {
		common->Printf( "...^3GLW_ChoosePFD failed^0\n");
		return false;
	}
	common->Printf( "...PIXELFORMAT %d selected\n", win32.pixelformat );

	// get the full info
	// DescribePixelFormat( win32.hDC, win32.pixelformat, sizeof( win32.pfd ), &win32.pfd );

	// the same SetPixelFormat is used either way
	if ( SetPixelFormat( win32.hDC, win32.pixelformat, &win32.pfd ) == FALSE ) {
		common->Printf( "...^3SetPixelFormat failed^0\n", win32.hDC );
		return false;
	}

	//
	// startup the OpenGL subsystem by creating a context and making it current
	// The wglCreateContext function creates a new OpenGL rendering context, 
	// which is suitable for drawing on the device referenced by hdc. 
	// The rendering context has the same pixel format as the device context.
	//
	common->Printf( "...creating GL context: " );
	if ( ( win32.hGLRC = wglCreateContext( win32.hDC ) ) == 0 ) {
		common->Printf( "wgl create contect failed\n" );
		return false;
	}
	common->Printf( "succeeded\n" );

	common->Printf( "...making context current: " );
	if ( !wglMakeCurrent( win32.hDC, win32.hGLRC ) ) {
		wglDeleteContext( win32.hGLRC );
		win32.hGLRC = NULL;
		common->Printf( "wgl create contec failed\n" );
		return false;
	}
	common->Printf( "succeeded\n" );

	return true;
}

/*
====================
GLW_CreateWindowClasses
====================
*/
static void GLW_CreateWindowClasses( void ) {
	WNDCLASS wc;

	//
	// register the window class if necessary
	//
	if ( win32.windowClassRegistered ) {
		return;
	}

	memset( &wc, 0, sizeof( wc ) );

	wc.style         = 0;
	wc.lpfnWndProc   = (WNDPROC) MainWndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = win32.hInstance;
	// wc.hIcon         = LoadIcon( win32.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor       = LoadCursor (NULL,IDC_ARROW);
	wc.hbrBackground = (struct HBRUSH__ *)COLOR_GRAYTEXT;
	wc.lpszMenuName  = 0;
	wc.lpszClassName = "win_glimp";

	if ( !RegisterClass( &wc ) ) {
		// common->FatalError( "GLW_CreateWindow: could not register window class" );
	}
	// common->Printf( "...registered window class\n" );

	// now register the fake window class that is only used
	// to get wgl extensions
	wc.style         = 0;
	wc.lpfnWndProc   = (WNDPROC) FakeWndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = win32.hInstance;
	//wc.hIcon         = LoadIcon( win32.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor       = LoadCursor (NULL,IDC_ARROW);
	wc.hbrBackground = (struct HBRUSH__ *)COLOR_GRAYTEXT;
	wc.lpszMenuName  = 0;
	wc.lpszClassName = "win_glimp";

	if ( !RegisterClass( &wc ) ) {
		// common->FatalError( "GLW_CreateWindow: could not register window class" );
	}
	common->Printf( "...registered fake window class\n" );

	win32.windowClassRegistered = true;
}

/*
=======================
GLW_CreateWindow

Responsible for creating the Win32 window.
If cdsFullscreen is true, it won't have a border
=======================
*/
static bool GLW_CreateWindow( ) {
	int				stylebits;
	int				x, y, w, h;
	int				exstyle;

	//
	// compute width and height
	//
	/*if ( parms.fullScreen ) {
	exstyle = WS_EX_TOPMOST;
	stylebits = WS_POPUP|WS_VISIBLE|WS_SYSMENU;

	x = 0;
	y = 0;
	w = parms.width;
	h = parms.height;
	} else {*/
		RECT	r;

		// adjust width and height for window border
		r.bottom = parms.height;
		r.left = 0;
		r.top = 0;
		r.right = parms.width;

		exstyle = 0;
		stylebits = WINDOW_STYLE|WS_SYSMENU;
		AdjustWindowRect (&r, stylebits, FALSE);

		w = r.right - r.left;
		h = r.bottom - r.top;

		x = win32.win_xpos;
		y = win32.win_ypos;

		// adjust window coordinates if necessary 
		// so that the window is completely on screen
		if ( x + w > win32.desktopWidth ) {
			x = ( win32.desktopWidth - w );
		}
		if ( y + h > win32.desktopHeight ) {
			y = ( win32.desktopHeight - h );
		}
		if ( x < 0 ) {
			x = 0;
		}
		if ( y < 0 ) {
			y = 0;
		}
	}

	win32.hWnd = CreateWindowEx (
		 exstyle, 
		 WIN32_WINDOW_CLASS_NAME,
		 GAME_NAME,
		 stylebits,
		 x, y, w, h,
		 NULL,
		 NULL,
		 win32.hInstance,
		 NULL);

	if ( !win32.hWnd ) {
		common->Printf( "^3GLW_CreateWindow() - Couldn't create window^0\n" );
		return false;
	}

	::SetTimer( win32.hWnd, 0, 100, NULL );

	ShowWindow( win32.hWnd, SW_SHOW );
	UpdateWindow( win32.hWnd );
	common->Printf( "...created window @ %d,%d (%dx%d)\n", x, y, w, h );

	if ( !GLW_InitDriver( parms ) ) {
		ShowWindow( win32.hWnd, SW_HIDE );
		DestroyWindow( win32.hWnd );
		win32.hWnd = NULL;
		return false;
	}

	SetForegroundWindow( win32.hWnd );
	SetFocus( win32.hWnd );

	glConfig.isFullscreen = parms.fullScreen;

	return true;
}



/*
===================
GLimp_Init

This is the platform specific OpenGL initialization function.  It
is responsible for loading OpenGL, initializing it,
creating a window of the appropriate size, doing
fullscreen manipulations, etc.  Its overall responsibility is
to make sure that a functional OpenGL subsystem is operating
when it returns to the ref.

If there is any failure, the renderer will revert back to safe
parameters and try again.
===================
*/
bool GLimp_Init( glimpParms_t parms ) {
	const char	*driverName;
	HDC		hDC;

	common->Printf( "Initializing OpenGL subsystem\n" );

	// check our desktop attributes
	hDC = GetDC( GetDesktopWindow() );
	win32.desktopBitsPixel = GetDeviceCaps( hDC, BITSPIXEL );
	win32.desktopWidth = GetDeviceCaps( hDC, HORZRES );
	win32.desktopHeight = GetDeviceCaps( hDC, VERTRES );
	ReleaseDC( GetDesktopWindow(), hDC );

	// we can't run in a window unless it is 32 bpp
	if ( win32.desktopBitsPixel < 32 && !parms.fullScreen ) {
		common->Printf("^3Windowed mode requires 32 bit desktop depth^0\n");
		return false;
	}

	// save the hardware gamma so it can be
	// restored on exit

	// create our window classes if we haven't already
	GLW_CreateWindowClasses();

	// this will load the dll and set all our qgl* function pointers,
	// but doesn't create a window

	// r_glDriver is only intended for using instrumented OpenGL
	// dlls.  Normal users should never have to use it, and it is
	// not archived.
	driverName = r_glDriver.GetString()[0] ? r_glDriver.GetString() : "opengl32";
	if ( !QGL_Init( driverName ) ) {
		common->Printf( "^3GLimp_Init() could not load r_glDriver \"%s\"^0\n", driverName );
		return false;
	}

	// getting the wgl extensions involves creating a fake window to get a context,
	// which is pretty disgusting, and seems to mess with the AGP VAR allocation
	GLW_GetWGLExtensionsWithFakeWindow();

	// try to change to fullscreen
	if ( parms.fullScreen ) {
		if ( !GLW_SetFullScreen( parms ) ) {
			GLimp_Shutdown();
			return false;
		}
	}

	// try to create a window with the correct pixel format
	// and init the renderer context
	if ( !GLW_CreateWindow( parms ) ) {
		GLimp_Shutdown();
		return false;
	}

	// wglSwapinterval, etc
	GLW_CheckWGLExtensions( win32.hDC );

	// check logging
	GLimp_EnableLogging( ( r_logFile.GetInteger() != 0 ) );

	return true;
}


/*
===================
GLimp_SetScreenParms

Sets up the screen based on passed parms.. 
===================
*/
bool GLimp_SetScreenParms( glimpParms_t parms ) {
	int exstyle;
	int stylebits;
	int x, y, w, h;
	DEVMODE dm;

	memset( &dm, 0, sizeof( dm ) );
	dm.dmSize = sizeof( dm );
	dm.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
	if ( parms.displayHz != 0 ) {
		dm.dmDisplayFrequency = parms.displayHz;
		dm.dmFields |= DM_DISPLAYFREQUENCY;
	}

	win32.cdsFullscreen = parms.fullScreen;
	glConfig.isFullscreen = parms.fullScreen;

	if ( parms.fullScreen ) {
		exstyle = WS_EX_TOPMOST;
		stylebits = WS_POPUP|WS_VISIBLE|WS_SYSMENU;
		SetWindowLong( win32.hWnd, GWL_STYLE, stylebits );
		SetWindowLong( win32.hWnd, GWL_EXSTYLE, exstyle );
		dm.dmPelsWidth  = parms.width;
		dm.dmPelsHeight = parms.height;
		dm.dmBitsPerPel = 32;
		x = y = w = h = 0;
	} else {
		RECT	r;

		// adjust width and height for window border
		r.bottom = parms.height;
		r.left = 0;
		r.top = 0;
		r.right = parms.width;

		w = r.right - r.left;
		h = r.bottom - r.top;

		x = win32.win_xpos.GetInteger();
		y = win32.win_ypos.GetInteger();

		// adjust window coordinates if necessary 
		// so that the window is completely on screen
		if ( x + w > win32.desktopWidth ) {
			x = ( win32.desktopWidth - w );
		}
		if ( y + h > win32.desktopHeight ) {
			y = ( win32.desktopHeight - h );
		}
		if ( x < 0 ) {
			x = 0;
		}
		if ( y < 0 ) {
			y = 0;
		}
		dm.dmPelsWidth  = win32.desktopWidth;
		dm.dmPelsHeight = win32.desktopHeight;
		dm.dmBitsPerPel = win32.desktopBitsPixel;
		exstyle = 0;
		stylebits = WINDOW_STYLE|WS_SYSMENU;
		AdjustWindowRect (&r, stylebits, FALSE);
		SetWindowLong( win32.hWnd, GWL_STYLE, stylebits );
		SetWindowLong( win32.hWnd, GWL_EXSTYLE, exstyle );
		common->Printf( "%i %i %i %i\n", x, y, w, h );
	}
	bool ret = ( ChangeDisplaySettings( &dm, parms.fullScreen ? CDS_FULLSCREEN : 0 ) == DISP_CHANGE_SUCCESSFUL );
	SetWindowPos( win32.hWnd, parms.fullScreen ? HWND_TOPMOST : HWND_NOTOPMOST, x, y, w, h, parms.fullScreen ? SWP_NOSIZE | SWP_NOMOVE : SWP_SHOWWINDOW );
	return ret;
}

/*
===================
GLimp_Shutdown

This routine does all OS specific shutdown procedures for the OpenGL
subsystem.
===================
*/
void GLimp_Shutdown( void ) {
	const char *success[] = { "failed", "success" };
	int retVal;

	common->Printf( "Shutting down OpenGL subsystem\n" );

	// set current context to NULL
	if ( wglMakeCurrent(NULL, NULL)) {
		retVal = wglMakeCurrent( NULL, NULL ) != 0;
		common->Printf( "...wglMakeCurrent( NULL, NULL ): %s\n", success[retVal] );
	}

	// delete HGLRC
	if ( win32.hGLRC ) {
		retVal = qwglDeleteContext( win32.hGLRC ) != 0;
		common->Printf( "...deleting GL context: %s\n", success[retVal] );
		win32.hGLRC = NULL;
	}

	// release DC
	if ( win32.hDC ) {
		retVal = ReleaseDC( win32.hWnd, win32.hDC ) != 0;
		common->Printf( "...releasing DC: %s\n", success[retVal] );
		win32.hDC   = NULL;
	}

	// destroy window
	if ( win32.hWnd ) {
		common->Printf( "...destroying window\n" );
		ShowWindow( win32.hWnd, SW_HIDE );
		DestroyWindow( win32.hWnd );
		win32.hWnd = NULL;
	}

	// reset display settings
	if ( win32.cdsFullscreen ) {
		common->Printf( "...resetting display\n" );
		ChangeDisplaySettings( 0, 0 );
		win32.cdsFullscreen = false;
	}

	// close the thread so the handle doesn't dangle
	if ( win32.renderThreadHandle ) {
		common->Printf( "...closing smp thread\n" );
		CloseHandle( win32.renderThreadHandle );
		win32.renderThreadHandle = NULL;
	}

	// restore gamma
	GLimp_RestoreGamma();

	// shutdown QGL subsystem
	QGL_Shutdown();
}


/*
=====================
GLimp_SwapBuffers
=====================
*/
void GLimp_SwapBuffers( void ) {
	//
	// wglSwapinterval is a windows-private extension,
	// so we must check for it here instead of portably
	//
	if ( r_swapInterval.IsModified() ) {
		r_swapInterval.ClearModified();

		if ( wglSwapIntervalEXT ) {
			wglSwapIntervalEXT( r_swapInterval.GetInteger() );
		}
	}

	qwglSwapBuffers( win32.hDC );

//Sys_DebugPrintf( "*** SwapBuffers() ***\n" );
}

/*
===========================================================

SMP acceleration

===========================================================
*/

//#define	REALLOC_DC

/*
===================
GLimp_ActivateContext

===================
*/
void GLimp_ActivateContext( void ) {
	if ( !qwglMakeCurrent( win32.hDC, win32.hGLRC ) ) {
		win32.wglErrors++;
	}
}

/*
===================
GLimp_DeactivateContext

===================
*/
void GLimp_DeactivateContext( void ) {
	qglFinish();
	if ( !qwglMakeCurrent( win32.hDC, NULL ) ) {
		win32.wglErrors++;
	}
#ifdef REALLOC_DC
	// makeCurrent NULL frees the DC, so get another
	if ( ( win32.hDC = GetDC( win32.hWnd ) ) == NULL ) {
		win32.wglErrors++;
	}
#endif

}

/*
===================
GLimp_RenderThreadWrapper

===================
*/
static void GLimp_RenderThreadWrapper( void ) {
	win32.glimpRenderThread();

	// unbind the context before we die
	qwglMakeCurrent( win32.hDC, NULL );
}

/*
=======================
GLimp_SpawnRenderThread

Returns false if the system only has a single processor
=======================
*/
bool GLimp_SpawnRenderThread( void (*function)( void ) ) {
	SYSTEM_INFO info;

	// check number of processors
	GetSystemInfo( &info );
	if ( info.dwNumberOfProcessors < 2 ) {
		return false;
	}
	
	// create the IPC elements
	win32.renderCommandsEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	win32.renderCompletedEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	win32.renderActiveEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	win32.glimpRenderThread = function;

	win32.renderThreadHandle = CreateThread(
	   NULL,	// LPSECURITY_ATTRIBUTES lpsa,
	   0,		// DWORD cbStack,
	   (LPTHREAD_START_ROUTINE)GLimp_RenderThreadWrapper,	// LPTHREAD_START_ROUTINE lpStartAddr,
	   0,			// LPVOID lpvThreadParm,
	   0,			//   DWORD fdwCreate,
	   &win32.renderThreadId );

	if ( !win32.renderThreadHandle ) {
		common->Error( "GLimp_SpawnRenderThread: failed" );
	}

	SetThreadPriority( win32.renderThreadHandle, THREAD_PRIORITY_ABOVE_NORMAL );
#if 0
	// make sure they always run on different processors
	SetThreadAffinityMask( GetCurrentThread, 1 );
	SetThreadAffinityMask( win32.renderThreadHandle, 2 );
#endif

	return true;
}


//#define	DEBUG_PRINTS

/*
===================
GLimp_BackEndSleep

===================
*/
void *GLimp_BackEndSleep( void ) {
	void	*data;

#ifdef DEBUG_PRINTS
OutputDebugString( "-->GLimp_BackEndSleep\n" );
#endif
	ResetEvent( win32.renderActiveEvent );

	// after this, the front end can exit GLimp_FrontEndSleep
	SetEvent( win32.renderCompletedEvent );

	WaitForSingleObject( win32.renderCommandsEvent, INFINITE );

	ResetEvent( win32.renderCompletedEvent );
	ResetEvent( win32.renderCommandsEvent );

	data = win32.smpData;

	// after this, the main thread can exit GLimp_WakeRenderer
	SetEvent( win32.renderActiveEvent );

#ifdef DEBUG_PRINTS
OutputDebugString( "<--GLimp_BackEndSleep\n" );
#endif
	return data;
}

/*
===================
GLimp_FrontEndSleep

===================
*/
void GLimp_FrontEndSleep( void ) {
#ifdef DEBUG_PRINTS
OutputDebugString( "-->GLimp_FrontEndSleep\n" );
#endif
	WaitForSingleObject( win32.renderCompletedEvent, INFINITE );

#ifdef DEBUG_PRINTS
OutputDebugString( "<--GLimp_FrontEndSleep\n" );
#endif
}

volatile bool	renderThreadActive;

/*
===================
GLimp_WakeBackEnd

===================
*/
void GLimp_WakeBackEnd( void *data ) {
	int		r;

#ifdef DEBUG_PRINTS
OutputDebugString( "-->GLimp_WakeBackEnd\n" );
#endif
	win32.smpData = data;

	if ( renderThreadActive ) {
		common->FatalError( "GLimp_WakeBackEnd: already active" );
	}

	r = WaitForSingleObject( win32.renderActiveEvent, 0 );
	if ( r == WAIT_OBJECT_0 ) {
		common->FatalError( "GLimp_WakeBackEnd: already signaled" );
	}

	r = WaitForSingleObject( win32.renderCommandsEvent, 0 );
	if ( r == WAIT_OBJECT_0 ) {
		common->FatalError( "GLimp_WakeBackEnd: commands already signaled" );
	}

	// after this, the renderer can continue through GLimp_RendererSleep
	SetEvent( win32.renderCommandsEvent );

	r = WaitForSingleObject( win32.renderActiveEvent, 5000 );

	if ( r == WAIT_TIMEOUT ) {
		common->FatalError( "GLimp_WakeBackEnd: WAIT_TIMEOUT" );
	}

#ifdef DEBUG_PRINTS
OutputDebugString( "<--GLimp_WakeBackEnd\n" );
#endif
}

//===================================================================

/*
===================
GLimp_ExtensionPointer

Returns a function pointer for an OpenGL extension entry point
===================
*/
GLExtension_t GLimp_ExtensionPointer( const char *name ) {
	void	(*proc)(void);

	proc = (GLExtension_t)qwglGetProcAddress( name );

	if ( !proc ) {
		common->Printf( "Couldn't find proc address for: %s\n", name );
	}

	return proc;
}

