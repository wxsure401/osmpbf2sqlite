// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _CRT_SECURE_NO_WARNINGS 1
//#define _HAS_ITERATOR_DEBUGGING 0
#include "targetver.h"

#include <stdio.h>

#include <assert.h>

#ifdef WIN32
 #include <tchar.h>
 #include <atlbase.h>
 #include <atlstr.h>
#endif

#include <boost/thread/thread.hpp>
#include <boost/filesystem.hpp>
/*#include <string>


inline bool operator< (const std::string &s1,const std::string &s2)
{
	return s1.compare(s2)<0;
}
*/
#include <string>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

#define ZLIB_CONST 1
#include "../Alien/sqlite3/sqlite3.h"

#ifdef WIN32
	#include "../Alien/zlib127/include/zlib.h"

#else
	#include <zlib.h>
	#include <netinet/in.h>
	#include <stddef.h>
    #define __int8 char
	#define _int32 int
	#define __int32 int
	#define __int64 long long int
	#define _THROW0()	throw ()
	#define _TCHAR char
	#define _T(a) a
	#define LONG long
	#define _tfopen fopen
    #define CComAutoCriticalSection boost::mutex
    #define DWORD unsigned
    #define HANDLE unsigned
    #define UINT unsigned
    #define LPVOID void*
    #define WM_USER 100
    #define WINAPI
    #define CComCritSecLock  boost::lock_guard
    #define MAX_PATH 512
    #define _tcsrchr strrchr
    #define DeleteFileW(a)

	//typedef int _int32
	//typedef int __int32
#endif

#ifdef _DEBUG
#define ASSERT(a) assert(a)
#else
#define ASSERT(a)

#endif //

#include "ConsoleOutput.h"


//#include "../alien/sqlite-amalgamation-3071502/sqlite3.h"


// TODO: reference additional headers your program requires here
