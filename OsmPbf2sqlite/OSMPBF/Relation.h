#pragma once
#include "Info.h"

namespace OSMPBF {

class Relation :
	public PBFRO::CMessage
{
public:
	Relation(void){};
	enum EMemberType {
		NODE = 0,
		WAY = 1,
		RELATION = 2,
	} ;
	virtual void Init(PBFRO::CInputStream *pis)
	{
		m_info.Init(pis);

		CMemoryAddOnly* pm=(CMemoryAddOnly*)pis->m_pParam;
		m_keys.Get_Alval()->m_pMemory=pm;
		m_vals.Get_Alval()->m_pMemory=pm;
		m_roles_sid.Get_Alval()->m_pMemory=pm;
		m_memids.Get_Alval()->m_pMemory=pm;
		m_types_N.Get_Alval()->m_pMemory=pm;
	}

	
	PBFRO::Fint64 m_id;                     //required int64 id = 1;
	CarTaxts m_keys;		  //repeated uint32 keys = 2 [packed = true];
	CarTaxts m_vals;		  //repeated uint32 vals = 3 [packed = true];
	Info m_info;					  //optional Info info = 4;
	PBFRO::TFarPak2<PBFRO::Fint32,TAllocatorAddOnly<PBFRO::Fint32> > m_roles_sid;	  //repeated int32 roles_sid = 8 [packed = true];
	PBFRO::TFarPak2<PBFRO::Fsint64,TAllocatorAddOnly<PBFRO::Fsint64> >  m_memids;	  //repeated sint64 memids = 9 [packed = true]; // DELTA encoded
	PBFRO::TFarPak2<PBFRO::Fint32,TAllocatorAddOnly<PBFRO::Fint32> > m_types_N;	  //repeated MemberType types = 10 [packed = true];              

	FIELDS_BEGIN(7)
		FIELDS_DEFINE(1,m_id )
		FIELDS_DEFINE(2,m_keys )
		FIELDS_DEFINE(3,m_vals )
		FIELDS_DEFINE(4,m_info )
		FIELDS_DEFINE(8,m_roles_sid )
		FIELDS_DEFINE(9,m_memids )
		FIELDS_DEFINE(10,m_types_N )
	FIELDS_END

	virtual bool IsInitialized()const
	{
			return m_id.m_val!=0;
	}

};



}//namespace OSMPBF 