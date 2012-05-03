/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#pragma once
#ifndef __LEXER_H
#define __LEXER_H

#include "Token.h"


namespace Audio
{

/*! \file Lexer.h*/ 


#pragma warning(disable:4786)
#pragma warning(disable:4251)

#ifndef DOXYGEN_SHOULD_SKIP_THIS
typedef std::stack<std::string> StringStack;


class Macro
{
public:
	Macro() {  Clear();  }
	virtual ~Macro() { }

	void Clear();

	std::string m_sName;
	StringVector m_ArgumentVector;
	TokenVector m_SubstitutionVector;
	TokenList m_MacroList;
	int m_iTotalArguments;
	int m_iCurrentArgument;
	
	bool operator < (const Macro& m) const
	{  return m_sName < m.m_sName;  }
	bool operator < (const char* psz) const
	{  return m_sName < psz;  }

	bool empty()
	{  return m_sName.length() ? false : true;  }
};

typedef std::set<Macro> MacroSet;
typedef MacroSet::iterator MacroSetItor;


#endif // DOXYGEN_SHOULD_SKIP_THIS

//! Used for tokenizing text files
/*! Lexer is used to tokenize any sort of text file or script
ranging from simple ini files to full-blown C++.  It provides the ability 
to define custom keywords and operators as well as more advanced features
such as automatic include file processing, macro expansion, and GUID
recognition and parsing.  Basic types recognized are integers, floats, bools
strings, keywords, operators, variables, and guids.
*/
class Lexer
{
public:
	//! Constructor
	Lexer()
	{  	
		Clear();  
	}
	//! Destructor
	virtual ~Lexer()
	{  Term();  }

	//! Clears all internal parameters
	void Clear();
	//! Creates the lexer object
	/*!
	This function initializes a number of common operators and keywords required
	by the Lexer class.  It optionally allows you to automatically reserve the
	most common operators used by scripts, ini files, or languages such as
	C++.  
	\param bReserveCommonOperators Allows the user to reserve common C++ style 
	operators
	\return true indicates success, false indicates failure
	*/
	bool Init(bool bReserveCommonOperators = true);
	//! Destroys the parser object and clears all stored data
	void Term();

	//! Reserve an operator
	/*!
	Note that one restriction exists for operators.  Any operator longer than
	a single character must be created of smaller operators.  This is done
	for efficiency reasons, as operators take the highest precedence in the
	parsing process and must be parsed first.
	\param sOperator String representing the operator to reserve
	*/
	void ReserveOperator(std::string sOperator);
	//! Reserve a keyword
	/*!
	Tell the parser to reserve a particular string as a keyword instead of
	making it a generic variable.  This can aid in lexical analysis of the token
	stream.
	\param sKeyword String representing the keyword to reserve
	*/
	void ReserveKeyword(std::string sKeyword);
	//! Places an end-of-line token after each parsed line.  This is off by default
	void MarkEndOfLine(bool bEOL)					{  m_bMarkEndOfLine = bEOL;  }
	//! Enables rejection of C and C++ style comments.  This is on by default
	void EnableComments(bool bComments)				{  m_bEnableComments = bComments;  }
	//! Enables automatic processing of C-style headers.  This is off by default
	void ProcessHeaders(bool bHeaders)				{  m_bProcessHeaders = bHeaders;  }
	//! Enables automatic processing of C-style macros.  This is off by default
	void ProcessMacros(bool bMacros)				{  m_bProcessMacros = bMacros;  }
	//! Tokenize a text file
	/*!
	This function performs a complete tokenization of the file, placing all
	tokens into the tokenlist passed to it.
	\param sFilename Name of the file to process.
	\param tokenlist TokenList to insert processed tokens into.
	*/
	bool Process(std::string sFilename, TokenList& tokenlist);
	//! Tokenize a text file
	/*!
	This function performs a complete tokenization of the file, placing all
	tokens into the tokenlist passed to it.
	\param pData Memory location of the data to process.
	\param nDataSize Size of the data in bytes.
	\param tokenlist TokenList to insert processed tokens into.
	*/
	bool Process(uint8* pData, uint32 nDataSize, TokenList& tokenlist);


private:
	void ReserveCommonOperators();
	bool ProcessHeaders(TokenList& tokenlist);
	bool ProcessMacros(TokenList& tokenlist);
	bool ParseLine(uint8* pLineBuffer, uint32 nLineLength);
	bool MakeToken(const std::string& sToken, Token::TOKEN_TYPE Type = Token::UNKNOWN_TOKEN);
	bool ExpandMacro(TokenList& tokenlist, TokenListItor& itor, MacroSetItor itr);
	bool ParseVector(TokenList& tokenlist, TokenListItor& itor);
	bool ParseGUID(TokenList& tokenlist, TokenListItor& itor);
	void ClearMacros();
	int GetTotalLinesParsed() const					{  return m_iTotalLinesParsed;  }
	int GetCurrentLineNumber() const				{  return m_iCurrentLineNumber;  }
	inline bool IsDigit(char c)	const				{  return ((c >= '0') && (c <= '9')) ? true : false;  }
	inline bool IsHexDigit(char c) const			{  return (IsDigit(c) || ((c >= 'a') && (c <= 'f')) || ((c >= 'A') && (c <= 'F'))) ? true : false;  }
	inline bool IsWhitespace(char c) const			{  return ((c == char(32)) || (c == char(9)) || (c == char(0x0D)) || (c == char(0x0A))) ? true : false;  }
	bool IsReal(const std::string& sToken) const;
	inline double GetReal(const std::string& sToken){  return atof(sToken.c_str());  }
	bool IsInteger(const std::string& sToken) const;
	inline int GetInteger(const std::string& sToken){  return atoi(sToken.c_str());  }
	bool IsHex(const std::string& sToken) const;
	DWORD GetHex(const std::string& sToken);
	bool IsBoolean(const std::string& sToken) const;
	bool GetBoolean(const std::string& sToken);
	bool IsKeyword(const std::string& sToken) const;
	bool IsOperator(const std::string& sToken) const;

	// indicates whether the end of line character should be 
	// included as a token
	bool m_bMarkEndOfLine;
	// Are C and C++ style comments supported (supported means they will be
	// excluded from the token list)?
	bool m_bEnableComments;
	// Should we process header files?
	bool m_bProcessHeaders;
	// Should we process macros?
	bool m_bProcessMacros;
	// Is a C style comment in effect?
	bool m_bCStyleCommentActive;
	// The current file being parsed
	StringStack m_CurrentFileStack;
	// The current line number of the file being parsed
	int m_iCurrentLineNumber;
	// Total number of lines parsed by the parser
	int m_iTotalLinesParsed;
	// contains a vector of operators defined as strings
	StringVector m_OperatorVector;
	// contains a vector of keywords defined as strings
	StringVector m_KeywordVector;
	// pointer to list of tokens as defined by the current list of keywords and operators
	TokenList* m_pTokenList;
	// We store macros here sorted by their string definition.  
	MacroSet m_MacroSet;

};

}; // namespace Audio

#endif // __PARSER_H