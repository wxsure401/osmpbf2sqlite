#include "stdafx.h"
#include "SQLite3DB.h"
#include "MyString.h"


CSQLite3DB::CSQLite3DB(void)
	: m_pdb(NULL)
{
}

CSQLite3DB::~CSQLite3DB(void)
{
	Close();
}

void CSQLite3DB::Close()
{
	if(m_pdb)
	{
		sqlite3_close(m_pdb);
		m_pdb = NULL;
	}
}

int CSQLite3DB::Open( 
		const wchar_t* filename,   // Database filename 
		int flags               // Flags  смотри  sqlite3_open_v2
		)
{
	std::string s;
	CMyString::W2C(filename, &s, CP_UTF8);
	return Open(s.c_str(),flags);
}

int CSQLite3DB::Open( 
		const char* filename,   // Database filename (UTF-8) 
		int flags               // Flags  смотри  sqlite3_open_v2
		)
{
	return sqlite3_open_v2(filename, &m_pdb, flags, NULL);
}


int CSQLite3DB::Execute(const wchar_t* pwszSQL)
{
	std::wstring ws(pwszSQL);
	std::string s;
	CMyString::W2C(ws, &s, CP_UTF8);

	return Execute(s.c_str());
}

int CSQLite3DB::Execute(const char* pszSQL)
{
	int ret = sqlite3_exec(
		  m_pdb,            // An open database 
		  pszSQL,			// SQL to be evaluated
		  NULL,				// Callback function 
		  NULL,             // 1st argument to callback 
		  NULL              // Error msg written here 
		);
	
	return ret;
}

void CSQLite3DB::DropTable(const wchar_t* pwszTableName)
{
	std::wstring ws(pwszTableName);
	std::string s;
	CMyString::W2C(ws, &s, CP_UTF8);

	DropTable(s.c_str());
}

void CSQLite3DB::DropTable(const char* pszTableName)
{
	CStringA s = "DROP TABLE IF EXISTS ";
	s += pszTableName;
	Execute(s);

}

void CSQLite3DB::DropView(const wchar_t* pwszTableName)
{
	std::wstring ws(pwszTableName);
	std::string s;
	CMyString::W2C(ws, &s, CP_UTF8);

	DropView(s.c_str());
}

void CSQLite3DB::DropView(const char* pszTableName)
{
	CStringA s = "DROP VIEW IF EXISTS ";
	s += pszTableName;
	Execute(s);
}

