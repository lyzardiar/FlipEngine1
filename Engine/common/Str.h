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
#define strcmp			strings::Cmp		// use_idStr_Cmp
#define strncmp			use_idStr_Cmpn

#if defined( StrCmpN )
#undef StrCmpN
#endif
#define StrCmpN			use_idStr_Cmpn

#if defined( strcmpi )
#undef strcmpi
#endif
#define strcmpi			use_idStr_Icmp

#if defined( StrCmpI )
#undef StrCmpI
#endif
#define StrCmpI			use_idStr_Icmp

#if defined( StrCmpNI )
#undef StrCmpNI
#endif
#define StrCmpNI		use_idStr_Icmpn

#define stricmp			strings::Icmp		// use_idStr_Icmp
#define _stricmp		use_idStr_Icmp
#define strcasecmp		use_idStr_Icmp
#define strnicmp		use_idStr_Icmpn
#define _strnicmp		use_idStr_Icmpn
#define _memicmp		use_idStr_Icmpn
#define snprintf		use_idStr_snPrintf
#define _snprintf		use_idStr_snPrintf
#define vsnprintf		use_idStr_vsnPrintf
#define _vsnprintf		use_idStr_vsnPrintf

class idVec4;

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

class strings {

public:
						strings( void );
						strings( const strings &text );
						strings( const strings &text, int start, int end );
						strings( const char *text );
						strings( const char *text, int start, int end );
						explicit strings( const bool b );
						explicit strings( const char c );
						explicit strings( const int i );
						explicit strings( const unsigned u );
						explicit strings( const float f );
						~strings( void );

	size_t				Size( void ) const;
	const char *		c_str( void ) const;
	operator			const char *( void ) const;
	operator			const char *( void );

	char				operator[]( int index ) const;
	char &				operator[]( int index );

	void				operator=( const strings &text );
	void				operator=( const char *text );

	friend strings		operator+( const strings &a, const strings &b );
	friend strings		operator+( const strings &a, const char *b );
	friend strings		operator+( const char *a, const strings &b );

	friend strings		operator+( const strings &a, const float b );
	friend strings		operator+( const strings &a, const int b );
	friend strings		operator+( const strings &a, const unsigned b );
	friend strings		operator+( const strings &a, const bool b );
	friend strings		operator+( const strings &a, const char b );

	strings &				operator+=( const strings &a );
	strings &				operator+=( const char *a );
	strings &				operator+=( const float a );
	strings &				operator+=( const char a );
	strings &				operator+=( const int a );
	strings &				operator+=( const unsigned a );
	strings &				operator+=( const bool a );

						// case sensitive compare
	friend bool			operator==( const strings &a, const strings &b );
	friend bool			operator==( const strings &a, const char *b );
	friend bool			operator==( const char *a, const strings &b );

						// case sensitive compare
	friend bool			operator!=( const strings &a, const strings &b );
	friend bool			operator!=( const strings &a, const char *b );
	friend bool			operator!=( const char *a, const strings &b );

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
	void				Append( const strings &text );
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
	strings &				RemoveColors( void );
	void				CapLength( int );
	void				Fill( const char ch, int newlen );

	int					Find( const char c, int start = 0, int end = -1 ) const;
	int					Find( const char *text, bool casesensitive = true, int start = 0, int end = -1 ) const;
	bool				Filter( const char *filter, bool casesensitive ) const;
	int					Last( const char c ) const;						// return the index to the last occurance of 'c', returns -1 if not found
	const char *		Left( int len, strings &result ) const;			// store the leftmost 'len' characters in the result
	const char *		Right( int len, strings &result ) const;			// store the rightmost 'len' characters in the result
	const char *		Mid( int start, int len, strings &result ) const;	// store 'len' characters starting at 'start' in result
	strings				Left( int len ) const;							// return the leftmost 'len' characters
	strings				Right( int len ) const;							// return the rightmost 'len' characters
	strings				Mid( int start, int len ) const;				// return 'len' characters starting at 'start'
	void				StripLeading( const char c );					// strip char from front as many times as the char occurs
	void				StripLeading( const char *string );				// strip string from front as many times as the string occurs
	bool				StripLeadingOnce( const char *string );			// strip string from front just once if it occurs
	void				StripTrailing( const char c );					// strip char from end as many times as the char occurs
	void				StripTrailing( const char *string );			// strip string from end as many times as the string occurs
	bool				StripTrailingOnce( const char *string );		// strip string from end just once if it occurs
	void				Strip( const char c );							// strip char from front and end as many times as the char occurs
	void				Strip( const char *string );					// strip string from front and end as many times as the string occurs
	void				StripTrailingWhitespace( void );				// strip trailing white space characters
	strings &				StripQuotes( void );							// strip quotes around string
	void				Replace( const char *old, const char *nw );

	// file name methods
	int					FileNameHash( void ) const;						// hash key for the filename (skips extension)
	strings &				BackSlashesToSlashes( void );					// convert slashes
	strings &				SetFileExtension( const char *extension );		// set the given file extension
	strings &				StripFileExtension( void );						// remove any file extension
	strings &				StripAbsoluteFileExtension( void );				// remove any file extension looking from front (useful if there are multiple .'s)
	strings &				DefaultFileExtension( const char *extension );	// if there's no file extension use the default
	strings &				DefaultPath( const char *basepath );			// if there's no path use the default
	void				AppendPath( const char *text );					// append a partial path
	strings &				StripFilename( void );							// remove the filename from a path
	strings &				StripPath( void );								// remove the path from the filename
	void				ExtractFilePath( strings &dest ) const;			// copy the file path to another string
	void				ExtractFileName( strings &dest ) const;			// copy the filename to another string
	void				ExtractFileBase( strings &dest ) const;			// copy the filename minus the extension to another string
	void				ExtractFileExtension( strings &dest ) const;		// copy the file extension to another string
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
	static void			StripMediaName( const char *name, strings &mediaName );
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
	static idVec4 &		ColorForIndex( int i );

	friend int			sprintf( strings &dest, const char *fmt, ... );
	friend int			vsprintf( strings &dest, const char *fmt, va_list ap );

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
	static strings		FormatNumber( int number );

protected:
	int					len;
	char *				data;
	int					alloced;
	char				baseBuffer[ STR_ALLOC_BASE ];

	void				Init( void );										// initialize string using base buffer
	void				EnsureAlloced( int amount, bool keepold = true );	// ensure string data buffer is large anough
};

//char *					va( const char *fmt, ... ) id_attribute((format(printf,1,2)));


ID_INLINE void strings::EnsureAlloced( int amount, bool keepold ) {
	if ( amount > alloced ) {
		ReAllocate( amount, keepold );
	}
}

ID_INLINE void strings::Init( void ) {
	len = 0;
	alloced = STR_ALLOC_BASE;
	data = baseBuffer;
	data[ 0 ] = '\0';
#ifdef ID_DEBUG_UNINITIALIZED_MEMORY
	memset( baseBuffer, 0, sizeof( baseBuffer ) );
#endif
}

ID_INLINE strings::strings( void ) {
	Init();
}

ID_INLINE strings::strings( const strings &text ) {
	int l;

	Init();
	l = text.Length();
	EnsureAlloced( l + 1 );
	strcpy( data, text.data );
	len = l;
}

ID_INLINE strings::strings( const strings &text, int start, int end ) {
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

ID_INLINE strings::strings( const char *text ) {
	int l;

	Init();
	if ( text ) {
		l = strlen( text );
		EnsureAlloced( l + 1 );
		strcpy( data, text );
		len = l;
	}
}

ID_INLINE strings::strings( const char *text, int start, int end ) {
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

ID_INLINE strings::strings( const bool b ) {
	Init();
	EnsureAlloced( 2 );
	data[ 0 ] = b ? '1' : '0';
	data[ 1 ] = '\0';
	len = 1;
}

ID_INLINE strings::strings( const char c ) {
	Init();
	EnsureAlloced( 2 );
	data[ 0 ] = c;
	data[ 1 ] = '\0';
	len = 1;
}

ID_INLINE strings::strings( const int i ) {
	char text[ 64 ];
	int l;

	Init();
	l = sprintf( text, "%d", i );
	EnsureAlloced( l + 1 );
	strcpy( data, text );
	len = l;
}

ID_INLINE strings::strings( const unsigned u ) {
	char text[ 64 ];
	int l;

	Init();
	l = sprintf( text, "%u", u );
	EnsureAlloced( l + 1 );
	strcpy( data, text );
	len = l;
}

ID_INLINE strings::strings( const float f ) {
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

ID_INLINE strings::~strings( void ) {
	FreeData();
}

ID_INLINE size_t strings::Size( void ) const {
	return sizeof( *this ) + Allocated();
}

ID_INLINE const char *strings::c_str( void ) const {
	return data;
}

ID_INLINE strings::operator const char *( void ) {
	return c_str();
}

ID_INLINE strings::operator const char *( void ) const {
	return c_str();
}

ID_INLINE char strings::operator[]( int index ) const {
	assert( ( index >= 0 ) && ( index <= len ) );
	return data[ index ];
}

ID_INLINE char &strings::operator[]( int index ) {
	assert( ( index >= 0 ) && ( index <= len ) );
	return data[ index ];
}

ID_INLINE void strings::operator=( const strings &text ) {
	int l;

	l = text.Length();
	EnsureAlloced( l + 1, false );
	memcpy( data, text.data, l );
	data[l] = '\0';
	len = l;
}

ID_INLINE strings operator+( const strings &a, const strings &b ) {
	strings result( a );
	result.Append( b );
	return result;
}

ID_INLINE strings operator+( const strings &a, const char *b ) {
	strings result( a );
	result.Append( b );
	return result;
}

ID_INLINE strings operator+( const char *a, const strings &b ) {
	strings result( a );
	result.Append( b );
	return result;
}

ID_INLINE strings operator+( const strings &a, const bool b ) {
	strings result( a );
	result.Append( b ? "true" : "false" );
	return result;
}

ID_INLINE strings operator+( const strings &a, const char b ) {
	strings result( a );
	result.Append( b );
	return result;
}

ID_INLINE strings operator+( const strings &a, const float b ) {
	char	text[ 64 ];
	strings	result( a );

	sprintf( text, "%f", b );
	result.Append( text );

	return result;
}

ID_INLINE strings operator+( const strings &a, const int b ) {
	char	text[ 64 ];
	strings	result( a );

	sprintf( text, "%d", b );
	result.Append( text );

	return result;
}

ID_INLINE strings operator+( const strings &a, const unsigned b ) {
	char	text[ 64 ];
	strings	result( a );

	sprintf( text, "%u", b );
	result.Append( text );

	return result;
}

ID_INLINE strings &strings::operator+=( const float a ) {
	char text[ 64 ];

	sprintf( text, "%f", a );
	Append( text );

	return *this;
}

ID_INLINE strings &strings::operator+=( const int a ) {
	char text[ 64 ];

	sprintf( text, "%d", a );
	Append( text );

	return *this;
}

ID_INLINE strings &strings::operator+=( const unsigned a ) {
	char text[ 64 ];

	sprintf( text, "%u", a );
	Append( text );

	return *this;
}

ID_INLINE strings &strings::operator+=( const strings &a ) {
	Append( a );
	return *this;
}

ID_INLINE strings &strings::operator+=( const char *a ) {
	Append( a );
	return *this;
}

ID_INLINE strings &strings::operator+=( const char a ) {
	Append( a );
	return *this;
}

ID_INLINE strings &strings::operator+=( const bool a ) {
	Append( a ? "true" : "false" );
	return *this;
}

ID_INLINE bool operator==( const strings &a, const strings &b ) {
	return ( !strings::Cmp( a.data, b.data ) );
}

ID_INLINE bool operator==( const strings &a, const char *b ) {
	assert( b );
	return ( !strings::Cmp( a.data, b ) );
}

ID_INLINE bool operator==( const char *a, const strings &b ) {
	assert( a );
	return ( !strings::Cmp( a, b.data ) );
}

ID_INLINE bool operator!=( const strings &a, const strings &b ) {
	return !( a == b );
}

ID_INLINE bool operator!=( const strings &a, const char *b ) {
	return !( a == b );
}

ID_INLINE bool operator!=( const char *a, const strings &b ) {
	return !( a == b );
}

ID_INLINE int strings::Cmp( const char *text ) const {
	assert( text );
	return strings::Cmp( data, text );
}

ID_INLINE int strings::Cmpn( const char *text, int n ) const {
	assert( text );
	return strings::Cmpn( data, text, n );
}

ID_INLINE int strings::CmpPrefix( const char *text ) const {
	assert( text );
	return strings::Cmpn( data, text, strlen( text ) );
}

ID_INLINE int strings::Icmp( const char *text ) const {
	assert( text );
	return strings::Icmp( data, text );
}

ID_INLINE int strings::Icmpn( const char *text, int n ) const {
	assert( text );
	return strings::Icmpn( data, text, n );
}

ID_INLINE int strings::IcmpPrefix( const char *text ) const {
	assert( text );
	return strings::Icmpn( data, text, strlen( text ) );
}

ID_INLINE int strings::IcmpNoColor( const char *text ) const {
	assert( text );
	return strings::IcmpNoColor( data, text );
}

ID_INLINE int strings::IcmpPath( const char *text ) const {
	assert( text );
	return strings::IcmpPath( data, text );
}

ID_INLINE int strings::IcmpnPath( const char *text, int n ) const {
	assert( text );
	return strings::IcmpnPath( data, text, n );
}

ID_INLINE int strings::IcmpPrefixPath( const char *text ) const {
	assert( text );
	return strings::IcmpnPath( data, text, strlen( text ) );
}

ID_INLINE int strings::Length( void ) const {
	return len;
}

ID_INLINE int strings::Allocated( void ) const {
	if ( data != baseBuffer ) {
		return alloced;
	} else {
		return 0;
	}
}

ID_INLINE void strings::Empty( void ) {
	EnsureAlloced( 1 );
	data[ 0 ] = '\0';
	len = 0;
}

ID_INLINE bool strings::IsEmpty( void ) const {
	return ( strings::Cmp( data, "" ) == 0 );
}

ID_INLINE void strings::Clear( void ) {
	FreeData();
	Init();
}

ID_INLINE void strings::Append( const char a ) {
	EnsureAlloced( len + 2 );
	data[ len ] = a;
	len++;
	data[ len ] = '\0';
}

ID_INLINE void strings::Append( const strings &text ) {
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

ID_INLINE void strings::Append( const char *text ) {
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

ID_INLINE void strings::Append( const char *text, int l ) {
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

ID_INLINE void strings::Insert( const char a, int index ) {
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

ID_INLINE void strings::Insert( const char *text, int index ) {
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

ID_INLINE void strings::ToLower( void ) {
	for (int i = 0; data[i]; i++ ) {
		if ( CharIsUpper( data[i] ) ) {
			data[i] += ( 'a' - 'A' );
		}
	}
}

ID_INLINE void strings::ToUpper( void ) {
	for (int i = 0; data[i]; i++ ) {
		if ( CharIsLower( data[i] ) ) {
			data[i] -= ( 'a' - 'A' );
		}
	}
}

ID_INLINE bool strings::IsNumeric( void ) const {
	return strings::IsNumeric( data );
}

ID_INLINE bool strings::IsColor( void ) const {
	return strings::IsColor( data );
}

ID_INLINE bool strings::HasLower( void ) const {
	return strings::HasLower( data );
}

ID_INLINE bool strings::HasUpper( void ) const {
	return strings::HasUpper( data );
}

ID_INLINE strings &strings::RemoveColors( void ) {
	strings::RemoveColors( data );
	len = Length( data );
	return *this;
}

ID_INLINE int strings::LengthWithoutColors( void ) const {
	return strings::LengthWithoutColors( data );
}

ID_INLINE void strings::CapLength( int newlen ) {
	if ( len <= newlen ) {
		return;
	}
	data[ newlen ] = 0;
	len = newlen;
}

ID_INLINE void strings::Fill( const char ch, int newlen ) {
	EnsureAlloced( newlen + 1 );
	len = newlen;
	memset( data, ch, len );
	data[ len ] = 0;
}

ID_INLINE int strings::Find( const char c, int start, int end ) const {
	if ( end == -1 ) {
		end = len;
	}
	return strings::FindChar( data, c, start, end );
}

ID_INLINE int strings::Find( const char *text, bool casesensitive, int start, int end ) const {
	if ( end == -1 ) {
		end = len;
	}
	return strings::FindText( data, text, casesensitive, start, end );
}

ID_INLINE bool strings::Filter( const char *filter, bool casesensitive ) const {
	return strings::Filter( filter, data, casesensitive );
}

ID_INLINE const char *strings::Left( int len, strings &result ) const {
	return Mid( 0, len, result );
}

ID_INLINE const char *strings::Right( int len, strings &result ) const {
	if ( len >= Length() ) {
		result = *this;
		return result;
	}
	return Mid( Length() - len, len, result );
}

ID_INLINE strings strings::Left( int len ) const {
	return Mid( 0, len );
}

ID_INLINE strings strings::Right( int len ) const {
	if ( len >= Length() ) {
		return *this;
	}
	return Mid( Length() - len, len );
}

ID_INLINE void strings::Strip( const char c ) {
	StripLeading( c );
	StripTrailing( c );
}

ID_INLINE void strings::Strip( const char *string ) {
	StripLeading( string );
	StripTrailing( string );
}

ID_INLINE bool strings::CheckExtension( const char *ext ) {
	return strings::CheckExtension( data, ext );
}

ID_INLINE int strings::Length( const char *s ) {
	int i;
	for ( i = 0; s[i]; i++ ) {}
	return i;
}

ID_INLINE char *strings::ToLower( char *s ) {
	for ( int i = 0; s[i]; i++ ) {
		if ( CharIsUpper( s[i] ) ) {
			s[i] += ( 'a' - 'A' );
		}
	}
	return s;
}

ID_INLINE char *strings::ToUpper( char *s ) {
	for ( int i = 0; s[i]; i++ ) {
		if ( CharIsLower( s[i] ) ) {
			s[i] -= ( 'a' - 'A' );
		}
	}
	return s;
}

ID_INLINE int strings::Hash( const char *string ) {
	int i, hash = 0;
	for ( i = 0; *string != '\0'; i++ ) {
		hash += ( *string++ ) * ( i + 119 );
	}
	return hash;
}

ID_INLINE int strings::Hash( const char *string, int length ) {
	int i, hash = 0;
	for ( i = 0; i < length; i++ ) {
		hash += ( *string++ ) * ( i + 119 );
	}
	return hash;
}

ID_INLINE int strings::IHash( const char *string ) {
	int i, hash = 0;
	for( i = 0; *string != '\0'; i++ ) {
		hash += ToLower( *string++ ) * ( i + 119 );
	}
	return hash;
}

ID_INLINE int strings::IHash( const char *string, int length ) {
	int i, hash = 0;
	for ( i = 0; i < length; i++ ) {
		hash += ToLower( *string++ ) * ( i + 119 );
	}
	return hash;
}

ID_INLINE bool strings::IsColor( const char *s ) {
	return ( s[0] == C_COLOR_ESCAPE && s[1] != '\0' && s[1] != ' ' );
}

ID_INLINE char strings::ToLower( char c ) {
	if ( c <= 'Z' && c >= 'A' ) {
		return ( c + ( 'a' - 'A' ) );
	}
	return c;
}

ID_INLINE char strings::ToUpper( char c ) {
	if ( c >= 'a' && c <= 'z' ) {
		return ( c - ( 'a' - 'A' ) );
	}
	return c;
}

ID_INLINE bool strings::CharIsPrintable( int c ) {
	// test for regular ascii and western European high-ascii chars
	return ( c >= 0x20 && c <= 0x7E ) || ( c >= 0xA1 && c <= 0xFF );
}

ID_INLINE bool strings::CharIsLower( int c ) {
	// test for regular ascii and western European high-ascii chars
	return ( c >= 'a' && c <= 'z' ) || ( c >= 0xE0 && c <= 0xFF );
}

ID_INLINE bool strings::CharIsUpper( int c ) {
	// test for regular ascii and western European high-ascii chars
	return ( c <= 'Z' && c >= 'A' ) || ( c >= 0xC0 && c <= 0xDF );
}

ID_INLINE bool strings::CharIsAlpha( int c ) {
	// test for regular ascii and western European high-ascii chars
	return ( ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) ||
			 ( c >= 0xC0 && c <= 0xFF ) );
}

ID_INLINE bool strings::CharIsNumeric( int c ) {
	return ( c <= '9' && c >= '0' );
}

ID_INLINE bool strings::CharIsNewLine( char c ) {
	return ( c == '\n' || c == '\r' || c == '\v' );
}

ID_INLINE bool strings::CharIsTab( char c ) {
	return ( c == '\t' );
}

ID_INLINE int strings::ColorIndex( int c ) {
	return ( c & 15 );
}

ID_INLINE int strings::DynamicMemoryUsed() const {
	return ( data == baseBuffer ) ? 0 : alloced;
}

#endif /* !__STR_H__ */
