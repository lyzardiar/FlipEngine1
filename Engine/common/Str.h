#ifndef __STR_H__
#define __STR_H__

#include "precompiled.h"

#define ID_INLINE						__forceinline


#define INTSIGNBITNOTSET(i)		((~((const unsigned long)(i))) >> 31)
/*
===============================================================================

	Character string

===============================================================================
*/

// these library functions should not be used for cross platform compatibility
//#define strcmp			lfStr::Cmp		// use_idStr_Cmp
//#define strncmp			use_idStr_Cmpn
//
//#if defined( StrCmpN )
//#undef StrCmpN
//#endif
//#define StrCmpN			use_idStr_Cmpn
//
//#if defined( strcmpi )
//#undef strcmpi
//#endif
//#define strcmpi			use_idStr_Icmp
//
//#if defined( StrCmpI )
//#undef StrCmpI
//#endif
//#define StrCmpI			use_idStr_Icmp
//
//#if defined( StrCmpNI )
//#undef StrCmpNI
//#endif
//#define StrCmpNI		use_idStr_Icmpn

#define stricmp			lfStr::Icmp		// use_idStr_Icmp
//#define _stricmp		use_idStr_Icmp
//#define strcasecmp		use_idStr_Icmp
//#define strnicmp		use_idStr_Icmpn
//#define _strnicmp		use_idStr_Icmpn
//#define _memicmp		use_idStr_Icmpn
//#define snprintf		use_idStr_snPrintf
//#define _snprintf		use_idStr_snPrintf
//#define vsnprintf		use_idStr_vsnPrintf
//#define _vsnprintf		use_idStr_vsnPrintf


#ifndef FILE_HASH_SIZE
#define FILE_HASH_SIZE		1024
#endif

// color escape character
const int C_COLOR_ESCAPE			= '^';
const int C_COLOR_DEFAULT			= '0';
const int C_COLOR_RED				= '1';
const int C_COLOR_GREEN				= '2';
const int C_COLOR_YELLOW			= '3';
const int C_COLOR_BLUE				= '4';
const int C_COLOR_CYAN				= '5';
const int C_COLOR_MAGENTA			= '6';
const int C_COLOR_WHITE				= '7';
const int C_COLOR_GRAY				= '8';
const int C_COLOR_BLACK				= '9';

// color escape string
#define S_COLOR_DEFAULT				"^0"
#define S_COLOR_RED					"^1"
#define S_COLOR_GREEN				"^2"
#define S_COLOR_YELLOW				"^3"
#define S_COLOR_BLUE				"^4"
#define S_COLOR_CYAN				"^5"
#define S_COLOR_MAGENTA				"^6"
#define S_COLOR_WHITE				"^7"
#define S_COLOR_GRAY				"^8"
#define S_COLOR_BLACK				"^9"

// make idStr a multiple of 16 bytes long
// don't make too large to keep memory requirements to a minimum
const int STR_ALLOC_BASE			= 20;
const int STR_ALLOC_GRAN			= 32;

typedef enum {
	MEASURE_SIZE = 0,
	MEASURE_BANDWIDTH
} Measure_t;

class lfStr {

public:
						lfStr( void );
						lfStr( const lfStr &text );
						lfStr( const lfStr &text, int start, int end );
						lfStr( const char *text );
						lfStr( const char *text, int start, int end );
						explicit lfStr( const bool b );
						explicit lfStr( const char c );
						explicit lfStr( const int i );
						explicit lfStr( const unsigned u );
						explicit lfStr( const float f );
						~lfStr( void );

	size_t				Size( void ) const;
	const char *		c_str( void ) const;
	operator			const char *( void ) const;
	operator			const char *( void );

	char				operator[]( int index ) const;
	char &				operator[]( int index );

	void				operator=( const lfStr &text );
	void				operator=( const char *text );

	friend lfStr		operator+( const lfStr &a, const lfStr &b );
	friend lfStr		operator+( const lfStr &a, const char *b );
	friend lfStr		operator+( const char *a, const lfStr &b );

	friend lfStr		operator+( const lfStr &a, const float b );
	friend lfStr		operator+( const lfStr &a, const int b );
	friend lfStr		operator+( const lfStr &a, const unsigned b );
	friend lfStr		operator+( const lfStr &a, const bool b );
	friend lfStr		operator+( const lfStr &a, const char b );

	lfStr &				operator+=( const lfStr &a );
	lfStr &				operator+=( const char *a );
	lfStr &				operator+=( const float a );
	lfStr &				operator+=( const char a );
	lfStr &				operator+=( const int a );
	lfStr &				operator+=( const unsigned a );
	lfStr &				operator+=( const bool a );

						// case sensitive compare
	friend bool			operator==( const lfStr &a, const lfStr &b );
	friend bool			operator==( const lfStr &a, const char *b );
	friend bool			operator==( const char *a, const lfStr &b );

						// case sensitive compare
	friend bool			operator!=( const lfStr &a, const lfStr &b );
	friend bool			operator!=( const lfStr &a, const char *b );
	friend bool			operator!=( const char *a, const lfStr &b );

						// case sensitive compare
	int					Cmp( const char *text ) const;
	int					Cmpn( const char *text, int n ) const;
	int					CmpPrefix( const char *text ) const;

						// case insensitive compare
	int					Icmp( const char *text ) const;
	int					Icmpn( const char *text, int n ) const;
	int					IcmpPrefix( const char *text ) const;

						// case insensitive compare ignoring color
	int					IcmpNoColor( const char *text ) const;

						// compares paths and makes sure folders come first
	int					IcmpPath( const char *text ) const;
	int					IcmpnPath( const char *text, int n ) const;
	int					IcmpPrefixPath( const char *text ) const;

	int					Length( void ) const;
	int					Allocated( void ) const;
	void				Empty( void );
	bool				IsEmpty( void ) const;
	void				Clear( void );
	void				Append( const char a );
	void				Append( const lfStr &text );
	void				Append( const char *text );
	void				Append( const char *text, int len );
	void				Insert( const char a, int index );
	void				Insert( const char *text, int index );
	void				ToLower( void );
	void				ToUpper( void );
	bool				IsNumeric( void ) const;
	bool				IsColor( void ) const;
	bool				HasLower( void ) const;
	bool				HasUpper( void ) const;
	int					LengthWithoutColors( void ) const;
	lfStr &				RemoveColors( void );
	void				CapLength( int );
	void				Fill( const char ch, int newlen );

	int					Find( const char c, int start = 0, int end = -1 ) const;
	int					Find( const char *text, bool casesensitive = true, int start = 0, int end = -1 ) const;
	bool				Filter( const char *filter, bool casesensitive ) const;
	int					Last( const char c ) const;						// return the index to the last occurance of 'c', returns -1 if not found
	const char *		Left( int len, lfStr &result ) const;			// store the leftmost 'len' characters in the result
	const char *		Right( int len, lfStr &result ) const;			// store the rightmost 'len' characters in the result
	const char *		Mid( int start, int len, lfStr &result ) const;	// store 'len' characters starting at 'start' in result
	lfStr				Left( int len ) const;							// return the leftmost 'len' characters
	lfStr				Right( int len ) const;							// return the rightmost 'len' characters
	lfStr				Mid( int start, int len ) const;				// return 'len' characters starting at 'start'
	void				StripLeading( const char c );					// strip char from front as many times as the char occurs
	void				StripLeading( const char *string );				// strip string from front as many times as the string occurs
	bool				StripLeadingOnce( const char *string );			// strip string from front just once if it occurs
	void				StripTrailing( const char c );					// strip char from end as many times as the char occurs
	void				StripTrailing( const char *string );			// strip string from end as many times as the string occurs
	bool				StripTrailingOnce( const char *string );		// strip string from end just once if it occurs
	void				Strip( const char c );							// strip char from front and end as many times as the char occurs
	void				Strip( const char *string );					// strip string from front and end as many times as the string occurs
	void				StripTrailingWhitespace( void );				// strip trailing white space characters
	lfStr &				StripQuotes( void );							// strip quotes around string
	void				Replace( const char *old, const char *nw );

	// file name methods
	int					FileNameHash( void ) const;						// hash key for the filename (skips extension)
	lfStr &				BackSlashesToSlashes( void );					// convert slashes
	lfStr &				SetFileExtension( const char *extension );		// set the given file extension
	lfStr &				StripFileExtension( void );						// remove any file extension
	lfStr &				StripAbsoluteFileExtension( void );				// remove any file extension looking from front (useful if there are multiple .'s)
	lfStr &				DefaultFileExtension( const char *extension );	// if there's no file extension use the default
	lfStr &				DefaultPath( const char *basepath );			// if there's no path use the default
	void				AppendPath( const char *text );					// append a partial path
	lfStr &				StripFilename( void );							// remove the filename from a path
	lfStr &				StripPath( void );								// remove the path from the filename
	void				ExtractFilePath( lfStr &dest ) const;			// copy the file path to another string
	void				ExtractFileName( lfStr &dest ) const;			// copy the filename to another string
	void				ExtractFileBase( lfStr &dest ) const;			// copy the filename minus the extension to another string
	void				ExtractFileExtension( lfStr &dest ) const;		// copy the file extension to another string
	bool				CheckExtension( const char *ext );

	// char * methods to replace library functions
	static int			Length( const char *s );
	static char *		ToLower( char *s );
	static char *		ToUpper( char *s );
	static bool			IsNumeric( const char *s );
	static bool			IsColor( const char *s );
	static bool			HasLower( const char *s );
	static bool			HasUpper( const char *s );
	static int			LengthWithoutColors( const char *s );
	static char *		RemoveColors( char *s );
	static int			Cmp( const char *s1, const char *s2 );
	static int			Cmpn( const char *s1, const char *s2, int n );
	static int			Icmp( const char *s1, const char *s2 );
	static int			Icmpn( const char *s1, const char *s2, int n );
	static int			IcmpNoColor( const char *s1, const char *s2 );
	static int			IcmpPath( const char *s1, const char *s2 );			// compares paths and makes sure folders come first
	static int			IcmpnPath( const char *s1, const char *s2, int n );	// compares paths and makes sure folders come first
	static void			Append( char *dest, int size, const char *src );
	static void			Copynz( char *dest, const char *src, int destsize );
	//static int			snPrintf( char *dest, int size, const char *fmt, ... ) id_attribute((format(printf,3,4)));
	static int			vsnPrintf( char *dest, int size, const char *fmt, va_list argptr );
	static int			FindChar( const char *str, const char c, int start = 0, int end = -1 );
	static int			FindText( const char *str, const char *text, bool casesensitive = true, int start = 0, int end = -1 );
	static bool			Filter( const char *filter, const char *name, bool casesensitive );
	static void			StripMediaName( const char *name, lfStr &mediaName );
	static bool			CheckExtension( const char *name, const char *ext );
	static const char *	FloatArrayToString( const float *array, const int length, const int precision );

	// hash keys
	static int			Hash( const char *string );
	static int			Hash( const char *string, int length );
	static int			IHash( const char *string );					// case insensitive
	static int			IHash( const char *string, int length );		// case insensitive

	// character methods
	static char			ToLower( char c );
	static char			ToUpper( char c );
	static bool			CharIsPrintable( int c );
	static bool			CharIsLower( int c );
	static bool			CharIsUpper( int c );
	static bool			CharIsAlpha( int c );
	static bool			CharIsNumeric( int c );
	static bool			CharIsNewLine( char c );
	static bool			CharIsTab( char c );
	static int			ColorIndex( int c );

	friend int			sprintf( lfStr &dest, const char *fmt, ... );
	friend int			vsprintf( lfStr &dest, const char *fmt, va_list ap );

	void				ReAllocate( int amount, bool keepold );				// reallocate string data buffer
	void				FreeData( void );									// free allocated string memory

						// format value in the given measurement with the best unit, returns the best unit
	int					BestUnit( const char *format, float value, Measure_t measure );
						// format value in the requested unit and measurement
	void				SetUnit( const char *format, float value, int unit, Measure_t measure );

	static void			InitMemory( void );
	static void			ShutdownMemory( void );
	static void			PurgeMemory( void );
	//static void			ShowMemoryUsage_f( const idCmdArgs &args );

	int					DynamicMemoryUsed() const;
	static lfStr		FormatNumber( int number );

protected:
	int					len;
	char *				data;
	int					alloced;
	char				baseBuffer[ STR_ALLOC_BASE ];

	void				Init( void );										// initialize string using base buffer
	void				EnsureAlloced( int amount, bool keepold = true );	// ensure string data buffer is large anough
};

//char *					va( const char *fmt, ... ) id_attribute((format(printf,1,2)));


ID_INLINE void lfStr::EnsureAlloced( int amount, bool keepold ) {
	if ( amount > alloced ) {
		ReAllocate( amount, keepold );
	}
}

ID_INLINE void lfStr::Init( void ) {
	len = 0;
	alloced = STR_ALLOC_BASE;
	data = baseBuffer;
	data[ 0 ] = '\0';
#ifdef ID_DEBUG_UNINITIALIZED_MEMORY
	memset( baseBuffer, 0, sizeof( baseBuffer ) );
#endif
}

ID_INLINE lfStr::lfStr( void ) {
	Init();
}

ID_INLINE lfStr::lfStr( const lfStr &text ) {
	int l;

	Init();
	l = text.Length();
	EnsureAlloced( l + 1 );
	strcpy( data, text.data );
	len = l;
}

ID_INLINE lfStr::lfStr( const lfStr &text, int start, int end ) {
	int i;
	int l;

	Init();
	if ( end > text.Length() ) {
		end = text.Length();
	}
	if ( start > text.Length() ) {
		start = text.Length();
	} else if ( start < 0 ) {
		start = 0;
	}

	l = end - start;
	if ( l < 0 ) {
		l = 0;
	}

	EnsureAlloced( l + 1 );

	for ( i = 0; i < l; i++ ) {
		data[ i ] = text[ start + i ];
	}

	data[ l ] = '\0';
	len = l;
}

ID_INLINE lfStr::lfStr( const char *text ) {
	int l;

	Init();
	if ( text ) {
		l = strlen( text );
		EnsureAlloced( l + 1 );
		strcpy( data, text );
		len = l;
	}
}

ID_INLINE lfStr::lfStr( const char *text, int start, int end ) {
	int i;
	int l = strlen( text );

	Init();
	if ( end > l ) {
		end = l;
	}
	if ( start > l ) {
		start = l;
	} else if ( start < 0 ) {
		start = 0;
	}

	l = end - start;
	if ( l < 0 ) {
		l = 0;
	}

	EnsureAlloced( l + 1 );

	for ( i = 0; i < l; i++ ) {
		data[ i ] = text[ start + i ];
	}

	data[ l ] = '\0';
	len = l;
}

ID_INLINE lfStr::lfStr( const bool b ) {
	Init();
	EnsureAlloced( 2 );
	data[ 0 ] = b ? '1' : '0';
	data[ 1 ] = '\0';
	len = 1;
}

ID_INLINE lfStr::lfStr( const char c ) {
	Init();
	EnsureAlloced( 2 );
	data[ 0 ] = c;
	data[ 1 ] = '\0';
	len = 1;
}

ID_INLINE lfStr::lfStr( const int i ) {
	char text[ 64 ];
	int l;

	Init();
	l = sprintf( text, "%d", i );
	EnsureAlloced( l + 1 );
	strcpy( data, text );
	len = l;
}

ID_INLINE lfStr::lfStr( const unsigned u ) {
	char text[ 64 ];
	int l;

	Init();
	l = sprintf( text, "%u", u );
	EnsureAlloced( l + 1 );
	strcpy( data, text );
	len = l;
}

ID_INLINE lfStr::lfStr( const float f ) {
	//char text[ 64 ];
	//int l;

	//Init();
	//l = idStr::snPrintf( text, sizeof( text ), "%f", f );
	//while( l > 0 && text[l-1] == '0' ) text[--l] = '\0';
	//while( l > 0 && text[l-1] == '.' ) text[--l] = '\0';
	//EnsureAlloced( l + 1 );
	//strcpy( data, text );
	//len = l;
}

ID_INLINE lfStr::~lfStr( void ) {
	FreeData();
}

ID_INLINE size_t lfStr::Size( void ) const {
	return sizeof( *this ) + Allocated();
}

ID_INLINE const char *lfStr::c_str( void ) const {
	return data;
}

ID_INLINE lfStr::operator const char *( void ) {
	return c_str();
}

ID_INLINE lfStr::operator const char *( void ) const {
	return c_str();
}

ID_INLINE char lfStr::operator[]( int index ) const {
	assert( ( index >= 0 ) && ( index <= len ) );
	return data[ index ];
}

ID_INLINE char &lfStr::operator[]( int index ) {
	assert( ( index >= 0 ) && ( index <= len ) );
	return data[ index ];
}

ID_INLINE void lfStr::operator=( const lfStr &text ) {
	int l;

	l = text.Length();
	EnsureAlloced( l + 1, false );
	memcpy( data, text.data, l );
	data[l] = '\0';
	len = l;
}

ID_INLINE lfStr operator+( const lfStr &a, const lfStr &b ) {
	lfStr result( a );
	result.Append( b );
	return result;
}

ID_INLINE lfStr operator+( const lfStr &a, const char *b ) {
	lfStr result( a );
	result.Append( b );
	return result;
}

ID_INLINE lfStr operator+( const char *a, const lfStr &b ) {
	lfStr result( a );
	result.Append( b );
	return result;
}

ID_INLINE lfStr operator+( const lfStr &a, const bool b ) {
	lfStr result( a );
	result.Append( b ? "true" : "false" );
	return result;
}

ID_INLINE lfStr operator+( const lfStr &a, const char b ) {
	lfStr result( a );
	result.Append( b );
	return result;
}

ID_INLINE lfStr operator+( const lfStr &a, const float b ) {
	char	text[ 64 ];
	lfStr	result( a );

	sprintf( text, "%f", b );
	result.Append( text );

	return result;
}

ID_INLINE lfStr operator+( const lfStr &a, const int b ) {
	char	text[ 64 ];
	lfStr	result( a );

	sprintf( text, "%d", b );
	result.Append( text );

	return result;
}

ID_INLINE lfStr operator+( const lfStr &a, const unsigned b ) {
	char	text[ 64 ];
	lfStr	result( a );

	sprintf( text, "%u", b );
	result.Append( text );

	return result;
}

ID_INLINE lfStr &lfStr::operator+=( const float a ) {
	char text[ 64 ];

	sprintf( text, "%f", a );
	Append( text );

	return *this;
}

ID_INLINE lfStr &lfStr::operator+=( const int a ) {
	char text[ 64 ];

	sprintf( text, "%d", a );
	Append( text );

	return *this;
}

ID_INLINE lfStr &lfStr::operator+=( const unsigned a ) {
	char text[ 64 ];

	sprintf( text, "%u", a );
	Append( text );

	return *this;
}

ID_INLINE lfStr &lfStr::operator+=( const lfStr &a ) {
	Append( a );
	return *this;
}

ID_INLINE lfStr &lfStr::operator+=( const char *a ) {
	Append( a );
	return *this;
}

ID_INLINE lfStr &lfStr::operator+=( const char a ) {
	Append( a );
	return *this;
}

ID_INLINE lfStr &lfStr::operator+=( const bool a ) {
	Append( a ? "true" : "false" );
	return *this;
}

ID_INLINE bool operator==( const lfStr &a, const lfStr &b ) {
	return ( !lfStr::Cmp( a.data, b.data ) );
}

ID_INLINE bool operator==( const lfStr &a, const char *b ) {
	assert( b );
	return ( !lfStr::Cmp( a.data, b ) );
}

ID_INLINE bool operator==( const char *a, const lfStr &b ) {
	assert( a );
	return ( !lfStr::Cmp( a, b.data ) );
}

ID_INLINE bool operator!=( const lfStr &a, const lfStr &b ) {
	return !( a == b );
}

ID_INLINE bool operator!=( const lfStr &a, const char *b ) {
	return !( a == b );
}

ID_INLINE bool operator!=( const char *a, const lfStr &b ) {
	return !( a == b );
}

ID_INLINE int lfStr::Cmp( const char *text ) const {
	assert( text );
	return lfStr::Cmp( data, text );
}

ID_INLINE int lfStr::Cmpn( const char *text, int n ) const {
	assert( text );
	return lfStr::Cmpn( data, text, n );
}

ID_INLINE int lfStr::CmpPrefix( const char *text ) const {
	assert( text );
	return lfStr::Cmpn( data, text, strlen( text ) );
}

ID_INLINE int lfStr::Icmp( const char *text ) const {
	assert( text );
	return lfStr::Icmp( data, text );
}

ID_INLINE int lfStr::Icmpn( const char *text, int n ) const {
	assert( text );
	return lfStr::Icmpn( data, text, n );
}

ID_INLINE int lfStr::IcmpPrefix( const char *text ) const {
	assert( text );
	return lfStr::Icmpn( data, text, strlen( text ) );
}

ID_INLINE int lfStr::IcmpNoColor( const char *text ) const {
	assert( text );
	return lfStr::IcmpNoColor( data, text );
}

ID_INLINE int lfStr::IcmpPath( const char *text ) const {
	assert( text );
	return lfStr::IcmpPath( data, text );
}

ID_INLINE int lfStr::IcmpnPath( const char *text, int n ) const {
	assert( text );
	return lfStr::IcmpnPath( data, text, n );
}

ID_INLINE int lfStr::IcmpPrefixPath( const char *text ) const {
	assert( text );
	return lfStr::IcmpnPath( data, text, strlen( text ) );
}

ID_INLINE int lfStr::Length( void ) const {
	return len;
}

ID_INLINE int lfStr::Allocated( void ) const {
	if ( data != baseBuffer ) {
		return alloced;
	} else {
		return 0;
	}
}

ID_INLINE void lfStr::Empty( void ) {
	EnsureAlloced( 1 );
	data[ 0 ] = '\0';
	len = 0;
}

ID_INLINE bool lfStr::IsEmpty( void ) const {
	return ( lfStr::Cmp( data, "" ) == 0 );
}

ID_INLINE void lfStr::Clear( void ) {
	FreeData();
	Init();
}

ID_INLINE void lfStr::Append( const char a ) {
	EnsureAlloced( len + 2 );
	data[ len ] = a;
	len++;
	data[ len ] = '\0';
}

ID_INLINE void lfStr::Append( const lfStr &text ) {
	int newLen;
	int i;

	newLen = len + text.Length();
	EnsureAlloced( newLen + 1 );
	for ( i = 0; i < text.len; i++ ) {
		data[ len + i ] = text[ i ];
	}
	len = newLen;
	data[ len ] = '\0';
}

ID_INLINE void lfStr::Append( const char *text ) {
	int newLen;
	int i;

	if ( text ) {
		newLen = len + strlen( text );
		EnsureAlloced( newLen + 1 );
		for ( i = 0; text[ i ]; i++ ) {
			data[ len + i ] = text[ i ];
		}
		len = newLen;
		data[ len ] = '\0';
	}
}

ID_INLINE void lfStr::Append( const char *text, int l ) {
	int newLen;
	int i;

	if ( text && l ) {
		newLen = len + l;
		EnsureAlloced( newLen + 1 );
		for ( i = 0; text[ i ] && i < l; i++ ) {
			data[ len + i ] = text[ i ];
		}
		len = newLen;
		data[ len ] = '\0';
	}
}

ID_INLINE void lfStr::Insert( const char a, int index ) {
	int i, l;

	if ( index < 0 ) {
		index = 0;
	} else if ( index > len ) {
		index = len;
	}

	l = 1;
	EnsureAlloced( len + l + 1 );
	for ( i = len; i >= index; i-- ) {
		data[i+l] = data[i];
	}
	data[index] = a;
	len++;
}

ID_INLINE void lfStr::Insert( const char *text, int index ) {
	int i, l;

	if ( index < 0 ) {
		index = 0;
	} else if ( index > len ) {
		index = len;
	}

	l = strlen( text );
	EnsureAlloced( len + l + 1 );
	for ( i = len; i >= index; i-- ) {
		data[i+l] = data[i];
	}
	for ( i = 0; i < l; i++ ) {
		data[index+i] = text[i];
	}
	len += l;
}

ID_INLINE void lfStr::ToLower( void ) {
	for (int i = 0; data[i]; i++ ) {
		if ( CharIsUpper( data[i] ) ) {
			data[i] += ( 'a' - 'A' );
		}
	}
}

ID_INLINE void lfStr::ToUpper( void ) {
	for (int i = 0; data[i]; i++ ) {
		if ( CharIsLower( data[i] ) ) {
			data[i] -= ( 'a' - 'A' );
		}
	}
}

ID_INLINE bool lfStr::IsNumeric( void ) const {
	return lfStr::IsNumeric( data );
}

ID_INLINE bool lfStr::IsColor( void ) const {
	return lfStr::IsColor( data );
}

ID_INLINE bool lfStr::HasLower( void ) const {
	return lfStr::HasLower( data );
}

ID_INLINE bool lfStr::HasUpper( void ) const {
	return lfStr::HasUpper( data );
}

ID_INLINE lfStr &lfStr::RemoveColors( void ) {
	lfStr::RemoveColors( data );
	len = Length( data );
	return *this;
}

ID_INLINE int lfStr::LengthWithoutColors( void ) const {
	return lfStr::LengthWithoutColors( data );
}

ID_INLINE void lfStr::CapLength( int newlen ) {
	if ( len <= newlen ) {
		return;
	}
	data[ newlen ] = 0;
	len = newlen;
}

ID_INLINE void lfStr::Fill( const char ch, int newlen ) {
	EnsureAlloced( newlen + 1 );
	len = newlen;
	memset( data, ch, len );
	data[ len ] = 0;
}

ID_INLINE int lfStr::Find( const char c, int start, int end ) const {
	if ( end == -1 ) {
		end = len;
	}
	return lfStr::FindChar( data, c, start, end );
}

ID_INLINE int lfStr::Find( const char *text, bool casesensitive, int start, int end ) const {
	if ( end == -1 ) {
		end = len;
	}
	return lfStr::FindText( data, text, casesensitive, start, end );
}

ID_INLINE bool lfStr::Filter( const char *filter, bool casesensitive ) const {
	return lfStr::Filter( filter, data, casesensitive );
}

ID_INLINE const char *lfStr::Left( int len, lfStr &result ) const {
	return Mid( 0, len, result );
}

ID_INLINE const char *lfStr::Right( int len, lfStr &result ) const {
	if ( len >= Length() ) {
		result = *this;
		return result;
	}
	return Mid( Length() - len, len, result );
}

ID_INLINE lfStr lfStr::Left( int len ) const {
	return Mid( 0, len );
}

ID_INLINE lfStr lfStr::Right( int len ) const {
	if ( len >= Length() ) {
		return *this;
	}
	return Mid( Length() - len, len );
}

ID_INLINE void lfStr::Strip( const char c ) {
	StripLeading( c );
	StripTrailing( c );
}

ID_INLINE void lfStr::Strip( const char *string ) {
	StripLeading( string );
	StripTrailing( string );
}

ID_INLINE bool lfStr::CheckExtension( const char *ext ) {
	return lfStr::CheckExtension( data, ext );
}

ID_INLINE int lfStr::Length( const char *s ) {
	int i;
	for ( i = 0; s[i]; i++ ) {}
	return i;
}

ID_INLINE char *lfStr::ToLower( char *s ) {
	for ( int i = 0; s[i]; i++ ) {
		if ( CharIsUpper( s[i] ) ) {
			s[i] += ( 'a' - 'A' );
		}
	}
	return s;
}

ID_INLINE char *lfStr::ToUpper( char *s ) {
	for ( int i = 0; s[i]; i++ ) {
		if ( CharIsLower( s[i] ) ) {
			s[i] -= ( 'a' - 'A' );
		}
	}
	return s;
}

ID_INLINE int lfStr::Hash( const char *string ) {
	int i, hash = 0;
	for ( i = 0; *string != '\0'; i++ ) {
		hash += ( *string++ ) * ( i + 119 );
	}
	return hash;
}

ID_INLINE int lfStr::Hash( const char *string, int length ) {
	int i, hash = 0;
	for ( i = 0; i < length; i++ ) {
		hash += ( *string++ ) * ( i + 119 );
	}
	return hash;
}

ID_INLINE int lfStr::IHash( const char *string ) {
	int i, hash = 0;
	for( i = 0; *string != '\0'; i++ ) {
		hash += ToLower( *string++ ) * ( i + 119 );
	}
	return hash;
}

ID_INLINE int lfStr::IHash( const char *string, int length ) {
	int i, hash = 0;
	for ( i = 0; i < length; i++ ) {
		hash += ToLower( *string++ ) * ( i + 119 );
	}
	return hash;
}

ID_INLINE bool lfStr::IsColor( const char *s ) {
	return ( s[0] == C_COLOR_ESCAPE && s[1] != '\0' && s[1] != ' ' );
}

ID_INLINE char lfStr::ToLower( char c ) {
	if ( c <= 'Z' && c >= 'A' ) {
		return ( c + ( 'a' - 'A' ) );
	}
	return c;
}

ID_INLINE char lfStr::ToUpper( char c ) {
	if ( c >= 'a' && c <= 'z' ) {
		return ( c - ( 'a' - 'A' ) );
	}
	return c;
}

ID_INLINE bool lfStr::CharIsPrintable( int c ) {
	// test for regular ascii and western European high-ascii chars
	return ( c >= 0x20 && c <= 0x7E ) || ( c >= 0xA1 && c <= 0xFF );
}

ID_INLINE bool lfStr::CharIsLower( int c ) {
	// test for regular ascii and western European high-ascii chars
	return ( c >= 'a' && c <= 'z' ) || ( c >= 0xE0 && c <= 0xFF );
}

ID_INLINE bool lfStr::CharIsUpper( int c ) {
	// test for regular ascii and western European high-ascii chars
	return ( c <= 'Z' && c >= 'A' ) || ( c >= 0xC0 && c <= 0xDF );
}

ID_INLINE bool lfStr::CharIsAlpha( int c ) {
	// test for regular ascii and western European high-ascii chars
	return ( ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) ||
			 ( c >= 0xC0 && c <= 0xFF ) );
}

ID_INLINE bool lfStr::CharIsNumeric( int c ) {
	return ( c <= '9' && c >= '0' );
}

ID_INLINE bool lfStr::CharIsNewLine( char c ) {
	return ( c == '\n' || c == '\r' || c == '\v' );
}

ID_INLINE bool lfStr::CharIsTab( char c ) {
	return ( c == '\t' );
}

ID_INLINE int lfStr::ColorIndex( int c ) {
	return ( c & 15 );
}

ID_INLINE int lfStr::DynamicMemoryUsed() const {
	return ( data == baseBuffer ) ? 0 : alloced;
}

#endif /* !__STR_H__ */
