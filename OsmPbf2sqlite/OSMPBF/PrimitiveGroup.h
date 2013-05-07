#pragma once
#include "Node.h"
#include "DenseNodes.h"
#include "Way.h"
#include "Relation.h"
#include "ChangeSet.h"


namespace OSMPBF {

class PrimitiveGroup :
	public PBFRO::CMessage
{
public:
	PrimitiveGroup(){};

	virtual void Init(PBFRO::CInputStream *pis)
	{
		m_dense.Init(pis);
		CMemoryAddOnly* pm=(CMemoryAddOnly*)pis->m_pParam;
		m_nodes.Get_Alval()->m_pMemory=pm;
		m_ways.Get_Alval()->m_pMemory=pm;
		m_relations.Get_Alval()->m_pMemory=pm;
		m_changesets.Get_Alval()->m_pMemory=pm;
	}

	PBFRO::TFar<Node ,TAllocatorAddOnly<Node> > m_nodes;           //repeated Node     nodes = 1;
	DenseNodes m_dense;				     //optional DenseNodes dense = 2;
	PBFRO::TFar<Way,TAllocatorAddOnly<Way> > m_ways;			 //repeated Way      ways = 3;
	PBFRO::TFar<Relation,TAllocatorAddOnly<Relation> > m_relations;	 //repeated Relation relations = 4;
	PBFRO::TFar<ChangeSet,TAllocatorAddOnly<ChangeSet> > m_changesets; //repeated ChangeSet changesets = 5;

	FIELDS_BEGIN(5)
		FIELDS_DEFINE(1,m_nodes )
		FIELDS_DEFINE(2,m_dense )
		FIELDS_DEFINE(3,m_ways )
		FIELDS_DEFINE(4,m_relations )
		FIELDS_DEFINE(5,m_changesets )
	FIELDS_END

};



}//namespace OSMPBF 