#pragma once
#include <memory.h>
#include <math.h>
#include <stdlib.h>

#include "../DLIB/TSlicedArray.h"

namespace PBFRO {

typedef _int32 Tag;
typedef TSlicedArray<char> CBuffer;

class CInputStream : public CBuffer
{
public:
	CInputStream(const char *pbuf, size_t count);
	~CInputStream(void);

	Tag ReadTag();

	template <class T>
	bool ReadVariant(T &value)
	{
		T result = 0;
		unsigned count = 0;
		unsigned char b;

		do {
			if (count > (sizeof(T)*8)/7 || empty())
				return false;
			b = *m_pBegin;
			result |= ((T)( b & 0x7F ))<< (7 * count) ;
			Advance();
			++count;
		} while (b & 0x80);

		value = result;
		return true;
	}
	bool SkipField();

	template <class T>
	bool ReadLittleEndian( T &value)
	{
		if(size()<sizeof(T))
			return false;
		value=*(T*)m_pBegin;
		Advance(sizeof(T));

		return true;
	}

	bool Skip(unsigned len)
	{
		if(m_pBegin + len > m_pEnd)
			return false;
		Advance(len);
		return true;
	}

	bool IncrementRecursionDepth()
	{
		if(recursion_depth_>=recursion_limit_)
			return false;
		++recursion_depth_;
		return true;
	}

	void DecrementRecursionDepth()
	{
		--recursion_depth_;
	}

	bool SkipMessage() ;

	//template <class T>
	bool ReadRaw(char *buf,size_t count)
	{
		if(size() < count)
			return false;
		memcpy(buf,m_pBegin,count);
		Advance(count);
		return true;
	}

	typedef const char* Limit;
	Limit PushLimit(size_t  len)
	{
		len=std::min(size(),len);
		Limit old=m_pEnd;
		m_pEnd=m_pBegin+len;
		return old;
	}
	void PopLimit(Limit li)
	{
		m_pEnd=li;
	}

	bool InternalReadStringInline(CBuffer *buf,  size_t sz)
	{
		if (sz > size())
			return false;  // security: size is often user-supplied

		*buf=*this;
		buf->m_pEnd=buf->m_pBegin+sz;
		Advance(sz);
		return true;

	}

	bool ExpectAtEnd()const
	{
		return empty();
	}


	void* m_pParam;  //Дополнитедбный параметр
private:

	Tag last_tag_;
	static const unsigned recursion_limit_=64;
	unsigned recursion_depth_;

    void Advance(unsigned u=1)
	{

		ASSERT(m_pBegin+u<=m_pEnd); //чтобы было nothrow
		m_pBegin+=u;
	}
};

inline Tag CInputStream::ReadTag()
{
	last_tag_=0;
	ReadVariant(last_tag_);
	return last_tag_;
}

}//namespace PBFRO
