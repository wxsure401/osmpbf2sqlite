#include "stdafx.h"
#include "CdbDictionary.h"

CtabUser::CtabUser(void)
{
}

CtabUser::~CtabUser(void)
{
}

//__int64 CdbDictionary::GetID(const char* sText, size_t sz)
int CtabUser::GetID(int id,const char* sText, size_t sz)
{

  	 CComCritSecLock<CComAutoCriticalSection> l(m_cs);


	CSet::const_iterator cit=m_set.find(id);

	if(cit==m_set.end())
	{
		m_set[id]=std::string(sText,sz);

	}
/*
	__int64 &id=m_map[s];

	if(id==0)
	{
		m_tab.reset();
		m_tab.bind_text(1,sText, sz,NULL);
		m_tab.step();
		id=m_tab.last_insert_rowid();

	}
	*/
	return id;

}
void CtabUser::Init(CSQLite3DB* db)
{
	m_tab.m_pDB=db;
	const char buf[]="INSERT INTO User (id,Name) VALUES (?,?);";
	int ret=m_tab.prepare(buf,sizeof(buf));
	if(ret!=SQLITE_OK)
			err(db->errmsg());

}
void CtabUser::Save()
{

	for(CSet::const_iterator cit=m_set.begin();cit!=m_set.end();++cit)
	{
		m_tab.reset();
		m_tab.bind_int(1,cit->first);
		m_tab.bind_text(2,cit->second.c_str(),cit->second.size(),NULL);
		m_tab.step();
	}


}
