#pragma once
#include "svector.h"

namespace PBFRO {

	//typedef std::allocator<int> other;




template <class T, class _Ax = std::allocator<T> >
class TFar : public CField,
	public svector<T, _Ax>
{
public:

	TFar(void){}

	virtual ~TFar(void)	{}


	virtual void Clear()
	{
		CField::Clear();
		svector<T, _Ax>::clear();

	}


	void clear()
	{
		Clear();
	}


	/// Читаем из потока
	virtual bool Read(CInputStream *pis)
	{
		{
			T f;
			f.Init(pis);
			push_back(f);
		}
		return (*this)[this->size()-1].Read(pis);
	}

	virtual EWireType GetWireType()const
		{return EWT_LENGTH_DELIMITED;}

	_Ax* Get_Alval()
	{
		return &this->_Alval;
	}
};

template <class T,class _Ax = std::allocator<T> >
class TFarPak : public TFar<T,_Ax>
{
public:

	TFarPak(void){}


	/// Читаем из потока
	virtual bool Read(CInputStream *pis)
	{
		unsigned length;
		if (!pis->ReadVariant(length))
			return false;
		CInputStream::Limit L=pis->PushLimit(length);
		while(!pis->ExpectAtEnd())
			if(!TFar<T,_Ax>::Read(pis))
				break;
		pis->PopLimit(L);
		return true;
	}

};

template <class T,class _Ax = std::allocator<T> >
class TFarPak2 : public TFar<T, _Ax>
{
public:

	TFarPak2(void){}


	/// Читаем из потока
	virtual bool Read(CInputStream *pis)
	{
		unsigned length;
		if (!pis->ReadVariant(length))
			return false;
		this->reserve(length/sizeof(T));
		CInputStream::Limit L=pis->PushLimit(length);
		while(!pis->ExpectAtEnd())
			if(!TFar<T, _Ax>::Read(pis))
				break;
		pis->PopLimit(L);
		return true;
	}

};

}//namespace PBFRO
