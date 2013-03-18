#pragma once

namespace PBFRO {
template <class T, class _Ax = std::allocator<T> >
class svector
{
public:

	svector(void)
		:m_pBegin(NULL)
		,m_pEnd(NULL)
		,m_pLast(NULL)
	{
	}

	virtual ~svector(void)
	{
		clear();
	}

	void clear();

	bool empty()const;

	size_t size()const;
	size_t capacity()const;
	
	void reserve(size_t count);
	void resize(size_t count, const T& t = T());
	void push_back(const T& t);	

	T& operator[](size_t ind);
	const T& operator[](size_t ind)const;

	const T& front()const;
	T& front();

	const T& back()const;
	T& back();

protected:
	_Ax _Alval;
	T* m_pBegin;
	T* m_pEnd;
	T* m_pLast;
};

template <class T, class _Ax >
inline void svector< T,_Ax>::clear()
{
//	resize(0);
	if(m_pBegin)
		_Alval.deallocate(m_pBegin,0);
	m_pBegin= m_pEnd= m_pLast=NULL;

//	m_pEnd=m_pBegin;
}

template <class T, class _Ax >
inline bool svector< T,_Ax>::empty()const
{
	return m_pBegin>=m_pEnd;
}

template <class T, class _Ax >
inline size_t svector< T,_Ax>::size()const
{
	return m_pEnd-m_pBegin;

}
template <class T, class _Ax >
inline size_t svector< T,_Ax>::capacity()const
{
	return  m_pLast-m_pBegin;

}
	


template <class T, class _Ax >
inline T& svector< T,_Ax>::operator[](size_t ind)
{
	ASSERT(ind<size());
	return m_pBegin[ind];

}
template <class T, class _Ax >
inline const T& svector< T,_Ax>::operator[](size_t ind)const
{
	ASSERT(ind<size());
	return m_pBegin[ind];
}

template <class T, class _Ax >
inline const T& svector< T,_Ax>::front()const
{
	ASSERT(m_pBegin<m_pEnd);
	return *m_pBegin;

}
template <class T, class _Ax >
inline T& svector< T,_Ax>::front()
{
	ASSERT(m_pBegin<m_pEnd);
	return *m_pBegin;
}

template <class T, class _Ax >
inline const T& svector< T,_Ax>::back()const
{
	ASSERT(m_pBegin<m_pEnd);
	return *(m_pEnd-1);
}
template <class T, class _Ax >
inline T& svector< T,_Ax>::back()
{
	ASSERT(m_pBegin<m_pEnd);
	return *(m_pEnd-1);
}

template <class T, class _Ax >
inline void svector< T,_Ax>::reserve(size_t count)
{
	if(count<=capacity())
		return;
	T* pN=_Alval.allocate(count);
	if(pN==NULL)
		throw 778;
	size_t sz=size();
	::memcpy(pN,m_pBegin,sz*sizeof(T));
	m_pBegin=pN;
	m_pEnd=m_pBegin+sz;
	m_pLast=m_pBegin+count;
}


template <class T, class _Ax >
inline void svector< T,_Ax>::resize(size_t count, const T& t)
{
	reserve(count);
	size_t sz=size();
	if(sz < count )
	{
		for(;sz < count;sz++)
			_Alval.construct(m_pEnd++,t);
	}
	else if(sz > count )
	{
		for(;sz > count;sz--)
			_Alval.destroy(m_pEnd--);
	}
}

template <class T, class _Ax >
inline void svector< T,_Ax>::push_back(const T& t)
{
	{
		size_t sz=size();
		if( capacity() <= sz)
		{
			reserve(max(32,sz*2));
		}
	}
	_Alval.construct(m_pEnd++,t);



}


} //namespace PBFRO 