#pragma once

#include "../SqLite3MemAloc.h"

class CMemoryAddOnly 
{
public:

	static const size_t g_nCountDef=4096;

	int m_test;
	CMemoryAddOnly(size_t nCountReserve=g_nCountDef)
		:m_pBegin(NULL)
		,m_pEnd(NULL)
		,m_pLast(NULL)
		,m_pNext(NULL)
		,m_test(778)

	{
		if(nCountReserve==0)
			nCountReserve=g_nCountDef;
		m_pBegin=m_pEnd=(__int8*)malloc(nCountReserve);
		if(m_pBegin==NULL)
			throw 777;
		m_pLast=m_pBegin + nCountReserve;
	}
	~CMemoryAddOnly()
	{
		delete m_pNext;
		free(m_pBegin);
	}

	void* New(size_t byteCount)
	{

		ASSERT(m_test==778);
		size_t nsz=size() + byteCount;
		size_t cap=capacity();

		if(nsz > cap)
		{
			if(m_pNext==NULL || !Can(byteCount))
			{
				while(nsz > cap)
					cap*=2;
				m_pNext=new CMemoryAddOnly(cap);
				swap();
			}
			else
			{
				void* pRet= m_pNext->New(byteCount);
				swap();
				return pRet;

			}
		}
		void* pRet=m_pEnd;
		m_pEnd += byteCount;

		return pRet;

	}

	 void clear()
	 {
		 ASSERT(m_test==778);

		 m_pEnd=m_pBegin;
		 if(m_pNext)
			 m_pNext->clear();
	 }
	 size_t size()const
	 {
		 return m_pEnd - m_pBegin;
	 }
	 size_t capacity()const
	 {
		 return m_pLast - m_pBegin;
	 }

	 bool Can(size_t byteCount)const
	 {
		 if(capacity()-size() >= byteCount)
			 return true;

		 if(m_pNext)
			 return m_pNext->Can(byteCount);
		 return false;

	 }

private: 
	__int8* m_pBegin;
	__int8* m_pEnd;
	__int8* m_pLast;
	CMemoryAddOnly *m_pNext;

	void swap()
	{
		std::swap(m_pBegin,m_pNext->m_pBegin);
		std::swap(m_pEnd,m_pNext->m_pEnd);
		std::swap(m_pLast,m_pNext->m_pLast);
	}
};

		// TEMPLATE CLASS allocator
template<class _Ty>
class TAllocatorAddOnly
//		: public _Allocator_base<_Ty>
{	// generic allocator for objects of class _Ty
public:
	//typedef _Allocator_base<_Ty> _Mybase;
	typedef typename _Ty value_type;
	typedef value_type _FARQ *pointer;
	typedef value_type _FARQ& reference;
	typedef const value_type _FARQ *const_pointer;
	typedef const value_type _FARQ& const_reference;

	typedef _SIZT size_type;
	typedef _PDFT difference_type;

	template<class _Other>
		struct rebind
		{	// convert an allocator<_Ty> to an allocator <_Other>
		typedef TAllocatorAddOnly<_Other> other;
		};

	pointer address(reference _Val) const
		{	// return address of mutable _Val
		return (&_Val);
		}

	const_pointer address(const_reference _Val) const
		{	// return address of nonmutable _Val
		return (&_Val);
		}

	TAllocatorAddOnly()
		_THROW0()
		:m_pMemory(NULL)
		{	// construct default allocator (do nothing)
		}

	TAllocatorAddOnly(const TAllocatorAddOnly<_Ty>& a) _THROW0()
		:m_pMemory(a.m_pMemory)
		{	// construct by copying (do nothing)
		}

	template<class _Other>
		TAllocatorAddOnly(const TAllocatorAddOnly<_Other>& a) _THROW0()
		:m_pMemory(a.m_pMemory)
		{	// construct from a related allocator (do nothing)
		}

	template<class _Other>
		TAllocatorAddOnly<_Ty>& operator=(const TAllocatorAddOnly<_Other>&a)
		{	// assign from a related allocator (do nothing)

			return (*this);
		}

	void deallocate(pointer _Ptr, size_type)
		{	// deallocate object at _Ptr, ignore size
			//return CSqLite3MemAloc::Free(_Ptr);
		}

	pointer allocate(size_type _Count)
		{	// allocate array of _Count elements
			//return (pointer)CSqLite3MemAloc::Malloc(_Count*sizeof(_Ty));
			return (pointer)m_pMemory->New(_Count*sizeof(_Ty));
		}

	pointer allocate(size_type _Count, const void _FARQ *)
		{	// allocate array of _Count elements, ignore hint
			return (allocate(_Count));

		}

	void construct(pointer _Ptr, const _Ty& _Val)
		{	// construct object at _Ptr with value _Val
			std::_Construct(_Ptr, _Val);
		}

	void destroy(pointer _Ptr)
		{	// destroy object at _Ptr
			std::_Destroy(_Ptr);
		//	assert(false);
			//return ;
		}

	_SIZT max_size() const _THROW0()
		{	// estimate maximum array size
		_SIZT _Count = (_SIZT)(-1) / sizeof (_Ty);
		return (0 < _Count ? _Count : 1);
		}

	CMemoryAddOnly* m_pMemory;
};
