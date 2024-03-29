#include "stdafx.h"
#include "CtabKeyValue.h"
#include "DB.h"

CtabKeyValue::CtabKeyValue(void)
{
}

CtabKeyValue::~CtabKeyValue(void)
{
}
void CtabKeyValue::Init(CDB* _pdb,const char*sName)
{

	CSQLite3DB* pdb=&_pdb->m_db;

	m_tabKeyVal.m_pDB=
	m_Key.m_tab.m_pDB=
	m_Val.m_tab.m_pDB=pdb;

	int ret;
	char s[MAX_PATH];
	sprintf(s,
		"CREATE TABLE IF NOT EXISTS Key%s ( "
		"id   INTEGER PRIMARY KEY NOT NULL UNIQUE,"
		"Name  TEXT    NOT NULL"
		");"
		,sName);
	ret=pdb->Execute(s);
	if(ret!=SQLITE_OK)
		err(pdb->errmsg());

	sprintf(s,
		"CREATE TABLE IF NOT EXISTS Value%s ( "
		"id   INTEGER PRIMARY KEY NOT NULL UNIQUE,"
		"Text  TEXT    NOT NULL"
		");"
		,sName);
	ret=pdb->Execute(s);
	if(ret!=SQLITE_OK)
		err(pdb->errmsg());


	sprintf(s,
		"CREATE TABLE IF NOT EXISTS KeyValue%s ( "
		"id   INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
		"id%s   INTEGER NOT NULL ,"
		"idKey  INTEGER    NOT NULL,"
		"idVal  INTEGER    NOT NULL"
		");"
		,sName,sName);
	ret=pdb->Execute(s);
	if(ret!=SQLITE_OK)
		err(pdb->errmsg());

	sprintf(s,"INSERT INTO KeyValue%s (id%s,idKey,idVal) VALUES (?,?,?);",sName,sName);
	ret=m_tabKeyVal.prepare(s,-1);
	if(ret!=SQLITE_OK)
		err(pdb->errmsg());


	sprintf(s,"INSERT INTO Key%s (id,Name) VALUES (?,?);",sName);
	ret=m_Key.m_tab.prepare(s,-1);
	if(ret!=SQLITE_OK)
		err(pdb->errmsg());

	sprintf(s,"INSERT INTO Value%s (id,Text) VALUES (?,?);",sName);
	ret=m_Val.m_tab.prepare(s,-1);
	if(ret!=SQLITE_OK)
		err(pdb->errmsg());

  // m_KV_cash.Init(&m_tabKeyVal,&_pdb->m_cs);
}

void CtabKeyValue::Add(__int64 id,const PBFRO::FBytes* sKey, const PBFRO::FBytes* sValue)
{


	m_tabKeyVal.reset();
	m_tabKeyVal.bind_int64(1,id);
	m_tabKeyVal.bind_int(2,m_Key.GetIdText(sKey));
	m_tabKeyVal.bind_int(3,m_Val.GetIdText(sValue));
	m_tabKeyVal.step();

}

unsigned  CtabKeyValue::SDic::GetIdText(const PBFRO::FBytes* sKey)
{
	std::string s=sKey->toString();
	{
		boost::shared_lock< boost::shared_mutex  > l(m_cs);
		CMap::iterator it=m_ar.find(s);
		if( it!=m_ar.end())
		{
			return it->second;
		}
	}
    boost::lock_guard<boost::shared_mutex> l(m_cs);
	int n=m_ar.size()+1;
	m_ar[s]= n;
	return n;
}

void  CtabKeyValue::SaveText()
{
	m_Key.Save();
	m_Val.Save();
	//m_KV_cash.Save();
}

void CtabKeyValue::SDic::Save()
{
	for(CMap::const_iterator cit=m_ar.begin();cit!=m_ar.end();++cit)
	{
		m_tab.reset();
		m_tab.bind_int(1,cit->second);
		m_tab.bind_text(2,cit->first.c_str(),-1,NULL);
		m_tab.step();

	}
}
void CtabKeyValue::Prepare(const OSMPBF::StringTable& arS,const OSMPBF::CarTaxts &arKeys,const OSMPBF::CarTaxts& arValues,SarInts* pAr)
{

	pAr->resize(arKeys.size());
  	
	 for(unsigned i=0;i< arKeys.size(); ++i)
	 {
		 Sint &si=(*pAr)[i];
		 si.nk=m_Key.GetIdText(&arS.m_s[arKeys[i].m_val]);
		 si.nv=m_Val.GetIdText(&arS.m_s[arValues[i].m_val]);
	 }
}

void CtabKeyValue::SaveId(__int64 id,const SarInts &ar,CKV_cash * pCash)
{

	 for(unsigned i=0;i< ar.size(); ++i)
	 {
	     STabKeyVal_Cash &t= pCash->Prepare();
	     t.m_n1=id;
	     t.m_n2=ar[i].nk;
	     t.m_n3=ar[i].nv;
	     pCash->Update();
	  }
}
