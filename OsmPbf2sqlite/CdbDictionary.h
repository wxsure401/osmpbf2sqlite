#pragma once

#include "DLIB/SQLite3Table.h"


class CtabUser
{
public:
	CtabUser(void);
	~CtabUser(void);

	int GetID(int id,const char* sText, size_t sz=-1);


	void Init(CSQLite3DB* db);

	CSQLite3Table m_tab;


	//CSQLite3DB* m_pdb;
//	std::string m_sTableName;
//	std::string m_sId;
//	std::string m_sText;


	typedef std::map<int,std::string>  CSet;
	CSet m_set;

	void Save();
	boost::mutex m_cs;

};
