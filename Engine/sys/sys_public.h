#ifndef __SYS_PUBLIC__
#define __SYS_PUBLIC__

#include <stdarg.h>

typedef struct
{
	int w;
	int h;
	unsigned char pData[1024*1024*4];
}sysTextContent_t;

typedef struct sysMemoryStats_s {
	int memoryLoad;
	int totalPhysical;
	int availPhysical;
	int totalPageFile;
	int availPageFile;
	int totalVirtual;
	int availVirtual;
	int availExtendedVirtual;
} sysMemoryStats_t;

typedef unsigned long address_t;

template<class type> class idList;		// for Sys_ListFiles

void			Sys_Error( const char *error, ...);
void			Sys_Quit( void );

// will go to the various text consoles
// NOT thread safe - never use in the async paths
void			Sys_Printf( const char *msg, ... );

// allow game to yield CPU time
// NOTE: due to SYS_MINSLEEP this is very bad portability karma, and should be completely removed
void			Sys_Sleep( int msec );

// Sys_Milliseconds should only be used for profiling purposes,
// any game related timing information should come from event timestamps
int			Sys_Milliseconds( void );

// for accurate performance testing
double		Sys_GetClockTicks( void );
double		Sys_ClockTicksPerSecond( void );

// returns amount of system ram
int			Sys_GetSystemRam( void );

// returns amount of video ram
int			Sys_GetVideoRam( void );

// returns amount of drive space in path
int			Sys_GetDriveFreeSpace( const char *path );

// returns memory stats
void			Sys_GetCurrentMemoryStatus( sysMemoryStats_t &stats );
void			Sys_GetExeLaunchMemoryStatus( sysMemoryStats_t &stats );

// lock and unlock memory
bool			Sys_LockMemory( void *ptr, int bytes );
bool			Sys_UnlockMemory( void *ptr, int bytes );

// set amount of physical work memory
void			Sys_SetPhysicalWorkMemory( int minBytes, int maxBytes );

// allows retrieving the call stack at execution points
void			Sys_GetCallStack( address_t *callStack, const int callStackSize );
const char *	Sys_GetCallStackStr( const address_t *callStack, const int callStackSize );
const char *	Sys_GetCallStackCurStr( int depth );
const char *	Sys_GetCallStackCurAddressStr( int depth );
void			Sys_ShutdownSymbols( void );

// DLL loading, the path should be a fully qualified OS path to the DLL file to be loaded
int			Sys_DLL_Load( const char *dllName );
void *		Sys_DLL_GetProcAddress( int dllHandle, const char *procName );
void			Sys_DLL_Unload( int dllHandle );

// event generation
void			Sys_GenerateEvents( void );
void			Sys_ClearEvents( void );

// input is tied to windows, so it needs to be started up and shut down whenever 
// the main window is recreated
void			Sys_InitInput( void );
void			Sys_ShutdownInput( void );
void			Sys_InitScanTable( void );
const unsigned char *Sys_GetScanTable( void );
unsigned char	Sys_GetConsoleKey( bool shifted );
// map a scancode key to a char
// does nothing on win32, as SE_KEY == SE_CHAR there
// on other OSes, consider the keyboard mapping
unsigned char	Sys_MapCharForKey( int key );

// keyboard input polling
int			Sys_PollKeyboardInputEvents( void );
int			Sys_ReturnKeyboardInputEvent( const int n, int &ch, bool &state );
void			Sys_EndKeyboardInputEvents( void );

// mouse input polling
int			Sys_PollMouseInputEvents( void );
int			Sys_ReturnMouseInputEvent( const int n, int &action, int &value );
void			Sys_EndMouseInputEvents( void );

// when the console is down, or the game is about to perform a lengthy
// operation like map loading, the system can release the mouse cursor
// when in windowed mode
void			Sys_GrabMouseCursor( bool grabIt );

void			Sys_ShowWindow( bool show );
bool			Sys_IsWindowVisible( void );
void			Sys_ShowConsole( int visLevel, bool quitOnClose );


void			Sys_Mkdir( const char *path );
//ID_TIME_T			Sys_FileTimeStamp( FILE *fp );
// NOTE: do we need to guarantee the same output on all platforms?
//const char *	Sys_TimeStampToStr( ID_TIME_T timeStamp );
const char *	Sys_DefaultCDPath( void );
const char *	Sys_DefaultBasePath( void );
const char *	Sys_DefaultSavePath( void );
const char *	Sys_EXEPath( void );

// use fs_debug to verbose Sys_ListFiles
// returns -1 if directory was not found (the list is cleared)
int				Sys_ListFiles( const char *directory, const char *extension, idList<class lfStr> &list );

// know early if we are performing a fatal error shutdown so the error message doesn't get lost
void			Sys_SetFatalError( const char *error );

// display perference dialog
void			Sys_DoPreferences( void );

/*
==============================================================

	Networking

==============================================================
*/

typedef enum {
	NA_BAD,					// an address lookup failed
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP
} netadrtype_t;

typedef struct {
	netadrtype_t	type;
	unsigned char	ip[4];
	unsigned short	port;
} netadr_t;

#define	PORT_ANY			-1

class idPort {
public:
				idPort();				// this just zeros netSocket and port
	virtual		~idPort();

	// if the InitForPort fails, the idPort.port field will remain 0
	bool		InitForPort( int portNumber );
	int			GetPort( void ) const { return bound_to.port; }
	netadr_t	GetAdr( void ) const { return bound_to; }
	void		Close();

	bool		GetPacket( netadr_t &from, void *data, int &size, int maxSize );
	bool		GetPacketBlocking( netadr_t &from, void *data, int &size, int maxSize, int timeout );
	void		SendPacket( const netadr_t to, const void *data, int size );

	int			packetsRead;
	int			bytesRead;

	int			packetsWritten;
	int			bytesWritten;

private:
	netadr_t	bound_to;		// interface and port
	int			netSocket;		// OS specific socket
};

class idTCP {
public:
				idTCP();
	virtual		~idTCP();

	// if host is host:port, the value of port is ignored
	bool		Init( const char *host, short port );
	void		Close();

	// returns -1 on failure (and closes socket)
	// those are non blocking, can be used for polling
	// there is no buffering, you are not guaranteed to Read or Write everything in a single call
	// (specially on win32, see recv and send documentation)
	int			Read( void *data, int size );
	int			Write( void *data, int size );

private:
	netadr_t	address;		// remote address
	int			fd;				// OS specific socket
};

				// parses the port number
				// can also do DNS resolve if you ask for it.
				// NOTE: DNS resolve is a slow/blocking call, think before you use
				// ( could be exploited for server DoS )
bool			Sys_StringToNetAdr( const char *s, netadr_t *a, bool doDNSResolve );
const char *	Sys_NetAdrToString( const netadr_t a );
bool			Sys_IsLANAddress( const netadr_t a );
bool			Sys_CompareNetAdrBase( const netadr_t a, const netadr_t b );

void			Sys_InitNetworking( void );
void			Sys_ShutdownNetworking( void );

double Sys_GetClockTicks( void );
double Sys_ClockTicksPerSecond( void );


bool        Sys_DrawText(const char* text, sysTextContent_t* img);


#endif /* !__SYS_PUBLIC__ */
