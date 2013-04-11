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

#define ZLIB_CONST 1

//#include "../Alien/sqlite3/sqlite3.h"
#include <sqlite3.h>



#ifdef _DEBUG
#define ASSERT(a) assert(a)
#else
#define ASSERT(a)
#endif //


#ifdef WIN32
	#include "../Alien/zlib127/include/zlib.h"
    #define __TLS __declspec(thread)
  	#define ATOMIC_INT volatile LONG
  	#define INTERLOCKED_INCREMENT(a) InterlockedIncrement(&a)
    #define INTERLOCKED_GET(a) (a)
    #define INTERLOCKED_SET(a,b)  a=b
	#define MIN min
	#define MAX max

#else
	#include <zlib.h>
	#include <netinet/in.h>
	#include <stddef.h>
	#include <boost/atomic.hpp>
    #define __int8 char
	#define _int32 int
	#define __int32 int
	#define __int64 long long int
	#define _THROW0()	throw ()
	#define _TCHAR const char
	#define _T(a) a
	#define LONG long
	#define _tfopen fopen
    #define DWORD unsigned
    #define HANDLE unsigned
    #define UINT unsigned
    #define LPVOID void*
    #define WM_QUIT 10
    #define WM_USER 100
    #define WINAPI
   // #define CComCritSecLock  boost::lock_guard
    #define MAX_PATH 512
    #define _tcsrchr strrchr
    #define DeleteFileW(a)
    #define __TLS __thread
    #define _tmain main
	#define MIN std::min
	#define MAX std::max

    inline void ZeroMemory(void* pBuf,size_t sz)
    {

        ASSERT(sz%4=0);
        int nC=sz/4;
        std::fill((int*)pBuf,((int*)pBuf+nC),0);
//        memset(pBuf,sz,0);
      //  assert(*(int*)pBuf==0);
    }
    #define ATOMIC_INT boost::atomic<LONG>

    #ifndef z_const
        #define z_const
    #endif

  	#define INTERLOCKED_INCREMENT(a)  a.fetch_add(1,  boost::memory_order_relaxed)
    #define INTERLOCKED_GET(a) a.load(boost::memory_order_relaxed)
    #define INTERLOCKED_SET(a,b)  a.store(b,boost::memory_order_relaxed)

/*
    inline long int INTERLOCKED_INCREMENT(volatile long int *pa)
    {
        assert(false);
        return *pa+1;

    }
*/
    /*
    template <class A>
    inline size_t _countof (const A a[])
    {
        return sizeof(a)/sizeof(A);
    }
    */
    #define _countof(a) (sizeof(a)/sizeof(a[0]))
    #define CP_ACP 0
    #define LPCSTR const char*
	//typedef int _int32
	//typedef int __int32
#endif


#include "ConsoleOutput.h"


//#include "../alien/sqlite-amalgamation-3071502/sqlite3.h"


// TODO: reference additional headers your program requires here
