#pragma once
#include "DLIB/ThreadManager.h"
#include "DB.h"
#include "OSMPBF/osmpbf.h"
#include "CTabNode_Cash.h"
#include "CTabNodeInfo_Cash.h"
#include "CTabWay_Cash.h"
#include "CTabRelation_Cash.h"

class CThreadLoader :
	public CThreadUnit
{
public:

	CThreadLoader(void);
	virtual ~CThreadLoader(void);


    void Init(int nThredNumber,FILE *fp,CDB* pDB);

	virtual void Start(CThreadUnit** pTasks, int countTasks);

	boost::mutex* m_pcs;
	FILE *m_fp;
	CDB* m_pDB;
	OSMPBF::PrimitiveBlock* m_pprimblock;
	int m_nThredNumber;

	static ATOMIC_INT m_nCount;
	void AddNode(const OSMPBF::Node& n);
	void AddDense(const OSMPBF::DenseNodes& d);
	void AddWay(const OSMPBF::Way& w);
	void AddRelations(const OSMPBF::Relation& r);


	CTabNode_Cash m_tabNode_Cash;
	CTabNode_Cash m_tabNode_NotVisible_Cash;

    CTabNodeInfo_Cash m_tabNI_Cash;
    CTabNodeInfo_Cash m_tabNI_NotVisible_Cash;

    CTabNodeInfo_Cash m_tabWI_Cash;
    CTabNodeInfo_Cash m_tabWI_NotVisible_Cash;

    CTabNodeInfo_Cash m_tabRI_Cash;
    CTabNodeInfo_Cash m_tabRI_NotVisible_Cash;

	CTabWay_Cash m_tabWay_Cash;
	CTabWay_Cash m_tabWay_NotVisible_Cash;

	CTabRelation_Cash m_tabRelation_Cash;
	CTabRelation_Cash m_tabRelation_NotVisible_Cash;

	CKV_cash m_tabkvNode_cash;
	CKV_cash m_tabkvWay_cash;
	CKV_cash m_tabkvRelation_cash;
};
