#ifndef TTABLE_CASH_H
#define TTABLE_CASH_H

#include "DLIB/SQLite3Table.h"

template <class T>
class TTable_cash
{
public:
        TTable_cash()
            :m_ar(32000)
            ,m_nAdd(0)
            ,m_ptab(NULL)
            ,m_pcs(NULL)

            {}
        virtual ~TTable_cash() {}

    void Init(CSQLite3Table* ptab,boost::mutex* pcs)
    {
        m_ptab=ptab;
        m_pcs=pcs;

    }

    T& Prepare()
    {
        return m_ar[m_nAdd];
    }

    void Update()
    {
        if(++m_nAdd==m_ar.size())
            Save();

    }

    void Save()
    {
        if(m_nAdd==0)
            return;

        {
            boost::lock_guard<boost::mutex> l(*m_pcs);
            for(unsigned i=0;i<m_nAdd;++i)
            {
                m_ptab->reset();
                m_ar[i].Bind(m_ptab);
                m_ptab->step();
            }

        }

        m_nAdd=0;
    }

 protected:
    std::vector<T> m_ar;
    unsigned m_nAdd; //количество добавленных
    CSQLite3Table* m_ptab;  //Опслуживаемая таблица
    boost::mutex* m_pcs;

 private:

};

#endif // TTABLE_CASH_H
