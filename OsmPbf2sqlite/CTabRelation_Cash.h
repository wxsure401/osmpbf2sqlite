#ifndef CTABRELATION_CASH_H
#define CTABRELATION_CASH_H

#include "TTable_cash.h"

struct STabRelation_Cash
{
    sqlite3_int64  m_n1;
    sqlite3_int64  m_n2;
    int            m_n3;
    int            m_n4;

    void Bind(CSQLite3Table*ptab)const
    {
        ptab->bind_int64(1,m_n1);
        ptab->bind_int64(2,m_n2);
        ptab->bind_int  (3,m_n3);
        ptab->bind_int  (4,m_n4);
    }
	bool operator<(const STabRelation_Cash& c)const 
	{
		return m_n1<c.m_n1;
	}

};

typedef TTable_cash<STabRelation_Cash> CTabRelation_Cash;


#endif // CTABRELATION_CASH_H
