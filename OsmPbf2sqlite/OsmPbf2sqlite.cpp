// OsmPbf2sqlite.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "OSMPBF/osmpbf.h"
#include "DLIB/Stopwatch.h"
#include "DLIB/ThreadManager.h"
#include "ThreadLoader.h"
#include "SqLite3MemAloc.h"

// should the output use color?


// buffer for reading a compressed blob from file
//char buffer[OSMPBF::max_uncompressed_blob_size];
//unsigned char unpack_buffer[OSMPBF::max_uncompressed_blob_size];

int _tmain(int argc, _TCHAR* argv[])
{
	//setlocale( LC_ALL, "English" );


	_TCHAR* fn= argc==2  ? argv[1] : NULL;
#ifdef WIN32

#else
    fn="/home/di/Загрузки/RU-MOW.osm.pbf";
#endif
     argc=2;

	CStopwatch sw;
	if(argc!=2 || fn[0]==L'-'
#ifdef WIN32
        || fn[0]==L'/'
#endif
    )
	{
		info("Convert osm-pbf file to sqlite database. \t\tby deep125 03.2013 v1.1\nuse:\nosmpbf2sqlite.exe pbffile");
		return 0;

	}

	// open specified file
	FILE *fp = _tfopen(fn , _T("rb"));
	if(  fp==NULL)
	{
		err("_wfopen failed!\n");
		return -1;
	}

	sw.Start();
	CSqLite3MemAloc ma;
	ma.Init();

	CDB m_db;
	m_db.m_pSource=fn;
	m_db.Init();

	{
		boost::mutex m_csFile;
		std::vector<CThreadLoader> ar(2);

			CThreadManager m_tm(ar.size());
			for(unsigned i=0;i<ar.size();++i)
			{
				ar[i].m_pcs=&m_csFile;
				ar[i].m_fp=fp;
				ar[i].m_nThredNumber=i;
				ar[i].m_pDB=&m_db;
				m_tm.BeginTaskG(&ar[i],1,0,0);
			}
			/*
			{
				CThreadConnect tcTest;
				CThreadUnit* m[2];
				m[0]=&ar[0];
				m[1]=&ar[1];
				tcTest.Start(m,2);
				err("stop");
			}
			*/

/*
		if(ar.size()==4)
		{
			CThreadConnect tc1;
			m_tm.BeginTaskG(&tc1,2,1,2);
			CThreadConnect tc2;
			m_tm.BeginTaskG(&tc2,2,1,2);
			CThreadConnect tc3;
			m_tm.BeginTaskG(&tc3,3,2,2);
			m_tm.Wait(3);
		}
		else
		*/
		m_tm.Wait(1);
		m_db.Close();
		/*
		int nCount=0;
		for(unsigned i=0;i<ar.size();++i)
		{
			nCount+=ar[i].m_nCount;

		}
		*/
		info("  Bloks=%d\n",CThreadLoader::m_nCount);

	}

	ma.Close();
	sw.Stop("All time=");
	fclose(fp);
	return 0;
}

