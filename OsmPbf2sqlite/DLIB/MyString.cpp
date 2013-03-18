#include "StdAfx.h"
#include "MyString.h"

CMyString::CMyString(void)
{
}

CMyString::~CMyString(void)
{
}
/*static*/void CMyString::W2C(const wchar_t* ws, unsigned size_ws, std::string* str, unsigned uCodePage/*=CP_ACP*/)
{
	if(ws==NULL || str==NULL)
		return;

	if(size_ws==-1)
		size_ws=wcslen(ws);

	LPCSTR pszDefChar="?";
	BOOL bUseDef=true, *pbUseDef=&bUseDef;

	switch(uCodePage)
	{
	case CP_UTF7:
	case CP_UTF8:
		{
			pszDefChar	= NULL;
			pbUseDef	= NULL;
			break;
		}
	}

	int n=WideCharToMultiByte(uCodePage,0,ws,size_ws,NULL,0,pszDefChar,pbUseDef);
	str->resize(n);
	WideCharToMultiByte(uCodePage,0,ws,size_ws,&((*str)[0]),n,pszDefChar,pbUseDef);
}
/*static*/ void CMyString::C2W(const char* str, unsigned size_str, std::wstring* pws, unsigned uCodePage/*=CP_ACP*/)
{
	if(pws==NULL || str==NULL)
		return;
	if(size_str==-1)
		size_str=strlen(str);

	int ns=MultiByteToWideChar(uCodePage,0,str,size_str,NULL,0);
	pws->resize(ns);
	MultiByteToWideChar(uCodePage,0,str,size_str,&((*pws)[0]),ns);

}

/* static */ void CMyString::C2UTF8(const char* str, unsigned size_str, std::string* psUtf8)
{
	std::wstring ws;
	CMyString::C2W(str, size_str, &ws);	
	CMyString::W2C(ws, psUtf8, CP_UTF8);	
}

//получим флоат из цифры
float  wcstod2(const wchar_t * s,  const wchar_t ** _EndPtr)
{

	double d=0;
	while(*s>=L'0' && *s<=L'9')
	{
		d=d*10+(*s-L'0');
		++s;

	}
	if(*s!=L'.')
	{
		* _EndPtr=s;
		return float(d);
	}
	++s;
	double d2=.1;
	while(*s>=L'0' && *s<=L'9')
	{
		d+=(*s-L'0')*d2;
		d2/=10;
		++s;

	}
	* _EndPtr=s;
	return float(d);


}


/*static*/const wchar_t * CMyString::GetFloat(float *pf,const wchar_t *s)
{
	if(!s)
		return NULL;
	bool bSign=false;
	for(; *s!=0; ++s)
	{
		if((*s>=L'0' && *s<=L'9')||*s==L'.')
		{
			const wchar_t* wc;
			*pf=wcstod2(s,&wc);
			if(bSign)
				*pf=-*pf;

			return wc;

		}
		else if(*s==L'-')
		{
			bSign=true;
		}
	}
	return NULL;

}