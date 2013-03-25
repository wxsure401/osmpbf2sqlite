#pragma once
#include "../PBFRO/Pbfro.h"

namespace OSMPBF {

class ChangeSet :
	public PBFRO::CMessage
{
public:
	ChangeSet(void){};
	
	PBFRO::Fint64 m_id;//required int64 id = 1;


	FIELDS_BEGIN(1)
		FIELDS_DEFINE(1,m_id )
	FIELDS_END

	virtual bool IsInitialized()const
	{
			return m_id.m_val!=0;
	}

};



}//namespace OSMPBF 
