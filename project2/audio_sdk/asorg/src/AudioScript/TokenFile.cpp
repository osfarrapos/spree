/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/

#include "Audio.h"
#include "TokenFile.h"

using namespace std;
using namespace Audio;

const char* PCS_HEADER = "PCSFILE";
const unsigned short PCS_MAJOR_VERSION = 1;
const unsigned short PCS_MINOR_VERSION = 0;


//------------------------------------------------------------------------//
void TokenFile::Clear()
{
	m_pTokenList = NULL;
	m_bCaseSensitive = false;
	m_bWordKeywordIndex = false;
	m_bWordOperatorIndex = false;
	m_bWordVariableIndex = false;
	m_vecKeyword.clear();
	m_vecOperator.clear();
	m_vecVariable.clear();
}

//------------------------------------------------------------------------//
void TokenFile::Term()
{
	Clear();
}



//------------------------------------------------------------------------//
bool TokenFile::Write(ostream &Output, TokenList* pTokenList, DWORD dwFlags)
{
	Clear();
	
	if(dwFlags & TF_CASE_SENSITIVE)
		m_bCaseSensitive = true;
	else
		m_bCaseSensitive = false;

	// clear the queue
	while(!m_queBuffer.empty())
		m_queBuffer.pop();

	m_pTokenList = pTokenList;
	WriteHeader();
	GenerateTables();
	WriteTables();
	WriteTokens();

	while(!m_queBuffer.empty())
	{
		Output.put(m_queBuffer.front());
		m_queBuffer.pop();
	}

	Output.flush();
	return true;
}

//------------------------------------------------------------------------//
bool TokenFile::Write(string sFilename, TokenList* pTokenList, DWORD dwFlags)
{
	ofstream File;

	if(pTokenList == NULL)
		return Error::Handle("Function requires a valid token list");

	File.open(sFilename.c_str(), ios::out | ios::binary | ios::trunc);

	if(!File.good())
		return Error::Handle("ERROR: file %s can't be opened for writing.\n",(const char*)sFilename.c_str());

	return Write(File, pTokenList);
}



//------------------------------------------------------------------------//
void TokenFile::WriteHeader()
{
	for(int i = 0; i < 7; i++)
		m_queBuffer.push(PCS_HEADER[i]);

	WriteWord(PCS_MAJOR_VERSION);
	WriteWord(PCS_MINOR_VERSION);
}


//------------------------------------------------------------------------//
// Iterate through the list and pre-generate string-based tokens
void TokenFile::GenerateTables()
{
	for(TokenListItor itor = m_pTokenList->begin(); itor != m_pTokenList->end(); ++itor)
	{
		if(itor->IsKeyword())
		{
			bool bFound = false;
			for(int i = 0; i < m_vecKeyword.size(); i++)
			{
				if(m_bCaseSensitive)
				{
					if(strcmp(m_vecKeyword[i].c_str(), itor->GetKeyword()) == 0)
					{
						bFound = true;
						break;
					}
				}
				else
				{
					if(stricmp(m_vecKeyword[i].c_str(), itor->GetKeyword()) == 0)
					{
						bFound = true;
						break;
					}
				}
			}
			if(!bFound)
				m_vecKeyword.push_back(itor->GetKeyword());
		}
		else if(itor->IsOperator())
		{
			bool bFound = false;
			for(int i = 0; i < m_vecOperator.size(); i++)
			{
				if(m_bCaseSensitive)
				{
					if(strcmp(m_vecOperator[i].c_str(), itor->GetOperator()) == 0)
					{
						bFound = true;
						break;
					}
				}
				else
				{
					if(stricmp(m_vecOperator[i].c_str(), itor->GetOperator()) == 0)
					{
						bFound = true;
						break;
					}
				}
			}
			if(!bFound)
				m_vecOperator.push_back(itor->GetOperator());
		}
		else if(itor->IsVariable())
		{
			bool bFound = false;
			for(int i = 0; i < m_vecVariable.size(); i++)
			{
				if(m_bCaseSensitive)
				{
					if(strcmp(m_vecVariable[i].c_str(), itor->GetVariable()) == 0)
					{
						bFound = true;
						break;
					}
				}
				else
				{
					if(stricmp(m_vecVariable[i].c_str(), itor->GetVariable()) == 0)
					{
						bFound = true;
						break;
					}
				}
			}
			if(!bFound)
				m_vecVariable.push_back(itor->GetVariable());
		}
	}

	// If there are fewer than 255 unique tokens of a given type, then we can 
	// index that type with a byte instead of a word.  Might as well save space 
	// where we can, eh?
	if(m_vecKeyword.size() > 255)
		m_bWordKeywordIndex = true;
	if(m_vecOperator.size() > 255)
		m_bWordOperatorIndex = true;
	if(m_vecVariable.size() > 255)
		m_bWordVariableIndex = true;
}

//------------------------------------------------------------------------//
void TokenFile::WriteTables()
{
	// Write the number of entries in each table, 
	// and then write the strings sequentially.  
	int index;
	WriteWord(m_vecKeyword.size());
	for(index = 0; index < m_vecKeyword.size(); index++)
		WriteString(m_vecKeyword[index]);
	WriteWord(m_vecOperator.size());
	for(index = 0; index < m_vecOperator.size(); index++)
		WriteString(m_vecOperator[index]);
	WriteWord(m_vecVariable.size());
	for(index = 0; index < m_vecVariable.size(); index++)
		WriteString(m_vecVariable[index]);
}

//------------------------------------------------------------------------//
void TokenFile::WriteTokens()
{
	
	int index;
	for(TokenListItor itor = m_pTokenList->begin(); itor != m_pTokenList->end(); ++itor)
	{
		// Write the token type
		WriteByte(itor->GetType());
		switch(itor->GetType())
		{
		case Token::KEYWORD:
			for(index = 0; index < m_vecKeyword.size(); index++)
			{
				if(m_bCaseSensitive)
				{
					if(strcmp(itor->GetKeyword(), m_vecKeyword[index].c_str()) == 0)
					{
						if(m_bWordKeywordIndex)
							WriteWord(index);
						else
							WriteByte(index);
						break;
					}
				}
				else
				{
					if(stricmp(itor->GetKeyword(), m_vecKeyword[index].c_str()) == 0)
					{
						if(m_bWordKeywordIndex)
							WriteWord(index);
						else
							WriteByte(index);
						break;
					}
				}
			}
			break;

		case Token::OPERATOR:
			for(index = 0; index < m_vecOperator.size(); index++)
			{
				if(m_bCaseSensitive)
				{
					if(strcmp(itor->GetOperator(), m_vecOperator[index].c_str()) == 0)
					{
						if(m_bWordOperatorIndex)
							WriteWord(index);
						else
							WriteByte(index);
						break;
					}
				}
				else
				{
					if(stricmp(itor->GetOperator(), m_vecOperator[index].c_str()) == 0)
					{
						if(m_bWordOperatorIndex)
							WriteWord(index);
						else
							WriteByte(index);
						break;
					}
				}
			}
			break;

		case Token::VARIABLE:
			for(index = 0; index < m_vecVariable.size(); index++)
			{
				if(m_bCaseSensitive)
				{
					if(strcmp(itor->GetVariable(), m_vecVariable[index].c_str()) == 0)
					{
						if(m_bWordVariableIndex)
							WriteWord(index);
						else
							WriteByte(index);
						break;
					}
				}
				else
				{
					if(stricmp(itor->GetVariable(), m_vecVariable[index].c_str()) == 0)
					{
						if(m_bWordVariableIndex)
							WriteWord(index);
						else
							WriteByte(index);
						break;
					}
				}
			}
			break;

		case Token::STRING:
			WriteString(itor->GetString());
			break;

		case Token::INTEGER:
			WriteDword(itor->GetInteger());
			break;

		case Token::REAL:
			WriteDouble(itor->GetReal());
			break;

		case Token::BOOLEAN:
			WriteByte(itor->GetBoolean());
			break;

		case Token::VECTOR:
			WriteVector(itor->GetVector());
			break;

		case Token::T_GUID:
			WriteGuid(itor->GetGuid());
			break;

		};
	}
}


//------------------------------------------------------------------------//
void TokenFile::WriteString(std::string sData)
{
	// Since a word is used as the data length, this implies a maximum
	// string length of 65,535 characters, which shouldn't be a problem
	// for most normal use.
	size_t size = sData.length();
	WriteWord(sData.length());
	for(int i = 0; i < sData.length(); i++)
		m_queBuffer.push(sData[i]);
}

//------------------------------------------------------------------------//
void TokenFile::WriteDword(DWORD dwData)
{
	// Note the dependence upon byte ordering - important if porting code
	BYTE* pByte = (BYTE*)&dwData;
	for(int i = 0; i < 4; i++)
		m_queBuffer.push(*pByte++);
}

//------------------------------------------------------------------------//
void TokenFile::WriteWord(WORD wData)
{
	// Note the dependence upon byte ordering - important if porting code
	BYTE* pByte = (BYTE*)&wData;
	m_queBuffer.push(*pByte++);
	m_queBuffer.push(*pByte++);
}

//------------------------------------------------------------------------//
void TokenFile::WriteByte(BYTE byteData)
{
	m_queBuffer.push(byteData);
}

//------------------------------------------------------------------------//
void TokenFile::WriteDouble(double dData)
{
	// Note the dependence upon byte ordering - important if porting code
	BYTE* pByte = (BYTE*)&dData;
	for(int i = 0; i < 8; i++)
		m_queBuffer.push(*pByte++);
}

//------------------------------------------------------------------------//
void TokenFile::WriteVector(TokenVector3D &vData)
{
	// Note the dependence upon byte ordering - important if porting code
	BYTE* pByte = (BYTE*)&vData;
	for(int i = 0; i < 12; i++)
		m_queBuffer.push(*pByte++);
}

//------------------------------------------------------------------------//
void TokenFile::WriteGuid(GUID &guidData)
{
	// Note the dependence upon byte ordering - important if porting code
	BYTE* pByte = (BYTE*)&guidData;
	for(int i = 0; i < 16; i++)
		m_queBuffer.push(*pByte++);
}




//------------------------------------------------------------------------//
bool TokenFile::Read(istream& Input, TokenList* pTokenList)
{
	Clear();
	if(pTokenList == NULL)
	{
		return Error::Handle("Must have a valid token list to operate on.\n");
	}
	
	m_pTokenList = pTokenList;

	if(!Input.good())
		return Error::Handle("ERROR: file can't be opened for parsing.\n");

	// clear the queue
	while(!m_queBuffer.empty())
		m_queBuffer.pop();

	// read in the entire file and store in in the queue buffer
	int iData = Input.get();
	while(Input.good())
	{
		m_queBuffer.push(BYTE(iData));
		iData = Input.get();
	}

	if(!ReadHeader())
		return Error::Handle("Not a valid pre-compiled script file.");

	if(!ReadTables())
		return Error::Handle("Could not read script token table");

	if(!ReadTokens())
		return Error::Handle("Could not read script token table");

	return true;
}

//------------------------------------------------------------------------//
bool TokenFile::Read(string sFilename, TokenList* pTokenList)
{
	ifstream File;

	if(pTokenList == NULL)
		return Error::Handle("Function requirest a valid token list");

	File.open(sFilename.c_str(), ios::binary);

	if( !File.good() )
		return Error::Handle("ERROR: file %s can't be opened for parsing.\n",(const char*)sFilename.c_str());

	return Read(File, pTokenList);
}


//------------------------------------------------------------------------//
bool TokenFile::ReadHeader()
{
	char szHeader[8];
	for(int i = 0; i < 7; i++)
	{
		szHeader[i] = m_queBuffer.front();
		m_queBuffer.pop();
	}
	szHeader[7] = NULL;
	if(strcmp(szHeader, PCS_HEADER) != 0)
		return Error::Handle("Header failed check");

	DWORD dwMajor = ReadWord();
	DWORD dwMinor = ReadWord();
	if(dwMajor > PCS_MAJOR_VERSION)
		return Error::Handle("Incompatible file version");

	return true;
}

//------------------------------------------------------------------------//
bool TokenFile::ReadTables()
{
	int index;

	// Read in the keyword table
	WORD wKeywordSize = ReadWord();
	m_vecKeyword.reserve((int)wKeywordSize);
	if(m_vecKeyword.size() > 255)
		m_bWordKeywordIndex = true;
	for(index = 0; index < wKeywordSize; index++)
		m_vecKeyword.push_back(ReadString());

	// Read in the operator table
	WORD wOperatorSize = ReadWord();
	m_vecOperator.reserve((int)wOperatorSize);
	if(m_vecOperator.size() > 255)
		m_bWordOperatorIndex = true;
	for(index = 0; index < wOperatorSize; index++)
		m_vecOperator.push_back(ReadString());

	// Read in the variable table
	WORD wVariableSize = ReadWord();
	m_vecVariable.reserve((int)wVariableSize);
	if(m_vecVariable.size() > 255)
		m_bWordVariableIndex = true;
	for(index = 0; index < wVariableSize; index++)
		m_vecVariable.push_back(ReadString());

	return true;
}

//------------------------------------------------------------------------//
bool TokenFile::ReadTokens()
{
	// first read in the token type
	Token::TOKEN_TYPE type;
	Token tok;

	while(!m_queBuffer.empty())
	{
		// first read in the token type
		type = (Token::TOKEN_TYPE)ReadByte();

		switch(type)
		{
		case Token::KEYWORD:
			if(m_bWordKeywordIndex)
			{
				WORD index = ReadWord();
				assert(index < m_vecKeyword.size());
				tok.InitKeyword(m_vecKeyword[index].c_str());
			}
			else
			{
				BYTE index = ReadByte();
				assert(index < m_vecKeyword.size());
				tok.InitKeyword(m_vecKeyword[index].c_str());
			}
			break;

		case Token::OPERATOR:
			if(m_bWordOperatorIndex)
			{
				WORD index = ReadWord();
				assert(index < m_vecOperator.size());
				tok.InitOperator(m_vecOperator[index].c_str());
			}
			else
			{
				BYTE index = ReadByte();
				assert(index < m_vecOperator.size());
				tok.InitOperator(m_vecOperator[index].c_str());
			}
			break;

		case Token::VARIABLE:
			if(m_bWordVariableIndex)
			{
				WORD index = ReadWord();
				assert(index < m_vecVariable.size());
				tok.InitVariable(m_vecVariable[index].c_str());
			}
			else
			{
				BYTE index = ReadByte();
				assert(index < m_vecVariable.size());
				tok.InitVariable(m_vecVariable[index].c_str());
			}
			break;

		case Token::STRING:
			tok.InitString(ReadString().c_str());
			break;

		case Token::INTEGER:
			tok.InitInteger((int)ReadDword());
			break;

		case Token::REAL:
			tok.InitReal(ReadDouble());
			break;

		case Token::BOOLEAN:
			tok.InitBoolean((ReadByte() != 0) ? true : false);
			break;

		case Token::VECTOR:
			tok.InitVector(ReadVector());
			break;

		case Token::T_GUID:
			tok.InitGuid(ReadGuid());
			break;

		default:
			return Error::Handle("Format error in pre-compiled file.");
		};

		m_pTokenList->push_back(tok);
		tok.Term();
	}
	return true;
}

//------------------------------------------------------------------------//
// Basic types
string TokenFile::ReadString()
{
	char* pszData;
	WORD wSize = ReadWord();
	pszData = new char[(int)wSize + 1];
	for(int i = 0; i < (int)wSize; i++)
	{
		pszData[i] = m_queBuffer.front();
		m_queBuffer.pop();
	}
	pszData[(int)wSize] = NULL;
	string sData(pszData);
	delete[] pszData;
	return sData;
}

//------------------------------------------------------------------------//
DWORD TokenFile::ReadDword()
{
	DWORD dwData;
	PBYTE p = (PBYTE)&dwData;
	for(int i = 0; i < 4; i++)
	{
		*p++ = m_queBuffer.front();
		m_queBuffer.pop();
	}
	return dwData;
}

//------------------------------------------------------------------------//
WORD TokenFile::ReadWord()
{
	WORD wData;
	PBYTE p = (PBYTE)&wData;
	*p++ = m_queBuffer.front();
	m_queBuffer.pop();
	*p++ = m_queBuffer.front();
	m_queBuffer.pop();
	return wData;
}

//------------------------------------------------------------------------//
BYTE TokenFile::ReadByte()
{
	BYTE byteData = m_queBuffer.front();
	m_queBuffer.pop();
	return byteData;
}

//------------------------------------------------------------------------//
double TokenFile::ReadDouble()
{
	double dData;
	PBYTE p = (PBYTE)&dData;
	for(int i = 0; i < 8; i++)
	{
		*p++ = m_queBuffer.front();
		m_queBuffer.pop();
	}
	return dData;
}

//------------------------------------------------------------------------//
TokenVector3D TokenFile::ReadVector()
{
	TokenVector3D vData;
	PBYTE p = (PBYTE)&vData;
	for(int i = 0; i < 12; i++)
	{
		*p++ = m_queBuffer.front();
		m_queBuffer.pop();
	}
	return vData;
}

//------------------------------------------------------------------------//
GUID TokenFile::ReadGuid()
{
	GUID guidData;
	PBYTE p = (PBYTE)&guidData;
	for(int i = 0; i < 16; i++)
	{
		*p++ = m_queBuffer.front();
		m_queBuffer.pop();
	}
	return guidData;
}




