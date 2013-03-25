#pragma once
#include "../PBFRO/Pbfro.h"
#include "../DLIB/TAllocatorAddOnly.h"

namespace OSMPBF {

class HeaderBBox :
	public PBFRO::CMessage
{
public:
	HeaderBBox(void){};


	PBFRO::Fsint64 m_left;//required sint64 left=1
	PBFRO::Fsint64 m_right; //required sint64 right=2
	PBFRO::Fsint64 m_top;//required sint64 top=3
	PBFRO::Fsint64 m_bottom;//required sint64 bottom=4

	FIELDS_BEGIN(4)
		FIELDS_DEFINE(1,m_left )
		FIELDS_DEFINE(2,m_right )
		FIELDS_DEFINE(3,m_top )
		FIELDS_DEFINE(4,m_bottom )
	FIELDS_END

	virtual bool IsInitialized()const
	{
		return m_left.m_val!=0 && 
			m_right.m_val!=0 && 
			m_top.m_val!=0 && 
			m_bottom.m_val!=0;
	}
};


class Headerblock :
	public PBFRO::CMessage
{
public:
	Headerblock(void);
	~Headerblock(void);
/*
	virtual void Init(PBFRO::CInputStream *pis)
	{
		m_bbox.Init(pis);
		m_writingprogram.Init(pis);
		m_source.Init(pis);
		CMemoryAddOnly* pm=(CMemoryAddOnly*)pis->m_pParam;
		m_required_features.Get_Alval()->m_pMemory=pm;
		m_optional_features.Get_Alval()->m_pMemory=pm;
	}
*/
	HeaderBBox m_bbox;  // optional HeaderBBox bbox = 1;
	PBFRO::TFar<PBFRO::FString > m_required_features; //repeated string required_features = 4;
	PBFRO::TFar<PBFRO::FString > m_optional_features; //repeated string optional_features = 5;
	PBFRO::FString  m_writingprogram; //optional string writingprogram = 16; 
	PBFRO::FString  m_source; //optional string source = 17;

	
	FIELDS_BEGIN(5)
		FIELDS_DEFINE(1,m_bbox )
		FIELDS_DEFINE(4,m_required_features )
		FIELDS_DEFINE(5,m_optional_features )
		FIELDS_DEFINE(16,m_writingprogram )
		FIELDS_DEFINE(17,m_source)
	FIELDS_END
};

}//namespace OSMPBF 
