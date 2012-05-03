/***********************************************************\
Copyright (C) James Boer, 2002. 
All rights reserved worldwide.

This software is provided "as is" without express or implied
warranties. You may freely copy and compile this source into
applications you distribute provided that the copyright text
below is included in the resulting source code, for example:
"Portions Copyright (C) James Boer, 2002"
\***********************************************************/
#ifndef SCRIPT_H__
#define SCRIPT_H__

#include "Audio.h"
#include "Lexer.h"

namespace Audio
{

class ScriptNode;

typedef std::vector<Token*> DataChunk;

class Script
{
public:

	enum SCRIPT_TYPE
	{
		NO_NODE_DATA = 0,
		VARIABLE,
		STRING,
		INTEGER,
		REAL,
		BOOLEAN,
		VECTOR,
		SCRIPT_GUID
	};

	Script();
	~Script();

	ScriptNode* GetRoot();

private:
	friend class ScriptLoader;
	void Clear();

	TokenList	m_TokenList;
	ScriptNode*	m_pRoot;
};


class ScriptNode
{
public:
	ScriptNode();
	~ScriptNode();

	// Name
	const char* GetName();

	// Data
	Script::SCRIPT_TYPE GetDataType();
	const char* GetString();
	const char* GetVariable();
	int GetInteger();
	bool GetBool();
	double GetReal();
	AUDIOVECTOR GetVector();
	GUID GetGuid();

	// Navigation
	bool HasData();
	ScriptNode* GetSibling();
	ScriptNode* GetChild();
	ScriptNode* GetParent();

	

private:
	friend ScriptLoader;
	friend Script;
	void Clear();

	Token*			m_pName;
	Token*			m_pData;
	ScriptNode*		m_pParent;
	ScriptNode*		m_pSibling;
	ScriptNode*		m_pChild;
};

class ScriptLoader
{
public:
	ScriptLoader();
	~ScriptLoader();

	void Clear();

	bool Init();
	void Term();

	bool Load(std::string sFileName, Script& script);
	bool Load(uint8* pData, uint32 nDataLength, Script& script);

private:
	bool BuildNodeTree(Script& script);

	Lexer	m_Lexer;

};

}; // namespace Audio

#endif // SCRIPT_H__