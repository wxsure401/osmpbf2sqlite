#pragma once

#include "PbfInputStream.h"


namespace PBFRO {

typedef unsigned FNumber;

/// основные типы полей
enum EWireType {
	EWT_VARINT           = 0,  ///Целое переменной длины.
	EWT_FIXED64          = 1,  ///8 байт
	EWT_LENGTH_DELIMITED = 2,  ///Сначало длина потом тело
	EWT_START_GROUP      = 3,  ///Начало группы
	EWT_END_GROUP        = 4,  ///Коней группы
	EWT_FIXED32          = 5,  ///4 байта
};

class CField
{
public:
	CField(void);
	virtual ~CField(void);

	virtual void Init(CInputStream *pis){}

	virtual void Clear(){}

	// Parse a protocol buffer contained in an array of bytes.
	bool ParseFromArray(const char* data, int size);

	virtual bool MergePartialFromCodedStream(CInputStream *pis){return false;}
	virtual bool IsInitialized()const {return true;}

	virtual EWireType GetWireType()const
		{return EWT_VARINT;}

		// Number of bits in a tag which identify the wire type.
	static const int kTagTypeBits = 3;
	static const unsigned kTagTypeMask = (1 << kTagTypeBits) - 1;

	static Tag MakeTag(FNumber field_number, EWireType type) 
	 { 
		 return Tag(field_number<<kTagTypeBits | type);
	 }

	static EWireType GetTagWireType(Tag tag)
	{
		return EWireType(tag & kTagTypeMask);
	}

	static FNumber GetTagFieldNumber(Tag tag)
	{
		 return FNumber(tag >> kTagTypeBits);
	}
	
	/// Читаем из потока
	virtual bool Read(CInputStream *pis)=0;

};

}//namespace PBFRO
