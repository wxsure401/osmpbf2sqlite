#pragma once

/*
Здесь собраны функции по работе со строками
*/

class  CMyString
{
public:
	CMyString(void);
	~CMyString(void);

	//Конвертируем строку из wchar_t то char и обратно кодировка на выходе задаётся параметром
	//если size_str ==-1 то считаем размеры сами
	static void W2C(const std::wstring& ws, std::string* str,unsigned uCodePage=CP_ACP);
	static void W2C(const wchar_t* ws, unsigned size_ws,std::string* str,unsigned uCodePage=CP_ACP);
	static void C2W(const std::string& ws, std::wstring* str,unsigned uCodePage=CP_ACP);
	static void C2W(const char* str, unsigned size_str, std::wstring* pws,unsigned uCodePage=CP_ACP);

	// C помощью описанных выше функций из char в UTF-8
	static void C2UTF8(const char* str, unsigned size_str, std::string* psUtf8);

	//Пропустить все прабельные символы
	static const wchar_t* SkeepSpace(const wchar_t*ws);

	//взять булл
	static bool GetBool(const wchar_t*ws);
	static unsigned GetHex(const wchar_t*ws);
	static int GetInt(const wchar_t*ws);

	static const wchar_t * GetFloat(float *pf,const wchar_t *s);

	typedef std::vector<std::wstring > CarWString;
};

#ifdef WIN32
inline	void CMyString::W2C(const std::wstring& ws, std::string* str,unsigned uCodePage/*=CP_ACP*/)
	{ W2C(ws.c_str(),ws.size(),str,uCodePage);}

inline	void CMyString::C2W(const std::string& ws, std::wstring* str,unsigned uCodePage/*=CP_ACP*/)
	{ C2W(ws.c_str(),ws.size(),str,uCodePage);}
#endif

inline	const wchar_t* CMyString::SkeepSpace(const wchar_t*ws)
	{
		if(ws)
		{
		    static const unsigned short a=L' ';
			while(*ws!=0  &&
                ( (unsigned short)(*ws))<=a)
			{
			    	++ws;
			}
		}
		return ws;
	}

//взять булл
inline	bool CMyString::GetBool(const wchar_t*ws)
{
	const wchar_t* w=SkeepSpace(ws);
	return !w || *w==0 ||w[0]==L'1' ||w[0]==L't' ||w[0]==L'T';
}

inline	unsigned CMyString::GetHex(const wchar_t*ws)
{
	 wchar_t *e;
	return wcstol(ws,&e,16);
}

inline	int CMyString::GetInt(const wchar_t*ws)
{
	 wchar_t *e;
	return wcstol(ws,&e,10);
}
