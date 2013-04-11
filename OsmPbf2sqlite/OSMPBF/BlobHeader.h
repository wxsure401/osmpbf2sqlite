#pragma once

#include "../PBFRO/Pbfro.h"


namespace OSMPBF {

class BlobHeader : public PBFRO::CMessage
{
public:
	BlobHeader(void);
	~BlobHeader(void);

	PBFRO::FString m_type;      // required string type = 1;
	PBFRO::FBytes m_indexdata;  //optional bytes indexdata = 2;
	PBFRO::Fint32 m_datasize;   //required int32 datasize = 3;
	
	FIELDS_BEGIN(3)
		FIELDS_DEFINE(1,m_type )
		FIELDS_DEFINE(2,m_indexdata )
		FIELDS_DEFINE(3,m_datasize )
	FIELDS_END

	virtual bool IsInitialized()const
	{
		return !m_type.empty() && m_datasize.m_val!=0;
	}

};
class Blob : public PBFRO::CMessage
{
public:
	Blob(){};

	PBFRO::FBytes m_raw;		//ptional bytes raw = 1;
	PBFRO::Fint32 m_raw_size;   //optional int32 raw_size = 2
	PBFRO::FBytes m_zlib_data;  //optional bytes zlib_data = 3;
	PBFRO::FBytes m_lzma_data;  //optional bytes lzma_data = 4;

	FIELDS_BEGIN(4)
		FIELDS_DEFINE(1,m_raw )
		FIELDS_DEFINE(2,m_raw_size )
		FIELDS_DEFINE(3,m_zlib_data )
		FIELDS_DEFINE(4,m_lzma_data )
	FIELDS_END
};

} //namespace OSMPBF 