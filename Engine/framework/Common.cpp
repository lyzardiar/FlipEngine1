#include "Common.h"
#include "../sys/sys_public.h"

#define	MAX_PRINT_MSG_SIZE	4096
#define MAX_WARNING_LIST	256

typedef enum {
	ERP_NONE,
	ERP_FATAL,						// exit the entire game with a popup window
	ERP_DROP,						// print to console and disconnect from game
	ERP_DISCONNECT					// don't kill server
} errorParm_t;

#if defined( _DEBUG )
	#define BUILD_DEBUG "-debug"
#else
	#define BUILD_DEBUG ""
#endif

//struct version_s {
//			version_s( void ) { sprintf( string, "%s.%d%s %s %s %s", ENGINE_VERSION, BUILD_NUMBER, BUILD_DEBUG, BUILD_STRING, __DATE__, __TIME__ ); }
//	char	string[256];
//} version;

//idCVar com_version( "si_version", version.string, CVAR_SYSTEM|CVAR_ROM|CVAR_SERVERINFO, "engine version" );
//idCVar com_skipRenderer( "com_skipRenderer", "0", CVAR_BOOL|CVAR_SYSTEM, "skip the renderer completely" );
//idCVar com_machineSpec( "com_machineSpec", "-1", CVAR_INTEGER | CVAR_ARCHIVE | CVAR_SYSTEM, "hardware classification, -1 = not detected, 0 = low quality, 1 = medium quality, 2 = high quality, 3 = ultra quality" );
//idCVar com_purgeAll( "com_purgeAll", "0", CVAR_BOOL | CVAR_ARCHIVE | CVAR_SYSTEM, "purge everything between level loads" );
//idCVar com_memoryMarker( "com_memoryMarker", "-1", CVAR_INTEGER | CVAR_SYSTEM | CVAR_INIT, "used as a marker for memory stats" );
//idCVar com_preciseTic( "com_preciseTic", "1", CVAR_BOOL|CVAR_SYSTEM, "run one game tick every async thread update" );
//idCVar com_asyncInput( "com_asyncInput", "0", CVAR_BOOL|CVAR_SYSTEM, "sample input from the async thread" );
//#define ASYNCSOUND_INFO "0: mix sound inline, 1: memory mapped async mix, 2: callback mixing, 3: write async mix"
//#if defined( MACOS_X )
//idCVar com_asyncSound( "com_asyncSound", "2", CVAR_INTEGER|CVAR_SYSTEM|CVAR_ROM, ASYNCSOUND_INFO );
//#elif defined( __linux__ )
//idCVar com_asyncSound( "com_asyncSound", "3", CVAR_INTEGER|CVAR_SYSTEM|CVAR_ROM, ASYNCSOUND_INFO );
//#else
//idCVar com_asyncSound( "com_asyncSound", "1", CVAR_INTEGER|CVAR_SYSTEM, ASYNCSOUND_INFO, 0, 1 );
//#endif
//idCVar com_forceGenericSIMD( "com_forceGenericSIMD", "0", CVAR_BOOL | CVAR_SYSTEM | CVAR_NOCHEAT, "force generic platform independent SIMD" );
//idCVar com_developer( "developer", "0", CVAR_BOOL|CVAR_SYSTEM|CVAR_NOCHEAT, "developer mode" );
//idCVar com_allowConsole( "com_allowConsole", "0", CVAR_BOOL | CVAR_SYSTEM | CVAR_NOCHEAT, "allow toggling console with the tilde key" );
//idCVar com_speeds( "com_speeds", "0", CVAR_BOOL|CVAR_SYSTEM|CVAR_NOCHEAT, "show engine timings" );
//idCVar com_showFPS( "com_showFPS", "0", CVAR_BOOL|CVAR_SYSTEM|CVAR_ARCHIVE|CVAR_NOCHEAT, "show frames rendered per second" );
//idCVar com_showMemoryUsage( "com_showMemoryUsage", "0", CVAR_BOOL|CVAR_SYSTEM|CVAR_NOCHEAT, "show total and per frame memory usage" );
//idCVar com_showAsyncStats( "com_showAsyncStats", "0", CVAR_BOOL|CVAR_SYSTEM|CVAR_NOCHEAT, "show async network stats" );
//idCVar com_showSoundDecoders( "com_showSoundDecoders", "0", CVAR_BOOL|CVAR_SYSTEM|CVAR_NOCHEAT, "show sound decoders" );
//idCVar com_timestampPrints( "com_timestampPrints", "0", CVAR_SYSTEM, "print time with each console print, 1 = msec, 2 = sec", 0, 2, idCmdSystem::ArgCompletion_Integer<0,2> );
//idCVar com_timescale( "timescale", "1", CVAR_SYSTEM | CVAR_FLOAT, "scales the time", 0.1f, 10.0f );
//idCVar com_logFile( "logFile", "0", CVAR_SYSTEM | CVAR_NOCHEAT, "1 = buffer log, 2 = flush after each print", 0, 2, idCmdSystem::ArgCompletion_Integer<0,2> );
//idCVar com_logFileName( "logFileName", "qconsole.log", CVAR_SYSTEM | CVAR_NOCHEAT, "name of log file, if empty, qconsole.log will be used" );
//idCVar com_makingBuild( "com_makingBuild", "0", CVAR_BOOL | CVAR_SYSTEM, "1 when making a build" );
//idCVar com_updateLoadSize( "com_updateLoadSize", "0", CVAR_BOOL | CVAR_SYSTEM | CVAR_NOCHEAT, "update the load size after loading a map" );
//idCVar com_videoRam( "com_videoRam", "64", CVAR_INTEGER | CVAR_SYSTEM | CVAR_NOCHEAT | CVAR_ARCHIVE, "holds the last amount of detected video ram" );
//
//idCVar com_product_lang_ext( "com_product_lang_ext", "1", CVAR_INTEGER | CVAR_SYSTEM | CVAR_ARCHIVE, "Extension to use when creating language files." );

// com_speeds times
int				time_gameFrame;
int				time_gameDraw;
int				time_frontend;			// renderSystem frontend time
int				time_backend;			// renderSystem backend time

int				com_frameTime;			// time for the current frame in milliseconds
int				com_frameNumber;		// variable frame number
volatile int	com_ticNumber;			// 60 hz tics
int				com_editors;			// currently opened editor(s)
bool			com_editorActive;		//  true if an editor has focus

#ifdef _WIN32
HWND			com_hwndMsg = NULL;
bool			com_outputMsg = false;
unsigned int	com_msgID = -1;
#endif

#ifdef __DOOM_DLL__
idGame *		game = NULL;
idGameEdit *	gameEdit = NULL;
#endif

// writes si_version to the config file - in a kinda obfuscated way
//#define ID_WRITE_VERSION

class idCommonLocal : public idCommon {
public:
								idCommonLocal( void );

	virtual void				Init( int argc, const char **argv, const char *cmdline );
	virtual void				Shutdown( void );
	virtual void				Quit( void );
	//virtual bool				IsInitialized( void ) const;
	virtual void				Frame( void );
	//virtual void				GUIFrame( bool execCmd, bool network );
	//virtual void				Async( void );
	//virtual void				StartupVariable( const char *match, bool once );
	////virtual void				InitTool( const toolFlag_t tool, const idDict *dict );
	//virtual void				ActivateTool( bool active );
	//virtual void				WriteConfigToFile( const char *filename );
	//virtual void				WriteFlaggedCVarsToFile( const char *filename, int flags, const char *setCmd );
	//virtual void				BeginRedirect( char *buffer, int buffersize, void (*flush)( const char * ) );
	//virtual void				EndRedirect( void );
	//virtual void				SetRefreshOnPrint( bool set );
	virtual void				Printf( const char *fmt, ... );
	//virtual void				VPrintf( const char *fmt, va_list arg );
	//virtual void				DPrintf( const char *fmt, ... ) id_attribute((format(printf,2,3)));
	//virtual void				Warning( const char *fmt, ... ) id_attribute((format(printf,2,3)));
	//virtual void				DWarning( const char *fmt, ...) id_attribute((format(printf,2,3)));
	//virtual void				PrintWarnings( void );
	//virtual void				ClearWarnings( const char *reason );
	//virtual void				Error( const char *fmt, ... ) id_attribute((format(printf,2,3)));
	//virtual void				FatalError( const char *fmt, ... ) id_attribute((format(printf,2,3)));
	//virtual const idLangDict *	GetLanguageDict( void );

	//virtual const char *		KeysFromBinding( const char *bind );
	//virtual const char *		BindingFromKey( const char *key );

	//virtual int					ButtonState( int key );
	//virtual int					KeyState( int key );

	//void						InitGame( void );
	//void						ShutdownGame( bool reloading );

	//// localization
	//void						InitLanguageDict( void );
	///*void						LocalizeGui( const char *fileName, idLangDict &langDict );
	//void						LocalizeMapData( const char *fileName, idLangDict &langDict );
	//void						LocalizeSpecificMapData( const char *fileName, idLangDict &langDict, const idLangDict &replaceArgs );*/

	//void						SetMachineSpec( void );

private:
	//void						InitCommands( void );
	//void						InitRenderSystem( void );
	//void						InitSIMD( void );
	//bool						AddStartupCommands( void );
	//void						ParseCommandLine( int argc, const char **argv );
	//void						ClearCommandLine( void );
	//bool						SafeMode( void );
	//void						CheckToolMode( void );
	//void						CloseLogFile( void );
	//void						WriteConfiguration( void );
	//void						DumpWarnings( void );
	//void						SingleAsyncTic( void );
	//void						LoadGameDLL( void );
	//void						UnloadGameDLL( void );
	//void						PrintLoadingMessage( const char *msg );
	////void						FilterLangList( idStrList* list, idStr lang );

	//bool						com_fullyInitialized;
	//bool						com_refreshOnPrint;		// update the screen every print for dmap
	//int							com_errorEntered;		// 0, ERP_DROP, etc
	//bool						com_shuttingDown;

	////idFile *					logFile;

	//char						errorMessage[MAX_PRINT_MSG_SIZE];

	//char *						rd_buffer;
	//int							rd_buffersize;
	//void						(*rd_flush)( const char *buffer );

	idStr						warningCaption;
	/*idStrList					warningList;
	idStrList					errorList;*/

	int							gameDLL;

	//idLangDict					languageDict;
};

idCommonLocal	commonLocal;
idCommon *		common = &commonLocal;


/*
==================
idCommonLocal::idCommonLocal
==================
*/
idCommonLocal::idCommonLocal( void ) {

}


#ifdef _WIN32

/*
==================
EnumWindowsProc
==================
*/
BOOL CALLBACK EnumWindowsProc( HWND hwnd, LPARAM lParam ) {
	//char buff[1024];

	//::GetWindowText( hwnd, buff, sizeof( buff ) );
	//if ( idStr::Icmpn( buff, EDITOR_WINDOWTEXT, strlen( EDITOR_WINDOWTEXT ) ) == 0 ) {
	//	com_hwndMsg = hwnd;
	//	return FALSE;
	//}
	return TRUE;
}

/*
==================
FindEditor
==================
*/
bool FindEditor( void ) {
	com_hwndMsg = NULL;
	EnumWindows( EnumWindowsProc, 0 );
	return !( com_hwndMsg == NULL );
}


void idCommonLocal::Init( int argc, const char **argv, const char *cmdline ) 
{
}

void idCommonLocal::Shutdown()
{

}

void idCommonLocal::Frame()
{

}

void idCommonLocal::Quit()
{
	Sys_Quit();
}

void idCommonLocal::Printf( const char *fmt, ... )
{
	va_list argptr;
	va_start( argptr, fmt );
	Sys_Printf( fmt, argptr );
	va_end( argptr );
}
#endif


