#pragma once

#include "PbfField.h"

namespace PBFRO {


class FBytes :
	public CField,
	public CBuffer

{
public:
	FBytes();
	virtual ~FBytes(void);

	/// Читаем из потока
	virtual bool Read(CInputStream *pis);

	//CBuffer m_b;
	static const EWireType m_wireType= EWT_LENGTH_DELIMITED;

	virtual EWireType GetWireType()const
		{return m_wireType;}
	
	virtual void Clear()
	{
		CField::Clear();
		clear();
	}


	std::string toString()const
	{
		
		return std::string(m_pBegin,m_pEnd);
	}


};

typedef FBytes FString;

}//namespace PBFRO