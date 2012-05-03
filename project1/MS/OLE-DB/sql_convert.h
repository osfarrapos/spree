#pragma once 

#include "../MyLib/MY_CRYPTO.H"



#define _SQL_CON_DATA_MAX	128
class CSqlConnectorString : public _j_Crypto
{
private:
	struct _SqlConn
	{
		char	cName[128] ;
		char	cConStr[512] ;
	};

	_SqlConn	m_sqlCon[_SQL_CON_DATA_MAX] ;

public:
	CSqlConnectorString( char * fileName ) : _j_Crypto(4096)
	{
		memset( m_sqlCon, 0, sizeof(m_sqlCon) ) ;
		if( Init( fileName ) == false ) throw ;
	}
	~CSqlConnectorString(){} ;

	bool ReadConStrFile( char * fileName )
	{
		if( fileName == NULL )
		{
			return false ;
		}

		char	buf[4096] = {0};
		
		FILE *	stream = NULL;
		try{
			stream = fopen(fileName,"r+b");
		}
		catch (...) {					
			return false;
		}

		if(stream == NULL) {			
			return false;
		}		

		int res = fread(buf, 1 , sizeof(buf), stream) ;
		if(fclose(stream) != 0) return false ;		

		if( Decryption( buf, sizeof(buf), 'N') == false )
		{
			return false ;
		}
		
		buf[res] = EOF ;
		
		// parsing		
		char * pCurBuf = buf ;
		char * checker = NULL ;
		int mode = 0 ;		// 0 - continue, 1 - line end, 2 - file end
		int index = 0 ;
		for(int i = 0 ; i<sizeof(buf); ++i )
		{	
			if( pCurBuf[0] == '/' || mode == 2 ) 
				break ;

			if( pCurBuf[0] == '-' )
			{
				mode = 0 ;
				++pCurBuf ;

				checker = pCurBuf ;
				while( mode == 0 )
				{
					switch( *checker )
					{
					case '\n':
					case '\r':
						mode = 1 ;
						break ;
					case EOF:
						mode = 2 ;
						*checker = 0 ;
						break ;
					}

					++checker ;
				}

				sscanf( pCurBuf, "%s %s", m_sqlCon[index].cName, m_sqlCon[index].cConStr ) ;
				++index ;
			}
			else
			{
				++pCurBuf ;
			}
		}


		return true ;
	}	

#ifdef _SCRIPT_CONVERT
	// 암호화된 환경 파일을 원본으로 되돌린다.
	bool ConvertScript( IN char * I_fileName, OUT char * O_FileName, bool mode /*t:암호화, f:복호화*/)
	{
		if( I_fileName == NULL )
		{
			return false ;
		}

		char	buf[4096] = {0};

		FILE *	stream = NULL;
		try{
			stream = fopen(I_fileName,"r+b");
		}
		catch (...) {					
			return false;
		}

		if(stream == NULL) {	
			return false;
		}		

		int res = fread(buf, 1 , sizeof(buf), stream) ;
		if(fclose(stream) != 0) return false ;		

		
		if( mode == true )
		{
			if( Encryption( buf, sizeof(buf), 'N') == false )
			{
				return false ;
			}
			stream = fopen( O_FileName, "wb" ) ;
			fwrite( buf, 1, res, stream) ;
			fflush(stream) ;
			fclose(stream) ;
		}
		else
		{
			if( Decryption( buf, sizeof(buf), 'N') == false )
			{
				return false ;
			}
			stream = fopen( O_FileName, "wb" ) ;
			fwrite( buf, 1, res, stream) ;
			fflush(stream) ;
			fclose(stream) ;
		}
		
		return true ;
	}
#endif

	// DB연결 스트링을 반환한다. 
	const char * GetConStr( char * name /* 연결스트링 이름(IDC_GAME_DB_CONNECT_STRING_0)*/)
	{
		for( int i = 0 ; i < _SQL_CON_DATA_MAX ; ++i )
		{
			if( strcmp( name, m_sqlCon[i].cName) == 0 )
				return m_sqlCon[i].cConStr ;
		}
		return NULL ;
	}

	void Reset()
	{
		::ZeroMemory( m_sqlCon, sizeof(m_sqlCon) ) ;
	}

};

