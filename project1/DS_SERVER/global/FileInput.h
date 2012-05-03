#ifndef _FILEINPUT_HEADER_2006_01_13__
#define _FILEINPUT_HEADER_2006_01_13__

#include <string.h>

#include <string>
#include <iostream>
#include <fstream>
using namespace std ;

enum {
	en_cmd_not_file_open=0,		// 
	en_cmd_end,					// file end
	en_cmd_comment,				// comment or zero.
	en_cmd_over_length,			// length of command is over array size
	en_cmd_open_bracket,		// '{'
	en_cmd_close_bracket,		// '}'
	en_cmd_string,				// found string
} ;

class NDStreamFileInput
{
public:
	// /*
	NDStreamFileInput()
	{
		m_iCurLine = 0 ;
		m_countCommentChar = 0 ;
		::ZeroMemory(m_carrCommentChar, sizeof(m_carrCommentChar) ) ;

		m_pPosInLine = NULL ;

		file_add_comment_char( '/' ) ;
		file_add_comment_char( ';' ) ;
	}
	// */	// 기본 생성자를 없앤다. 즉, 반드시, 파일 이름을 입력으로 쓰게 만든다.
	NDStreamFileInput( const char * fname ) : m_ifsStream( fname ), m_strFileName( fname )
	{
		m_iCurLine = 0 ;
		m_countCommentChar = 0 ;
		::ZeroMemory(m_carrCommentChar, sizeof(m_carrCommentChar) ) ;

		m_pPosInLine = NULL ;

		file_add_comment_char( '/' ) ;
		file_add_comment_char( ';' ) ;
	}
	virtual ~NDStreamFileInput()
	{
		file_close() ;
	}

	// wrap methods.
	inline void file_open( const char * fname )		
	{
		m_strFileName.assign( fname ) ;
		m_ifsStream.open( fname, ios_base::in ) ; 
	}
	inline bool file_is_open()	{ return m_ifsStream.is_open() ; }

	inline void file_clear()	{ m_ifsStream.clear() ; } 
	
	inline void file_close()	{ if( file_is_open() ) m_ifsStream.close() ; }

	// add charactor that using as comment
	void file_add_comment_char( char ch )
	{
		if( m_countCommentChar < en_cmt_max_count )
		{
			m_carrCommentChar[m_countCommentChar] = ch ;
			++m_countCommentChar ;
		}
	}

	// show current comment charactor
	void file_show_cur_comment()
	{
		cout << m_carrCommentChar ;
	}

	// 명령어 얻기 : 워드 단위로 얻게 된다.
	int file_get_word( char * word, const int len ) ;	// return en_cmd. : word 
	int file_get_int() ;
	unsigned int file_get_uint() ;
	float file_get_float() ;
	int file_get_string( char * line, const int len ) ;	// 리턴값은 문자열 길이.

	// 특정 명령어의 경우, 뒤에 특정 개수의 인자를 취하게 되는데, 
	// 스트림의 경우, 인자 개수를 검사하기 힘들어서, 
	// 함수로 만들어 놨음. 명령어 뒤로, 라인이 끝나기 전까지의 인자의 개수를 리턴한다.
	int file_count_arguement() ;
	void file_skip_cur_line() ;			// 현재 라인을 건너 뛴다.

	// 파일 이름 리턴.
	const char * file_get_name()	{ return m_strFileName.c_str() ; }
	// 현재 파싱중인 라인수 : 파싱을 모두 끝냈을 경우, 파일의 총라인수이다.
	int file_get_cur_line_count()	{ return m_iCurLine ; }
	char * file_get_cur_line()		{ return &m_tempLine[0] ; }
	void file_make_lower( char * cmd ) ;


protected:
	bool file_is_comment( char ch )
	{
		for( int i = 0 ; i < m_countCommentChar ; ++i )
		{
			if( ch == m_carrCommentChar[i] )
				return true ;
		}

		return false ;
	}
	bool file_is_valid_char( char ch )
	{
		if( isalnum( ch ) || ch == '_' || ch == '{' || ch == '}' || ch < 0 || ch == '.' || ch == '-' || ch == '+' || ch == ':' )
			return true ;
		else
			return false ;
	}

protected:
	enum {
		en_cmt_max_count=12,	// 코맨트 문자로 취급되는 문자들의 개수. 최대 10개.
	} ;

	enum {
		en_count_max_line=512	// 한 라인의 최대 글자수.
	} ;

	

	ifstream		m_ifsStream ;		//
	string			m_strFileName ;		//

	int				m_iCurLine ;		// 현재 줄수.

	int				m_countCommentChar ;					//
	char			m_carrCommentChar[en_cmt_max_count] ;	//

	char			m_tempLine[en_count_max_line] ;		// 읽은 라인.
	char *			m_pPosInLine ;			// 읽은 라인에서의 현재 위치.

} ;



#endif