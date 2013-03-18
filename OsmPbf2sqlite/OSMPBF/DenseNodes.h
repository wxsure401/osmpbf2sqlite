#pragma once
#include "..\PBFRO\pbfro.h"
#include "../DLIB/TAllocatorAddOnly.h"

namespace OSMPBF {

class DenseInfo :
	public PBFRO::CMessage
{
public:
	DenseInfo(void){};

	virtual void Init(PBFRO::CInputStream *pis)
	{
		CMemoryAddOnly* pm=(CMemoryAddOnly*)pis->m_pParam;
		m_version.Get_Alval()->m_pMemory=pm;
		m_timestamp.Get_Alval()->m_pMemory=pm;
		m_changeset.Get_Alval()->m_pMemory=pm;
		m_uid.Get_Alval()->m_pMemory=pm;
		m_user_sid.Get_Alval()->m_pMemory=pm;
		m_visible.Get_Alval()->m_pMemory=pm;
	}

	PBFRO::TFarPak2<PBFRO::Fint32,TAllocatorAddOnly<PBFRO::Fint32> >  m_version;    //repeated int32 version = 1 [packed = true]; 
	PBFRO::TFarPak2<PBFRO::Fsint64,TAllocatorAddOnly<PBFRO::Fsint64> > m_timestamp;	 //repeated sint64 timestamp = 2 [packed = true]; // DELTA coded
	PBFRO::TFarPak2<PBFRO::Fsint64,TAllocatorAddOnly<PBFRO::Fsint64> > m_changeset;	 //repeated sint64 changeset = 3 [packed = true]; // DELTA coded
	PBFRO::TFarPak2<PBFRO::Fsint32,TAllocatorAddOnly<PBFRO::Fsint32> > m_uid;		 //repeated sint32 uid = 4 [packed = true]; // DELTA coded
	PBFRO::TFarPak2<PBFRO::Fsint32,TAllocatorAddOnly<PBFRO::Fsint32> > m_user_sid;	 //repeated sint32 user_sid = 5 [packed = true]; // String IDs for usernames. DELTA coded
	PBFRO::TFarPak2<PBFRO::Fbool,TAllocatorAddOnly<PBFRO::Fbool>  > m_visible;	 //repeated bool visible = 6 [packed = true];                                                  

	FIELDS_BEGIN(6)
		FIELDS_DEFINE(1,m_version )
		FIELDS_DEFINE(2,m_timestamp )
		FIELDS_DEFINE(3,m_changeset )
		FIELDS_DEFINE(4,m_uid )
		FIELDS_DEFINE(5,m_user_sid )
		FIELDS_DEFINE(6,m_visible )
	FIELDS_END

	bool empty()const
	{
		return m_uid.empty();
	}
};


class DenseNodes :
	public PBFRO::CMessage
{
public:
	DenseNodes(void)
	{

	};

	virtual void Init(PBFRO::CInputStream *pis)
	{
		CMemoryAddOnly* pm=(CMemoryAddOnly*)pis->m_pParam;
		m_id.Get_Alval()->m_pMemory=pm;
		m_lat.Get_Alval()->m_pMemory=pm;
		m_lon.Get_Alval()->m_pMemory=pm;
		m_keys_vals.Get_Alval()->m_pMemory=pm;
		m_denseinfo.Init(pis);
	}

	PBFRO::TFarPak2<PBFRO::Fsint64,TAllocatorAddOnly<PBFRO::Fsint64> > m_id;        //repeated sint64 id = 1 [packed = true]; // DELTA coded
	DenseInfo m_denseinfo;		//optional DenseInfo denseinfo = 5;
	PBFRO::TFarPak2<PBFRO::Fsint64 ,TAllocatorAddOnly<PBFRO::Fsint64> >m_lat;		//repeated sint64 lat = 8 [packed = true]; // DELTA coded
	PBFRO::TFarPak2<PBFRO::Fsint64 ,TAllocatorAddOnly<PBFRO::Fsint64> > m_lon;		//repeated sint64 lon = 9 [packed = true]; // DELTA coded
	PBFRO::TFarPak2<PBFRO::Fint32 ,TAllocatorAddOnly<PBFRO::Fint32> >m_keys_vals;	//repeated int32 keys_vals = 10 [packed = true];             


	FIELDS_BEGIN(5)
		FIELDS_DEFINE(1,m_id )
		FIELDS_DEFINE(5,m_denseinfo )
		FIELDS_DEFINE(8,m_lat )
		FIELDS_DEFINE(9,m_lon )
		FIELDS_DEFINE(10,m_keys_vals )
	FIELDS_END

	bool empty()const
	{
		return m_id.empty();
	}

};



}//namespace OSMPBF 