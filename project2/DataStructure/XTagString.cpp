// XTagString.cpp: implementation of the XTagString class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "EODEXTDEF_GlobalObject.h"
#include "XTagString.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XTagString::XTagString() : m_ltString()
{
}

XTagString::~XTagString(void)
{
	list<TagInfo*>::iterator iter;

	for( iter = m_ltString.begin(); iter != m_ltString.end(); ++iter )
	{
		//(*iter)->strString.clear();
		delete (*iter);
	}
	m_ltString.clear();
}

void XTagString::StringClear()
{
	list<TagInfo*>::iterator iter;

	for( iter = m_ltString.begin(); iter != m_ltString.end(); ++iter )
	{
		//(*iter)->strString.clear();
		delete (*iter);
	}
	m_ltString.clear();
}

void XTagString::Update()
{
	if(m_ltString.empty())
		return;

	list<TagInfo*>::iterator iter = m_ltString.begin();

	//왼쪽위치는 넘어오고... 줄의위치..
	int nCurrentDrawPosY = 0;

	for( ; iter != m_ltString.end(); ++iter )
	{
		if( !((*iter)->strString.empty()) )
		{
            //printf( (*iter)->strString.c_str() );
			//printf( "\n↑스트링,,    볼드 여부 : %d, 컬러값 : %x\n\n", (*iter)->bStrong, (*iter)->dwColor);
		}
        //g_XBaseFont->SetBoldMode((*iter)->bStrong);
		//g_XBaseFont->SetColor((*iter)->dwColor);
		
		//두개변수로받자
		//g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, (*iter)->strString );
		//nCurrentDrawPosY += 16; //다음줄 부터는 아랫줄로 출력해야 함으로....

		//g_XBaseFont->SetColor(_XSC_DEFAULT);
		//g_XBaseFont->SetBoldMode(FALSE);
	}
}

void XTagString::UpdateListBox( _XListBox& listbox, int& linecount)
{
	if(m_ltString.empty())
		return;

	TCHAR buffer[2048];

	list<TagInfo*>::iterator iter = m_ltString.begin();

	for( ; iter != m_ltString.end(); ++iter )
	{
		//리스트 박스에 넣을때..
		_XStringSplitInfo splitinfo; //글씨를 나눠보자
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			g_XBaseFont->SplitString( (*iter)->strString.c_str(), 300, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);
		}
		else
		{
			g_XBaseFont->SplitStringByCR( (*iter)->strString.c_str(), splitinfo); //스트링을 \n으로 나눈다..
		}

		for(int i = 0 ; i < splitinfo.splitcount ; i++) //나눈 만큼..
		{
			memset(buffer, 0, sizeof(buffer));
			strncpy(buffer, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
			listbox.InsertListItem(_T(""), 0, 0);
			listbox.SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 0, 0, 0));

			listbox.SetItemText(linecount, 1, buffer, (*iter)->bStrong); //3번째 인자가 글씨체 진하기 여부
			listbox.SetItemAttrib(linecount, 1, (*iter)->dwColor);

			linecount++;
		}
	}
	//끝..
}

void XTagString::StringCompare(string pStirng, BOOL bBold, DWORD dwColor)
{
	if( pStirng.empty() )
		return; 

	string::size_type colorindex = 0;
	string::size_type boldindex = 0;
	string::size_type Endcolorindex = 0;
	string::size_type Endboldindex = 0;

	string tempStr(pStirng);
	
	boldindex = tempStr.find( "<bold>" );
	colorindex = tempStr.find( "<color=");

	if( (boldindex == string::npos) && (colorindex == string::npos) ) // 태그가 없는 가장 행복한 경우 //재귀 호출시 스트링저장
	{
		TagString(pStirng, bBold, dwColor);
		return;
	}

	if( boldindex != string::npos ) //볼드가 있다.. 
	{
		Endboldindex = tempStr.find("</bold>");
		if( Endboldindex != string::npos ) // 예외 검사
		{
			string boldClearStr(tempStr.begin(), tempStr.begin()+boldindex); 
			StringCompare(boldClearStr, FALSE, 0xFF000000);

			//볼드 스트링
            string saveStr( tempStr.begin()+boldindex + 6, tempStr.begin()+Endboldindex); //볼드 뒤에 부터 태그막는곳 앞까지
			StringCompare(saveStr, TRUE, dwColor);

			//스트링 저장한 부분은 제거한다...
			tempStr.erase( tempStr.begin(), tempStr.begin()+Endboldindex + 7);
			StringCompare(tempStr, FALSE, 0xFF000000);

			return;
		}
	}

	if( colorindex != string::npos ) //컬러가 있음...
	{
		//컬러가 제대로 셋팅되어있는지 검사한다
		string checkStr(tempStr.begin()+colorindex, tempStr.end());
		Endcolorindex = tempStr.find("</color>");
		
		if( checkStr.find(">") == 15 )
		{
			if( Endcolorindex != string::npos ) // 컬러가 있음..
			{
				//컬러값 추출
				string colorStr( checkStr.begin()+7, checkStr.begin()+15);
				SetStringColor( colorStr.c_str(), dwColor );
			}
			string colorClearStr(tempStr.begin(), tempStr.begin()+colorindex); 
			StringCompare(colorClearStr, FALSE, 0xFF000000);

			//컬러 색 들어가는 스트링
			string printStr(tempStr.begin()+colorindex+16, tempStr.begin()+Endcolorindex);
			StringCompare(printStr, bBold, dwColor );

			//스트링 저장한 부분은 제거한다...
			tempStr.erase( tempStr.begin(), tempStr.begin()+Endcolorindex + 8);
			StringCompare(tempStr, FALSE, 0xFF000000);
			return;
		}
	}

	TagString(pStirng, bBold, dwColor);
}

void XTagString::CheckBold(const string::size_type& index, const string pString)
{
}

void XTagString::CheckColor(const string::size_type& index, const string pString)
{
}

void XTagString::SetStringColor( const char* szString, DWORD& dwColor )
{
	dwColor = 0;
	//각자리의 문자를 숫자로 변환
	int nNum[255] = {0,};
	size_t nStrlen = strlen(szString);

	for( unsigned int i = 0; i < nStrlen; i++ )
	{
		if( szString[i] >= '0' && szString[i] <= '9' )
		{
			nNum[i] = szString[i] - '0';
		}
		else if( szString[i] >= 'a' && szString[i] <= 'f' )
		{
			nNum[i] = szString[i] - 'a' + 10;
		}
		else if( szString[i] >= 'A' && szString[i] <= 'F' )
		{
			nNum[i] = szString[i] - 'A' + 10;
		}
		else
		{
			//16진수를 나타내는 문자가 아닐경우
			nNum[i] = 0;
		}
	}

	//각자리의 숫자를 승수를 곱해서.. 10진수로 만든다
	i = 0;
	size_t j = nStrlen;
	for( ; i < nStrlen; i++, j-- )
	{
		//16의 j승 // 숫자는 앞에서 부터 대입 
		int multiplier = 1;
		for( size_t h = j-1; h >= 1; h-- )
			multiplier *= 16;

		dwColor = dwColor + ( nNum[i] * multiplier );
	}
}