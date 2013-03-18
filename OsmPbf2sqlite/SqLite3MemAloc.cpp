#include "StdAfx.h"
#include "SqLite3MemAloc.h"

sqlite3_mem_methods CSqLite3MemAloc::m_OldMem;
sqlite3_mem_methods CSqLite3MemAloc::m_Mem;

int CSqLite3MemAloc::m_nIndTls=0;

CSqLite3MemAloc::CSqLite3MemAloc(void)
{
	ASSERT(m_nIndTls==0);
	m_nIndTls=TlsAlloc();
}

CSqLite3MemAloc::~CSqLite3MemAloc(void)
{
}
void CSqLite3MemAloc::Init()
{
	ASSERT(m_OldMem.pAppData==0);
	sqlite3_config(SQLITE_CONFIG_GETMALLOC, &m_OldMem);

	m_Mem.xMalloc=Malloc;
	m_Mem.xFree=Free;
	m_Mem.xRealloc=Realloc;
	m_Mem.xSize=Size;
	m_Mem.xRoundup=Roundup;
	m_Mem.xInit=Init;
	m_Mem.xShutdown=Shutdown;
	m_Mem.pAppData=m_OldMem.pAppData;

#ifndef _DEBUG  //В дебаге обычный менеджер памяти в релизе потоко независимый
	sqlite3_config(SQLITE_CONFIG_MALLOC, &m_Mem);
#endif
}
void CSqLite3MemAloc::Close()
{
	sqlite3_config(SQLITE_CONFIG_MALLOC, &m_OldMem);
}

void* CSqLite3MemAloc::Malloc(int n)         /* Memory allocation function */
{
	SHips* pH=(SHips*)::TlsGetValue(m_nIndTls);
	if(pH==0)
	{
		pH=new SHips;
		::TlsSetValue(m_nIndTls,pH);
		
	}

	return pH->Malloc(n);

	//return m_OldMem.xMalloc(n);
}
void CSqLite3MemAloc::Free(void* p)          /* Free a prior allocation */
{
	SHips* pH=(SHips*)::TlsGetValue(m_nIndTls);
	if(pH)
		pH->Free(p);

}
void* CSqLite3MemAloc::Realloc(void* p,int n)  /* Resize an allocation */
{
	SHips* pH=(SHips*)::TlsGetValue(m_nIndTls);
	return pH->Realloc(p,n);

	//return m_OldMem.xRealloc(p,n);
}

int CSqLite3MemAloc::Size(void* p)           /* Return the size of an allocation */
{
	SHips* pH=(SHips*)::TlsGetValue(m_nIndTls);
	return pH->Size(p);
//	return m_OldMem.xSize(p);
}

int CSqLite3MemAloc::Roundup(int n)          /* Round up request size to allocation size */
{
	//return m_OldMem.xRoundup(n);
	return (((n)+7)&~7);
}

int CSqLite3MemAloc::Init(void* p)           /* Initialize the memory allocator */
{
	//return m_OldMem.xInit(p);
	return SQLITE_OK;
}
void CSqLite3MemAloc::Shutdown(void*p)      /* Deinitialize the memory allocator */
{
	m_OldMem.xShutdown(p);
}

//////////////////////////////////////////////////////////

//struct SHips
CSqLite3MemAloc::SHips::SHips()
{
	ZeroMemory(m_hip,sizeof(m_hip));
}
CSqLite3MemAloc::SHips::~SHips()
{
	for(int i=0;i<_countof(m_hip);++i)
		delete m_hip[i];
}
//	SHip* m_hip[9]; // массив выделенных блоков памяти m_hip[0] -> это по 64 байта,m_hip[1]- 128 байт и тд 

void* CSqLite3MemAloc::SHips::Malloc(int n)
{
	return GetHip(n)->Malloc(n);
}
void CSqLite3MemAloc::SHips::Free(void* p)
{
	GetHip(Size(p))->Free(p);
}
void* CSqLite3MemAloc::SHips::Realloc(void* p,int n)
{
	size_t szOld=Size(p);
	if(szOld==n)
		return p;

	SHip* hN=GetHip(n);
	SHip* hO=GetHip(szOld);
	if(hN==hO)
	{
		//меняем только размеры
		SShunk &pSh=((SShunk *)p)[-1];
		pSh.nSize=n;
		return p;
	}

	void* pNew=Malloc(n);
	memcpy(pNew,p,min(n,(int)szOld));
	Free(p);
	return pNew;
}
size_t CSqLite3MemAloc::SHips::Size(void* p)
{
	if(!p)
		return 0;
	SShunk &pSh=((SShunk *)p)[-1];
	return pSh.nSize;
}           

CSqLite3MemAloc::SHip* CSqLite3MemAloc::SHips::GetHip(int n)
{
	int pos=0;

	for(;int((64<<pos)-sizeof(SShunk)) < n;++pos);

	ASSERT(pos < _countof(m_hip));

	if(m_hip[pos]==NULL)
		m_hip[pos]=new SHip(64<<pos);

	return m_hip[pos];

}
////////////////////////////////////////

CSqLite3MemAloc::SHip::SHip(int SizeShunk,int nVer)
:m_SizeShunk( SizeShunk)
,m_nVer(nVer)
,m_CountShunk(max(16,nVer*65536/SizeShunk))
,m_nPosNext(0)
,m_pNext(NULL)
,m_pBuf(new __int8[m_CountShunk*m_SizeShunk])
{
	//Инициализируем
	__int8 *pb=m_pBuf;
	for(unsigned i=1;i<m_CountShunk;++i,pb+=SizeShunk)
		((SShunk*)pb)->nPosNext=i;
   ((SShunk*)pb)->nPosNext=-1; //Последний ни куда не указывает


}
CSqLite3MemAloc::SHip::~SHip()
{
	delete m_pNext;
	delete []m_pBuf;
}
/*
size_t  m_SizeShunk; //Размер кусочка
size_t  m_CountShunk; //Количество кусочков в к этом хипе
size_t m_nPosNext; //Позиция первого не занятого
SHip * m_pNext; //Позиция следующего в этой куче
		void*  m_pBuf; //непосредственно буфер памяти
*/
void* CSqLite3MemAloc::SHip::Malloc(int n)
{
	if(m_nPosNext!=-1)
	{

		SShunk* sh=(SShunk*)(m_pBuf + m_nPosNext*m_SizeShunk);
		m_nPosNext=sh->nPosNext;
		sh->nSize=n;
		return sh+1;
	}
	if(m_pNext==NULL)
		m_pNext= new SHip(m_SizeShunk, m_nVer*2);

	void* ret = m_pNext->Malloc(n);

	swap();

	return ret;
}
void CSqLite3MemAloc::SHip::swap()
{
	std::swap(m_CountShunk,m_pNext->m_CountShunk);
	std::swap(m_nPosNext,m_pNext->m_nPosNext);
	std::swap(m_pBuf,m_pNext->m_pBuf);
}

void CSqLite3MemAloc::SHip::Free(void* p)
{
	if(p> m_pBuf && p < m_pBuf+m_CountShunk*m_SizeShunk)
	{
		SShunk* sh=&((SShunk*)p)[-1];
		int n=( (__int8*)sh  - m_pBuf)/ m_SizeShunk;
		ASSERT( ( (__int8*)sh  - m_pBuf)% m_SizeShunk == 0);
		sh->nPosNext=m_nPosNext;
		m_nPosNext=n;

	}
	else if(m_pNext)
	{
		m_pNext->Free(p);
		swap();
	}
	else
	{
		ASSERT(FALSE);

	}

}
