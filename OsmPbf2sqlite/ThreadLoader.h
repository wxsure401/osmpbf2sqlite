#pragma once
#include "dlib\threadmanager.h"
#include "DB.h"
#include "OSMPBF/osmpbf.h"


class CThreadLoader :
	public CThreadUnit
{
public:
	CThreadLoader(void);
	virtual ~CThreadLoader(void);


	virtual void Start(CThreadUnit** pTasks, int countTasks);

	OSMPBF::PrimitiveBlock* m_pprimblock;
	CComAutoCriticalSection* m_pcs;
	FILE *m_fp;
	CDB* m_pDB;

	volatile static  LONG m_nCount;
	int m_nThredNumber;
	void AddNode(const OSMPBF::Node& n);
	void AddDense(const OSMPBF::DenseNodes& d);
	void AddWay(const OSMPBF::Way& w);
	void AddRelations(const OSMPBF::Relation& r);

	//std::map<int,int> m_mapCount;
};
