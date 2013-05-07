#pragma once
#include "PBFRO/PbfFBytes.h"
#include "DLIB/SQLite3Table.h"
#include "OSMPBF/Info.h"
#include "OSMPBF/StringTable.h"
#include "TTable_cash.h"
class CDB;

struct STabKeyVal_Cash
{
        sqlite3_int64  m_n1;
        int            m_n2;
        int            m_n3;

    void Bind(CSQLite3Table*ptab)
    {
        ptab->bind_int64(1,m_n1);
        ptab->bind_int  (2,m_n2);
        ptab->bind_int  (3,m_n3);
    }
};
typedef TTable_cash<STabKeyVal_Cash> CKV_cash;


class CtabKeyValue
{
public:
	CtabKeyValue(void);
	~CtabKeyValue(void);
	void Close()
	{
		m_Key.m_tab.Close();
		m_Val.m_tab.Close();
		m_tabKeyVal.Close();
	}


	void Init(CDB* pdb,const char*sName);


	struct Sint
	{
		int nk;
		int nv;
	};
	typedef std::vector<Sint> SarInts;
	void Prepare(const OSMPBF::StringTable& arS,const OSMPBF::CarTaxts &arKeys,const OSMPBF::CarTaxts& arValues,SarInts *par);
	void SaveId(__int64 id,const SarInts &ar,CKV_cash * pCash);


	void Add(__int64 id,const PBFRO::FBytes* sKey, const PBFRO::FBytes* sValue);
	void SaveText();

	class SDic
	{
	public:
		typedef std::map< std::string ,unsigned > CMap;
		CMap m_ar;
		CSQLite3Table m_tab;
		boost::shared_mutex m_cs;

		unsigned GetIdText(const PBFRO::FBytes* sKey);
		void Save();
	};
	SDic m_Key;
	SDic m_Val;

	CSQLite3Table m_tabKeyVal;


};
