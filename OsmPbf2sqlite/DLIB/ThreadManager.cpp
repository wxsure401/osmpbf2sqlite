#include "../stdafx.h"
#include "ThreadManager.h"


CThreadManager::CThreadManager(unsigned nThread)
//:m_arHandle(nThread)
{
	if(nThread==0)
		nThread=GetCountKernels();

	//SThread st;
	m_arThreads.resize(nThread);
	m_arHandle.resize(nThread);
	for(unsigned i=0; i<nThread; ++i)
	{
        m_arThreads[i]=new SThread;

	    m_arHandle[i] =new boost::thread(boost::ref(*m_arThreads[i]));
	    m_arThreads[i]->m_ThreadId=m_arHandle[i]->get_id();
	    m_arThreads[i]->m_MessageQueue.PostThreadMessage(TM_INIT);

		//m_arHandle[i]=::CreateThread(NULL,0,Thread,this,0,&m_arThreads[i].m_ThreadId );
		//while(!PostThreadMessage(m_arThreads[i].m_ThreadId,TM_INIT,0,0))
			//Sleep(0);

	}

}

CThreadManager::~CThreadManager(void)
{
	for(unsigned i=0; i<m_arThreads.size(); ++i)
        m_arThreads[i]->m_MessageQueue.PostThreadMessage( WM_QUIT);
	for(unsigned i=0; i<m_arThreads.size(); ++i)
        m_arHandle[i]->join();


	for(unsigned i=0; i<m_arHandle.size(); ++i)
        delete m_arHandle[i];
    for(unsigned i=0; i<m_arThreads.size(); ++i)
        delete m_arThreads[i];
		//CloseHandle(m_arHandle[i]);

    /*
	for(unsigned i=0; i<m_arThreads.size(); ++i)
		::PostThreadMessage(m_arThreads[i].m_ThreadId, WM_QUIT,0,0);

	DWORD dw=::WaitForMultipleObjects(m_arHandle.size(),&m_arHandle[0],TRUE,INFINITE);
	assert(dw!=WAIT_TIMEOUT);


	for(unsigned i=0; i<m_arThreads.size(); ++i)
		::CloseHandle(m_arHandle[i]);
*/

}


CThreadManager::TaskId CThreadManager::BeginTaskG(CThreadUnit* pThread,
												 CThreadManager::GrupId nGrupId,
												 GrupId gidAfter,int nCount,int nCountCoin)
{
	 STasksGrup tg;
	 tg.m_bGrup=true;
	 tg.m_gidAfter=gidAfter;
	 tg.m_nCount=nCount;
	 return BeginTaskComplex(pThread,nGrupId,&tg,1,nCountCoin);


}


CThreadManager::TaskId CThreadManager::BeginTaskT(CThreadUnit* pThread,GrupId gidMy,CThreadUnit* pThreadAfter,int nCountCoin)
{
	 STasksGrup tg;
	 tg.m_bGrup=false;
	 tg.m_pThreadAfter=pThreadAfter;

	 return BeginTaskComplex(pThread,gidMy,&tg,1, nCountCoin);


}


CThreadManager::TaskId CThreadManager::BeginTaskComplex(
	CThreadUnit* pThread,GrupId gidMy,const STasksGrup* pTasks,int nCountTassks,int nCountCoin)
{

	STask ta;
	ta.m_taskId=pThread;
	ta.m_Grup=gidMy;
	ta.m_nCountCoinReady=ta.m_nCountCoin=nCountCoin;
	ta.m_arDefsTasks.assign(pTasks,pTasks+nCountTassks);
	{
		//Считаем сколько задач нужно ждать
		int nW=0;
		for(int i=0;i<nCountTassks;++i)
		{
			if(pTasks[i].m_bGrup)
				nW+=pTasks[i].m_nCount;
			else
				++nW;

		}

		ta.m_etiState=  nW  ? etiWaitPrev : etiWaitThread;
		ta.m_arTasks.resize(nW);
	}

	pThread->m_GrupId=gidMy;
	pThread->m_pThreadManager=this;

	CComCritSecLock<CComAutoCriticalSection> l(m_cs);
	m_arTask[ta.m_taskId]=ta;
	STask &t=GetTask(ta.m_taskId);

	if(t.m_etiState==etiWaitPrev)
	{ //ищем все предшествующие задачи
		int nPosOut=0;
		int nIns=0;
		for(unsigned i=0;i<t.m_arDefsTasks.size();++i)
		{
			const STasksGrup &tg=t.m_arDefsTasks[i];
			if(tg.m_bGrup)
			{
				for(int j=0;j<tg.m_nCount;++j)
				{
					 //Ищем готовую групп
					for(CTasks::iterator ti=m_arTask.begin();ti!=m_arTask.end();++ti)
					{
						//if(ti->m_etiState==etiReadyUnкnown && ti->m_Grup==tg.m_gidAfter)
						if(ti->second.m_etiState == etiReadyUnknown  &&  ti->second.m_Grup==tg.m_gidAfter)
						{

							if(std::find(&t.m_arTasks[0],&t.m_arTasks[nPosOut],ti->second.m_taskId)!=&t.m_arTasks[nPosOut])  //убираем зацикливание
							{
								t.m_arTasks[nPosOut]=ti->second.m_taskId;
								if(--ti->second.m_nCountCoin == 0)
								{
									ti->second.m_etiState=etiReady;
									++nIns;
								}
								break;
							}
						}
					}
					++nPosOut;
				}

			}
			else
			{
				STask &to=GetTask(tg.m_pThreadAfter);
				if(to.m_etiState==etiReadyUnknown)
				{
					t.m_arTasks[nPosOut]=to.m_taskId;
					if(--to.m_nCountCoin == 0)
					{
						to.m_etiState=etiReady;
						++nIns;
					}

				}
				++nPosOut;
			}

		}
		if(nPosOut==nIns)
			t.m_etiState=etiWaitThread;

	}



	if(t.m_etiState==etiWaitThread)
		WakeUpThread();

	return pThread;
}

//static

//static
unsigned CThreadManager::GetCountKernels() // сколько в системя ядер
{
    return boost::thread::hardware_concurrency();
    /*

	SYSTEM_INFO sysinfo;
	::GetSystemInfo(&sysinfo);
	return sysinfo.dwNumberOfProcessors;
	*/
}

//DWORD WINAPI CThreadManager::Thread(  LPVOID lpThreadParameter  )
void CThreadManager::SThread::operator()()
{
		//setlocale( LC_ALL, "English" );

	unsigned msg;
	TaskId *pMyTask=NULL;
	//CThreadManager *ptm=(CThreadManager *)lpThreadParameter;
	while (m_MessageQueue.GetMessage(&msg))
	{
		switch(msg)
		{
		case TM_INIT:
			{
				pMyTask=&m_taskWork;
			}
			break;
		case TM_START:
			for(;;)
			{
				std::vector<TaskId> art;
				{
					CComCritSecLock<CComAutoCriticalSection> l(m_ptm->m_cs);
					//ищем своюодную задачу
					for(CTasks::iterator ti=m_ptm->m_arTask.begin();ti!=m_ptm->m_arTask.end();++ti)
					{
						if(ti->second.m_etiState==etiWaitThread)
						{
							if(*pMyTask==NULL)
							{

								*pMyTask=ti->second.m_taskId;
								art=ti->second.m_arTasks;
								ti->second.m_etiState=etiWork;
							}
							else
							{
								m_ptm->WakeUpThread();
								break;
							}
						}
					}
					if(*pMyTask==NULL)
						break; //засыпаем


				}
				CThreadUnit* ptMyTask=(CThreadUnit*)(*pMyTask);
				try
				{
					if(art.size()==0)
						ptMyTask->Start(NULL,0);
					else
						ptMyTask->Start((CThreadUnit **)&art[0],art.size());
				}
				catch(...){}

				{
					//Разбираемся с задачами

					CComCritSecLock<CComAutoCriticalSection> l(m_ptm->m_cs);
					STask &t=m_ptm->GetTask(*pMyTask);
					t.m_etiState=etiReadyUnknown;
					//Добавляем себя в ожидающие задачи
					//идём по всем задачам
					for(CTasks::iterator ti=m_ptm->m_arTask.begin();ti!=m_ptm->m_arTask.end();++ti)
					{
						if(t.m_nCountCoin==0)
						{
							t.m_etiState=etiReady;
							break;
						}
						if(ti->second.m_etiState==etiWaitPrev)
						{
							//бежим по ожидаемым
							int nPos=0;
							for(unsigned i=0;i<ti->second.m_arDefsTasks.size();++i)
							{
								if(t.m_nCountCoin==0)
									break;

								STasksGrup &tg=ti->second.m_arDefsTasks[i];
								if(tg.m_bGrup)
								{
									if(tg.m_gidAfter==t.m_Grup)
									{
										for(int j=0;j<tg.m_nCount;++j)
										{
											if(ti->second.m_arTasks[nPos]==NULL)
											{
												ti->second.m_arTasks[nPos]=ptMyTask;
												ti->second.ChekStateWP();
												--t.m_nCountCoin;

												nPos+=tg.m_nCount-j;

												break;

											}

											++nPos;
										}

									}

								}
								else
								{
									if(tg.m_pThreadAfter==ptMyTask)
									{
										ti->second.m_arTasks[nPos]=ptMyTask;
										ti->second.ChekStateWP();
										if(--t.m_nCountCoin==0)
											break;
									}
									++nPos;
								}
							}
						}
					}

					//Отпускаем отработанные
					for(unsigned i=0;i<art.size();++i)
					{
						CTasks::iterator t2=m_ptm->GetTaskIterator(art[i]);
						if(--t2->second.m_nCountCoinReady==0)
							m_ptm->m_arTask.erase(t2);
					}

					*pMyTask=NULL;



				}
			}
			break;
		}

	}
	//return 0;
}
/*
void CThreadManager::SetThreadPriority(int nPriority)
{
	for(unsigned i=0;i<m_arHandle.size();++i)
		::SetThreadPriority(m_arHandle[i],nPriority);
}
*/


class CWait : public CThreadUnit
{
public:
	CWait(): m_bDataReady(false)	{	}
	~CWait(){	}


    void Wait()
    {
            boost::unique_lock<boost::mutex> lock(m_Mut);
            while(!m_bDataReady)
                m_Cond.wait(lock);

    }
private:
    boost::condition_variable m_Cond; //событие
    boost::mutex m_Mut;
    bool m_bDataReady;
	 void Start(CThreadUnit** pTasks, int countTasks)
	 {
        {
            boost::lock_guard<boost::mutex> lock(m_Mut);
            m_bDataReady=true;
        }
        m_Cond.notify_one();
	 }


};

//Ожидание завершения выполнения задач из группы giW если 0 то ждем всех
void CThreadManager::Wait(GrupId giW,int nCount)
{
	CWait w;
	if(nCount==0)
	{
		//ищем
		CComCritSecLock<CComAutoCriticalSection> l(m_cs);
		for(CTasks::iterator ti=m_arTask.begin();ti!=m_arTask.end();++ti)
			if(ti->second.m_Grup==giW)
				nCount+=ti->second.m_nCountCoin;

	}

	BeginTaskG(&w,0,giW,nCount,0);


    w.Wait();

}



CThreadManager::CTasks::iterator CThreadManager::GetTaskIterator(CThreadManager::TaskId tid)
{
	//STask t;
	//t.m_taskId=tid;
	return m_arTask.find(tid);
}

void CThreadManager::WakeUpThread()
{
	for(unsigned i=0;i<m_arThreads.size();++i)
		if(m_arThreads[i]->m_taskWork==0)
		{
		    m_arThreads[i]->m_MessageQueue.PostThreadMessage(TM_START);
			break;
		}
}

//Проверка состояния etiWaitPrev и если пора то  перевод в состояние etiWaitThread

void CThreadManager::STask::ChekStateWP()
{
	if(m_etiState!=etiWaitPrev)
		return;
	for(unsigned i=0;i<m_arTasks.size();++i)
		if(m_arTasks[i]==0)
			return;
	m_etiState=etiWaitThread;
}

/////////////////////////////////////////////////////////////

bool CThreadManager::SThread::CMessageQueue::PostThreadMessage(unsigned message)
{
    {
        boost::lock_guard<boost::mutex> lock(m_mut);
        if(m_arMessage[0]==WM_QUIT)
            return false;

        if(message==WM_QUIT)
        {
            m_arMessage[0]=WM_QUIT;
            m_nRead=0;
        }
        else
        {
            int nNext=m_nWrite+1;
            if(nNext>=(int)m_arMessage.size())
                nNext=0;
            if(nNext==m_nWrite)
            { //Увеличиваем очередь сообщений
                int sz=m_arMessage.size();
                std::vector<unsigned> ar(sz*2);
                memcpy(&ar[sz+m_nRead],&m_arMessage[m_nRead],sizeof(int)*(sz-m_nRead));
                memcpy(&ar[0],&m_arMessage[0],sizeof(int)*(m_nRead));
                ar.swap(m_arMessage);
                m_nRead+=sz;

            }
            m_nWrite=nNext;
            m_arMessage[m_nWrite]=message;

        }
    }


    m_cond.notify_one();
    return true;
}

 bool CThreadManager::SThread::CMessageQueue::GetMessage(unsigned *pMessage)
 {
    boost::unique_lock<boost::mutex> lock(m_mut);
    if(m_arMessage[0]==WM_QUIT)
    {

       *pMessage=WM_QUIT;
       return false;

    }
    while(m_nRead==m_nWrite)
    {
        m_cond.wait(lock);
        if(m_arMessage[0]==WM_QUIT)
        {

           *pMessage=WM_QUIT;
           return false;

        }
    }
    *pMessage=m_arMessage[m_nRead++];
    if(m_nRead>=(int)m_arMessage.size())
        m_nRead=0;
    return true;

 }
