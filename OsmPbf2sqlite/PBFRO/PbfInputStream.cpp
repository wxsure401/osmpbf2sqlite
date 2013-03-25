#include "../StdAfx.h"
#include "PbfField.h"

namespace PBFRO {
CInputStream::CInputStream(const char *pbuf,size_t count)
:TSlicedArray<char>(pbuf,count)
,recursion_depth_(0)

{
}

CInputStream::~CInputStream(void)
{
}

bool CInputStream::SkipField()
{
	switch (CField::GetTagWireType(last_tag_)) 
	{
	case EWT_VARINT: 
		{
			unsigned __int64 value;
			return ReadVariant(value);
		}

	case EWT_FIXED64: 
		{
			unsigned __int64 value;
			return ReadLittleEndian(value);
		}

	case EWT_LENGTH_DELIMITED: 
		{
			unsigned length;
			return ReadVariant(length) && Skip(length);
		}

	case EWT_START_GROUP: 
		{
			Tag t=last_tag_;
			if (!IncrementRecursionDepth()) 
				return false;

			if (!SkipMessage()) 
				return false;

			DecrementRecursionDepth();
			// Check that the ending tag matched the starting tag.
			return CField::MakeTag(
				CField::GetTagFieldNumber(t), EWT_END_GROUP) == last_tag_;
		}

	case EWT_END_GROUP: 
			return false;

	case EWT_FIXED32: 
		{
			unsigned value;
			return ReadLittleEndian(value);
		}
	default:
		break;
	}
	return false;
}

bool  CInputStream::SkipMessage() 
{
	for(;;) 
	{
		Tag tag = ReadTag();
		if (tag == 0) 
			return true;// End of input.  This is a valid place to end, so return true.

		if (CField::GetTagWireType(tag) == EWT_END_GROUP)
			return true;
		if (!SkipField())
			return false;
	}
}



}//namespace PBFRO 
