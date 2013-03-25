#include "../StdAfx.h"
#include "ThreadManager.h"

CThreadManager::CThreadManager(unsigned nThread)
{
	if(nThread==0)
		nThread=GetCountKernels();

	SThread st;
	m_arThreads.resize(nThread,st);
	m_arHandle.resize(nThread);
	for(unsigned i=0; i<nThread; ++i)
	{
		
		m_arHandle[i]=::CreateThread(NULL,0,Thread,this,0,&m_arThreads[i].m_ThreadId );
		while(!::PostThreadMessage(m_arThreads[i].m_ThreadId,TM_INIT,0,0))
			Sleep(0);

	}

}

CThreadManager::~CThreadManager(void)
{
	for(unsigned i=0; i<m_arThreads.size(); ++i)
		::PostThreadMessage(m_arThreads[i].m_ThreadId, WM_QUIT,0,0);
	
	DWORD dw=::WaitForMultipleObjects(m_arHandle.size(),&m_arHandle[0],TRUE,INFINITE);
	assert(dw!=WAIT_TIMEOUT);


	for(unsigned i=0; i<m_arThreads.size(); ++i)
		::CloseHandle(m_arHandle[i]);


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
	m_arTask.insert(ta);
	STask &t=GetTask(ta.m_taskId);
	
	if(t.m_etiState==etiWaitPrev)
	{ //ищем все предшествующие задачи
		int nPosOut=0;
		int nIns=0;
		for(unsigned i=0;i<t.m_arDefsTasks.size();++i)
		{
			STasksGrup &tg=t.m_arDefsTasks[i];
			if(tg.m_bGrup)
			{
				for(int j=0;j<tg.m_nCount;++j)
				{
					 //Ищем готовую групп
					for(CTasks::iterator ti=m_arTask.begin();ti!=m_arTask.end();++ti)
					{
						if(ti->m_etiState==etiReadyUnкnown && ti->m_Grup==tg.m_gidAfter)
						{

							if(std::find(&t.m_arTasks[0],&t.m_arTasks[nPosOut],ti->m_taskId)!=&t.m_arTasks[nPosOut])  //убираем зацикливание
							{
								t.m_arTasks[nPosOut]=ti->m_taskId;
								if(--ti->m_nCountCoin == 0)
								{
									ti->m_etiState=etiReady;
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
				if(to.m_etiState==etiReadyUnкnown)
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
int CThreadManager::GetCountKernels() // сколько в системя ядер  
{
	SYSTEM_INFO sysinfo;
	::GetSystemInfo(&sysinfo);
	return sysinfo.dwNumberOfProcessors;
}

DWORD WINAPI CThreadManager::Thread(  LPVOID lpThreadParameter  )
{
		//setlocale( LC_ALL, "English" );

	MSG msg;
	TaskId *pMyTask=NULL;
	CThreadManager *ptm=(CThreadManager *)lpThreadParameter;
	while (GetMessage(&msg,NULL,0,0))
	{
		switch(msg.message)
		{
		case TM_INIT:
			{
				DWORD tid=::GetCurrentThreadId();
				for(unsigned i=0;i<ptm->m_arThreads.size();++i)
				{
					if(ptm->m_arThreads[i].m_ThreadId==tid)
					{
						pMyTask=&ptm->m_arThreads[i].m_taskWork;
						break;
					}
				}
			}
			break;
		case TM_START:
			for(;;)
			{
				std::vector<TaskId> art;
				{
					CComCritSecLock<CComAutoCriticalSection> l(ptm->m_cs);
					//ищем своюодную задачу
					for(CTasks::iterator ti=ptm->m_arTask.begin();ti!=ptm->m_arTask.end();++ti)
					{
						if(ti->m_etiState==etiWaitThread)
						{
							if(*pMyTask==NULL)
							{

								*pMyTask=ti->m_taskId;
								art=ti->m_arTasks;
								ti->m_etiState=etiWork;
							}
							else
							{
								ptm->WakeUpThread();
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

					CComCritSecLock<CComAutoCriticalSection> l(ptm->m_cs);
					STask &t=ptm->GetTask(*pMyTask);
					t.m_etiState=etiReadyUnкnown;
					//Добавляем себя в ожидающие задачи
					//идём по всем задачам
					for(CTasks::iterator ti=ptm->m_arTask.begin();ti!=ptm->m_arTask.end();++ti)
					{
						if(t.m_nCountCoin==0)
						{
							t.m_etiState=etiReady;
							break;
						}
						if(ti->m_etiState==etiWaitPrev)
						{
							//бежим по ожидаемым
							int nPos=0;
							for(unsigned i=0;i<ti->m_arDefsTasks.size();++i)
							{
								if(t.m_nCountCoin==0)
									break;

								STasksGrup &tg=ti->m_arDefsTasks[i];
								if(tg.m_bGrup)
								{
									if(tg.m_gidAfter==t.m_Grup)
									{
										for(int j=0;j<tg.m_nCount;++j)
										{
											if(ti->m_arTasks[nPos]==NULL)
											{
												ti->m_arTasks[nPos]=ptMyTask;
												ti->ChekStateWP();
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
										ti->m_arTasks[nPos]=ptMyTask;
										ti->ChekStateWP();
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
						CTasks::iterator t2=ptm->GetTaskIterator(art[i]);
						if(--t2->m_nCountCoinReady==0)
							ptm->m_arTask.erase(t2);
					}

					*pMyTask=NULL;



				}
			}
			break;
		}

	}
	return 0;
}
void CThreadManager::SetThreadPriority(int nPriority)
{
	for(unsigned i=0;i<m_arHandle.size();++i)
		::SetThreadPriority(m_arHandle[i],nPriority);
}



class CWait : public CThreadUnit
{
public:
	CWait()
		: m_hEvent(::CreateEvent(NULL,TRUE,FALSE,NULL))
		{

		}
	~CWait()
	{
		::CloseHandle(m_hEvent);
	}

	HANDLE  m_hEvent;
private:
	 void Start(CThreadUnit** pTasks, int countTasks)
	 {
		::SetEvent(m_hEvent);
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
			if(ti->m_Grup==giW)
				nCount+=ti->m_nCountCoin;

	}
	
	BeginTaskG(&w,0,giW,nCount,0); 



	::WaitForSingleObject(w.m_hEvent,INFINITE);


}



CThreadManager::CTasks::iterator CThreadManager::GetTaskIterator(CThreadManager::TaskId tid)
{
	STask t;
	t.m_taskId=tid;
	return m_arTask.find(t);
}

void CThreadManager::WakeUpThread()
{
	for(unsigned i=0;i<m_arThreads.size();++i)
		if(m_arThreads[i].m_taskWork==0)
		{
			::PostThreadMessage(m_arThreads[i].m_ThreadId,TM_START,0,0);
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
