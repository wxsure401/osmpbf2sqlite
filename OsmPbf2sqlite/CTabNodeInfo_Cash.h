#ifndef CTABNODEINFO_CASH_H
#define CTABNODEINFO_CASH_H

#include "TTable_cash.h"

struct STabNodeInfo_Cash
{
        sqlite3_int64  m_n1;
        sqlite3_int64  m_n2;
        int            m_n3;
        sqlite3_int64  m_n4;

    void Bind(CSQLite3Table*ptab)
    {
        ptab->bind_int64(1,m_n1);
        ptab->bind_int64(2,m_n2);
        ptab->bind_int  (3,m_n3);
        ptab->bind_int64(4,m_n4);
    }
};

typedef TTable_cash<STabNodeInfo_Cash> CTabNodeInfo_Cash;


#endif // CTABNODEINFO_CASH_H
