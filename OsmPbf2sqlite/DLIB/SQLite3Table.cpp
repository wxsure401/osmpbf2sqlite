#include "stdafx.h"
#include "SQLite3Table.h"
#include "MyString.h"


CSQLite3Table::CSQLite3Table(CSQLite3DB* pdb)
	: m_pDB(pdb)
	, m_pStmt(NULL)
{
}

CSQLite3Table::~CSQLite3Table(void)
{
	Close();
}

void CSQLite3Table::Close()
{
	if(m_pStmt)
	{
		sqlite3_finalize(m_pStmt);
		m_pStmt=NULL;
	}	 
}

int CSQLite3Table::prepare(const wchar_t* wszSql, int nSize)
{
	std::wstring ws(wszSql, nSize);
	std::string s;
	CMyString::W2C(ws, &s, CP_UTF8);

	return prepare(s.c_str(), s.size());
}

int CSQLite3Table::prepare(const char* szSql, int nSize)
{
	Close();

	return sqlite3_prepare_v2(
		m_pDB->m_pdb,        // Database handle
		szSql,               // SQL statement, UTF-8 encoded 
		nSize,               // Maximum length of szSql in bytes. 
		&m_pStmt,            // OUT: Statement handle 
		NULL                 // OUT: Pointer to unused portion of zSql
		);

}

void CSQLite3Table::column_text(int iCol, std::string* pStr) //.ACP
{
	//CP_UTF8
	std::wstring ws;
	column_text(iCol, &ws);
	CMyString::W2C(ws, pStr);
}

void CSQLite3Table::column_text(int iCol, std::wstring * pStr) //..юникод
{
	CMyString::C2W((const char*)column_text(iCol), column_bytes(iCol), pStr, CP_UTF8);
}
