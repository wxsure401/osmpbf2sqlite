#include "StdAfx.h"
#include "PbfFBytes.h"

namespace PBFRO {

FBytes::FBytes(void)
{
}

FBytes::~FBytes(void)
{
}
//bool FBytes::Read(CodedInputStream input)
//virtual 
bool FBytes::Read(CInputStream *pis)
{
	size_t length;
	if (!pis->ReadVariant(length)) 
		return false;
	return  pis->InternalReadStringInline(this,length);
}

}//namespace PBFRO