#ifndef CTABWAY_CASH_H
#define CTABWAY_CASH_H

#include "TTable_cash.h"

struct STabWay_Cash
{
    sqlite3_int64  m_n1;
    sqlite3_int64  m_n2;
    int            m_n3;

    void Bind(CSQLite3Table*ptab)const
    {
        ptab->bind_int64(1,m_n1);
        ptab->bind_int64(2,m_n2);
        ptab->bind_int  (3,m_n3);
    }
	bool operator<(const STabWay_Cash& c)const 
	{
		return m_n1<c.m_n1;
	}

};

typedef TTable_cash<STabWay_Cash> CTabWay_Cash;


#endif // CTABWAY_CASH_H
