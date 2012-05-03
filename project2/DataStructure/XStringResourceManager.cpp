// XStringResourceManager.cpp: implementation of the _XStringResourceManager class.
//
// Author : Sohyun, Park
// Created Date : 2004.01.05
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "emperorofdragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XStringResourceManager.h"

#ifdef _XDEF_NEWSTRINGMANAGER
#include "XStringDataBase.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XStringResourceManager::_XStringResourceManager()
{

}

_XStringResourceManager::~_XStringResourceManager()
{
//	Release();
}

void _XStringResourceManager::Release(void)
{
	map <int, LPTSTR>::iterator iter_string;
	for(iter_string = m_mapStringList.begin() ; iter_string != m_mapStringList.end() ; ++iter_string)
	{
		LPTSTR	currentstring = iter_string->second;
		if(currentstring)
		{
			delete[] currentstring;
			currentstring = NULL;
		}
	}
	m_mapStringList.clear();
}

#ifdef _XTS_NEWSTRING

BOOL _XStringResourceManager::LoadStringResourceFromFile(void)
{
	FILE* fileptr = NULL;
	
	_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);
	
#ifdef _XTESTSERVER	
	switch(g_LanguageType)
	{
	case _XLANGUAGE_TYPE_KOREAN :
		{
			fileptr = fopen("tsEODString.xsr", "rb");
		}
		break;
	case _XLANGUAGE_TYPE_CHINESE :
		{
			fileptr = fopen("tsEODString_cn.xsr", "rb");
		}
		break;
	case _XLANGUAGE_TYPE_ENGLISH :
		{
			fileptr = fopen("tsEODString_en.xsr", "rb");
		}
		break;
	case _XLANGUAGE_TYPE_VIETNAMESE :
		{
			fileptr = fopen("tsEODString_vn.xsr", "rb");
		}
		break;
	case _XLANGUAGE_TYPE_JAPANESE :
		{
			fileptr = fopen("tsEODString_jp.xsr", "rb");
		}
		break;
	}

	if(!fileptr)
	{
		_XFatalError("Can't load tsEODString.xsr");
		return FALSE;
	}	
#else
	switch(g_LanguageType)
	{
	case _XLANGUAGE_TYPE_KOREAN :
		{
			fileptr = fopen("EODString.xsr", "rb");
		}
		break;
	case _XLANGUAGE_TYPE_CHINESE :
		{
			fileptr = fopen("EODString_cn.xsr", "rb");
		}
		break;
	case _XLANGUAGE_TYPE_ENGLISH :
		{
			fileptr = fopen("EODString_en.xsr", "rb");
		}
		break;
	case _XLANGUAGE_TYPE_VIETNAMESE :
		{
			fileptr = fopen("EODString_vn.xsr", "rb");
		}
		break;
	case _XLANGUAGE_TYPE_JAPANESE :
		{
			fileptr = fopen("EODString_jp.xsr", "rb");
		}
		break;
	}

	if(!fileptr)
	{
		_XFatalError("Can't load EODString.xsr");
		return FALSE;
	}
#endif

	int version = 0, categorycount = 0, categoryid = 0, strlength = 0;
	TCHAR categorystring[1024];

	if(fread(&version, sizeof(int), 1, fileptr) < 1)
	{
		fclose(fileptr);
		return FALSE;
	}

	if(fread(&categorycount, sizeof(int), 1, fileptr) < 1)
	{
		fclose(fileptr);
		return FALSE;
	}

	for(int i = 0 ; i < categorycount ; ++i)
	{
		categoryid = 0;
		if(fread(&categoryid, sizeof(int), 1, fileptr) < 1)
		{
			fclose(fileptr);
			return FALSE;
		}

		strlength = 0;
		if(fread(&strlength, sizeof(int), 1, fileptr) < 1)
		{
			fclose(fileptr);
			return FALSE;
		}

		if(strlength > 0)
		{
			memset(categorystring, 0, sizeof(_TCHAR)*1024);
			if(fread(categorystring, strlength, 1, fileptr) < 1)
			{
				fclose(fileptr);
				return FALSE;
			}
		}
	}

	int resourcecount = 0;
	if(fread(&resourcecount, sizeof(int), 1, fileptr) < 1)
	{
		fclose(fileptr);
		return FALSE;
	}

	int category = 0, resourceid = 0;
	TCHAR idstring[1024];

	for(i = 0 ; i < resourcecount ; ++i)
	{
		category = 0;
		if(fread(&category, sizeof(int), 1, fileptr) < 1)
		{
			fclose(fileptr);
			return FALSE;
		}

		resourceid = 0;
		if(fread(&resourceid, sizeof(int), 1, fileptr) < 1)
		{
			fclose(fileptr);
			return FALSE;
		}

		strlength = 0;
		if(fread(&strlength, sizeof(int), 1, fileptr) < 1)
		{
			fclose(fileptr);
			return FALSE;
		}

		if(strlength > 0)
		{
			memset(idstring, 0, sizeof(_TCHAR)*1024);
			if(fread(idstring, strlength, 1, fileptr) < 1)
			{
				fclose(fileptr);
				return FALSE;
			}
		}

		// kroean
		strlength = 0;
		if(fread(&strlength, sizeof(int), 1, fileptr) < 1)
		{
			fclose(fileptr);
			return FALSE;
		}

		TCHAR*	szKorean = NULL;
		if(strlength > 0)
		{
			szKorean = new TCHAR[strlength];
			memset(szKorean, 0, sizeof(TCHAR)*strlength);
			if(fread(szKorean, strlength, 1, fileptr) < 1)
			{
				SAFE_DELETE_ARRAY(szKorean);
				fclose(fileptr);
				return FALSE;
			}
		}

		// chinese
		strlength = 0;
		if(fread(&strlength, sizeof(int), 1, fileptr) < 1)
		{
			fclose(fileptr);
			return FALSE;
		}
		
		TCHAR*	szChinese = NULL;
		if(strlength > 0)
		{
			szChinese = new TCHAR[strlength];
			memset(szChinese, 0, sizeof(TCHAR)*strlength);
			if(fread(szChinese, strlength, 1, fileptr) < 1)
			{
				SAFE_DELETE_ARRAY(szKorean);
				SAFE_DELETE_ARRAY(szChinese);
				fclose(fileptr);
				return FALSE;
			}
		}

		// english
		strlength = 0;
		if(fread(&strlength, sizeof(int), 1, fileptr) < 1)
		{
			fclose(fileptr);
			return FALSE;
		}
		
		TCHAR*	szEnglish = NULL;
		if(strlength > 0)
		{
			szEnglish = new TCHAR[strlength];
			memset(szEnglish, 0, sizeof(TCHAR)*strlength);
			if(fread(szEnglish, strlength, 1, fileptr) < 1)
			{
				SAFE_DELETE_ARRAY(szKorean);
				SAFE_DELETE_ARRAY(szChinese);
				SAFE_DELETE_ARRAY(szEnglish);
				fclose(fileptr);
				return FALSE;
			}
		}

		// vietnamese
		strlength = 0;
		if(fread(&strlength, sizeof(int), 1, fileptr) < 1)
		{
			fclose(fileptr);
			return FALSE;
		}
		
		TCHAR*	szVietnamese = NULL;
		if(strlength > 0)
		{
			szVietnamese = new TCHAR[strlength];
			memset(szVietnamese, 0, sizeof(TCHAR)*strlength);
			if(fread(szVietnamese, strlength, 1, fileptr) < 1)
			{
				SAFE_DELETE_ARRAY(szKorean);
				SAFE_DELETE_ARRAY(szChinese);
				SAFE_DELETE_ARRAY(szEnglish);
				SAFE_DELETE_ARRAY(szVietnamese);
				fclose(fileptr);
				return FALSE;
			}
		}

		int buildversion = 0;
		if(fread(&buildversion, sizeof(int), 1, fileptr) < 1)
		{
			SAFE_DELETE_ARRAY(szKorean);
			SAFE_DELETE_ARRAY(szChinese);
			SAFE_DELETE_ARRAY(szEnglish);
			SAFE_DELETE_ARRAY(szVietnamese);
			fclose(fileptr);
			return FALSE;
		}

		switch(g_LanguageType)
		{
		case _XLANGUAGE_TYPE_KOREAN :
			{
				// "\n"을 New line으로 변경
				strlength = strlen(szKorean);
				
				BOOL bNewLine = FALSE;
				int sublength = 0;

				for(int i = 1 ; i < strlength ; ++i)
				{
					if(szKorean[i] == 'n')
					{
						if(szKorean[i-1] == '\\')
						{
							szKorean[i-1] = '\n';
							sublength ++;
							bNewLine = TRUE;

							for(int j = i ; j < strlength-1 ; ++j)
							{
								szKorean[j] = szKorean[j+1];
							}
						}
					}
					if(szKorean[i] == '"')
					{
						if(szKorean[i-1] == '\\')
						{
							szKorean[i-1] = '\"';
							sublength++;
							bNewLine = TRUE;

							for(int j = i ; j < strlength-1 ; ++j)
							{
								szKorean[j] = szKorean[j+1];
							}
						}
					}
				}

				if(bNewLine)
				{
					int newlength = strlength - sublength;
					for(int j = newlength ; j < strlength ; ++j)
					{
						szKorean[j] = 0;
					}
				}

				m_mapStringList[resourceid] = szKorean;
				SAFE_DELETE_ARRAY(szChinese);
				SAFE_DELETE_ARRAY(szEnglish);
				SAFE_DELETE_ARRAY(szVietnamese);
			}
			break;
		case _XLANGUAGE_TYPE_CHINESE :
			{
				// "\n"을 New line으로 변경
				strlength = strlen(szChinese);
				
				BOOL bNewLine = FALSE;
				int sublength = 0;
				
				for(int i = 1 ; i < strlength ; ++i)
				{
					if(szChinese[i] == 'n')
					{
						if(szChinese[i-1] == '\\')
						{
							szChinese[i-1] = '\n';
							sublength ++;
							bNewLine = TRUE;
							
							for(int j = i ; j < strlength-1 ; ++j)
							{
								szChinese[j] = szChinese[j+1];
							}
						}
					}
				}
				
				if(bNewLine)
				{
					int newlength = strlength - sublength;
					for(int j = newlength ; j < strlength ; ++j)
					{
						szChinese[j] = 0;
					}
				}

				m_mapStringList[resourceid] = szChinese;
				SAFE_DELETE_ARRAY(szKorean);
				SAFE_DELETE_ARRAY(szEnglish);
				SAFE_DELETE_ARRAY(szVietnamese);
			}
			break;
		case _XLANGUAGE_TYPE_JAPANESE :
			{
				// "\n"을 New line으로 변경
				strlength = strlen(szChinese);
				
				BOOL bNewLine = FALSE;
				int sublength = 0;
				
				for(int i = 1 ; i < strlength ; ++i)
				{
					if(szChinese[i] == 'n')
					{
						if(szChinese[i-1] == '\\')
						{
							szChinese[i-1] = '\n';
							sublength ++;
							bNewLine = TRUE;
							
							for(int j = i ; j < strlength-1 ; ++j)
							{
								szChinese[j] = szChinese[j+1];
							}
						}
					}
				}
				
				if(bNewLine)
				{
					int newlength = strlength - sublength;
					for(int j = newlength ; j < strlength ; ++j)
					{
						szChinese[j] = 0;
					}
				}

				m_mapStringList[resourceid] = szChinese;
				SAFE_DELETE_ARRAY(szKorean);
				SAFE_DELETE_ARRAY(szEnglish);
				SAFE_DELETE_ARRAY(szVietnamese);
			}
			break;
		case _XLANGUAGE_TYPE_ENGLISH :
			{
				// "\n"을 New line으로 변경
				strlength = strlen(szEnglish);
				
				BOOL bNewLine = FALSE;
				int sublength = 0;
				
				for(int i = 1 ; i < strlength ; ++i)
				{
					if(szEnglish[i] == 'n')
					{
						if(szEnglish[i-1] == '\\')
						{
							szEnglish[i-1] = '\n';
							sublength ++;
							bNewLine = TRUE;
							
							for(int j = i ; j < strlength-1 ; ++j)
							{
								szEnglish[j] = szEnglish[j+1];
							}
						}
					}
				}
				
				if(bNewLine)
				{
					int newlength = strlength - sublength;
					for(int j = newlength ; j < strlength ; ++j)
					{
						szEnglish[j] = 0;
					}
				}

				m_mapStringList[resourceid] = szEnglish;
				SAFE_DELETE_ARRAY(szKorean);
				SAFE_DELETE_ARRAY(szChinese);
				SAFE_DELETE_ARRAY(szVietnamese);
			}
			break;
		case _XLANGUAGE_TYPE_RUSSIAN :
			{
				// "\n"을 New line으로 변경
				strlength = strlen(szEnglish);
				
				BOOL bNewLine = FALSE;
				int sublength = 0;
				
				for(int i = 1 ; i < strlength ; ++i)
				{
					if(szEnglish[i] == 'n')
					{
						if(szEnglish[i-1] == '\\')
						{
							szEnglish[i-1] = '\n';
							sublength ++;
							bNewLine = TRUE;
							
							for(int j = i ; j < strlength-1 ; ++j)
							{
								szEnglish[j] = szEnglish[j+1];
							}
						}
					}
				}
				
				if(bNewLine)
				{
					int newlength = strlength - sublength;
					for(int j = newlength ; j < strlength ; ++j)
					{
						szEnglish[j] = 0;
					}
				}

				m_mapStringList[resourceid] = szEnglish;
				SAFE_DELETE_ARRAY(szKorean);
				SAFE_DELETE_ARRAY(szChinese);
				SAFE_DELETE_ARRAY(szVietnamese);
			}
			break;
		case _XLANGUAGE_TYPE_VIETNAMESE :
			{
				// "\n"을 New line으로 변경
				strlength = strlen(szVietnamese);
				
				BOOL bNewLine = FALSE;
				int sublength = 0;
				
				for(int i = 1 ; i < strlength ; ++i)
				{
					if(szVietnamese[i] == 'n')
					{
						if(szVietnamese[i-1] == '\\')
						{
							szVietnamese[i-1] = '\n';
							sublength ++;
							bNewLine = TRUE;
							
							for(int j = i ; j < strlength-1 ; ++j)
							{
								szVietnamese[j] = szVietnamese[j+1];
							}
						}
					}
				}
				
				if(bNewLine)
				{
					int newlength = strlength - sublength;
					for(int j = newlength ; j < strlength ; ++j)
					{
						szVietnamese[j] = 0;
					}
				}

				m_mapStringList[resourceid] = szVietnamese;
				SAFE_DELETE_ARRAY(szKorean);
				SAFE_DELETE_ARRAY(szChinese);
				SAFE_DELETE_ARRAY(szEnglish);
			}
			break;
		default :
			{
				SAFE_DELETE_ARRAY(szKorean);
				SAFE_DELETE_ARRAY(szChinese);
				SAFE_DELETE_ARRAY(szEnglish);
				SAFE_DELETE_ARRAY(szVietnamese);
			}
			break;
		}
	}

	fclose(fileptr);
	return TRUE;
}

#else

BOOL _XStringResourceManager::LoadStringResourceFromFile(void)
{
	FILE* fileptr = NULL;

	_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);

#ifdef _XTESTSERVER		
	fileptr = fopen("tsEODString.xsr", "rb");
	if(!fileptr)
	{
		_XFatalError("Can't load tsEODString.xsr");
		return FALSE;
	}	
#else
	fileptr = fopen("EODString.xsr", "rb");
	if(!fileptr)
	{
		_XFatalError("Can't load EODString.xsr");
		return FALSE;
	}
#endif
	
	int categorycount = 0;
	if(fread(&categorycount, sizeof(int), 1, fileptr) < 1)
	{
		fclose(fileptr);
		return FALSE;
	}

	int		categoryid = 0;
	int		strlength = 0;
	_TCHAR	categorystring[512];
	for(int i = 0 ; i < categorycount ; ++i)
	{
		categoryid = 0;
		if(fread(&categoryid, sizeof(int), 1, fileptr) < 1)
		{
			fclose(fileptr);
			return FALSE;
		}

		strlength = 0;
		if(fread(&strlength, sizeof(int), 1, fileptr) < 1)
		{
			fclose(fileptr);
			return FALSE;
		}

		if(strlength > 0)
		{
			memset(categorystring, 0, sizeof(_TCHAR)*512);
			if(fread(categorystring, strlength, 1, fileptr) < 1)
			{
				fclose(fileptr);
				return FALSE;
			}
		}
	}

	int resourcecount = 0;
	if(fread(&resourcecount, sizeof(int), 1, fileptr) < 1)
	{
		fclose(fileptr);
		return FALSE;
	}

	int		category = 0;
	int		resourceid = 0;
	int		idstrlength = 0;
	int		koreanstrlength = 0;
	int		chinesestrlength = 0;
	int		englishstrlength = 0;
	_TCHAR	idstring[512];
	for(i = 0 ; i < resourcecount ; ++i)
	{
		category = 0;
		if(fread(&category, sizeof(int), 1, fileptr) < 1)
		{
			fclose(fileptr);
			return FALSE;
		}

		resourceid = 0;
		if(fread(&resourceid, sizeof(int), 1, fileptr) < 1)
		{
			fclose(fileptr);
			return FALSE;
		}

		idstrlength = 0;
		if(fread(&idstrlength, sizeof(int), 1, fileptr) < 1)
		{
			fclose(fileptr);
			return FALSE;
		}
		if(idstrlength > 0)
		{
			memset(idstring, 0, sizeof(_TCHAR)*512);
			if(fread(idstring, idstrlength, 1, fileptr) < 1)
			{
				fclose(fileptr);
				return FALSE;
			}
		}

		koreanstrlength = 0;
		if(fread(&koreanstrlength, sizeof(int), 1, fileptr) < 1)
		{
			fclose(fileptr);
			return FALSE;
		}

		wchar_t	*widestring = NULL;
		LPTSTR	koreanstring = NULL;
		if(koreanstrlength > 0)
		{
			widestring = new wchar_t[koreanstrlength];
			memset(widestring, 0, sizeof(wchar_t)*koreanstrlength);
			if(fread(widestring, koreanstrlength, 1, fileptr) < 1)
			{
				if(widestring)
				{
					delete[] widestring;
					widestring = NULL;
				}
				fclose(fileptr);
				return FALSE;
			}
			
			int reqlength = WideCharToMultiByte(949/*임시로 강제 한글 변환 CP_ACP*/, 0, widestring, -1, NULL, 0, NULL, NULL);
			koreanstring = new _TCHAR[reqlength];
			memset(koreanstring, 0, sizeof(_TCHAR)*reqlength);
			if(!WideCharToMultiByte(949/*임시로 강제 한글 변환 CP_ACP*/, 0, widestring, -1, koreanstring, reqlength, NULL, NULL))
			{
				if(widestring)
				{
					delete[] widestring;
					widestring = NULL;
				}
				if(koreanstring)
				{
					delete[] koreanstring;
					koreanstring = NULL;
				}
				fclose(fileptr);
				return FALSE;
			}
			if(widestring)
			{
				delete[] widestring;
				widestring = NULL;
			}
		}

		chinesestrlength = 0;
		if(fread(&chinesestrlength, sizeof(int), 1, fileptr) < 1)
		{
			if(koreanstring)
			{
				delete[] koreanstring;
				koreanstring = NULL;
			}
			fclose(fileptr);
			return FALSE;
		}

		if(widestring)
		{
			delete[] widestring;
			widestring = NULL;
		}
		LPTSTR	chinesestring = NULL;
		if(chinesestrlength > 0)
		{
			widestring = new wchar_t[chinesestrlength];
			memset(widestring, 0, sizeof(wchar_t)*chinesestrlength);
			if(fread(widestring, chinesestrlength, 1, fileptr) < 1)
			{
				if(koreanstring)
				{
					delete[] koreanstring;
					koreanstring = NULL;
				}
				if(widestring)
				{
					delete[] widestring;
					widestring = NULL;
				}
				fclose(fileptr);
				return FALSE;
			}
			
			int reqlength = WideCharToMultiByte(949/*임시로 강제 한글 변환 CP_ACP*/, 0, widestring, -1, NULL, 0, NULL, NULL);
			chinesestring = new _TCHAR[reqlength];
			memset(chinesestring, 0, sizeof(_TCHAR)*reqlength);
			if(!WideCharToMultiByte(949/*임시로 강제 한글 변환 CP_ACP*/, 0, widestring, -1, chinesestring, reqlength, NULL, NULL))
			{
				if(koreanstring)
				{
					delete[] koreanstring;
					koreanstring = NULL;
				}
				if(widestring)
				{
					delete[] widestring;
					widestring = NULL;
				}
				if(chinesestring)
				{
					delete[] chinesestring;
					chinesestring = NULL;
				}
				fclose(fileptr);
				return FALSE;
			}
			
			if(widestring)
			{
				delete[] widestring;
				widestring = NULL;
			}
		}

		englishstrlength = 0;
		if(fread(&englishstrlength, sizeof(int), 1, fileptr) < 1)
		{
			if(koreanstring)
			{
				delete[] koreanstring;
				koreanstring = NULL;
			}
			if(chinesestring)
			{
				delete[] chinesestring;
				chinesestring = NULL;
			}
			fclose(fileptr);
			return FALSE;
		}
		
		LPTSTR	englishstring = NULL;
		if(englishstrlength > 0)
		{
			englishstring = new _TCHAR[englishstrlength];
			memset(englishstring, 0, sizeof(_TCHAR)*englishstrlength);
			if(fread(englishstring, englishstrlength, 1, fileptr) < 1)
			{
				if(koreanstring)
				{
					delete[] koreanstring;
					koreanstring = NULL;
				}
				if(chinesestring)
				{
					delete[] chinesestring;
					chinesestring = NULL;
				}
				if(englishstring)
				{
					delete[] englishstring;
					englishstring = NULL;
				}
				fclose(fileptr);
				return FALSE;
			}
		}

		switch(g_LanguageType)
		{
		// 2005.07.26 무조건 한글로 보이도록 - 임시
		case _XLANGUAGE_TYPE_CHINESE :
		case _XLANGUAGE_TYPE_KOREAN :
			{
				m_mapStringList[resourceid] = koreanstring;
				if(chinesestring)
				{
					delete[] chinesestring;
					chinesestring = NULL;
				}
				if(englishstring)
				{
					delete[] englishstring;
					englishstring = NULL;
				}
			}
			break;
//		case _XLANGUAGE_TYPE_CHINA :
//			{
//				m_mapStringList[resourceid] = chinesestring;
//				if(koreanstring)
//				{
//					delete[] koreanstring;
//					koreanstring = NULL;
//				}
//				if(englishstring)
//				{
//					delete[] englishstring;
//					englishstring = NULL;
//				}
//			}
//			break;
		default :
			{
				if(koreanstring)
				{
					delete[] koreanstring;
					koreanstring = NULL;
				}
				if(chinesestring)
				{
					delete[] chinesestring;
					chinesestring = NULL;
				}
				if(englishstring)
				{
					delete[] englishstring;
					englishstring = NULL;
				}
			}
			break;
		}

	}

	fclose(fileptr);
	return TRUE;
}
#endif

LPTSTR _XStringResourceManager::GetString(int id)
{

#ifdef _XDEF_NEWSTRINGMANAGER

	return g_StringDataBase.GetTextString( _XSTRINGDB_SN_EODSTRING_XSR, id );

#else	

	map <int, LPTSTR>::iterator iter_string = m_mapStringList.find(id);
	if(iter_string != m_mapStringList.end())
	{
		LPTSTR currentstring = iter_string->second;
		if(currentstring)
		{
			return currentstring;
		}
	}

	_XLog("[string] === Not Found %d ===]", id);
	return _T("None");

#endif	

}