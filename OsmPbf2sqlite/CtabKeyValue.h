#pragma once
#include "PBFRO/PbfFBytes.h"
#include "DLIB/SQLite3Table.h"
#include "OSMPBF/Info.h"
#include "OSMPBF/StringTable.h"
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


	void Init(CSQLite3DB* pdb,const char*sName);
	

	struct Sint
	{
		int nk;
		int nv;
	};
	typedef std::vector<Sint> SarInts;
	CComAutoCriticalSection m_cs;
	void Prepare(const OSMPBF::StringTable& arS,const OSMPBF::CarTaxts &arKeys,const OSMPBF::CarTaxts& arValues,SarInts *par);
	void SaveId(__int64 id,const SarInts &ar);
	

	void Add(__int64 id,const PBFRO::FBytes* sKey, const PBFRO::FBytes* sValue);
	void SaveText();

	class SDic
	{
	public:
		typedef std::map< std::string ,unsigned > CMap;
		CMap m_ar;
		CSQLite3Table m_tab;

		unsigned GetIdText(const PBFRO::FBytes* sKey);
		void Save();
	};
	SDic m_Key;
	SDic m_Val;

	CSQLite3Table m_tabKeyVal;
};
