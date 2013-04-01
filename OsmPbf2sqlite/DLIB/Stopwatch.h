#pragma once

class CStopwatch
{
public:
	CStopwatch(bool bStart=false);
	~CStopwatch(void);

	void Start();

	void Stop(const char* sText);

private:
#ifdef WIN32
	static double m_dFreq;
	__int64 m_liStart;
#else
	 boost::xtime m_xtStart;
#endif

};

