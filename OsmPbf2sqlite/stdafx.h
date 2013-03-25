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
//#include "../Alien/zlib127/include/zlib.h"
//#include "../Alien/sqlite3/sqlite3.h"
#include <zlib.h>
#include "../Alien/sqlite3/sqlite3.h"

#ifndef WIN32
#include <netinet/in.h>
#endif

#ifdef _DEBUG
#define ASSERT(a) assert(a)
#else
#define ASSERT(a) 

#endif //

#include "ConsoleOutput.h"


//#include "../alien/sqlite-amalgamation-3071502/sqlite3.h"


// TODO: reference additional headers your program requires here
