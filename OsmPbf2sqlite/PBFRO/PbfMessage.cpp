#include "stdafx.h"
#include "PbfMessage.h"
#include "PbfInputStream.h"

namespace PBFRO {

CMessage::CMessage(void)
{
}

CMessage::~CMessage(void)
{
}

//virtual
bool CMessage::MergePartialFromCodedStream(CInputStream *pis)
{
	Tag tag;
	while ((tag = pis->ReadTag()) != 0)
	{
		EWireType wire_type=GetTagWireType(tag);
		if(wire_type==EWT_END_GROUP)
			return true;
		FNumber field_number=GetTagFieldNumber(tag);
		bool bFound=false;
		size_t tCountField=CountField();
		for(unsigned i=0 ; i < tCountField;++i)
		{
			FNumber Number;
			CField *pf=GetField(i,&Number);
			assert(pf);
			if(field_number==Number && wire_type == pf->GetWireType())
			{
				if(!pf->Read( pis ))
					return false;
				bFound=true;
				break;
			}
		}
		if(!bFound )
			if(!pis->SkipField())
				return false;
	}
	return true;

}

//virtual

bool CMessage::Read(CInputStream *pis)
{
	size_t length;
	if (!pis->ReadVariant(length))
		return false;

	if(!pis->IncrementRecursionDepth())
		return false;

	CInputStream::Limit limit = pis->PushLimit(length);

	if(!MergePartialFromCodedStream(pis))
		return false;
	pis->PopLimit(limit);
	pis->DecrementRecursionDepth();

	//Has=true;
	return true;
}

//virtual
void CMessage::Clear()
{
	CField::Clear();
	size_t tCountField=CountField();
	for(unsigned i=0 ; i < tCountField;++i)
	{
		FNumber Number;
		GetField(i,&Number)->Clear();
	}
}


}// namespace PBFRO
