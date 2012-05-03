#include "../stdafx.h"
#include "FileInput.h"

int NDStreamFileInput::file_get_word( char * word, const int len )
{
	if( file_is_open() )
	{
		if( m_pPosInLine && *m_pPosInLine == 0 )
			m_pPosInLine = NULL ;

		if( m_pPosInLine )	// 이전에 읽던 것이 남아있다면, 
		{
			if( file_is_comment( *m_pPosInLine ) )	// 코멘트 이거나, 파일의 끝이면
			{
				m_pPosInLine = NULL ;
				return en_cmd_comment ;
			}

			while( false == file_is_valid_char( *m_pPosInLine ) && 0 != *m_pPosInLine )
			{
				if( file_is_comment( *m_pPosInLine ) )	// 코멘트 이거나, 파일의 끝이면
				{
					m_pPosInLine = NULL ;
					return en_cmd_comment ;
				}
				++m_pPosInLine ;
			}

			if( *m_pPosInLine == 0 )
			{
				m_pPosInLine = NULL ;
				return en_cmd_comment ;
			}
			else if( *m_pPosInLine == '{' )
			{
				++m_pPosInLine ;
				return en_cmd_open_bracket ;
			}
			else if( *m_pPosInLine == '}' )
			{
				++m_pPosInLine ;
				return en_cmd_close_bracket ;
			}
			else 
			{
				int count = 0 ;
				while( file_is_valid_char( *m_pPosInLine ) )
				{
					word[count] = *m_pPosInLine ;

					++m_pPosInLine ; 
					++count ; 

					if( count > len )
					{
						return en_cmd_over_length ;
					}
				}

				word[count] = 0 ;

				return en_cmd_string ;

			}
		}
		else	// 
		{
			
			while( 1 )
			{
				if( m_ifsStream.eof() )
					return en_cmd_end ;

				m_ifsStream.getline( m_tempLine, en_count_max_line ) ;
				m_tempLine[en_count_max_line-1] = 0 ;

				++m_iCurLine ;

				if( 0 < strlen( m_tempLine ) )
					break ;
			}

			m_pPosInLine = &m_tempLine[0] ;

			return file_get_word( word, len ) ;

		}
	}
	else
		return en_cmd_not_file_open ;
}

int NDStreamFileInput::file_count_arguement()
{
	// 
	char * p = m_pPosInLine ;

	int count = 0 ;

	while( 1 )
	{
		if( *p == 0 )
			break ;

		while( *p && !(isalnum(*p) || *p == '_' || *p < 0 || *p == '-' || *p == '+' ) )
		{
			if( file_is_comment(*p) )
			{
				*p = 0 ;
				break ;
			}
			++p ;
		}

		if( (isalnum(*p) || *p == '_' || *p < 0 || *p == '-' || *p == '+' ) )
		{
			++count ;
			++p ;

			while( (isalnum(*p) || *p == '_' || *p < 0 || *p == '-' || *p == '+' ) )
			{
				++p ;
			}
		}
	}

	return count ;

}

int NDStreamFileInput::file_get_int()
{
	char cmd[26] ;
	file_get_word( cmd, 25 ) ;

	return atoi( cmd ) ;
}

u_int NDStreamFileInput::file_get_uint()
{
	char cmd[26] ;
	file_get_word( cmd, 25 ) ;

	return static_cast<u_int>(atoi( cmd )) ;
}

float NDStreamFileInput::file_get_float()
{
	char cmd[26] ;
	file_get_word( cmd, 25 ) ;

	return (float)atof( cmd ) ;
}


int NDStreamFileInput::file_get_string( char * line, const int len ) // 리턴값은 문자열 길이.
{
	// skip white space and fill line.
	if( m_pPosInLine == NULL )
	{
		line[0] = 0 ;
		return 0 ;
	}

	while( 1 )
	{
		if( *m_pPosInLine == ' ' || *m_pPosInLine == '\t' )
			++m_pPosInLine ;
		else
			break ;
	}

	int real_len = 0 ;

	while( *m_pPosInLine && real_len < len )
	{
		line[real_len] = *m_pPosInLine ;
		++m_pPosInLine ;
		++real_len ;
	}
	
	if( real_len < len )
	{
		line[real_len] = 0 ;
		m_pPosInLine = NULL ;

		return real_len ;
	}
	else
	{
		line[0] = 0 ;
		m_pPosInLine = NULL ;

		return -1 ;
	}
}



/*
	현재 라인을 건너 뛴다.
*/
void NDStreamFileInput::file_skip_cur_line() 
{
	if( file_is_open() )
	{
		if( m_pPosInLine )	// 이전에 읽던 것이 남아있다면, 
		{
			m_pPosInLine = NULL ;
		}
	}
}


void NDStreamFileInput::file_make_lower( char * cmd )
{
	//
	for( int i = 0 ; i < 100 ; ++i )
	{
		if( cmd[i] )
		{
			if( isupper(cmd[i] ) )
				cmd[i] = _tolower(cmd[i]) ;
		}
		else
			break ;
	}

}