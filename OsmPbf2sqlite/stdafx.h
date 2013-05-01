// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef WIN32
    #define _UNICODE
#endif

#define _CRT_SECURE_NO_WARNINGS 1
//#define _HAS_ITERATOR_DEBUGGING 0

#ifdef _MSC_VER
#include "targetver.h"
#endif

#include <stdio.h>

#include <assert.h>

#ifdef WIN32
 #include <tchar.h>
 #include <Windows.h>
 //#include <atlbase.h>
// #include <atlstr.h>
#endif

#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/filesystem.hpp>
#include <boost/timer/timer.hpp>

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
#include <stdlib.h>


#define ZLIB_CONST 1

//#include "../Alien/sqlite3/sqlite3.h"
#define SQLITE_OMIT_LOAD_EXTENSION
#include <sqlite3.h>



#ifdef _DEBUG
#define ASSERT(a) assert(a)
#else
#define ASSERT(a)
#endif //

#include <zlib.h>
#ifndef z_const
	#define z_const
#endif

#ifdef WIN32

  	#define ATOMIC_INT volatile LONG
  	#define INTERLOCKED_INCREMENT(a) InterlockedIncrement(&a)
    #define INTERLOCKED_GET(a) (a)
    #define INTERLOCKED_SET(a,b)  a=b





#else
//piouepvg
	#include <netinet/in.h>
	#include <stddef.h>
	#include <boost/atomic.hpp>
	#define __int64 long long int
	#define __int32 int
	#define _T(a) a
    #define MAX_PATH 512
    #define _tcsrchr strrchr
	#define _TCHAR char
	#define _tfopen fopen
    #define WM_QUIT 10
    #define WM_USER 100
    #define WINAPI
    #define _tmain main


    inline void ZeroMemory(void* pBuf,size_t sz)
    {

        ASSERT(sz%4=0);
        int nC=sz/4;
        std::fill((int*)pBuf,((int*)pBuf+nC),0);
//        memset(pBuf,sz,0);
      //  assert(*(int*)pBuf==0);
    }

    #define ATOMIC_INT boost::atomic<LONG>
    #define INTERLOCKED_INCREMENT(a)  a.fetch_add(1,  boost::memory_order_relaxed)
    #define INTERLOCKED_GET(a) a.load(boost::memory_order_relaxed)
    #define INTERLOCKED_SET(a,b)  a.store(b,boost::memory_order_relaxed)


#endif



#ifdef _MSC_VER

//piouepvg
	// #include <atlbase.h>
// #include <atlstr.h>

    #define __TLS __declspec(thread)
	#define MIN min
	#define MAX max
#else
	#define MIN std::min
	#define MAX std::max

	#define __int8 char
	#define _int32 int


	#define _THROW0()	throw ()

	#define LONG long
    #define DWORD unsigned
    #define HANDLE unsigned
    #define UINT unsigned
    #define LPVOID void*
   // #define CComCritSecLock  boost::lock_guard

    #define DeleteFileW(a)
    #define __TLS __thread



    #ifndef z_const
        #define z_const
    #endif


    #define _countof(a) (sizeof(a)/sizeof(a[0]))
    #define CP_ACP 0
    #define LPCSTR const char*

#endif


#include "ConsoleOutput.h"


//#include "../alien/sqlite-amalgamation-3071502/sqlite3.h"


// TODO: reference additional headers your program requires here
