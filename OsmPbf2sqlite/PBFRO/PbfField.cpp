#include "StdAfx.h"
#include "PbfField.h"
#include "PbfInputStream.h"

namespace PBFRO {

CField::CField(void)
{

}

CField::~CField(void)
{

}

// Parse a protocol buffer contained in an array of bytes.
bool CField::ParseFromArray(const char* data, int size)
{
	CInputStream is(data,size);
	Clear();
	Init(&is);
	return MergePartialFromCodedStream(&is) && IsInitialized();
}


}//namespace PBFRO