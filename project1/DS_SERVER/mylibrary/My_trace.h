/*=========================================================
Trace Util
class CSystemTrace, CConvertHTMLFormationString

설명
Trace 로그를 지정한 출력장치로 출력한다. 
Trace log 종류를 미리 여러개 설정해서 Trace ID 로 구분하며
동시에 여러 Trace가 가능하다. 실시간으로 출력 여부를 결정해서
불필요할땐 꺼놓고 필요할때만 Trace하도록 설정할 수 있다.
File 로 출력할경우 HTML 포멧으로 출력하여 Trace log 내용에 따라
Color 를 지정하거나 BLINK 옵션을 설정하면 로그 검색이 편리하다.

2004.06.03		by. ozzywow 
==========================================================*/

#pragma	  once


namespace _ns_trace_ 
{
	enum _TraceFormat								// Log string format code
	{
		_FORMAT_TEXT = 0,
		_FORMAT_HTML
	};

	enum _FontColor									// Log string color code
	{
		_FONT_COLOR_RED = 0xff0000,
		_FONT_COLOR_GREEN = 0x00ff00,
		_FONT_COLOR_BLUE = 0x0000ff,
		_FONT_COLOR_BLACK = 0x000000,
	};
} ;

// BLINK option ( for HTML format log )
#define _BLINK_		true		
#define _NBLINK_	false

// Trace log buf size
#define _TRACE_HEAD_BUF_SIZE	128					// log header buf size
#define _TRACE_BADY_BUF_SIZE	1024				// log bady buf size

// Trace data 
struct  _TraceData {
	bool	bActive ;								// 실시간으로 출력할지 여부( true : on, false : off )
	char	cHeadTitle[_TRACE_HEAD_BUF_SIZE] ;		// head ( 헤더에 제목을 설정해서 출력할때 구분이 용이하게 )
	char	cBuf[_TRACE_BADY_BUF_SIZE] ;			// bady ( 트레이스 로그 내용 )
	CRITICAL_SECTION	cs ;						// TRACE ID 별로 동기화 시킨다.
} ;	






// Trace base class
class CSystemTrace
{

private:

	int			m_iTraceSize ;						// 등록된 Trace 갯수
	char		m_cTraceFormat ;					// Trace string format
	_TraceData * m_pTraceData ;			



public:

	CSystemTrace( const int iSize, const char cFormat = _ns_trace_::_FORMAT_TEXT ) 
		: m_iTraceSize(iSize)
		, m_cTraceFormat(cFormat)
	{		
		if( iSize <= 0 )
		{
			throw "" ;
		}

		m_pTraceData = new _TraceData[m_iTraceSize] ;
		_TraceData * pTrace = m_pTraceData ;
		ZeroMemory( m_pTraceData, sizeof(_TraceData) * m_iTraceSize ) ;

		// TraceData Object 초기화
		for( int i = 0  ; i < m_iTraceSize ; i++ )
		{			
#if (_WIN32_WINNT > 0x0403)
			if( FALSE == InitializeCriticalSectionAndSpinCount( &pTrace->cs, (0x80000000)|2000 ) )
			{
				throw "" ;
			}
#else
			InitializeCriticalSection( &pTrace->cs ) ;
#endif
			++pTrace ;
		}		
	}

	virtual ~CSystemTrace()
	{
		_TraceData * pTrace = m_pTraceData ;
		for( int i = 0  ; i < m_iTraceSize ; i++ )
		{
			DeleteCriticalSection( &pTrace->cs ) ;
			++pTrace ;
		}

		delete [] m_pTraceData ;
	}





	// Set header title of trace
	inline void SetHeadTitle(  const int iTraceClassNum, const char * cHeadTitle )
	{
		if( iTraceClassNum > 0 && iTraceClassNum < m_iTraceSize )
		{
			memcpy( (&m_pTraceData[iTraceClassNum])->cHeadTitle, cHeadTitle, strlen(cHeadTitle) ) ;
		}		
	}	

	// Show header title list
	_TraceData * ShowHeadTitleList()
	{
		return m_pTraceData ;
	}

	// Print trace log string
	inline void OutputString( const int iTraceClassNum /*TRACE ID*/ , const char * cLogString,... ) 
	{
		if( iTraceClassNum > 0 && iTraceClassNum < m_iTraceSize )
		{
			_TraceData * pTrace = m_pTraceData + iTraceClassNum ;
			if( pTrace->bActive == true )
			{			
				EnterCriticalSection( &pTrace->cs ) ;
				va_list arglist ;
				va_start(arglist, cLogString) ;
				vsprintf( pTrace->cBuf, cLogString, arglist) ;
				va_end(arglist) ;


				// TODO : Output string function
				OutputDevice( pTrace->cHeadTitle, pTrace->cBuf ) ;			

				LeaveCriticalSection( &pTrace->cs ) ;
			}		
		}		
	}

	// Set trace mode ( 실시간으로 Trace ID 별로 active 상태를 설정해서 해당 trace의 출력 역부를 결정한다 )
	void ShowTraceString( const int iTraceClassNum/*TRACE ID*/ , const bool bMode /*true:show, false:hide*/)
	{
		if( iTraceClassNum > 0 && iTraceClassNum < m_iTraceSize )
		{
			_TraceData * pTrace = m_pTraceData + iTraceClassNum ;
			pTrace->bActive = bMode ;
		}
	}

	// TODO : 상속받는 Class 에서 이 함수를 제정의 해서 출력대상(콘솔창, 파일 등..)을 정해줘라.
	virtual void OutputDevice( const char * title, const char * str ) = 0 ;
	
};



// Trace log 를 파일로 출력할때 HTML 포멧으로 출력하도록 컨버팅 해준다.
class CConvertHTMLFormationString
{
public:
	static char * strBuf ;
	static char * ConvertHTMLFormation( char * str, int color = 0xffffff, bool blink = false )
	{
		if( _BLINK_ == blink )
		{
			sprintf( strBuf, "<BR><U><I><FONT COLOR = %x>%s</FONT></I></U>", color, str ) ;
		}
		else
		{
			sprintf( strBuf, "<BR><FONT COLOR = %x>%s</FONT>", color, str ) ;
		}		
		return strBuf ;
	}
};


// HTML Format 으로 변환
#ifndef _DEFINE_HTML_MACRO_FUNC
#define _DEFINE_HTML_MACRO_FUNC

//#pragma warning (disable:4003)
#ifdef _USE_HTML_FORMAION_
// HTML Format 으로 변환
#define HTML(str, color, blink )			( CConvertHTMLFormationString::ConvertHTMLFormation(str, color, blink ) )
#else
#define HTML(str, color, blink )			( str )
#endif

#endif