#pragma once
#include "PbfField.h"

namespace PBFRO {

class CMessage : public CField
{
public:
	CMessage(void);
	~CMessage(void);

	virtual void Clear();

	virtual CField* GetField(size_t nField, FNumber* pFn)=0;
	virtual size_t CountField()const=0;

	virtual EWireType GetWireType()const
		{return EWT_LENGTH_DELIMITED;}
protected:
	virtual bool MergePartialFromCodedStream(CInputStream *pis);


public:
		/// Читаем из потока
	virtual bool Read(CInputStream *pis);

	///friend template TFar;


};

}//namespace PBFRO 

#define FIELDS_BEGIN(nCount)	\
	static const size_t m_tCountField=nCount;\
	virtual size_t CountField()const\
		{return m_tCountField;}\
	virtual PBFRO::CField* GetField(size_t nField, PBFRO::FNumber* pFn)\
	{\
		struct{\
			PBFRO::FNumber m_n;\
			PBFRO::CField* m_pf;\
		} a[]={

#define FIELDS_DEFINE(n,v )	\
			{n,&v},

#define FIELDS_END	\
		};\
		ASSERT(_countof(a)==m_tCountField);\
		if(nField>=m_tCountField)\
			return NULL;\
		*pFn=a[nField].m_n;\
		return a[nField].m_pf;}
