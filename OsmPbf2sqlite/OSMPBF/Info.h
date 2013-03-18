#pragma once
#include "..\PBFRO\pbfro.h"
#include "..\DLIB\TAllocatorAddOnly.h"

namespace OSMPBF {

typedef PBFRO::TFarPak2<PBFRO::Fuint32,TAllocatorAddOnly<PBFRO::Fuint32> > CarTaxts;

class Info :
	public PBFRO::CMessage
{
public:
	Info(void){m_visible.m_val=true;}


	
	PBFRO::Fint32 m_version;  //optional int32 version = 1 [default = -1];
	PBFRO::Fint64 m_timestamp; //optional int64 timestamp = 2;
	PBFRO::Fint64 m_changeset; //optional int64 changeset = 3;
	PBFRO::Fint32 m_uid;		 //optional int32 uid = 4;
	PBFRO::Fuint32 m_user_sid; //optional uint32 user_sid = 5; // String IDs
	PBFRO::Fbool m_visible;	 //optional bool visible = 6;                  


	FIELDS_BEGIN(6)
		FIELDS_DEFINE(1,m_version )
		FIELDS_DEFINE(2,m_timestamp )
		FIELDS_DEFINE(3,m_changeset )
		FIELDS_DEFINE(4,m_uid )
		FIELDS_DEFINE(5,m_user_sid )
		FIELDS_DEFINE(6,m_visible )
	FIELDS_END

	virtual void Clear()
	{
		CMessage::Clear();
		m_version.m_val=-1;
		m_visible.m_val=true;
	}
	bool empty()const
	{
		return m_uid.m_val==0;
	}
};



}//namespace OSMPBF 