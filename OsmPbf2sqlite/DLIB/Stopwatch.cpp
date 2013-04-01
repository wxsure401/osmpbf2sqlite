#include "../stdafx.h"
#include "Stopwatch.h"

#ifdef WIN32
double CStopwatch::m_dFreq=0;
#endif	//WIN32


CStopwatch::CStopwatch(bool bStart)
#ifdef WIN32
:m_liStart(0)
#endif	//WIN32

{
#ifdef WIN32
	if(m_dFreq==0)
	{
		 LARGE_INTEGER  ic;
		BOOL b=::QueryPerformanceFrequency( &ic );
		m_dFreq=(1./ic.QuadPart);

	}
#else //WIN32

#endif	//WIN32

	if(bStart)
		Start();

}

CStopwatch::~CStopwatch(void)
{
}
void CStopwatch::Start()
{
#ifdef WIN32
	::QueryPerformanceCounter( (LARGE_INTEGER*)&m_liStart );
#else //WIN32

    boost::xtime_get(&m_xtStart, boost::TIME_UTC_);

#endif	//WIN32
}

void CStopwatch::Stop(const char* sText)
{
    double  rez;

#ifdef WIN32

    {
        LARGE_INTEGER ic;
        ::QueryPerformanceCounter( &ic );
        rez=double((ic.QuadPart-m_liStart)*m_dFreq);
    }
#else //WIN32
    {
        boost::xtime xtEnd;
        boost::xtime_get(&xtEnd, boost::TIME_UTC_);
        rez=xtEnd.sec + double(xtEnd.nsec/1000000000);
        rez-=m_xtStart.sec + double(m_xtStart.nsec/1000000000);

    }

#endif	//WIN32

	printf("%s%6f\n",sText,rez);
}
