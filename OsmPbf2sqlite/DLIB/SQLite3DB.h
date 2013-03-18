#pragma once


//База данных

class  CSQLite3DB
{
public:
	CSQLite3DB(void);
	virtual ~CSQLite3DB(void);
	void Close();

	int Open( 
		const wchar_t *filename,   // Database filename
		int flags               // Flags  смотри  sqlite3_open_v2
		);

	int Open( 
		const char* filename,   // Database filename (UTF-8) 
		int flags               // Flags  смотри  sqlite3_open_v2
		);

	sqlite3 *m_pdb; //указатель на базу

	int Execute(const wchar_t* pwszSQL);
	int Execute(const char *pszSQL);

	void DropTable(const wchar_t *pszTableName);
	void DropTable(const char *pszTableName);

	void DropView(const wchar_t *pszTableName);
	void DropView(const char *pszTableName);

	const char * errmsg();
};

inline const char *CSQLite3DB::errmsg()
{
	return sqlite3_errmsg(m_pdb);
}