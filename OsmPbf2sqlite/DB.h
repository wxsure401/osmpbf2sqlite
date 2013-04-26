#pragma once
#include "DLIB/SQLite3Table.h"
#include "CtabKeyValue.h"
#include "CdbDictionary.h"


class CDB
{
public:
	CDB(void);
	~CDB(void);

	void Init();
	void Close();
	CSQLite3DB m_db;
	boost::mutex m_cs;

	CSQLite3Table m_tabNode;
	CSQLite3Table m_tabNodeInfo;
	//CSQLite3Table m_tabUser;

	CSQLite3Table m_tabNode_NotVisible;
	CSQLite3Table m_tabNodeInfo_NotVisible;

	CSQLite3Table m_tabWay;
	CSQLite3Table m_tabWayInfo;
	CSQLite3Table m_tabWay_NotVisible;
	CSQLite3Table m_tabWayInfo_NotVisible;

	CSQLite3Table m_tabRelation;
	CSQLite3Table m_tabRelationInfo;
	CSQLite3Table m_tabRelation_NotVisible;
	CSQLite3Table m_tabRelationInfo_NotVisible;
	CtabKeyValue::SDic m_dicRelationRole;


	CtabUser m_dicUser;

	CtabKeyValue m_tkvNode;
	CtabKeyValue m_tkvWay;
	CtabKeyValue m_tkvRelation;

	_TCHAR* m_pSource;
	//std::wstring GetFileNameOut()const;
	boost::filesystem::path GetFileNameOut()const;
	void  CreateTables();
	void  PrepareTables();



};
