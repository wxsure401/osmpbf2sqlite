#pragma once

#include "Info.h"


namespace OSMPBF {

class Way :
	public PBFRO::CMessage
{
public:
	Way(void){};

	virtual void Init(PBFRO::CInputStream *pis)
	{
		m_info.Init(pis);

		CMemoryAddOnly* pm=(CMemoryAddOnly*)pis->m_pParam;
		m_keys.Get_Alval()->m_pMemory=pm;
		m_vals.Get_Alval()->m_pMemory=pm;
		m_refs.Get_Alval()->m_pMemory=pm;
	}

	PBFRO::Fint64 m_id;                //required int64 id = 1;
	CarTaxts m_keys;	 //repeated uint32 keys = 2 [packed = true];
	CarTaxts m_vals;	 //repeated uint32 vals = 3 [packed = true];
	Info m_info;				 //optional Info info = 4;
	PBFRO::TFarPak2<PBFRO::Fsint64,TAllocatorAddOnly<PBFRO::Fsint64> > m_refs;	 //repeated sint64 refs = 8 [packed = true];  // DELTA coded



	FIELDS_BEGIN(5)
		FIELDS_DEFINE(1,m_id )
		FIELDS_DEFINE(2,m_keys )
		FIELDS_DEFINE(3,m_vals )
		FIELDS_DEFINE(4,m_info )
		FIELDS_DEFINE(8,m_refs )
	FIELDS_END


	virtual bool IsInitialized()const
	{
		return m_id.m_val!=0;
	}

};



}//namespace OSMPBF 