#pragma once
#include "Info.h"
#include "..\dlib\TAllocatorAddOnly.h"

namespace OSMPBF {

class Node :
	public PBFRO::CMessage
{
public:
	Node(void){};

	virtual void Init(PBFRO::CInputStream *pis)
	{
		m_info.Init(pis);
		SetMemory((CMemoryAddOnly*)pis->m_pParam);
	}

	void SetMemory(CMemoryAddOnly*pm)
	{
		m_keys.Get_Alval()->m_pMemory=pm;
		m_vals.Get_Alval()->m_pMemory=pm;

	}
	PBFRO::Fsint64 m_id;              //required sint64 id = 1;
	CarTaxts m_keys;	//repeated uint32 keys = 2 [packed = true]; // String IDs.
	CarTaxts m_vals;	//repeated uint32 vals = 3 [packed = true]; // String IDs.
	Info m_info;				//optional Info info = 4; // May be omitted in omitmeta
	PBFRO::Fsint64 m_lat;				//required sint64 lat = 8;
	PBFRO::Fsint64 m_lon;				//required sint64 lon = 9;                                      

	FIELDS_BEGIN(6)
		FIELDS_DEFINE(1,m_id )
//		FIELDS_DEFINE(2,m_keys )
//		FIELDS_DEFINE(3,m_vals )
		FIELDS_DEFINE(4,m_info )
		FIELDS_DEFINE(8,m_lat )
		FIELDS_DEFINE(9,m_lon )
	FIELDS_END

};


}//namespace OSMPBF 