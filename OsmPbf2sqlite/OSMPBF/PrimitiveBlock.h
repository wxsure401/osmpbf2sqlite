#pragma once
#include "..\PBFRO\pbfro.h"
#include "StringTable.h"
#include "PrimitiveGroup.h"

namespace OSMPBF {

class PrimitiveBlock :
	public PBFRO::CMessage
{
public:
	PrimitiveBlock(void)
	{
	};

	virtual void Init(PBFRO::CInputStream *pis)
	{
		pis->m_pParam=&m_Memory;
		m_primitivegroup.Get_Alval()->m_pMemory=&m_Memory;
		m_stringtable.Init(pis);
	}

	CMemoryAddOnly m_Memory;

	StringTable m_stringtable; //required StringTable stringtable = 1;
	PBFRO::TFar<PrimitiveGroup,TAllocatorAddOnly<PrimitiveGroup> > m_primitivegroup;//repeated PrimitiveGroup primitivegroup = 2;
	PBFRO::Fsint32 m_granularity;//optional int32 granularity = 17 [default=100]; 
	PBFRO::Fsint64 m_lat_offset;//optional int64 lat_offset = 19 [default=0];
	PBFRO::Fsint64 m_lon_offset;//optional int64 lon_offset = 20 [default=0]; 
	PBFRO::Fsint32 m_date_granularity;//optional int32 date_granularity = 18 [default=1000]; 


	FIELDS_BEGIN(6)
		FIELDS_DEFINE(1,m_stringtable )
		FIELDS_DEFINE(2,m_primitivegroup )
		FIELDS_DEFINE(17,m_granularity )
		FIELDS_DEFINE(19,m_lat_offset )
		FIELDS_DEFINE(20,m_lon_offset )
		FIELDS_DEFINE(18,m_date_granularity )
	FIELDS_END

	virtual void Clear()
	{
		CMessage::Clear();
		m_granularity.m_val=100;
		m_date_granularity.m_val=1000;
		m_Memory.clear();
	}

	virtual bool IsInitialized()const
	{
		return !m_stringtable.m_s.empty();
	}

};



}//namespace OSMPBF 