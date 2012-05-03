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
#include "Lexer.h"

using namespace std;
using namespace Audio;

const int MAX_LINE_LENGTH = 1024;

const int INITIAL_RESERVED_OPERATORS = 	50;
const int INITIAL_RESERVED_KEYWORDS =	20;

// These strings are part of the standard set of operators required in order to 
// perform standard parsing functions
const char* g_pszStringDelimiter		= "\"";
const char* g_pszSingleLineComment		= "//";
const char* g_pszOpeningComment			= "/*";
const char* g_pszClosingComment			= "*/";
const char* g_pszDecimalPoint			= ".";
const char* g_pszNewLine				= "/n";
const char* g_pszPoundSign				= "#";
const char* g_pszLeftParen				= "(";
const char* g_pszRightParen				= ")";
const char* g_pszMinusSign				= "-";
const char* g_pszDivideSign				= "/";
const char* g_pszMultiplySign			= "*";
const char* g_pszBackSlash				= "\\";
const char* g_pszComma					= ",";
const char* g_pszSemiColon				= ";";

// Several keywords are predefined for built-in functionality
const char* g_pszInclude				= "include";
const char* g_pszDefine					= "define";
const char* g_pszDefineGUID				= "DEFINE_GUID";
const char* g_pszVector					= "vector";
const char* g_pszTrue					= "true";
const char* g_pszFalse					= "false";



//------------------------------------------------------------------------//
void Macro::Clear()
{
	m_sName = "";
	m_ArgumentVector.clear();
	m_SubstitutionVector.clear();
	m_MacroList.clear();
	m_iTotalArguments = 0;
	m_iCurrentArgument = 0;
}


//------------------------------------------------------------------------//
void Lexer::Clear()
{
	while(!m_CurrentFileStack.empty())
		m_CurrentFileStack.pop();
	m_iCurrentLineNumber = 0;
	m_iTotalLinesParsed = 0;
	m_bMarkEndOfLine = false;
	m_OperatorVector.clear();
	m_KeywordVector.clear();
	m_pTokenList = NULL;
	m_bEnableComments = true;
	m_bCStyleCommentActive = false;
	m_bProcessHeaders = false;
	m_bProcessMacros = false;

}

//------------------------------------------------------------------------//
bool Lexer::Init(bool bReserveCommonOperators)
{
	m_OperatorVector.reserve(INITIAL_RESERVED_OPERATORS);

	// These are all required operators for standard functionality
	m_OperatorVector.push_back(g_pszDecimalPoint);
	m_OperatorVector.push_back(g_pszOpeningComment);
	m_OperatorVector.push_back(g_pszClosingComment);
	m_OperatorVector.push_back(g_pszSingleLineComment);
	m_OperatorVector.push_back(g_pszStringDelimiter);
	m_OperatorVector.push_back(g_pszNewLine);
	m_OperatorVector.push_back(g_pszMinusSign);
	m_OperatorVector.push_back(g_pszPoundSign);
	m_OperatorVector.push_back(g_pszDivideSign);
	m_OperatorVector.push_back(g_pszMultiplySign);
	m_OperatorVector.push_back(g_pszLeftParen);
	m_OperatorVector.push_back(g_pszRightParen);
	m_OperatorVector.push_back(g_pszBackSlash);
	m_OperatorVector.push_back(g_pszComma);
	m_OperatorVector.push_back(g_pszSemiColon);
	sort(m_OperatorVector.begin(), m_OperatorVector.end());

	// define the most common operators
	if(bReserveCommonOperators)
		ReserveCommonOperators();

	// reserved default number of keywords
	m_KeywordVector.reserve(INITIAL_RESERVED_KEYWORDS);
	m_KeywordVector.push_back(g_pszInclude);
	m_KeywordVector.push_back(g_pszDefine);
	m_KeywordVector.push_back(g_pszDefineGUID);
	m_KeywordVector.push_back(g_pszVector);
	m_KeywordVector.push_back(g_pszTrue);
	m_KeywordVector.push_back(g_pszFalse);
	sort(m_KeywordVector.begin(), m_KeywordVector.end());

	return true;
}

//------------------------------------------------------------------------//
void Lexer::Term()
{
	ClearMacros();
	Clear();
}

//------------------------------------------------------------------------//
void Lexer::ReserveOperator(string sOperator)
{
	m_OperatorVector.push_back(sOperator);
	sort(m_OperatorVector.begin(), m_OperatorVector.end());
}

//------------------------------------------------------------------------//
void Lexer::ReserveKeyword(string sKeyword)
{
	m_KeywordVector.push_back(sKeyword);
	sort(m_KeywordVector.begin(), m_KeywordVector.end());
}

//------------------------------------------------------------------------//
void Lexer::ReserveCommonOperators()
{
	// You can add or remove any operators from this list, or
	// create specialized functions to create custom lists of
	// common operators.
	m_OperatorVector.push_back("=");
	m_OperatorVector.push_back("==");
	m_OperatorVector.push_back("!");
	m_OperatorVector.push_back("!=");
	m_OperatorVector.push_back("+");
	m_OperatorVector.push_back("++");
	m_OperatorVector.push_back("--");
	m_OperatorVector.push_back("+=");
	m_OperatorVector.push_back("-=");
	m_OperatorVector.push_back("*=");
	m_OperatorVector.push_back("/=");
	m_OperatorVector.push_back("<");
	m_OperatorVector.push_back(">");
	m_OperatorVector.push_back("<=");
	m_OperatorVector.push_back(">=");
	m_OperatorVector.push_back("<<");
	m_OperatorVector.push_back(">>");
	m_OperatorVector.push_back("&");
	m_OperatorVector.push_back("&&");
	m_OperatorVector.push_back("^");
	m_OperatorVector.push_back("{");
	m_OperatorVector.push_back("}");
	m_OperatorVector.push_back("[");
	m_OperatorVector.push_back("]");
	m_OperatorVector.push_back(":");
	m_OperatorVector.push_back("::");
	m_OperatorVector.push_back("?");
	m_OperatorVector.push_back("->");
	sort(m_OperatorVector.begin(), m_OperatorVector.end());
}


//------------------------------------------------------------------------//
// Read and tokenize a single file and store the results in the token list
bool Lexer::Process(string sFilename, TokenList& tokenlist)
{
	uint8* pData;
	uint32 nDataSize;

	IAudioStream* pStream;
	CreateAudioStream cas(pStream);
	if(!pStream)
		return Error::Handle("ERROR: file %s can't be created",(const char*)sFilename.c_str());
	
	HRESULT hr = pStream->Open(sFilename);
	if(FAILED(hr))
		return Error::Handle("ERROR: file %s can't be opened",(const char*)sFilename.c_str());

	// Get the stream size
	STATSTG stats;
	pStream->Stat(&stats, 0);
	nDataSize = (uint32)stats.cbSize.QuadPart;

	// Read the stream into a data buffer
	pData = new uint8[nDataSize];
	hr = pStream->Read(pData, nDataSize, 0);
	if(FAILED(hr))
	{
		_XASAFE_DELETE_ARRAY(pData);
		return Error::Handle("ERROR: file %s can't be read",(const char*)sFilename.c_str());
	}

	// Process the data buffer
	m_CurrentFileStack.push(sFilename);
	bool bErr = Process(pData, nDataSize, tokenlist);
	_XASAFE_DELETE_ARRAY(pData);
	m_CurrentFileStack.pop();

	if(!bErr)
		return Error::Handle("Could not parse file %s.", (const char*)sFilename.c_str());

	return true;
}

//------------------------------------------------------------------------//
// Read and tokenize a memory buffer and store the results in the token list
bool Lexer::Process(uint8* pData, uint32 nDataSize, TokenList& tokenlist)
{
	m_pTokenList = &tokenlist;
	m_iCurrentLineNumber = 0;
	
	uint32 nCharactersProcessed = 0;
	uint8* pCurrentPos = pData;
	uint8* pLineStart = pData;
	int32  iLineLength = 0;

	bool bFormFeed = false;
	bool bCarriageReturn = false;

	// cycle through the input buffer and check each character
	while(nCharactersProcessed < nDataSize)
	{
		// Because we're usually opening the file in binary mode (because
		// of the zip file packer), we have to explicitly check for
		// formfeed and carriage return bytes.
		if(pCurrentPos[0] == 0x0D)
			bFormFeed = true;
		if(pCurrentPos[0] == 0x0A)
			bCarriageReturn = true;
		if(bCarriageReturn)
		{
			ParseLine(pLineStart, iLineLength);
			if(m_bMarkEndOfLine)
				MakeToken(string(g_pszNewLine), Token::OPERATOR);
			pLineStart = pCurrentPos + 1;
			iLineLength = 0;
			bFormFeed = false;
			bCarriageReturn = false;
			m_iCurrentLineNumber++;
			m_iTotalLinesParsed++;
		}

		nCharactersProcessed++;
		pCurrentPos++;
		iLineLength++;
	}

	// After the initial processing is done, process any header
	// files located in the list
	if(m_bProcessHeaders)
	{
		if(!ProcessHeaders(tokenlist))
		{
			return false;
		}
	}
	// Finally, perform all macro substitutions
	if(m_bProcessMacros)
	{
		if(!ProcessMacros(tokenlist))
		{
			return false;
		}
	}

	return true;
}


//------------------------------------------------------------------------//
bool Lexer::ParseLine(uint8* pLineBuffer, uint32 nLineLength)
{
	// Use this as a temporary buffer for token parsing
	string sTokenBuffer;

	char cTempBuffer[MAX_LINE_LENGTH];

	// flag to indicate that an opening string has been found
	bool bStringOpened = false;

	// flag to indicate that the string has been closed
	bool bStringClosed = false;

	// comments should become active at the end of the parse cycle
	bool bCommentEnded = false;

	// marker for where the current token starts
	int iTokenStart = 0;

	int iCurrentPosition = 0;
	int iCopiedCharacters = 1;

	// store off our last operator match
	string sLastMatch;

	// store off our last operator match's position
	int iLastMatchPosition = 0;

	// lets us know when we've found an operator match
	bool bFoundMatch = false;

	// signals the end of a line, so we know to look for the last token
	bool bEndOfLine = false;

	// Did we find an operator?
	bool bFoundOperator = false;

	// Now we're ready to begin parsing.  Here's basically how this works:

	// We cycle through the line one character at a time, checking all the
	// time for operators.  If an operator is found, we then check to see
	// if it is a subset of a longer operator which then gets precedence.
	// The reason we look for operators first is that they can be used to
	// break up other tokens in addition to whitespace.  After we've checked
	// for operators (both in combination with other tokens and those which
	// are standing alone) we then extract all tokens by passing them into
	// the function MakeToken().  

	// Handling strings is another headache entirely.  Since we want to
	// preserve the strings in their entirety, we make some special cases
	// to explicitly handle them by checking for opening and closing
	// double-quote marks.  Strings read by this parsing routine cannot
	// extend past a single line.

	// Cycle through the entire string and break into tokens in a single pass
	while(iCurrentPosition <= nLineLength)
	{
		// We need to read in the strings without regard to any tokens or
		// whitespace, so we'll special-case that here
		if(bStringOpened)
		{
			// Check to see if the quote character has been read, but exclude it if
			// it is preceded by the backslash character.  This allows double-quote
			// characters to reside within strings.
			if((pLineBuffer[iCurrentPosition] == '\"') && 
				(pLineBuffer[iCurrentPosition - 1] != '\\'))
			{
				bStringOpened = false;
				bStringClosed = true;
				sLastMatch = "";
			}
		}
		else
		{
			// As long as we're finding operator matches, we'll continue to look for
			// longer and longer matches.
			do
			{
				bFoundMatch = false;

				// if we're trying to copy too many characters then break out of the loop
				if((iCurrentPosition + iCopiedCharacters) > nLineLength)
				{
					// signal that we've hit the end of the line so we'll know later
					// to check for the last remaining token
					bEndOfLine = true;
					break;
				}

				// copy the appropriate characters to the temporary string buffer
				// If the number of copied characters doesn't match what we though, then
				// we've hit the end of the line.
				//sLineBuffer.copy(cTempBuffer, iCopiedCharacters, iCurrentPosition);
				memcpy(cTempBuffer, pLineBuffer + iCurrentPosition, iCopiedCharacters);
				// place a null terminator after the copied character
				cTempBuffer[iCopiedCharacters] = NULL;

				// assign the character buffer to a string
				sTokenBuffer = cTempBuffer;

				// next let's see if this character or characters belong match to any
				// operators stored in our operator list
				if(binary_search(m_OperatorVector.begin(), m_OperatorVector.end(), sTokenBuffer))
				{
					// store the match in the last match string
					sLastMatch = sTokenBuffer;
					bFoundMatch = true;
					bFoundOperator = true;
					iLastMatchPosition = iCurrentPosition;

				}
				// increment the number of characters to copy for the next loop
				iCopiedCharacters++;

			}
			while(bFoundMatch);
		}

		// what do we do if we find an operator.
		// We to a pre-check because we don't want to place some operators, such
		// as comment delimitters in the final token list
		if(bFoundOperator)
		{
			// special exclusions for . and - operators (they can be part of
			// a number)

			// If a period or minus is the operator, we need to verify that it isn't part
			// of a number.  If the period isn't the last number in a line
			// and the next character is a digit
			if((sLastMatch == g_pszDecimalPoint) || (sLastMatch == g_pszMinusSign))
			{
				// make sure we don't go out of bounds
				if(iLastMatchPosition + 1 < nLineLength)
				{
					// Looks like this is part of a real number instead of
					// a period or minus operator
					if(IsDigit(pLineBuffer[iLastMatchPosition + 1]))
					{
						sLastMatch = "";
						bFoundOperator = false;
					}
				}
			}
			else if(sLastMatch == g_pszStringDelimiter)
			{
				// toggle the string flag
				bStringOpened = true;
				sLastMatch = "";
				bFoundOperator = false;
			}
			else if(m_bEnableComments)
			{			
				if(sLastMatch == g_pszOpeningComment)
				{
					m_bCStyleCommentActive = true;
				}
				else if(sLastMatch == g_pszClosingComment)
				{
					bCommentEnded = true;
				}
			}
		}

		// we've found an operator (possibly combined with another token)
		if(bFoundOperator)
		{
			// the operator needs to be split apart from the previous token
			if(iCurrentPosition != iTokenStart)
			{
				// for the first token, the number of characters should be the
				// starting position of the second token minus the starting
				// position of the original token.
				int iNumToCopy = iCurrentPosition - iTokenStart;
				//sLineBuffer.copy(cTempBuffer, iNumToCopy, iTokenStart);
				memcpy(cTempBuffer, pLineBuffer + iTokenStart, iNumToCopy);
				cTempBuffer[iNumToCopy] = NULL;
				sTokenBuffer = cTempBuffer;
				
				// create the first token
				MakeToken(sTokenBuffer);
			}

			// If the latest token is the single-line comment operator, don't bother
			// storing the token.
			if(sLastMatch == g_pszSingleLineComment)
			{
				// simply return out of this function to stop processing any more lines.
				// Remember to add an end of line token if it's required.
				if(m_bMarkEndOfLine)
					MakeToken(string(g_pszNewLine), Token::OPERATOR);
				return true;
			}

			// create the second token
			MakeToken(sLastMatch, Token::OPERATOR);

			// set the new starting token position
			iTokenStart = iCurrentPosition + sLastMatch.length();
		}
		// we're at the end of a single non-operator token
		else if((bEndOfLine || IsWhitespace(pLineBuffer[iCurrentPosition])) 
			&& !bStringOpened)
		{			
			int iNumToCopy = iCurrentPosition - iTokenStart;

			// ignore all "tokens" which are really only empty whitespace
			if(iNumToCopy > 0)
			{
				//sLineBuffer.copy(cTempBuffer, iNumToCopy, iTokenStart);
				memcpy(cTempBuffer, pLineBuffer + iTokenStart, iNumToCopy);
				cTempBuffer[iNumToCopy] = NULL;
				sTokenBuffer = cTempBuffer;
				
				// create the first token
				MakeToken(sTokenBuffer);
			}

			iTokenStart = iCurrentPosition + 1;
		}
		else if(bStringClosed)
		{
			bStringClosed = false;

			// we remove 1 from the number of characters to copy because we
			// don't want to copy the last quote mark along with the string
			int iNumToCopy = iCurrentPosition - iTokenStart - 1;

			// we also start from iTokenStart + 1 to avoid copying the first
			// quote mark as well
			//sLineBuffer.copy(cTempBuffer, iNumToCopy, iTokenStart + 1);
			memcpy(cTempBuffer, pLineBuffer + iTokenStart + 1, iNumToCopy);
			cTempBuffer[iNumToCopy] = NULL;
			sTokenBuffer = cTempBuffer;
			
			// create the first token
			MakeToken(sTokenBuffer, Token::STRING);

			// move the token position ahead one character to avoid
			// reparsing the last quote mark
			iTokenStart = iCurrentPosition + 1;
		}

		// a C style comment has ended, so reset all flags and resume
		// normal parsing
		if(bCommentEnded)
		{
			m_bCStyleCommentActive = false;
			bCommentEnded = false;
		}

		// increment the start position based on how many characters were copied
		if(bFoundOperator)
			iCurrentPosition += sLastMatch.length();
		else
			iCurrentPosition++;


		// set the number of copied character back to one.
		iCopiedCharacters = 1;

		// reset the found operator flag
		bFoundOperator = false;
	}


	return true;
}


//------------------------------------------------------------------------//
// Inserts the tokenized contents of header files into
// a token list.
bool Lexer::ProcessHeaders(TokenList& tokenlist)
{
	TokenList IncludeTokenList;
	bool bPreProcessorSymbolFound = false;
	bool bIncludeFound = false;

	Token CurrentToken;

	// reset the class tokenlist to null, since we will be using another
	// token list if any include directives are found.
	m_pTokenList = NULL;

	// store the previous end of line settings
	bool bPreviousEOLSetting = m_bMarkEndOfLine;
	
	// we want to see the end of the line for macro parsing
	m_bMarkEndOfLine = true;

	for(TokenList::iterator itor = tokenlist.begin(); itor != tokenlist.end(); ++itor)
	{
		CurrentToken = (*itor);

		switch(itor->GetType())
		{
		case Token::OPERATOR:
			if(string(g_pszPoundSign) == itor->GetOperator())
				bPreProcessorSymbolFound = true;
			else if(string(g_pszNewLine) == itor->GetOperator())
				bPreProcessorSymbolFound = false;
			break;
		case Token::KEYWORD:
			if(string(g_pszInclude) == itor->GetKeyword())
				bIncludeFound = true;
			break;
		case Token::STRING:
			if(bPreProcessorSymbolFound)
			{
				if(bIncludeFound)
				{
					// After finding a file to include, we then proceed to parse it
					// and search for includes in that file.  This is a recursive
					// process which ends up calling this same function again.

					// reset the include and preprocessor flags for use in this
					// include file parsing loop.
					bIncludeFound = false;
					bPreProcessorSymbolFound = false;

					// If the file successfully parsed, insert
					if(!Process(itor->GetString(), IncludeTokenList))
						return Error::Handle("Could not parse include file %s.", itor->GetString());

					// Find and include other files recursively if needed
					if(!ProcessHeaders(IncludeTokenList))
						return Error::Handle("Was unable to process header files."); 

					// Replace the #include "someheader.h" tokens with the actual
					// content of the files.
					TokenList::iterator bitor = itor;
					--bitor;  --bitor;
					++itor;
					itor = tokenlist.erase(bitor, itor);

#ifdef TOKEN_DEBUGGING_INFO
					// Create a file to index map for merging the file references
					vector<int> filemap;
					filemap.reserve(IncludeTokenList.GetNumFileRefs());

					// Add the file references to the base token list and retrieve the indices
					for(int i = 0; i < IncludeTokenList.GetNumFileRefs(); i++)
						filemap.push_back(tokenlist.GetFileIndex(IncludeTokenList.GetFileName(i)));

					// Replace the file in the include token list with new indices
					for(TokenListItor itr = IncludeTokenList.begin(); itr != IncludeTokenList.end(); ++itr)
						itr->SetFileIndex(filemap[itr->GetFileIndex()]);
#endif
					// splice the new parsed list into the existing list
					tokenlist.splice(itor, IncludeTokenList);

					--itor;
					// Make sure the include and preprocessor flags are cleared at the
					// end of this cycle.
					bIncludeFound = false;
					bPreProcessorSymbolFound = false;
				}
			}
			break;
		};
	}
	// restore original end of line settings
	m_bMarkEndOfLine = bPreviousEOLSetting;
	return true;
}


//------------------------------------------------------------------------//
bool Lexer::ProcessMacros(TokenList& tokenlist)
{
	TokenList TokList;
	TokenList MacroList;

	// iterators and flag use for erasing any line
	// beginning with a preprocessor symbol
	TokenListItor PreProcBegin;
	TokenListItor PreProcEnd;
	bool bFoundPreprocessorLine = false;
	bool bErasePreprocessorLine = false;

	// A ton of state flags used to find and process any macro
	bool bPreProcessorSymbolFound = false;
	bool bDefineSymbolFound = false;
	bool bDefineMacro = false;
	bool bContinueLineFound = false;
	bool bMacroName = false;
	bool bFirstMacroToken = false;
	bool bMacroArguments = false;
	bool bMacroArgVar = false;
	bool bMacroBody = false;
	bool bMacroArgEnd = false;

	Token CurrentToken;
	Macro CurrentMacro;

	// reset the class tokenlist to null, since we will be using another
	// token list if any include directives are found.
	m_pTokenList = NULL;

	for(TokenList::iterator itor = tokenlist.begin(); itor != tokenlist.end(); ++itor)
	{
		CurrentToken = (*itor);

		switch(itor->GetType())
		{
		case Token::OPERATOR:
			if(strcmp(g_pszPoundSign, itor->GetOperator()) == 0)
			{
				bPreProcessorSymbolFound = true;
				bFoundPreprocessorLine = true;
				// mark this entire line for removal from the token list
				PreProcBegin = itor;
			}
			else if(strcmp(g_pszBackSlash, itor->GetOperator()) == 0)
				bContinueLineFound = true;
			else if(strcmp(g_pszNewLine, itor->GetOperator()) == 0)
			{
				if(bContinueLineFound)
					bContinueLineFound = false;
				else
				{
					if(bFoundPreprocessorLine)
					{
						bErasePreprocessorLine = true;
						// mark the end of the tokens to be removed
						PreProcEnd = itor;
						++PreProcEnd;
					}

					bDefineSymbolFound = false;
					bDefineMacro = false;
					bPreProcessorSymbolFound = false;
					bMacroBody = false;
					if(!CurrentMacro.empty())
					{
						m_MacroSet.insert(CurrentMacro);
						CurrentMacro.Clear();
					}

					// We've hit the end of the macro and haven't found the closing
					// right paren.  Obviously an error.
					if(bMacroArguments)
					{
						return Error::Handle("Syntax error in header macro.");
					}
				}
			}
			break;

		case Token::KEYWORD:
			if(bPreProcessorSymbolFound && (strcmp(itor->GetKeyword(), g_pszDefine) == 0))
			{
				bDefineSymbolFound = true;
			}
			// Check for vector definitions: vector(x,y,z)
			else if(strcmp(itor->GetKeyword(), g_pszVector) == 0)
			{
				if(!ParseVector(tokenlist, itor))
					return Error::Handle("Unable to parse Vector definition");
			}
			// GUID recognition is a special case for us to check for
			else if(strcmp(itor->GetKeyword(), g_pszDefineGUID) == 0)
			{
				if(!ParseGUID(tokenlist, itor))
					return Error::Handle("Unable to parse GUID definition");
			}
			break;

		case Token::VARIABLE:
			// we don't want to expand macros when we're in the middle of defining
			// another macro
			if(!bDefineMacro)
			{
				Macro TmpMacro;
				TmpMacro.m_sName = (*itor).GetVariable();
				MacroSetItor itr = m_MacroSet.find(TmpMacro);
				if(itr != m_MacroSet.end())
				{
					if(!ExpandMacro(tokenlist, itor, itr))
						return Error::Handle("Could not expand macro");
				}
			}
			break;
		};

		// Here's where we define the macro
		if(bDefineMacro)
		{
			bDefineSymbolFound = false;

			// First time through we define the macro name
			if(bMacroName)
			{
				bMacroName = false;
				CurrentMacro.Clear();
				if(itor->GetType() != Token::VARIABLE)
				{
					return Error::Handle("Illegal syntax in macro.");
				}
				CurrentMacro.m_sName = itor->GetVariable();
				bFirstMacroToken = true;
			}
			// this indicates that it is the first macro token found after the name, so we
			// check for a left paren to see if the macro takes arguments
			else if(bFirstMacroToken)
			{
				bFirstMacroToken = false;
				if((itor->GetType() == Token::OPERATOR) && (strcmp(itor->GetOperator(), "(") == 0))
				{
					bMacroArguments = true;
					bMacroArgVar = true;
				}
				else
				{
					bMacroBody = true;
				}
			}
			else if(bMacroArgEnd)
			{
				bMacroBody = true;
				bMacroArgEnd = false;
			}
			// This case process all the macro arguments
			else if(bMacroArguments)
			{
				// stop processing macro arguments if we find the right paren
				if((itor->GetType() == Token::OPERATOR) && (strcmp(itor->GetOperator(), ")") == 0))
				{
					// if we've already parsed a comma and are expecting another variable, then
					// flag an error when the closing paren is reached.
					if(bMacroArgVar)
					{
						return Error::Handle("Missing argument in macro.");
					}
					// Indicate that we're no longer processing arguments
					bMacroArguments = false;
					// and we're ready to start processing the macro body
					bMacroArgEnd = true;
				}
				// We know that what we're processing in this case either has to be
				// argument variables or separating commas
				else
				{
					// If this flag is set, we know to expect a variable (the arg name)
					if(bMacroArgVar)
					{
						bMacroArgVar = false;
						if(itor->GetType() != Token::VARIABLE)
						{
							return Error::Handle("Incorrect syntax for macro variable.");
						}
						// push the argument onto the back of the argument vector and
						// increment the argument count
						CurrentMacro.m_ArgumentVector.push_back(itor->GetVariable());
						CurrentMacro.m_SubstitutionVector.push_back(Token());
						CurrentMacro.m_iTotalArguments++;
					}
					// Otherwise we know this has to be a comma separating the arguments
					else
					{
						bMacroArgVar = true;
						// signal an error on anything but a comma operator
						if(!((itor->GetType() == Token::OPERATOR) && (strcmp(itor->GetOperator(), ",") == 0)))
						{
							return Error::Handle("Missing comma operator between macro arguments.");
						}
					}
				}
			}
			// here's where we actually process the macro body
			if(bMacroBody)
			{
				// Ignore backslashes and end of line markers
				if(!((itor->GetType() == Token::OPERATOR) && 
					((strcmp(itor->GetOperator(), g_pszBackSlash) == 0) || 
					((strcmp(itor->GetOperator(), g_pszNewLine) == 0)))))
				{
					CurrentMacro.m_MacroList.push_back(*itor);
				}
			}
		}

		if(bDefineSymbolFound)
		{
			// we're now defining a new macro
			bDefineMacro = true;
			// for the first symbol we're defining the name of the macro
			bMacroName = true;
		}

		// erase any line of tokens beginning with a preprocessor symbol
		if(bErasePreprocessorLine && bFoundPreprocessorLine)
		{
			bErasePreprocessorLine = false;
			bFoundPreprocessorLine = false;

			CurrentToken = (*PreProcBegin);
			CurrentToken = (*PreProcEnd);
			itor = tokenlist.erase(PreProcBegin, PreProcEnd);
			--itor;
			PreProcBegin = itor;
			PreProcEnd = itor;
		}
		// otherwise clean out any newline symbols
		else
		{
			if((itor->GetType() == Token::OPERATOR) &&
				(strcmp(itor->GetOperator(), g_pszNewLine) == 0))
			{
				itor = tokenlist.erase(itor);
				--itor;
			}
		}

	}

	MacroList.clear();

	return true;
}


//------------------------------------------------------------------------//
bool Lexer::ExpandMacro(TokenList& tokenlist, TokenListItor& itor, MacroSetItor itr)
{
	// watch variables.  remove when done debugging
	Macro CurrentMacro = (*itr);
	Token CurrentToken = (*itor);

	TokenList InsertionList;

	int iCodeTokens = 0;
	TokenListItor BeginningItor = itor;
	TokenListItor CurrentItor = itor;

	// first, we need to figure out how many arguments the macro definition has
	// and if the macro has the proper format.  If we have a macro with arguments,
	// we store the arguments in the macro for later substitution when we insert
	// the macro tokens into the main token list.
	if(CurrentMacro.m_iTotalArguments > 0)
	{
		// Calculate the number of code tokens needed to call this macro
		iCodeTokens = (CurrentMacro.m_iTotalArguments * 2) + 2;

		// advance to where the opening parentheses should be
		++CurrentItor;
		// error check to make sure the opening parentheses
		// are in the correct spot
		if(((*CurrentItor).GetType() != Token::OPERATOR) || 
			(string((*CurrentItor).GetOperator()) != g_pszLeftParen))
		{
			return Error::Handle("Unable to find opening paren in macro definition");
		}

		++CurrentItor;
		for(int i = 0; i < CurrentMacro.m_iTotalArguments; i++)
		{
			CurrentMacro.m_SubstitutionVector[i] = (*CurrentItor);
			++CurrentItor;

			// error check to make sure that commas and ending
			// parentheses are in the right place
			if(i == (CurrentMacro.m_iTotalArguments - 1))
			{
				// fail if a right paren is missing
				if(((*CurrentItor).GetType() != Token::OPERATOR) || 
				(string((*CurrentItor).GetOperator()) != g_pszRightParen))
				{
					return Error::Handle("Unable to find closing paren in macro definition");
				}
			}
			else
			{
				// fail if a comma is missing
				if(((*CurrentItor).GetType() != Token::OPERATOR) || 
				(string((*CurrentItor).GetOperator()) != ","))
				{
					return Error::Handle("Missing comma in macro definition");
				}
				++CurrentItor;
			}
		}
	}
	else
	{
		iCodeTokens = 1;
	}

	// Now that we've set up the macro structure, we're ready to proceed with
	// removal of the tokens which make up the macro in the script, then
	// substitute the macro tokens in its place.

	// remove the macro code tokens first.  CurrentItor should be pointing
	// to the last token in the macro (it may be the same as the first).  We
	// need to push it forward one token so we can just erase this from 
	// the token stream.

	++CurrentItor;

	CurrentItor = tokenlist.erase(BeginningItor, CurrentItor);
	itor = CurrentItor;
	--itor;

	// Let's us know whether we're substituting argument text on this cycle
	bool bArgSubst = false;

	// Cycle through the list of tokens stored in the macro an insert them into
	// the token list.  If the token is found to be an argument, then find the
	// text that needs to be substituted and insert that token instead of the
	// argument token.
	for(TokenListItor TItor = CurrentMacro.m_MacroList.begin(); 
		TItor != CurrentMacro.m_MacroList.end(); ++TItor)
	{
		bArgSubst = false;
		if((*TItor).GetType() == Token::VARIABLE)
		{
			for(int iArgIndex = 0; iArgIndex < CurrentMacro.m_iTotalArguments; iArgIndex++)
			{
				if(CurrentMacro.m_ArgumentVector[iArgIndex] == (*TItor).GetVariable())
				{
					bArgSubst = true;
					CurrentMacro.m_iCurrentArgument = iArgIndex;
				}
			}
		}
		// Here we either insert the actual macro token or the argument which is
		// to be substituted for the token.
		if(bArgSubst)
			CurrentItor = tokenlist.insert(CurrentItor, CurrentMacro.m_SubstitutionVector[CurrentMacro.m_iCurrentArgument]);
		else
			CurrentItor = tokenlist.insert(CurrentItor, (*TItor));
		// Next!
		++CurrentItor;
	}

	return true;
}

//------------------------------------------------------------------------//
// ParseVector is a specialized class that substitutes a vector for individual
// tokens using the built-in definition vector(a, b, c), technically not stored as
// a macro but parsed during the macro processing phase.  Thus, a client must
// turn on macro processing in order to have built-in vector support.
bool Lexer::ParseVector(TokenList& tokenlist, TokenListItor& itor)
{
	TokenListItor CurrentItor = itor;
	TokenListItor BeginningItor = itor;
	Token CurrentToken = (*CurrentItor);
	TokenVector3D vVector;
	vVector.x = 0;
	vVector.y = 0;
	vVector.z = 0;

#ifdef TOKEN_DEBUGGING_INFO
	int iFileIndex = CurrentToken.GetFileIndex();
	int iLineNumber = CurrentToken.GetLineNumber();
	string sFileName = tokenlist.GetFileName(iFileIndex);
#endif


	// Look for opening paren
	CurrentItor++;
	if((*CurrentItor).IsNewline())
		CurrentItor++;
	if((CurrentItor == tokenlist.end()) || 
		((*CurrentItor).GetType() != Token::OPERATOR) ||
		(string((*CurrentItor).GetOperator()) != "("))
	{
		return Error::Handle("Missing opening paren in vector definition.");
	}
	
	// check for first vector value (real)
	CurrentItor++;
	if((*CurrentItor).IsNewline())
		CurrentItor++;
	CurrentToken = (*CurrentItor);
	if((CurrentItor == tokenlist.end()) || 
		(CurrentItor->GetType() != Token::REAL) ||
		(CurrentItor->GetType() != Token::INTEGER))
	{
		return Error::Handle("Error in first vector value.");
	}
	if(CurrentItor->GetType() == Token::REAL)
		vVector.x = CurrentItor->GetReal();
	else
		vVector.x = CurrentItor->GetInteger();

	// check for comma
	CurrentItor++;
	CurrentToken = (*CurrentItor);
	if((CurrentItor == tokenlist.end()) || 
		((*CurrentItor).GetType() != Token::OPERATOR) ||
		(string((*CurrentItor).GetOperator()) != ","))
	{
		return Error::Handle("Missing comma in vector definition.");
	}
		
	// check for second vector value (real)
	CurrentItor++;
	if((*CurrentItor).IsNewline())
		CurrentItor++;
	CurrentToken = (*CurrentItor);
	if((CurrentItor == tokenlist.end()) || 
		(CurrentItor->GetType() != Token::REAL) ||
		(CurrentItor->GetType() != Token::INTEGER))
	{
		return Error::Handle("Error in first vector value.");
	}
	if(CurrentItor->GetType() == Token::REAL)
		vVector.y = CurrentItor->GetReal();
	else
		vVector.y = CurrentItor->GetInteger();

	// check for comma
	CurrentItor++;
	CurrentToken = (*CurrentItor);
	if((CurrentItor == tokenlist.end()) || 
		((*CurrentItor).GetType() != Token::OPERATOR) ||
		(string((*CurrentItor).GetOperator()) != ","))
	{
		return Error::Handle("Missing comma in vector definition.");
	}
		
	// check for third vector value (real)
	CurrentItor++;
	if((*CurrentItor).IsNewline())
		CurrentItor++;
	CurrentToken = (*CurrentItor);
	if((CurrentItor == tokenlist.end()) || 
		(CurrentItor->GetType() != Token::REAL) ||
		(CurrentItor->GetType() != Token::INTEGER))
	{
		return Error::Handle("Error in first vector value.");
	}
	if(CurrentItor->GetType() == Token::REAL)
		vVector.z = CurrentItor->GetReal();
	else
		vVector.z = CurrentItor->GetInteger();

	// Look for closing paren
	CurrentItor++;
	if((*CurrentItor).IsNewline())
		CurrentItor++;
	if((CurrentItor == tokenlist.end()) || 
		((*CurrentItor).GetType() != Token::OPERATOR) ||
		(string((*CurrentItor).GetOperator()) != ")"))
	{
		return Error::Handle("Missing closing paren in vector definition.");
	}
		
	// erase the definition from the token list
	CurrentItor++;
	CurrentItor = tokenlist.erase(BeginningItor, CurrentItor);
	CurrentToken = (*CurrentItor);

	// create a token from the Vector and insert it into the list
	Token VectorTok;
	VectorTok.InitVector(vVector);
	CurrentItor = tokenlist.insert(CurrentItor, VectorTok);

	itor = CurrentItor;

#ifdef TOKEN_DEBUGGING_INFO
	VectorTok.SetFileIndex(iFileIndex);
	VectorTok.SetLineNumber(iLineNumber);
#endif

	return true;
}


//------------------------------------------------------------------------//
// ParseGUID is a specialized function which turns the DEFINE_GUID() macro
// into a Lexer class macro to be used for later substitution.

// Sample format:
// DEFINE_GUID(SomeIdentifier, 
// 0xf9f5c237, 0x45e0, 0x11d3, 0x94, 0x56, 0x0, 0xa0, 0xc9, 0x69, 0x6b, 0x73);

// Note that there is a semi-colon needed at the end of the macro.  Also, the
// parser can handle whitespace anywhere in the definition as well, so the above
// example would be parsed correctly.

// NOTE!!! Fix so whitespace can be anywhere in the definition!!!

bool Lexer::ParseGUID(TokenList& tokenlist, TokenListItor& itor)
{
	TokenListItor CurrentItor = itor;
	TokenListItor BeginningItor = itor;
	Token CurrentToken = (*CurrentItor);
	GUID guidCurrent;
	Macro CurrentMacro;

#ifdef TOKEN_DEBUGGING_INFO
	int iFileIndex = CurrentToken.GetFileIndex();
	int iLineNumber = CurrentToken.GetLineNumber();
	string sFileName = tokenlist.GetFileName(iFileIndex);
#endif

	// Look for opening paren
	CurrentItor++;
	if((*CurrentItor).IsNewline())
		CurrentItor++;
	if((CurrentItor == tokenlist.end()) || 
		((*CurrentItor).GetType() != Token::OPERATOR) ||
		(string((*CurrentItor).GetOperator()) != "("))
	{
		return Error::Handle("Missing opening paren in GUID definition.");
	}
	
	// check for macro name
	CurrentItor++;
	if((*CurrentItor).IsNewline())
		CurrentItor++;
	CurrentToken = (*CurrentItor);
	if((CurrentItor == tokenlist.end()) || 
		((*CurrentItor).GetType() != Token::VARIABLE))
	{
		return Error::Handle("Invalid GUID name in GUID definition.");
	}
	CurrentMacro.m_sName = (*CurrentItor).GetVariable();

	// check for comma
	CurrentItor++;
	if((*CurrentItor).IsNewline())
		CurrentItor++;
	CurrentToken = (*CurrentItor);
	if((CurrentItor == tokenlist.end()) || 
		((*CurrentItor).GetType() != Token::OPERATOR) ||
		(string((*CurrentItor).GetOperator()) != ","))
	{
		return Error::Handle("Missing comma in GUID definition.");
	}
	
	// check for first GUID value (DWORD)
	CurrentItor++;
	if((*CurrentItor).IsNewline())
		CurrentItor++;
	CurrentToken = (*CurrentItor);
	if((CurrentItor == tokenlist.end()) || 
		((*CurrentItor).GetType() != Token::INTEGER))
	{
		return Error::Handle("Error in first GUID value.");
	}
	guidCurrent.Data1 = (*CurrentItor).GetInteger();

	// check for comma
	CurrentItor++;
	CurrentToken = (*CurrentItor);
	if((CurrentItor == tokenlist.end()) || 
		((*CurrentItor).GetType() != Token::OPERATOR) ||
		(string((*CurrentItor).GetOperator()) != ","))
	{
		return Error::Handle("Missing comma in GUID definition.");
	}
	
	// get the next two word values
	for(int i = 0; i < 2; i++)
	{
		// check for next GUID values (WORD)
		CurrentItor++;
		if((*CurrentItor).IsNewline())
			CurrentItor++;
		CurrentToken = (*CurrentItor);
		if((CurrentItor == tokenlist.end()) || 
			((*CurrentItor).GetType() != Token::INTEGER))
		{
			return Error::Handle("Error in GUID value.");
		}
		if(i)
			guidCurrent.Data3 = (*CurrentItor).GetInteger();
		else
			guidCurrent.Data2 = (*CurrentItor).GetInteger();

		// check for comma
		CurrentItor++;
		if((*CurrentItor).IsNewline())
			CurrentItor++;
		CurrentToken = (*CurrentItor);
		if((CurrentItor == tokenlist.end()) || 
			((*CurrentItor).GetType() != Token::OPERATOR) ||
			(string((*CurrentItor).GetOperator()) != ","))
		{
			return Error::Handle("Missing comma in GUID definition.");
		}
	}
	
	// get the next 8 byte values
	for(i = 0; i < 8; i++)
	{
		// check for next GUID values (BYTE)
		CurrentItor++;
		if((*CurrentItor).IsNewline())
			CurrentItor++;
		CurrentToken = (*CurrentItor);
		if((CurrentItor == tokenlist.end()) || 
			((*CurrentItor).GetType() != Token::INTEGER))
		{
			return Error::Handle("Error in GUID value.");
		}
		guidCurrent.Data4[i] = (*CurrentItor).GetInteger();

		if(i == 7)
		{
			// check for ending paren
			CurrentItor++;
			if((*CurrentItor).IsNewline())
				CurrentItor++;
			CurrentToken = (*CurrentItor);
			if((CurrentItor == tokenlist.end()) || 
				((*CurrentItor).GetType() != Token::OPERATOR) ||
				(string((*CurrentItor).GetOperator()) != ")"))
			{
				return Error::Handle("Missing ending paren in GUID definition.");
			}
		}
		else
		{
			// check for comma
			CurrentItor++;
			if((*CurrentItor).IsNewline())
				CurrentItor++;
			CurrentToken = (*CurrentItor);
			if((CurrentItor == tokenlist.end()) || 
				((*CurrentItor).GetType() != Token::OPERATOR) ||
				(string((*CurrentItor).GetOperator()) != ","))
			{
				return Error::Handle("Missing comma in GUID definition.");
			}
		}
	}
	
	// check for ending semicolon
	CurrentItor++;
	if((*CurrentItor).IsNewline())
		CurrentItor++;
	CurrentToken = (*CurrentItor);
	if((CurrentItor == tokenlist.end()) || 
		((*CurrentItor).GetType() != Token::OPERATOR) ||
		(string((*CurrentItor).GetOperator()) != g_pszSemiColon))
	{
		return Error::Handle("Missing semicolon at end of GUID definition.");
	}
	

	// erase the definition from the token list
	CurrentItor++;
	CurrentItor = tokenlist.erase(BeginningItor, CurrentItor);
	CurrentToken = (*CurrentItor);
	itor = CurrentItor;

	// create a token from the GUID, add it to a macro definition, and 
	// insert it into the Lexer's macro set
	Token GuidTok;
	GuidTok.InitGuid(guidCurrent);

#ifdef TOKEN_DEBUGGING_INFO
	GuidTok.SetFileIndex(iFileIndex);
	GuidTok.SetLineNumber(iLineNumber);
#endif
	CurrentMacro.m_MacroList.push_back(GuidTok);
	m_MacroSet.insert(CurrentMacro);
	
	return true;
}


//------------------------------------------------------------------------//
void Lexer::ClearMacros()
{
	m_MacroSet.clear();
}


//------------------------------------------------------------------------//
bool Lexer::MakeToken(const string& sToken, Token::TOKEN_TYPE Type)
{
	bool bSuccess = false;
	if(m_bEnableComments && m_bCStyleCommentActive)
		return true;

	Token token;

	switch(Type)
	{
	case Token::UNKNOWN_TOKEN:
		if(IsOperator(sToken))
			bSuccess = token.InitOperator(sToken.c_str());
		else if(IsInteger(sToken))
			bSuccess = token.InitInteger(GetInteger(sToken));
		else if(IsReal(sToken))
			bSuccess = token.InitReal(GetReal(sToken));
		else if(IsBoolean(sToken))
			bSuccess = token.InitBoolean(GetBoolean(sToken));
		else if(IsHex(sToken))
			bSuccess = token.InitInteger(GetHex(sToken));
		else if(IsKeyword(sToken))
			bSuccess = token.InitKeyword(sToken.c_str());
		else
			bSuccess = token.InitVariable(sToken.c_str());
		break;

	case Token::OPERATOR:
		if(IsOperator(sToken))
			bSuccess = token.InitOperator(sToken.c_str());
		break;

	case Token::KEYWORD:
		if(IsKeyword(sToken))
			bSuccess = token.InitKeyword(sToken.c_str());
		break;

	case Token::VARIABLE:
		bSuccess = token.InitVariable(sToken.c_str());
		break;

	case Token::STRING:
		bSuccess = token.InitString(sToken.c_str());
		break;

	case Token::INTEGER:
		if(IsInteger(sToken))
			bSuccess = token.InitInteger(GetInteger(sToken));
		break;

	case Token::REAL:
		if(IsReal(sToken))
			bSuccess = token.InitReal(GetReal(sToken));
		break;

	case Token::BOOLEAN:
		if(IsBoolean(sToken))
			bSuccess = token.InitBoolean(GetBoolean(sToken));
		break;
	default:
		bSuccess = false;
	};

	if(bSuccess)
	{	
#ifdef TOKEN_DEBUGGING_INFO
		token.SetLineNumber(m_iCurrentLineNumber);
#endif
		m_pTokenList->push_back(token);
		int index = m_pTokenList->GetFileIndex(m_CurrentFileStack.top());
	}
	return bSuccess;
}


//------------------------------------------------------------------------//
bool Lexer::IsReal(const string& sToken) const
{
	for(int i = 0; i < sToken.length(); ++i)
		if(!IsDigit(sToken[i]))
			if(!((i == 0) && (sToken[i] == '-')) && (sToken[i] != '.'))
				return false;
	return true;
}


//------------------------------------------------------------------------//
bool Lexer::IsInteger(const string& sToken) const
{
	for(int i = 0; i < sToken.length(); ++i)
		if(!IsDigit(sToken[i]))
			if(!((i == 0) && (sToken[i] == '-')))
				return false;
	return true;
}

//------------------------------------------------------------------------//
bool Lexer::IsHex(const string& sToken) const
{
	if(sToken.length() < 3)
		return false;
	if(!((sToken[0] == '0') && ((sToken[1] == 'x') || (sToken[1] == 'X'))))
		return false;
	for(int i = 2; i < sToken.length(); ++i)
		if(!IsHexDigit(sToken[i]))
			return false;
	return true;
}

//------------------------------------------------------------------------//
bool Lexer::IsKeyword(const string& sToken) const
{
	if(find(m_KeywordVector.begin(), m_KeywordVector.end(), sToken) 
		!= m_KeywordVector.end())
		return true;
	else
		return false;
}

//------------------------------------------------------------------------//
bool Lexer::IsOperator(const string& sToken) const
{
	if(find(m_OperatorVector.begin(), m_OperatorVector.end(), sToken) 
		!= m_OperatorVector.end())
		return true;
	else
		return false;
}

//------------------------------------------------------------------------//
bool Lexer::IsBoolean(const string& sToken) const
{
	if(sToken == g_pszTrue || sToken == g_pszFalse)
		return true;
	else
		return false;
}

//------------------------------------------------------------------------//
bool Lexer::GetBoolean(const string& sToken)
{
	if(sToken == g_pszTrue)
		return true;
	else
		return false;
}


//------------------------------------------------------------------------//
DWORD Lexer::GetHex(const std::string& sToken)	
{
	DWORD i = 0;
	sscanf(sToken.c_str(), "%x", &i);  
	return i;
}


