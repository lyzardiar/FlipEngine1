#ifndef __PRECOMPILED_H__
#define __PRECOMPILED_H__

#ifdef __cplusplus

//-----------------------------------------------------

#define ID_TIME_T time_t

#ifdef _WIN32

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// prevent auto literal to string conversion

#ifndef _D3SDK
#ifndef GAME_DLL

//#define WINVER				0x501

#define DIRECTINPUT_VERSION  0x0800			// was 0x0700 with the old mssdk
#define DIRECTSOUND_VERSION  0x0800

#endif /* !GAME_DLL */
#endif /* !_D3SDK */

#pragma warning(disable : 4100)				// unreferenced formal parameter
#pragma warning(disable : 4244)				// conversion to smaller type, possible loss of data
#pragma warning(disable : 4714)				// function marked as __forceinline not inlined
#pragma warning(disable : 4996)				// unsafe string operations

#include <malloc.h>							// no malloc.h on mac or unix
#undef FindText								// stupid namespace poluting Microsoft monkeys

#endif /* _WIN32 */

//-----------------------------------------------------

#if !defined( _DEBUG ) && !defined( NDEBUG )
	// don't generate asserts
	#define NDEBUG
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>

typedef unsigned char byte;

//-----------------------------------------------------

#endif	/* __cplusplus */

#endif /* !__PRECOMPILED_H__ */
