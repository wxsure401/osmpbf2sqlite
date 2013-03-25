#pragma once
template<class _Ty>
class TSlicedArray
{
public:
	TSlicedArray(const TSlicedArray &sa)
		:m_pBegin(sa.m_pBegin)
		,m_pEnd(sa.m_pEnd)
	{
	}

	TSlicedArray(const _Ty* pBegin, const _Ty* pEnd)
		:m_pBegin(pBegin)
		,m_pEnd(pEnd)
	{
	}

	TSlicedArray(const _Ty* pBegin, size_t uCount)
		:m_pBegin(pBegin)
		,m_pEnd(pBegin+uCount)
	{
	}

	TSlicedArray(void)
		:m_pBegin(NULL)
		,m_pEnd(NULL)
	{
	}

	virtual ~TSlicedArray(void)
	{
	}

	const _Ty* m_pBegin;
	const _Ty* m_pEnd;

	bool empty()const
		{	return m_pBegin>=m_pEnd;	}

	size_t size()const
		{	return m_pEnd-m_pBegin;	}

	void clear()
		{	 m_pEnd=m_pBegin=NULL;	}

	// nFrom < 0 то берем с конца -1 это последний символ. 
	// nTo =0 берм все -1 берём кроме последнего
	TSlicedArray SliceTo(int nFrom, int nTo=0);


	// nFrom < 0 то берем с конца -1 это последний символ. 
	// nСount =0 берм все -1 берём кроме последнего
	TSlicedArray Slice(int nFrom,int nCount);

	void Atttach(const std::vector<_Ty> &ar);
	std::vector<_Ty> Detach();

	//Сравнение с нультерминантной строкой
	int compareString(const _Ty* pb)const
	{
		if(m_pBegin==NULL)
			return *pb==0 ? 0 : -1;

		const _Ty* pIn=m_pBegin;
		for(;pIn<m_pEnd && *pb!=0;pIn++,pb++)
		{
			if(*pIn<*pb)
				return -1;

			if(*pIn>*pb)
				return 1;
		}
		return pIn==m_pEnd && *pb==0 ? 0 :(pIn==m_pEnd ? -1 : 1);
	}
};
