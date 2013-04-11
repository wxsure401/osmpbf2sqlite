#pragma once
#include "../PBFRO/Pbfro.h"
namespace OSMPBF {

class StringTable :
	public PBFRO::CMessage
{
public:
	StringTable(void){};

	virtual void Init(PBFRO::CInputStream *pis)
	{
		CMemoryAddOnly* pm=(CMemoryAddOnly*)pis->m_pParam;
		m_s.Get_Alval()->m_pMemory=pm;
	}

	PBFRO::TFar<PBFRO::FBytes,TAllocatorAddOnly<PBFRO::FBytes> > m_s;  //repeated bytes s = 1;
	//PBFRO::TFar<PBFRO::FBytes > m_s;  //repeated bytes s = 1;

	FIELDS_BEGIN(1)
		FIELDS_DEFINE(1,m_s )
	FIELDS_END




	//virtual bool IsInitialized()const
	//{
	//	return m_left.m_stringtable.IsInitialized();
	//}

};



}//namespace OSMPBF 
