// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS
//#define _HAS_ITERATOR_DEBUGGING 0
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <assert.h>

#include <atlbase.h>
#include <atlstr.h>

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

#define ZLIB_CONST
#include "../Alien/zlib127/include/zlib.h"
#include "../Alien/sqlite3/sqlite3.h"

#ifdef _DEBUG
#define ASSERT(a) assert(a)
#else
#define ASSERT(a) 

#endif //

#include "ConsoleOutput.h"


//#include "../alien/sqlite-amalgamation-3071502/sqlite3.h"


// TODO: reference additional headers your program requires here
