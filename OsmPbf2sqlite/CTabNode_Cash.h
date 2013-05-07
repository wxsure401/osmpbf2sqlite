#ifndef CTABNODE_CASH_H
#define CTABNODE_CASH_H

#include "TTable_cash.h"

struct STabNode_Cash
{
    sqlite3_int64  m_n1;
    sqlite3_int64  m_n2;
    sqlite3_int64  m_n3;

    void Bind(CSQLite3Table*ptab)const
    {
        ptab->bind_int64(1,m_n1);
        ptab->bind_int64(2,m_n2);
        ptab->bind_int64(3,m_n3);
    }
	bool operator<(const STabNode_Cash& c)const 
	{
		return m_n1<c.m_n1;
	}

};

typedef TTable_cash<STabNode_Cash> CTabNode_Cash;


#endif // CTABNODE_CASH_H
