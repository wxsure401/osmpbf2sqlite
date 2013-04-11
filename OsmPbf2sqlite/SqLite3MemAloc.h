#pragma once

class CSqLite3MemAloc
{
public:
	CSqLite3MemAloc(void);
	~CSqLite3MemAloc(void);

	static sqlite3_mem_methods m_OldMem;
	static sqlite3_mem_methods m_Mem;

	void Init();
	void Close();

	static  void* Malloc(int n);         /* Memory allocation function */
	static  void Free(void* p);          /* Free a prior allocation */
    static  void* Realloc(void* p,int n);  /* Resize an allocation */
	static  int Size(void* p);           /* Return the size of an allocation */
	static  int Roundup(int n);          /* Round up request size to allocation size */
	static int Init(void* p);           /* Initialize the memory allocator */
	static void Shutdown(void*p);      /* Deinitialize the memory allocator */



	//кусочек
	struct SShunk
	{
		//union
		//{
			size_t nSize; //Размер куска
			size_t nPosNext; //Позиция Следующего не занятого
		//};
	};

	//Кусочки одинакового размера
	struct SHip
	{
		SHip(int SizeShunk,int nVer=1);
		~SHip();
		int m_nVer;  //нужно для очередного выделения памяти
		size_t  m_SizeShunk; //Размер кусочка
		size_t  m_CountShunk; //Количество кусочков в к этом хипе
		size_t m_nPosNext; //Позиция первого не занятого
		SHip * m_pNext; //Позиция следующего в этой куче
		__int8*  m_pBuf; //непосредственно буфер памяти

		void* Malloc(int n);
		void Free(void* p);
	private:
		void swap();
	};

	struct SHips
	{
		SHips();
		~SHips();

		SHip* m_hip[13]; // массив выделенных блоков памяти m_hip[0] -> это по 64 байта,m_hip[1]- 128 байт и тд

		void* Malloc(int n);
		void Free(void* p);
		void* Realloc(void* p,int n);
		size_t Size(void* p);

		SHip* GetHip(int n);
	};


    static __TLS  SHips* m_pH;
};
