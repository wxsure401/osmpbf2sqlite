#pragma once
#include "SQLite3DB.h"


class CSQLite3Table
{
public:

	CSQLite3Table(CSQLite3DB* pdb=NULL);
	virtual ~CSQLite3Table(void);
	void Close();

	
	CSQLite3DB* m_pDB;

	int prepare(
		const wchar_t* wszSql,
		int nSize
		);

	int prepare(
		const char *szSql,      // SQL statement, UTF-8 encoded
		int nSize               // Maximum length of szSql in bytes.
		);

	int reset();
	int step();
	
	sqlite3_int64 last_insert_rowid();

	int bind_blob( int, const void*, int n, void(*)(void*));
	int bind_double( int, double);
	int bind_int( int, int);
	int bind_int64( int, sqlite3_int64);
	int bind_null( int);
	int bind_text( int, const char*, int n, void(*)(void*));
	int bind_text16( int, const void*, int n, void(*)(void*));
	int bind_value( int, const sqlite3_value*);
	int bind_zeroblob( int, int n);
	int bind_parameter_count();

	const void *column_blob(int iCol);
	int column_bytes(int iCol);
	int column_bytes16(int iCol);
	double column_double(int iCol);
	int column_int(int iCol);
	sqlite3_int64 column_int64(int iCol);
	int column_type(int iCol);
	sqlite3_value *column_value(int iCol);
	int column_count();
	bool column_IsNull(int iCol);

	const char *column_text(int iCol); //UTF8
	const void *column_text16(int iCol); //UTF16
	void column_text(int iCol, std::string* pStr); //.ACP
	void column_text(int iCol, std::wstring* pStr); //..юникод

	//CComAutoCriticalSection m_cs;

protected:
	sqlite3_stmt *m_pStmt;

};

inline int CSQLite3Table::bind_parameter_count()
{
	return ::sqlite3_bind_parameter_count(m_pStmt);
}

inline int CSQLite3Table::bind_zeroblob( int nIndex, int n)
{
	return ::sqlite3_bind_zeroblob(m_pStmt, nIndex, n);
}

inline sqlite3_int64 CSQLite3Table::last_insert_rowid()
{
	return ::sqlite3_last_insert_rowid(m_pDB->m_pdb);
}

inline int CSQLite3Table::reset()
{
	return ::sqlite3_reset(m_pStmt);
}

inline int CSQLite3Table::step()
{
	return ::sqlite3_step(m_pStmt);
}

inline int CSQLite3Table::bind_double( int nIndex, double d)
{
	return ::sqlite3_bind_double(m_pStmt, nIndex,  d);
}

inline int CSQLite3Table::bind_int( int nIndex, int n)
{
	return ::sqlite3_bind_int(m_pStmt, nIndex, n);
}

inline int CSQLite3Table::bind_int64( int nIndex, sqlite3_int64 n64)
{
	return ::sqlite3_bind_int64(m_pStmt, nIndex,  n64);
}

inline int CSQLite3Table::bind_null(int nIndex)
{
	return ::sqlite3_bind_null(m_pStmt, nIndex);
}

inline int CSQLite3Table::bind_value( int nIndex, const sqlite3_value* v)
{
	return ::sqlite3_bind_value(m_pStmt, nIndex,  v);
}

inline int CSQLite3Table::bind_blob( int nIndex, const void* pv, int n, void(*fDestruct)(void* ))
{
	return ::sqlite3_bind_blob( m_pStmt, nIndex, pv, n, fDestruct);
}

inline int CSQLite3Table::bind_text( int nIndex, const char* str, int n, void(*fDestruct)(void* ))
{
	return ::sqlite3_bind_text(m_pStmt, nIndex, str,  n, fDestruct);
}

inline int CSQLite3Table::bind_text16( int nIndex, const void* str, int n, void(*fDestruct)(void* ))
{
	return ::sqlite3_bind_text16(m_pStmt, nIndex, str, n, fDestruct);
}

inline const void *CSQLite3Table::column_blob(int iCol)
{
	return ::sqlite3_column_blob(m_pStmt,  iCol);
}

inline int CSQLite3Table::column_bytes(int iCol)
{
	return ::sqlite3_column_bytes(m_pStmt, iCol);
}

inline int CSQLite3Table::column_bytes16(int iCol)
{
	return ::sqlite3_column_bytes16(m_pStmt, iCol);
}

inline double CSQLite3Table::column_double(int iCol)
{
	return ::sqlite3_column_double(m_pStmt, iCol);
}

inline int CSQLite3Table::column_int(int iCol)
{
	return ::sqlite3_column_int(m_pStmt, iCol);
}

inline sqlite3_int64 CSQLite3Table::column_int64(int iCol)
{
	return ::sqlite3_column_int64(m_pStmt, iCol);
}

inline const char *CSQLite3Table::column_text(int iCol)
{
	return (const char *)::sqlite3_column_text(m_pStmt, iCol);
}

inline const void *CSQLite3Table::column_text16(int iCol)
{
	return ::sqlite3_column_text16(m_pStmt, iCol);
}

inline int CSQLite3Table::column_type(int iCol)
{
	return ::sqlite3_column_type(m_pStmt, iCol);
}

inline sqlite3_value *CSQLite3Table::column_value(int iCol)
{
	return ::sqlite3_column_value(m_pStmt,iCol);
}

inline int CSQLite3Table::column_count()
{
	return ::sqlite3_column_count(m_pStmt);
}

inline bool CSQLite3Table::column_IsNull(int iCol)
{
	return column_type(iCol)==SQLITE_NULL;
}
