#pragma once

class CThreadUnit;


//Класс выстраивает цепочку исполняемых потоков
class CThreadManager
{
public:
	//Создать пул рабочих потоков
	//nThread количество рабочих потоков 0 - создаем столько сколько ядер
	CThreadManager(unsigned nThread=0);

	~CThreadManager(void);

	/*Планирование запуска потокоа

	pThread - Объект непосредственно занимающийся вычислением чего нибудь
	nGrupId - идентификатор экземпляра uheggs задания. Не обязательно уникалный
	nGrupIdStartAfter - Идентификатор группы заданий после выполнения которых запустить этот поток. 0 Запустить сразу
	nGrupIdWaitKey - Способ возврата в текущий поток. 0 вернуться сразу. Иначе ждем выпонения этой группы

	возвращаем уникальный идентификатор задания

	*/

	typedef unsigned GrupId;  //идентификатор группы заданий
	typedef const CThreadUnit* TaskId;  //уникальный идентификатор задания


	//Установка задачи pThread в очередь задач
	//Задача имеет идентификатор группы gidMy и должна запускаться когда
	// завершаться nCount задач из группы  gidAfter
	TaskId BeginTaskG(CThreadUnit* pThread,GrupId gidMy,GrupId gidAfter,int nCount,int nCountCoin=1);

	//Установка задачи pThread в очередь задач
	//Задача имеет идентификатор группы gidMy и должна запускаться после задачи pThreadAfter
	TaskId BeginTaskT(CThreadUnit* pThread,GrupId gidMy,CThreadUnit* pThreadAfter,int nCountCoin=1);

	struct STasksGrup
	{
		bool m_bGrup;
		union
		{
			CThreadUnit* m_pThreadAfter;
			struct
			{
				GrupId m_gidAfter;
				int m_nCount;
			};
		};
	};


	//Установка задачи pThread в очередь задач
	//Задача имеет идентификатор группы gidMy и должна запускаться когда
	// завершаться все задачи описанные в массиве pTasks. Массив содержит nCountTassks групп
	TaskId BeginTaskComplex(CThreadUnit* pThread,GrupId gidMy,const STasksGrup* pTasks,int nCountTassks,int nCountCoin=1);



	//Состояния заддачи
	enum ETaskInfo
	{
		etiUnknown,  //неизвестно
		etiWaitPrev,     //ожидает выполнения предыдущих задач
		etiWaitThread,    //ожидает свободного потока
		etiWork,     //выполняется
		etiReadyUnknown,  //Задача выполненна и её никто не ждёт
		etiReady,  //Задача выполненна и установлена в очередь для последующей отработки
	};

	//Узнать что происходит с задачей
	ETaskInfo GetInfo(TaskId ti)const;

	//Удалить задачу даже если она выполняется
	void KillTask(TaskId ti);


	//Ожидание завершения выполнения задач из группы giW если 0 то ждем всех
	void Wait(GrupId giW,int nCount=0);



	//void SetThreadPriority(int nPriority);

	static unsigned GetCountKernels(); // сколько в системя ядер

	size_t GetCountThread()const
		{return m_arHandle.size();} // сколько потоков обрабатывают задачи в рамках этого объекьа

private:
	mutable CComAutoCriticalSection m_cs;

	struct STask
	{
		TaskId m_taskId;  //сама задача
		GrupId m_Grup;  //Группа
		int m_nCountCoin;   //Скольким задачам потребуется выход этой задачи
		int m_nCountCoinReady;   //Скольким задач ечо не отработали
		ETaskInfo m_etiState;//состояние

		std::vector<STasksGrup> m_arDefsTasks; //Массив зависимостей
		std::vector<TaskId> m_arTasks; //Массив готовых задач нужных для начала выполнения этой задачи
		void ChekStateWP();

	};

	typedef std::map<TaskId,STask> CTasks;
	CTasks m_arTask;

	STask &GetTask(TaskId tid);
	CTasks::iterator GetTaskIterator(TaskId tid);



	struct SThread
	{
		SThread():m_taskWork(0){}
		boost::thread::id m_ThreadId;
		TaskId m_taskWork;
		CThreadManager *m_ptm;
		class CMessageQueue
		{
        public:
            CMessageQueue():m_nWrite(0),m_nRead(0),m_arMessage(8){}

            bool PostThreadMessage(unsigned message);
            //Взять из очерели  Ели WM_QUIT то возвращает фальш
            bool GetMessage(unsigned *pMessage);

            private:
            int m_nWrite; //Позицция записи
            int m_nRead;//Позицция чтения
            std::vector<unsigned> m_arMessage;
            boost::condition_variable m_cond; //событие
            boost::mutex m_mut; //ожидание

		};
		 CMessageQueue m_MessageQueue;
		 void operator()();

		//Добавим задачу на выполнение после  задачи с идом GrupId. Если Групп ид ==0 то
		//добавляем полюбому
		// возвращает истину если добавила
		bool AddTaskAfter(GrupId ngId, TaskId tId);
	};


	std::vector<SThread*> m_arThreads;
//	std::vector<HANDLE> m_arHandle;
	std::vector<boost::thread *> m_arHandle;

	static const UINT TM_START=WM_USER+1;
	static const UINT TM_INIT= WM_USER+2;
	static DWORD WINAPI Thread(  LPVOID lpThreadParameter  );
	friend class CWait;


	void WakeUpThread();


};



//Базовй класс для объектов задач
class CThreadUnit
{
public:
	CThreadUnit():m_GrupId(0),m_pThreadManager(NULL){}
	virtual ~CThreadUnit(){};

	virtual void Start(CThreadUnit** pTasks, int countTasks)=0;


	CThreadManager::TaskId GetTaskId()const { return this;}
	CThreadManager::GrupId GetGrupId()const { return m_GrupId;}
	CThreadManager* GetThreadManager()const { return m_pThreadManager;}

private:
	CThreadManager::GrupId m_GrupId;
	CThreadManager* m_pThreadManager;
	friend class CThreadManager;


};

/*
inline bool CThreadManager::STask::operator<(const CThreadManager::STask::STask& t)const
{	return m_taskId<t.m_taskId;
}
*/
inline CThreadManager::STask &CThreadManager::GetTask(CThreadManager::TaskId tid)
{

    CTasks::iterator ti=GetTaskIterator(tid);
	return ti->second;
}
