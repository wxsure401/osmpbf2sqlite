#include "StdAfx.h"
#include "Stopwatch.h"
double CStopwatch::m_dFreq=0;


CStopwatch::CStopwatch(bool bStart)
:m_liStart(0)
{
	if(m_dFreq==0)
	{
		 LARGE_INTEGER  ic;
		BOOL b=::QueryPerformanceFrequency( &ic );
		m_dFreq=(1./ic.QuadPart);

	}
	if(bStart)
		Start();

}

CStopwatch::~CStopwatch(void)
{
}
void CStopwatch::Start()
{
	::QueryPerformanceCounter( (LARGE_INTEGER*)&m_liStart );
}

void CStopwatch::Stop(const char* sText)
{
	LARGE_INTEGER ic;
	::QueryPerformanceCounter( &ic );
	printf("%s%6f\n",sText,double((ic.QuadPart-m_liStart)*m_dFreq));
}
